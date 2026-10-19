/* SOURCE ONLY, NOT BUILT OR RUN.
 * Include the reviewed candidate bridge, with real CPython allocation APIs
 * faulted at two explicit sites and selected ABI callees counted in C.
 * This is a CPython/Wasm bridge contract fixture, not native ROS qualification.
 * Other ROS2 imports remain imports and need the actual Runtime resolver.
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "ros2/abi.h"

static int fixture_fault;
static uint32_t fixture_errno;
static uint64_t fixture_calls, fixture_last_handle, fixture_next_handle = 1;
static int fixture_last_kind;

static PyObject* fixture_output(const char* value, Py_ssize_t size);
static PyObject* fixture_result(const char* format, ...);
static uint32_t fixture_close(uint64_t handle);
static uint32_t fixture_revoke(uint64_t handle);
static uint32_t fixture_open(uint32_t memory, uint64_t alias, uint64_t length, uint64_t output);
static uint32_t fixture_subscribe(uint64_t handle);
static uint32_t fixture_poll(uint64_t handle, uint32_t memory, uint64_t output, uint64_t capacity, uint64_t length);
static uint32_t fixture_version(void);

#define PyBytes_FromStringAndSize fixture_output
#define Py_BuildValue fixture_result
#define uwvm_ros2_capability_close fixture_close
#define uwvm_ros2_capability_revoke fixture_revoke
#define uwvm_ros2_capability_open fixture_open
#define uwvm_ros2_topic_subscribe fixture_subscribe
#define uwvm_ros2_event_poll fixture_poll
#define uwvm_ros2_abi_version fixture_version
#define PyInit__roswasm_native fixture_original_init
#define ROSWASM_BACKEND 2
#include "../roswasm/_native.c"
#undef PyInit__roswasm_native
#undef uwvm_ros2_abi_version
#undef uwvm_ros2_event_poll
#undef uwvm_ros2_topic_subscribe
#undef uwvm_ros2_capability_open
#undef uwvm_ros2_capability_revoke
#undef uwvm_ros2_capability_close
#undef Py_BuildValue
#undef PyBytes_FromStringAndSize

static PyObject* fixture_output(const char* value, Py_ssize_t size) {
    if (fixture_fault == 1) {
        fixture_fault = 0;
        return PyErr_NoMemory();
    }
    return PyBytes_FromStringAndSize(value, size);
}

static PyObject* fixture_result(const char* format, ...) {
    PyObject* result;
    va_list args;
    if (fixture_fault == 2) {
        fixture_fault = 0;
        return PyErr_NoMemory();
    }
    va_start(args, format);
    result = Py_VaBuildValue(format, args);
    va_end(args);
    return result;
}

static uint32_t fixture_close(uint64_t handle) {
    ++fixture_calls; fixture_last_handle = handle; fixture_last_kind = 1;
    return fixture_errno;
}
static uint32_t fixture_revoke(uint64_t handle) {
    ++fixture_calls; fixture_last_handle = handle; fixture_last_kind = 2;
    return fixture_errno;
}
static uint32_t fixture_open(uint32_t memory, uint64_t alias, uint64_t length, uint64_t output) {
    uint64_t value = fixture_next_handle++;
    (void)memory; (void)alias; (void)length;
    memcpy((void*)(uintptr_t)output, &value, sizeof(value));
    return 0;
}
static uint32_t fixture_subscribe(uint64_t handle) { (void)handle; return 0; }
static uint32_t fixture_version(void) { return 1; }
static uint32_t fixture_poll(uint64_t handle, uint32_t memory, uint64_t output, uint64_t capacity, uint64_t length) {
    (void)handle; (void)memory; (void)output; (void)capacity; (void)length;
    return 11;
}

static PyObject* fixture_arm(PyObject* self, PyObject* args) {
    int fault; unsigned int error;
    (void)self;
    if (!PyArg_ParseTuple(args, "iI:fixture_arm", &fault, &error)) return NULL;
    if (fault < 0 || fault > 2) {
        PyErr_SetString(PyExc_ValueError, "fault must be 0, 1 or 2"); return NULL;
    }
    fixture_fault = fault; fixture_errno = error;
    fixture_calls = fixture_last_handle = 0; fixture_last_kind = 0;
    Py_RETURN_NONE;
}
static PyObject* fixture_stats(PyObject* self, PyObject* unused) {
    (void)self; (void)unused;
    return Py_BuildValue("(KKi)", (unsigned long long)fixture_calls,
                         (unsigned long long)fixture_last_handle, fixture_last_kind);
}
static PyMethodDef fixture_methods[] = {
    {"fixture_arm", fixture_arm, METH_VARARGS, "Arm one allocation site and a counted ABI errno."},
    {"fixture_stats", fixture_stats, METH_NOARGS, "C callee count, full u64 handle, close/revoke kind."},
    {NULL, NULL, 0, NULL}
};
PyMODINIT_FUNC PyInit__roswasm_native(void) {
    PyObject* module = fixture_original_init();
    if (module && PyModule_AddFunctions(module, fixture_methods) < 0) {
        Py_DECREF(module); return NULL;
    }
    return module;
}
