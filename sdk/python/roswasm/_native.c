/** @file
 * @brief CPython built-in bridge to one selected ROS ABI v1 backend.
 *
 * Generate _native.c with sdk/tools/generate_managed_raw.py. Compile this file
 * into CPython/WASI, not into the host build-Python. ROSWASM_BACKEND selects 1
 * or 2 at compile time. The resulting interpreter imports that backend only.
 * Py_buffer pins the input for the entire synchronous import; an owned Python
 * bytes object retains output storage. No Python object header, GC reference,
 * native host pointer, callback or arbitrary address crosses the ABI.
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <string.h>
#ifndef ROSWASM_BACKEND
#define ROSWASM_BACKEND 2
#endif
#if ROSWASM_BACKEND == 1
#include "ros1/abi.h"
#elif ROSWASM_BACKEND == 2
#include "ros2/abi.h"
#else
#error "ROSWASM_BACKEND must be 1 or 2"
#endif
#if !defined(__wasm__) && !defined(ROSWASM_NATIVE_BRIDGE_CONTRACT_TEST)
#error "Build the ROS Python bridge for WebAssembly; native builds confer no ROS support"
#endif
#define ROSWASM_LIMIT 4194304
#define ROSWASM_INPUT_LIMIT 4193786
#define ROSWASM_OFFSET(p) ((uint64_t)(uintptr_t)(p))
static int roswasm_cleanup_known;
static uint32_t roswasm_cleanup_error;
#ifndef ROSWASM_OWNED_SERVICE
#define ROSWASM_OWNED_SERVICE 0
#endif
#if ROSWASM_OWNED_SERVICE != 0 && ROSWASM_OWNED_SERVICE != 1
#error "ROSWASM_OWNED_SERVICE must be exactly 0 or 1"
#endif
#if ROSWASM_OWNED_SERVICE
#include "_open_witness.h"
#endif

/* Called only after exact-list/False validation, immediately before import.
 * No Python callback, allocation or fallible Python API occurs in this step. */
static void roswasm_mark_retirement(PyObject* witness) {
    if (witness) {
        Py_INCREF(Py_True);
        PyList_SET_ITEM(witness, 0, Py_True);
        Py_DECREF(Py_False);
    }
}

/** Invoke exactly once, retaining every buffer until the host has returned.
 * Arguments are name, handle, contiguous input, output capacity, other handle,
 * correlation. Return (errno, owned output, conditional required size, value).
 * value is used by version/open/submit. A length sentinel is never interpreted
 * as a capacity. No failure exposes partially written output or replays I/O.
 */
