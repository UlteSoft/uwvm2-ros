/* PRIVATE DEVELOPMENT SOURCE. No build, GC, Guest or debugger result yet. */
#include <Python.h>
#include <frameobject.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdatomic.h>
#include <string.h>
#include "frame_snapshot.h"
#include "../../debug/include/uwvm_language_snapshot.h"

#if PY_MAJOR_VERSION != 3 || PY_MINOR_VERSION != 14 || PY_MICRO_VERSION != 8
#error "This provider requires the explicitly admitted CPython3.14.8 full ABI"
#endif
#if defined(Py_GIL_DISABLED) || defined(Py_LIMITED_API)
#error "Free-threaded and Limited API builds are not admitted"
#endif
#if !defined(__wasm32__) || defined(__wasm64__)
#error "This producer profile is the actual wasm32 CPython build"
#endif
#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "Snapshot structs serialize only on the admitted little-endian target"
#endif
_Static_assert(sizeof(long long) == 8 && sizeof(double) == 8 &&
               DBL_MANT_DIG == 53 && DBL_MAX_EXP == 1024, "scalar layout");

static uwvm_language_snapshot snapshot;
static PyThreadState *trace_thread;
static uint64_t sequence;
static int busy;

/* A separately compiled -fno-lto TU is deliberately opaque to this producer.
 * Actual final IR/Wasm must retain the whole copy and the current DWARF root. */
extern void uwvm_python_debug_anchor(uwvm_language_snapshot const *current);
static void publish_flags(uint32_t value) {
    atomic_signal_fence(memory_order_seq_cst);
    *(volatile uint32_t *)&snapshot.header.flags = value;
    atomic_signal_fence(memory_order_seq_cst);
}

/* Exact Unicode only; encode directly without __str__, repr, getters or an
 * allocating UTF-8 cache. An overlong identifier is unavailable, not aliased
 * to a truncated identifier. Value strings may carry a marked prefix. */
static int unicode_copy(PyObject *value, uint32_t maximum, int prefix,
                        uint32_t *offset, uint32_t *length, int *truncated) {
    uint8_t buffer[4096]; uint32_t used = 0;
    if (!PyUnicode_CheckExact(value) || maximum > 1024) return 0;
    Py_ssize_t n = PyUnicode_GET_LENGTH(value), count = n;
    *truncated = 0;
    if (count > (Py_ssize_t)maximum) {
        if (!prefix) return 0;
        count = maximum; *truncated = 1;
    }
    for (Py_ssize_t i = 0; i < count; ++i) {
        Py_UCS4 ch = PyUnicode_READ_CHAR(value, i);
        if (ch > 0x10ffff || (ch >= 0xd800 && ch <= 0xdfff)) return 0;
        if (ch < 0x80) buffer[used++] = (uint8_t)ch;
        else if (ch < 0x800) {
            buffer[used++] = (uint8_t)(0xc0 | (ch >> 6));
            buffer[used++] = (uint8_t)(0x80 | (ch & 63));
        } else if (ch < 0x10000) {
            buffer[used++] = (uint8_t)(0xe0 | (ch >> 12));
            buffer[used++] = (uint8_t)(0x80 | ((ch >> 6) & 63));
            buffer[used++] = (uint8_t)(0x80 | (ch & 63));
        } else {
            buffer[used++] = (uint8_t)(0xf0 | (ch >> 18));
            buffer[used++] = (uint8_t)(0x80 | ((ch >> 12) & 63));
            buffer[used++] = (uint8_t)(0x80 | ((ch >> 6) & 63));
            buffer[used++] = (uint8_t)(0x80 | (ch & 63));
        }
    }
    if (!uwvm_language_snapshot_bytes(&snapshot, buffer, used, offset)) return 0;
    *length = used; return 1;
}

static void unavailable(uwvm_language_snapshot_value *out) {
    out->flags |= UWVM_LS_UNAVAILABLE;
    out->payload = 0; out->bytes_offset = out->bytes_length = 0;
    out->child_first = out->child_count = 0; out->known &= UWVM_LS_NAME_KNOWN;
}

static void index_name(uwvm_language_snapshot_value *out, uint32_t i) {
    char buffer[16]; int n = snprintf(buffer, sizeof(buffer), "[%u]", (unsigned)i);
    if (n > 0 && (size_t)n < sizeof(buffer) &&
        uwvm_language_snapshot_bytes(&snapshot, buffer, (uint32_t)n, &out->name_offset)) {
        out->name_length = (uint32_t)n; out->known |= UWVM_LS_NAME_KNOWN;
    }
}

