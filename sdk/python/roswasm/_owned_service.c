/** Optional same-Wasm CPython bridge; no Python result allocation after import.
 * A capsule is an opaque, one-use attempt record, not a capability or grant.
 * Its exact bytes roots, status bytes and guarded C output exist before entry.
 * Destruction ONLY releases Guest memory; it never invokes ROS cleanup.
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <string.h>
#ifndef ROSWASM_OWNED_SERVICE
#define ROSWASM_OWNED_SERVICE 0
#endif
#if ROSWASM_OWNED_SERVICE != 0 && ROSWASM_OWNED_SERVICE != 1
#error "ROSWASM_OWNED_SERVICE must be exactly 0 or 1"
#endif
#if ROSWASM_OWNED_SERVICE
#if !defined(ROSWASM_BACKEND) || ROSWASM_BACKEND != 2
#error "owned Service requires the explicit ROS2 backend"
#endif
#if !defined(__wasm__) && !defined(ROSWASM_OWNED_SERVICE_CONTRACT_TEST)
#error "the production builtin must be linked into the actual Wasm interpreter"
#endif
#include "roswasm/rmw_service_v1.h"
#define LIMIT 4193786
#define GUARD 16
#define OFFSET(p) ((uint64_t)(uintptr_t)(p))
#define CAPSULE "roswasm.owned-service.attempt.v1"
typedef struct {
    unsigned index;
    uint64_t handle, incarnation, a, b;
    PyObject *input, *output, *status, *version_value;
    unsigned char *scratch;
    Py_ssize_t capacity;
    uint32_t code, version;
    int attempted, returned;
} Attempt;
static int invoking;
static void dispose(PyObject *object);
static Attempt *get(PyObject *object) {
    if (!PyCapsule_IsValid(object, CAPSULE) || PyCapsule_GetDestructor(object) != dispose) {
        PyErr_SetString(PyExc_ValueError, "actual owned-Service context required"); return NULL;
    }
    return (Attempt *)PyCapsule_GetPointer(object, CAPSULE);
}
static void dispose(PyObject *object) {
    Attempt *p = get(object);
    if (!p) { PyErr_Clear(); return; }
    Py_XDECREF(p->input); Py_XDECREF(p->output); Py_XDECREF(p->status); Py_XDECREF(p->version_value);
    PyMem_Free(p->scratch); PyMem_Free(p);
}
static int u64(PyObject *object, uint64_t *out) {
    if (!PyLong_CheckExact(object)) {
        PyErr_SetString(PyExc_TypeError, "exact unsigned integer required"); return 0;
    }
    *out = PyLong_AsUnsignedLongLong(object);
    return !PyErr_Occurred();
}
static PyObject *prepare(PyObject *self, PyObject *args) {
    unsigned index; PyObject *index_object, *capacity_object, *h, *inc, *a, *b, *input, *result;
    uint64_t selected_index, selected_capacity;
    Py_ssize_t capacity, n; Attempt *p;
    (void)self;
    if (!PyArg_ParseTuple(args, "OOOOOOO:prepare", &index_object, &h, &inc, &a, &b, &input, &capacity_object)) return NULL;
    if (!u64(index_object, &selected_index) || !u64(capacity_object, &selected_capacity)) return NULL;
    if (invoking || selected_index > 11 || selected_capacity > LIMIT || !PyBytes_CheckExact(input)) {
        PyErr_SetString(PyExc_ValueError, "bounded exact owned-Service call required"); return NULL;
    }
    index = (unsigned)selected_index; capacity = (Py_ssize_t)selected_capacity;
    n = PyBytes_GET_SIZE(input);
    if (n > LIMIT || ((index != 4 && index != 6) && n != 0) ||
        ((index == 1 || index == 2 || index == 3) ? capacity != 256 :
         index == 4 ? capacity != 8 : index == 7 ? capacity < 352 :
         index == 10 ? capacity != 1 : capacity != 0)) {
        PyErr_SetString(PyExc_ValueError, "wrong input/output shape for raw12 operation"); return NULL;
    }
    p = PyMem_Calloc(1, sizeof(*p));
    if (!p) return PyErr_NoMemory();
    p->index = index; p->capacity = capacity;
    if (!u64(h, &p->handle) || !u64(inc, &p->incarnation) || !u64(a, &p->a) || !u64(b, &p->b)) goto fail;
    if ((index == 7 && p->a > UINT32_MAX) ||
        (index == 0 && (p->handle || p->incarnation || p->a || p->b)) ||
        ((index == 1 || index == 2) && (p->incarnation || p->b)) ||
        ((index == 3 || index == 4 || index == 11) && (p->a || p->b)) ||
        ((index == 5 || index == 7) && p->b)) {
        PyErr_SetString(PyExc_ValueError, "noncanonical unused arguments or writer extent"); goto fail;
    }
    p->input = Py_NewRef(input);
    p->output = PyBytes_FromStringAndSize(NULL, capacity);
    p->status = PyBytes_FromStringAndSize(NULL, 4);
    p->version_value = PyBytes_FromStringAndSize(NULL, 4);
    p->scratch = PyMem_Malloc((size_t)capacity + 2 * GUARD);
    if (!p->output || !p->status || !p->version_value || !p->scratch) {
        if (!PyErr_Occurred()) PyErr_NoMemory(); goto fail;
    }
    memset(PyBytes_AS_STRING(p->output), 0, (size_t)capacity);
    memset(PyBytes_AS_STRING(p->status), 0, 4);
    memset(PyBytes_AS_STRING(p->version_value), 0, 4);
    memset(p->scratch, 0xa5, (size_t)capacity + 2 * GUARD);
    result = PyCapsule_New(p, CAPSULE, dispose);
    if (result) return result;
fail:
    Py_XDECREF(p->input); Py_XDECREF(p->output); Py_XDECREF(p->status); Py_XDECREF(p->version_value);
    PyMem_Free(p->scratch); PyMem_Free(p); return NULL;
}
static PyObject *execute(PyObject *self, PyObject *object) {
    Attempt *p = get(object); unsigned char *out; uint64_t in; uint32_t code = 0;
    (void)self;
    if (!p) return NULL;
    if (invoking || p->attempted) {
        PyErr_SetString(PyExc_RuntimeError, "attempt is busy or permanently consumed"); return NULL;
    }
    out = p->scratch + GUARD; in = OFFSET(PyBytes_AS_STRING(p->input));
    invoking = 1; p->attempted = 1;
    /* No allocation, callback, release of roots or fallible Python API below. */
    switch (p->index) {
    case 0: p->version = uwvm_ros2_rmw_srv_v1_abi_version(); break;
    case 1: code = uwvm_ros2_rmw_srv_v1_client_activate(p->handle,p->a,0,OFFSET(out)); break;
    case 2: code = uwvm_ros2_rmw_srv_v1_service_activate(p->handle,p->a,0,OFFSET(out)); break;
    case 3: code = uwvm_ros2_rmw_srv_v1_endpoint_inspect(p->handle,p->incarnation,0,OFFSET(out)); break;
    case 4: code = uwvm_ros2_rmw_srv_v1_client_submit(p->handle,p->incarnation,0,in,(uint64_t)PyBytes_GET_SIZE(p->input),OFFSET(out)); break;
    case 5: code = uwvm_ros2_rmw_srv_v1_client_abandon(p->handle,p->incarnation,p->a); break;
    case 6: code = uwvm_ros2_rmw_srv_v1_service_respond(p->handle,p->incarnation,p->a,p->b,0,in,(uint64_t)PyBytes_GET_SIZE(p->input)); break;
    case 7: code = uwvm_ros2_rmw_srv_v1_endpoint_peek(p->handle,p->incarnation,(uint32_t)p->a,0,OFFSET(out),(uint64_t)p->capacity); break;
    case 8: code = uwvm_ros2_rmw_srv_v1_endpoint_commit(p->handle,p->incarnation,p->a,p->b); break;
    case 9: code = uwvm_ros2_rmw_srv_v1_endpoint_cancel(p->handle,p->incarnation,p->a,p->b); break;
    case 10: code = uwvm_ros2_rmw_srv_v1_client_available(p->handle,p->incarnation,p->a,p->b,0,OFFSET(out)); break;
    case 11: code = uwvm_ros2_rmw_srv_v1_endpoint_retire(p->handle,p->incarnation); break;
    }
    for (unsigned i=0; i<GUARD; ++i) {
        if (p->scratch[i] != 0xa5 || out[(size_t)p->capacity+i] != 0xa5) __builtin_trap();
    }
    p->code = code;
    for (unsigned i=0; i<4; ++i) PyBytes_AS_STRING(p->status)[i] = (char)(code >> (8*i));
    for (unsigned i=0; i<4; ++i) PyBytes_AS_STRING(p->version_value)[i] = (char)(p->version >> (8*i));
    if (!code) memcpy(PyBytes_AS_STRING(p->output), out, (size_t)p->capacity);
    p->returned = 1; invoking = 0;
    Py_RETURN_NONE;
}
/* All observations return existing rooted bytes/singletons. Numeric diagnostic
 * conversion may be done later in Python, without losing this opaque record. */