static PyObject* roswasm_invoke(PyObject* self, PyObject* args) {
    char const* name;
    PyObject *handle_object, *input, *other_object, *correlation_object;
    PyObject *retirement_witness = NULL;
#if ROSWASM_OWNED_SERVICE
    PyObject *open_witness_object = NULL;
    roswasm_open_witness *open_witness = NULL;
#endif
    PyObject *output = NULL, *required = NULL, *value = NULL, *result = NULL;
    Py_buffer request = {0};
    Py_ssize_t capacity;
    uint64_t handle, other, correlation, slot = UINT64_MAX;
    uint32_t code = 0;
    int found = 0, output_kind = 0, owns_open_handle = 0;
    (void)self;
    roswasm_cleanup_known = 0;
#if ROSWASM_OWNED_SERVICE
    if (!PyArg_ParseTuple(args, "sOOnOO|OO:invoke", &name, &handle_object, &input,
            &capacity, &other_object, &correlation_object, &retirement_witness,
            &open_witness_object)) return NULL;
#else
    if (!PyArg_ParseTuple(args, "sOOnOO|O:invoke", &name, &handle_object, &input,
            &capacity, &other_object, &correlation_object, &retirement_witness)) return NULL;
#endif
    /* Unlike the K parse format, these conversions reject negative/overflow. */
    handle = PyLong_AsUnsignedLongLong(handle_object);
    if (PyErr_Occurred()) return NULL;
    other = PyLong_AsUnsignedLongLong(other_object);
    if (PyErr_Occurred()) return NULL;
    correlation = PyLong_AsUnsignedLongLong(correlation_object);
    if (PyErr_Occurred()) return NULL;
    if (capacity < 0 || capacity > ROSWASM_LIMIT) {
        PyErr_SetString(PyExc_ValueError, "output capacity must be 0..4194304");
        return NULL;
    }
#if ROSWASM_OWNED_SERVICE
    if (open_witness_object && open_witness_object != Py_None) {
        if (retirement_witness != Py_None || strcmp(name, "capability_open") != 0 ||
            handle || other || correlation || capacity != 0) {
            PyErr_SetString(PyExc_ValueError, "canonical tracked-open form required"); return NULL;
        }
        open_witness = roswasm_open_get(open_witness_object);
        if (!open_witness) return NULL;
        if (open_witness->entered || open_witness->input != input ||
            !PyList_CheckExact(open_witness->retirement) ||
            PyList_GET_SIZE(open_witness->retirement) != 1 ||
            PyList_GET_ITEM(open_witness->retirement, 0) != Py_False) {
            PyErr_SetString(PyExc_ValueError, "one-use exact reserved alias root required"); return NULL;
        }
        retirement_witness = NULL;
    }
#endif
    if (retirement_witness &&
            (!PyList_CheckExact(retirement_witness) ||
             PyList_GET_SIZE(retirement_witness) != 1 ||
             PyList_GET_ITEM(retirement_witness, 0) != Py_False ||
             (strcmp(name, "capability_close") != 0 && strcmp(name, "capability_revoke") != 0) ||
             !handle || capacity != 0 || other != 0 || correlation != 0 ||
             !PyBytes_CheckExact(input) || PyBytes_GET_SIZE(input) != 0)) {
        PyErr_SetString(PyExc_ValueError, "invalid unattempted retirement witness/form");
        return NULL;
    }
    if (PyObject_GetBuffer(input, &request, PyBUF_CONTIG_RO) < 0) return NULL;
    if (request.len < 0 || request.len > ROSWASM_INPUT_LIMIT) {
        PyErr_SetString(PyExc_ValueError, "input exceeds the ROS request bound");
        goto done;
    }
    output = PyBytes_FromStringAndSize(NULL, capacity);
    if (!output) goto done;
    /* Generated dispatch uses actual i32 memory index and i64 offsets/lengths.
     * output_kind: 0=no output, 1=length output, 2=value slot, 3=ready32. */
#if ROSWASM_OWNED_SERVICE
    if (open_witness) {
        found = 1; output_kind = 2; slot = 0;
        open_witness->entered = 1;
        code = uwvm_ros2_capability_open(0, ROSWASM_OFFSET(request.buf),
                (uint64_t)request.len, ROSWASM_OFFSET(&slot));
        open_witness->code = code; open_witness->handle = slot;
        open_witness->returned = 1; owns_open_handle = code == 0;
    } else {
#endif
#if ROSWASM_BACKEND == 1
    if (strcmp(name, "abi_version") == 0) {
        found = 1;
        slot = uwvm_ros1_abi_version(); output_kind = 4;
    }
    else if (strcmp(name, "capability_open") == 0) {
        found = 1;
        if (request.len == 0 || request.len > 255) { PyErr_SetString(PyExc_ValueError, "alias length must be 1..255 UTF-8 bytes"); goto done; }
        slot = 0;
        code = uwvm_ros1_capability_open(0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(&slot));
        owns_open_handle = 1;
        output_kind = 2;
    }
    else if (strcmp(name, "capability_close") == 0) {
        found = 1;
        roswasm_mark_retirement(retirement_witness);
        code = uwvm_ros1_capability_close(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "capability_revoke") == 0) {
        found = 1;
        roswasm_mark_retirement(retirement_witness);
        code = uwvm_ros1_capability_revoke(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "topic_publish") == 0) {
        found = 1;
        code = uwvm_ros1_topic_publish(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len);
        output_kind = 0;
    }
    else if (strcmp(name, "topic_subscribe") == 0) {
        found = 1;
        code = uwvm_ros1_topic_subscribe(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "event_poll") == 0) {
        found = 1;
        code = uwvm_ros1_event_poll(handle, 0, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "event_watch") == 0) {
        found = 1;
        code = uwvm_ros1_event_watch(handle, other, correlation);
        output_kind = 0;
    }
    else if (strcmp(name, "event_poll_v1") == 0) {
        found = 1;
        code = uwvm_ros1_event_poll_v1(handle, 0, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "event_ready") == 0) {
        found = 1;
        if (capacity != 32) { PyErr_SetString(PyExc_ValueError, "event_ready requires capacity32"); goto done; }
        code = uwvm_ros1_event_ready(handle, 0, ROSWASM_OFFSET(PyBytes_AS_STRING(output)));
        output_kind = 3;
    }
    else if (strcmp(name, "service_submit") == 0) {
        found = 1;
        slot = 0;
        code = uwvm_ros1_service_submit(handle, other, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(&slot));
        output_kind = 2;
    }
    else if (strcmp(name, "service_call") == 0) {
        found = 1;
        code = uwvm_ros1_service_call(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "service_advertise") == 0) {
        found = 1;
        code = uwvm_ros1_service_advertise(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "service_respond") == 0) {
        found = 1;
        code = uwvm_ros1_service_respond(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_send_goal") == 0) {
        found = 1;
        code = uwvm_ros1_action_send_goal(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_cancel_own_goal") == 0) {
        found = 1;
        code = uwvm_ros1_action_cancel_own_goal(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_cancel_any_goal") == 0) {
        found = 1;
        code = uwvm_ros1_action_cancel_any_goal(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_receive_feedback") == 0) {
        found = 1;
        code = uwvm_ros1_action_receive_feedback(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_receive_result") == 0) {
        found = 1;
        code = uwvm_ros1_action_receive_result(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_advertise") == 0) {
        found = 1;
        code = uwvm_ros1_action_advertise(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_respond") == 0) {
        found = 1;
        code = uwvm_ros1_action_respond(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "tf_lookup") == 0) {
        found = 1;
        code = uwvm_ros1_tf_lookup(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "tf_broadcast") == 0) {
        found = 1;
        code = uwvm_ros1_tf_broadcast(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_get") == 0) {
        found = 1;
        code = uwvm_ros1_parameter_get(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_set") == 0) {
        found = 1;
        code = uwvm_ros1_parameter_set(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_delete") == 0) {
        found = 1;
        code = uwvm_ros1_parameter_delete(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_exists") == 0) {
        found = 1;
        code = uwvm_ros1_parameter_exists(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_list") == 0) {
        found = 1;
        code = uwvm_ros1_parameter_list(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_search") == 0) {
        found = 1;
        code = uwvm_ros1_parameter_search(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_topic") == 0) {
        found = 1;
        code = uwvm_ros1_graph_inspect_topic(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_node") == 0) {
        found = 1;
        code = uwvm_ros1_graph_inspect_node(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_service") == 0) {
        found = 1;
        code = uwvm_ros1_graph_inspect_service(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_list_topics") == 0) {
        found = 1;
        code = uwvm_ros1_graph_list_topics(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_list_nodes") == 0) {
        found = 1;
        code = uwvm_ros1_graph_list_nodes(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_list_services") == 0) {
        found = 1;
        code = uwvm_ros1_graph_list_services(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "time_read_ros") == 0) {
        found = 1;
        code = uwvm_ros1_time_read_ros(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "time_read_wall") == 0) {
        found = 1;
        code = uwvm_ros1_time_read_wall(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "log_write") == 0) {
        found = 1;
        code = uwvm_ros1_log_write(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len);
        output_kind = 0;
    }
#elif ROSWASM_BACKEND == 2
    if (strcmp(name, "abi_version") == 0) {
        found = 1;
        slot = uwvm_ros2_abi_version(); output_kind = 4;
    }
    else if (strcmp(name, "capability_open") == 0) {
        found = 1;
        if (request.len == 0 || request.len > 255) { PyErr_SetString(PyExc_ValueError, "alias length must be 1..255 UTF-8 bytes"); goto done; }
        slot = 0;
        code = uwvm_ros2_capability_open(0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(&slot));
        owns_open_handle = 1;
        output_kind = 2;
    }
    else if (strcmp(name, "capability_close") == 0) {
        found = 1;
        roswasm_mark_retirement(retirement_witness);
        code = uwvm_ros2_capability_close(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "capability_revoke") == 0) {
        found = 1;
        roswasm_mark_retirement(retirement_witness);
        code = uwvm_ros2_capability_revoke(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "topic_publish") == 0) {
        found = 1;
        code = uwvm_ros2_topic_publish(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len);
        output_kind = 0;
    }
    else if (strcmp(name, "topic_subscribe") == 0) {
        found = 1;
        code = uwvm_ros2_topic_subscribe(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "event_poll") == 0) {
        found = 1;
        code = uwvm_ros2_event_poll(handle, 0, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "event_watch") == 0) {
        found = 1;
        code = uwvm_ros2_event_watch(handle, other, correlation);
        output_kind = 0;
    }
    else if (strcmp(name, "event_poll_v1") == 0) {
        found = 1;
        code = uwvm_ros2_event_poll_v1(handle, 0, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "event_ready") == 0) {
        found = 1;
        if (capacity != 32) { PyErr_SetString(PyExc_ValueError, "event_ready requires capacity32"); goto done; }
        code = uwvm_ros2_event_ready(handle, 0, ROSWASM_OFFSET(PyBytes_AS_STRING(output)));
        output_kind = 3;
    }
    else if (strcmp(name, "service_submit") == 0) {
        found = 1;
        slot = 0;
        code = uwvm_ros2_service_submit(handle, other, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(&slot));
        output_kind = 2;
    }
    else if (strcmp(name, "service_call") == 0) {
        found = 1;
        code = uwvm_ros2_service_call(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "service_advertise") == 0) {
        found = 1;
        code = uwvm_ros2_service_advertise(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "service_respond") == 0) {
        found = 1;
        code = uwvm_ros2_service_respond(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_send_goal") == 0) {
        found = 1;
        code = uwvm_ros2_action_send_goal(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_cancel_own_goal") == 0) {
        found = 1;
        code = uwvm_ros2_action_cancel_own_goal(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_cancel_any_goal") == 0) {
        found = 1;
        code = uwvm_ros2_action_cancel_any_goal(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_receive_feedback") == 0) {
        found = 1;
        code = uwvm_ros2_action_receive_feedback(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_receive_result") == 0) {
        found = 1;
        code = uwvm_ros2_action_receive_result(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_advertise") == 0) {
        found = 1;
        code = uwvm_ros2_action_advertise(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "action_respond") == 0) {
        found = 1;
        code = uwvm_ros2_action_respond(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "tf_lookup") == 0) {
        found = 1;
        code = uwvm_ros2_tf_lookup(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "tf_broadcast") == 0) {
        found = 1;
        code = uwvm_ros2_tf_broadcast(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_get") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_get(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_set") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_set(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_list") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_list(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_describe") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_describe(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_declare") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_declare(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_undeclare") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_undeclare(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_topic") == 0) {
        found = 1;
        code = uwvm_ros2_graph_inspect_topic(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_node") == 0) {
        found = 1;
        code = uwvm_ros2_graph_inspect_node(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_service") == 0) {
        found = 1;
        code = uwvm_ros2_graph_inspect_service(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_inspect_action") == 0) {
        found = 1;
        code = uwvm_ros2_graph_inspect_action(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "parameter_subscribe_events") == 0) {
        found = 1;
        code = uwvm_ros2_parameter_subscribe_events(handle);
        output_kind = 0;
    }
    else if (strcmp(name, "graph_list_topics") == 0) {
        found = 1;
        code = uwvm_ros2_graph_list_topics(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_list_nodes") == 0) {
        found = 1;
        code = uwvm_ros2_graph_list_nodes(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "graph_list_services") == 0) {
        found = 1;
        code = uwvm_ros2_graph_list_services(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "lifecycle_get_state") == 0) {
        found = 1;
        code = uwvm_ros2_lifecycle_get_state(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "lifecycle_change_state") == 0) {
        found = 1;
        code = uwvm_ros2_lifecycle_change_state(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "time_read_ros") == 0) {
        found = 1;
        code = uwvm_ros2_time_read_ros(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "time_read_system") == 0) {
        found = 1;
        code = uwvm_ros2_time_read_system(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "time_read_steady") == 0) {
        found = 1;
        code = uwvm_ros2_time_read_steady(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len, ROSWASM_OFFSET(PyBytes_AS_STRING(output)), (uint64_t)capacity, ROSWASM_OFFSET(&slot));
        output_kind = 1;
    }
    else if (strcmp(name, "log_write") == 0) {
        found = 1;
        code = uwvm_ros2_log_write(handle, 0, ROSWASM_OFFSET(request.buf), (uint64_t)request.len);
        output_kind = 0;
    }
#endif
#if ROSWASM_OWNED_SERVICE
    }
#endif
    if (!found) {
        PyErr_SetString(PyExc_KeyError, "operation is absent from this ROS backend");
        goto done;
    }
    required = Py_NewRef(Py_None);
    value = Py_NewRef(Py_None);
    if (output_kind == 1) {
        if (code == 0) {
            if (slot == UINT64_MAX || slot > (uint64_t)capacity) {
                PyErr_SetString(PyExc_RuntimeError, "host returned an invalid output length");
                goto done;
            }
            if (_PyBytes_Resize(&output, (Py_ssize_t)slot) < 0) goto done;
        } else if (code == 90 && slot != UINT64_MAX) {
            Py_SETREF(required, PyLong_FromUnsignedLongLong(slot));
            if (!required) goto done;
        }
    } else if ((output_kind == 2 || output_kind == 4) && code == 0) {
        if (output_kind == 2 && slot == 0) {
            PyErr_SetString(PyExc_RuntimeError, "host left its successful value slot unwritten");
            goto done;
        }
        Py_SETREF(value, PyLong_FromUnsignedLongLong(slot));
        if (!value) goto done;
    }
    if (code != 0 || (output_kind != 1 && output_kind != 3)) {
        if (_PyBytes_Resize(&output, 0) < 0) goto done;
    }
    result = Py_BuildValue("IOOO", code, output, required, value);
#if ROSWASM_OWNED_SERVICE
    if (result && open_witness) open_witness->delivered = 1;
#endif
done:
    /* If constructing the Python result failed after a successful open, close
     * the real handle once while its numeric value is still available in C.
     * Preserve the primary Python exception. An admitted service ID is not a
     * handle and is never sent to capability_close. */
    if (!result && owns_open_handle && code == 0 && slot != 0) {
        PyObject* primary = PyErr_GetRaisedException();
        PyObject* diagnostic;
#if ROSWASM_OWNED_SERVICE
        if (open_witness) {
            roswasm_open_primary(open_witness, primary);
            roswasm_cleanup_error = roswasm_open_cleanup(open_witness);
        } else
#endif
        {
#if ROSWASM_BACKEND == 1
        roswasm_cleanup_error = uwvm_ros1_capability_close(slot);
#else
        roswasm_cleanup_error = uwvm_ros2_capability_close(slot);
#endif
        }
        roswasm_cleanup_known = 1;
        diagnostic = PyLong_FromUnsignedLong(roswasm_cleanup_error);
        if (diagnostic) {
            if (primary && PyObject_SetAttrString(primary, "roswasm_cleanup_error", diagnostic) < 0)
                PyErr_Clear();
            Py_DECREF(diagnostic);
        } else PyErr_Clear();
        PyErr_SetRaisedException(primary);
    }
    PyBuffer_Release(&request);
    Py_XDECREF(output);
    Py_XDECREF(required);
    Py_XDECREF(value);
    return result;
}

/** OOM-safe cleanup's latest numeric diagnostic, retained until next invoke. */
static PyObject* roswasm_last_cleanup(PyObject* self, PyObject* unused) {
    (void)self; (void)unused;
    if (!roswasm_cleanup_known) Py_RETURN_NONE;
    return PyLong_FromUnsignedLong(roswasm_cleanup_error);
}

/** Only a copied-buffer invocation bridge is exposed; no memory-address API. */
static PyMethodDef roswasm_methods[] = {
#if ROSWASM_OWNED_SERVICE
    ROSWASM_OPEN_METHODS
#endif
    {"invoke", roswasm_invoke, METH_VARARGS,
     "Invoke one ROS import once using owned output and pinned input storage."},
    {"last_open_cleanup", roswasm_last_cleanup, METH_NOARGS,
     "Return cleanup errno after a failed open-result allocation, otherwise None."},
    {NULL, NULL, 0, NULL}
};
static struct PyModuleDef roswasm_module = {
    PyModuleDef_HEAD_INIT, "_roswasm_native",
    "Built-in CPython/WASI ROS v1 bridge. Creates no permissions.",
    -1, roswasm_methods, NULL, NULL, NULL, NULL
};
/** Register through Modules/Setup.local in the actual target CPython build. */
PyMODINIT_FUNC PyInit__roswasm_native(void) {
#if ROSWASM_OWNED_SERVICE
    if (roswasm_open_ready() < 0) return NULL;
#endif
    PyObject* module = PyModule_Create(&roswasm_module);
    if (module && PyModule_AddIntConstant(module, "backend", ROSWASM_BACKEND) < 0) {
        Py_DECREF(module); return NULL;
    }
    return module;
}