static void copy_value(uint32_t at, PyObject *value, uint32_t depth,
                       PyObject *const *ancestors, uint32_t ancestor_count) {
    uwvm_language_snapshot_value *out = &snapshot.values[at];
    if (value == NULL) { unavailable(out); return; }
    if (value == Py_None) { out->kind = UWVM_LS_NONE; out->known |= UWVM_LS_DATA_KNOWN; return; }
    if (PyBool_Check(value)) {
        out->kind = UWVM_LS_BOOL; out->payload = value == Py_True;
        out->known |= UWVM_LS_DATA_KNOWN; return;
    }
    if (PyLong_CheckExact(value)) {
        int overflow = 0;
        long long number = PyLong_AsLongLongAndOverflow(value, &overflow);
        if (PyErr_Occurred()) { PyErr_Clear(); unavailable(out); return; }
        if (!overflow) { out->kind = UWVM_LS_I64; out->payload = (uint64_t)number; }
        else if (overflow > 0) {
            unsigned long long number_unsigned = PyLong_AsUnsignedLongLong(value);
            if (PyErr_Occurred()) { PyErr_Clear(); unavailable(out); return; }
            out->kind = UWVM_LS_U64; out->payload = (uint64_t)number_unsigned;
        } else { unavailable(out); return; }
        out->known |= UWVM_LS_DATA_KNOWN; return;
    }
    if (PyFloat_CheckExact(value)) {
        double number = PyFloat_AS_DOUBLE(value); out->kind = UWVM_LS_F64;
        memcpy(&out->payload, &number, sizeof(number));
        out->known |= UWVM_LS_DATA_KNOWN; return;
    }
    if (PyUnicode_CheckExact(value)) {
        int truncated; out->kind = UWVM_LS_UTF8;
        out->payload = (uint64_t)PyUnicode_GET_LENGTH(value);
        if (!unicode_copy(value, 1024, 1, &out->bytes_offset, &out->bytes_length, &truncated)) {
            unavailable(out); return;
        }
        out->known |= UWVM_LS_DATA_KNOWN;
        if (truncated) out->flags |= UWVM_LS_VALUE_TRUNCATED;
        return;
    }
    if (PyBytes_CheckExact(value)) {
        Py_ssize_t n = PyBytes_GET_SIZE(value); uint32_t take = n > 1024 ? 1024 : (uint32_t)n;
        out->kind = UWVM_LS_BYTES; out->payload = (uint64_t)n;
        if (!uwvm_language_snapshot_bytes(&snapshot, PyBytes_AS_STRING(value), take, &out->bytes_offset)) {
            unavailable(out); return;
        }
        out->bytes_length = take; out->known |= UWVM_LS_DATA_KNOWN;
        if (n > 1024) out->flags |= UWVM_LS_VALUE_TRUNCATED;
        return;
    }
    if (!PyList_CheckExact(value) && !PyTuple_CheckExact(value) && !PyDict_CheckExact(value)) {
        unavailable(out); return; /* No arbitrary object/property/iterator call. */
    }
    int dictionary = PyDict_CheckExact(value);
    out->kind = dictionary ? UWVM_LS_DICT : (PyList_CheckExact(value) ? UWVM_LS_LIST : UWVM_LS_TUPLE);
    Py_ssize_t n = dictionary ? PyDict_Size(value) :
        (out->kind == UWVM_LS_LIST ? PyList_GET_SIZE(value) : PyTuple_GET_SIZE(value));
    out->payload = (uint64_t)n; out->known |= UWVM_LS_DATA_KNOWN;
    for (uint32_t i = 0; i < ancestor_count; ++i) {
        if (ancestors[i] == value) { out->flags |= UWVM_LS_VALUE_TRUNCATED; return; }
    }
    if (depth >= UWVM_LS_MAX_DEPTH) { out->flags |= UWVM_LS_VALUE_TRUNCATED; return; }
    uint32_t count = n > UWVM_LS_MAX_CHILDREN ? UWVM_LS_MAX_CHILDREN : (uint32_t)n;
    if (count != (uint64_t)n) out->flags |= UWVM_LS_VALUE_TRUNCATED;
    uint32_t first = uwvm_language_snapshot_reserve(&snapshot, count);
    if (first == UWVM_LS_NO_PARENT) { out->flags |= UWVM_LS_VALUE_TRUNCATED; return; }
    out->child_first = first; out->child_count = count; out->known |= UWVM_LS_CHILDREN_KNOWN;
    PyObject *chain[UWVM_LS_MAX_DEPTH + 1];
    for (uint32_t i = 0; i < ancestor_count; ++i) chain[i] = ancestors[i];
    chain[ancestor_count] = value;
    Py_ssize_t cursor = 0;
    for (uint32_t i = 0; i < count; ++i) {
        uwvm_language_snapshot_value *child = &snapshot.values[first + i];
        child->frame_id = out->frame_id; child->parent_value = at; index_name(child, i);
        if (!dictionary) {
            PyObject *item = out->kind == UWVM_LS_LIST ? PyList_GET_ITEM(value, i) : PyTuple_GET_ITEM(value, i);
            copy_value(first + i, item, depth + 1, chain, ancestor_count + 1);
        } else {
            PyObject *key, *item;
            if (!PyDict_Next(value, &cursor, &key, &item)) { unavailable(child); continue; }
            /* A pair preserves the actual key, including unsupported key types;
             * __hash__, __eq__, repr and mapping getters are never invoked. */
            child->kind = UWVM_LS_TUPLE; child->payload = 2; child->known |= UWVM_LS_DATA_KNOWN;
            if (depth + 1 >= UWVM_LS_MAX_DEPTH) {
                child->flags |= UWVM_LS_VALUE_TRUNCATED; continue;
            }
            uint32_t pair = uwvm_language_snapshot_reserve(&snapshot, 2);
            if (pair == UWVM_LS_NO_PARENT) { child->flags |= UWVM_LS_VALUE_TRUNCATED; continue; }
            child->child_first = pair; child->child_count = 2; child->known |= UWVM_LS_CHILDREN_KNOWN;
            for (uint32_t k = 0; k < 2; ++k) {
                snapshot.values[pair + k].frame_id = out->frame_id;
                snapshot.values[pair + k].parent_value = first + i; index_name(&snapshot.values[pair + k], k);
                copy_value(pair + k, k ? item : key, depth + 2, chain, ancestor_count + 1);
            }
        }
    }
}