static PyObject *attempted(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; return Py_NewRef(p->attempted?Py_True:Py_False); }
static PyObject *returned(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; return Py_NewRef(p->returned?Py_True:Py_False); }
static PyObject *success(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; return Py_NewRef(p->returned&&!p->code?Py_True:Py_False); }
static PyObject *version_one(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; return Py_NewRef(p->returned&&!p->index&&p->version==1?Py_True:Py_False); }
static PyObject *output(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; if(!p->returned){PyErr_SetString(PyExc_RuntimeError,"no returned output");return NULL;} return Py_NewRef(p->output); }
static PyObject *status(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; if(!p->returned){PyErr_SetString(PyExc_RuntimeError,"no returned status");return NULL;} return Py_NewRef(p->status); }
static PyObject *version_bytes(PyObject *s, PyObject *o) { Attempt *p=get(o); (void)s; if(!p)return NULL; if(!p->returned||p->index){PyErr_SetString(PyExc_RuntimeError,"not a returned ABI version");return NULL;} return Py_NewRef(p->version_value); }
static PyMethodDef methods[] = {
    {"prepare",prepare,METH_VARARGS,"Allocate a genuine one-use context before entry."},
    {"execute",execute,METH_O,"Enter the actual numeric import once; retain its result."},
    {"attempted",attempted,METH_O,"Actual entry witness."}, {"returned",returned,METH_O,"Actual return witness."},
    {"success",success,METH_O,"Nonallocating successful status observation."},
    {"version_one",version_one,METH_O,"Actual optional ABI version observation."},
    {"output",output,METH_O,"Existing owned output root."}, {"status_bytes",status,METH_O,"Existing four-byte errno root."},
    {"version_bytes",version_bytes,METH_O,"Existing four-byte actual ABI version root."},
    {NULL,NULL,0,NULL}
};
static struct PyModuleDef definition = {PyModuleDef_HEAD_INIT,"_roswasm_owned_service",NULL,-1,methods};
PyMODINIT_FUNC PyInit__roswasm_owned_service(void) { return PyModule_Create(&definition); }
#endif