static void copy_frame(PyFrameObject *frame, uint32_t at) {
    uwvm_language_snapshot_frame *out = &snapshot.frames[at];
    out->id = at + 1; out->parent = at ? at : UWVM_LS_NO_PARENT;
    PyCodeObject *code = PyFrame_GetCode(frame);
    if (code == NULL) { PyErr_Clear(); snapshot.header.flags |= UWVM_LS_CAPTURE_ERROR; return; }
    int truncated;
    if (unicode_copy(code->co_filename, 512, 0, &out->file_offset, &out->file_length, &truncated)) out->known |= UWVM_LS_FILE_KNOWN;
    if (unicode_copy(code->co_qualname, 512, 0, &out->function_offset, &out->function_length, &truncated)) out->known |= UWVM_LS_FUNCTION_KNOWN;
    int line = PyFrame_GetLineNumber(frame), instruction = PyFrame_GetLasti(frame);
    if (line > 0) { out->line = (uint32_t)line; out->known |= UWVM_LS_LINE_KNOWN; }
    if (instruction >= 0) { out->instruction = (uint64_t)instruction; out->known |= UWVM_LS_INSTRUCTION_KNOWN; }
    /* Public co_* tuples are finite here. Do not allocate a tuple proportional
     * to an arbitrary huge code object merely to truncate the displayed list. */
    if (code->co_nlocalsplus < 0 || code->co_nlocalsplus > 256) {
        out->flags |= UWVM_LS_TRUNCATED; Py_DECREF(code); return;
    }
    PyObject *sets[3] = {PyCode_GetVarnames(code), NULL, NULL};
    if (sets[0]) sets[1] = PyCode_GetCellvars(code);
    if (sets[1]) sets[2] = PyCode_GetFreevars(code);
    PyObject *names[32]; uint32_t count = 0, arguments = 0;
    int formal = -1;
    if (code->co_argcount >= 0 && code->co_argcount <= 256 &&
        code->co_kwonlyargcount >= 0 && code->co_kwonlyargcount <= 256) {
        formal = code->co_argcount + code->co_kwonlyargcount +
            !!(code->co_flags & CO_VARARGS) + !!(code->co_flags & CO_VARKEYWORDS);
    }
    if (!sets[2]) { PyErr_Clear(); snapshot.header.flags |= UWVM_LS_CAPTURE_ERROR; }
    else for (uint32_t group = 0; group < 3; ++group) {
        Py_ssize_t n = PyTuple_GET_SIZE(sets[group]);
        for (Py_ssize_t i = 0; i < n; ++i) {
            PyObject *name = PyTuple_GET_ITEM(sets[group], i); int duplicate = 0;
            if (!PyUnicode_CheckExact(name)) { snapshot.header.flags |= UWVM_LS_CAPTURE_ERROR; continue; }
            for (uint32_t j = 0; j < count; ++j) if (PyUnicode_Compare(names[j], name) == 0) duplicate = 1;
            if (duplicate) continue;
            if (count == 32) { out->flags |= UWVM_LS_TRUNCATED; continue; }
            names[count++] = name;
            if (group == 0 && formal >= 0 && i < formal) ++arguments;
        }
    }
    out->value_first = snapshot.header.value_count;
    uint32_t first = uwvm_language_snapshot_reserve(&snapshot, count);
    if (first != UWVM_LS_NO_PARENT) {
        out->value_first = first; out->value_count = count;
        if (formal >= 0 && arguments == (uint32_t)formal) {
            out->argument_count = arguments; out->known |= UWVM_LS_ARGUMENTS_KNOWN;
        }
        for (uint32_t i = 0; i < count; ++i) {
            uwvm_language_snapshot_value *local = &snapshot.values[first + i];
            local->frame_id = out->id; local->parent_value = UWVM_LS_NO_PARENT;
            if (i < arguments) local->flags |= UWVM_LS_ARGUMENT;
            if (unicode_copy(names[i], 128, 0, &local->name_offset, &local->name_length, &truncated)) local->known |= UWVM_LS_NAME_KNOWN;
            PyObject *value = PyFrame_GetVar(frame, names[i]);
            if (value == NULL) { PyErr_Clear(); unavailable(local); }
            else { copy_value(first + i, value, 0, NULL, 0); Py_DECREF(value); }
        }
    }
    for (int i = 0; i < 3; ++i) Py_XDECREF(sets[i]);
    Py_DECREF(code);
}

static int trace(PyObject *owner, PyFrameObject *frame, int event, PyObject *arg) {
    (void)arg;
    if (busy || owner != Py_None || PyThreadState_Get() != trace_thread ||
        (event != PyTrace_LINE && event != PyTrace_RETURN && event != PyTrace_EXCEPTION)) return 0;
    if (sequence > UINT64_MAX - 2) { snapshot.header.flags = 0; return 0; }
    busy = 1; sequence += 2;
    PyObject *saved_error = PyErr_GetRaisedException();
    int gc_was_enabled = PyGC_Disable();
    if (!uwvm_language_snapshot_reset(&snapshot, UWVM_LS_PYTHON, sequence)) {
        if (gc_was_enabled) PyGC_Enable();
        PyErr_SetRaisedException(saved_error); busy = 0; return 0;
    }
    snapshot.header.event = (uint32_t)event;
    PyFrameObject *frames[UWVM_LS_MAX_FRAMES]; uint32_t count = 0;
    PyFrameObject *cursor = (PyFrameObject *)Py_NewRef(frame);
    while (cursor && count < UWVM_LS_MAX_FRAMES) {
        frames[count++] = cursor; cursor = PyFrame_GetBack(cursor);
    }
    if (cursor) { snapshot.header.flags |= UWVM_LS_TRUNCATED; Py_DECREF(cursor); }
    if (PyErr_Occurred()) { PyErr_Clear(); snapshot.header.flags |= UWVM_LS_CAPTURE_ERROR; }
    snapshot.header.frame_count = count;
    for (uint32_t i = 0; i < count; ++i) copy_frame(frames[count - 1 - i], i);
    for (uint32_t i = 0; i < count; ++i) Py_DECREF(frames[i]);
    if (PyErr_Occurred()) { PyErr_Clear(); snapshot.header.flags |= UWVM_LS_CAPTURE_ERROR; }
    snapshot.header.status = !!(snapshot.header.flags & UWVM_LS_CAPTURE_ERROR);
    publish_flags(snapshot.header.flags | UWVM_LS_LIVE | UWVM_LS_INSTRUMENTED);
    uwvm_python_debug_anchor(&snapshot);
    publish_flags(snapshot.header.flags & ~UWVM_LS_LIVE);
    if (gc_was_enabled) PyGC_Enable();
    PyErr_SetRaisedException(saved_error);
    busy = 0; return 0;
}

int uwvm_python_debug_install(void) {
    PyThreadState *current = PyThreadState_Get();
    if (trace_thread || current->c_tracefunc || current->c_traceobj) {
        PyErr_SetString(PyExc_RuntimeError, "debug snapshot trace slot is already owned"); return -1;
    }
    trace_thread = current; PyEval_SetTrace(trace, Py_None);
    if (current->c_tracefunc != trace || current->c_traceobj != Py_None) {
        trace_thread = NULL;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError, "debug trace installation failed");
        return -1;
    }
    return 0;
}

int uwvm_python_debug_uninstall(void) {
    PyThreadState *current = PyThreadState_Get();
    if (current != trace_thread || current->c_tracefunc != trace || current->c_traceobj != Py_None || busy) {
        PyErr_SetString(PyExc_RuntimeError, "debug snapshot trace ownership changed"); return -1;
    }
    snapshot.header.flags = 0; PyEval_SetTrace(NULL, NULL); trace_thread = NULL;
    return 0;
}
