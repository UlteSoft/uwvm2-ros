/** @file
 * @brief Static CPython/WASI entry with an SDK-owned built-in ROS extension.
 *
 * Compile with the matching target pyconfig.h, CPython Include directory,
 * generated roswasm_frozen.h, _native.c object and real target libpython.a.
 * Register before interpreter initialization; no dynamic module loading, host
 * Python execution, directory preopen or implicit authority is required by this
 * launcher. Its actual module/builtin closure must be tested on the producer.
 */
#include <Python.h>
#include "roswasm_frozen.h"
#ifndef UWVM_DEBUG_LANGUAGE_PYTHON
# ifdef UWVM_PYTHON_DEBUG_SNAPSHOT
#  define UWVM_DEBUG_LANGUAGE_PYTHON UWVM_PYTHON_DEBUG_SNAPSHOT
# else
#  define UWVM_DEBUG_LANGUAGE_PYTHON 0
# endif
#endif
#ifndef UWVM_PYTHON_DEBUG_SNAPSHOT
#define UWVM_PYTHON_DEBUG_SNAPSHOT UWVM_DEBUG_LANGUAGE_PYTHON
#endif
#if UWVM_DEBUG_LANGUAGE_PYTHON != 0 && UWVM_DEBUG_LANGUAGE_PYTHON != 1
#error "UWVM_DEBUG_LANGUAGE_PYTHON must be exactly 0 or 1"
#endif
#if UWVM_PYTHON_DEBUG_SNAPSHOT != UWVM_DEBUG_LANGUAGE_PYTHON
#error "Python debugger build switches disagree"
#endif
#if UWVM_PYTHON_DEBUG_SNAPSHOT != 0 && UWVM_PYTHON_DEBUG_SNAPSHOT != 1
#error "UWVM_PYTHON_DEBUG_SNAPSHOT must be exactly 0 or 1"
#endif
#if UWVM_PYTHON_DEBUG_SNAPSHOT
#include "../debug/frame_snapshot.h"
#endif
PyMODINIT_FUNC PyInit__roswasm_native(void);
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
PyMODINIT_FUNC PyInit__roswasm_owned_service(void);
#endif

int main(int argc, char** argv) {
    if (PyImport_AppendInittab("_roswasm_native", PyInit__roswasm_native) < 0)
        return 1;
#if ROSWASM_OWNED_SERVICE
    if (PyImport_AppendInittab("_roswasm_owned_service", PyInit__roswasm_owned_service) < 0)
        return 1;
#endif
    PyImport_FrozenModules = roswasm_frozen;
    PyPreConfig pre;
    PyPreConfig_InitIsolatedConfig(&pre);
    pre.utf8_mode = 1;
    pre.configure_locale = 0;
    PyStatus status = Py_PreInitialize(&pre);
    if (PyStatus_Exception(status)) Py_ExitStatusException(status);
    PyConfig config;
    PyConfig_InitIsolatedConfig(&config);
    config.site_import = config.user_site_directory = config.use_environment = 0;
    config.parse_argv = config.write_bytecode = config.install_signal_handlers = 0;
    config.use_frozen_modules = config.module_search_paths_set = 1;
    config.pathconfig_warnings = config.optimization_level = 0;
    /* A deterministic, explicit interpreter hash seed avoids an ambient random
     * capability. This does not implement cryptography or signature checking. */
    config.use_hash_seed = 1;
    config.hash_seed = 0;
    status = PyConfig_SetString(&config, &config.program_name, L"roswasm-python");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.home, L"/__roswasm_frozen__");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.filesystem_encoding, L"utf-8");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.stdio_encoding, L"utf-8");
    if (!PyStatus_Exception(status)) status = PyConfig_SetBytesArgv(&config, argc, argv);
    if (!PyStatus_Exception(status)) status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    if (PyStatus_Exception(status)) Py_ExitStatusException(status);
    int code = 1;
#if UWVM_PYTHON_DEBUG_SNAPSHOT
    if (uwvm_python_debug_install() < 0) {
        PyErr_Print();
        (void)Py_FinalizeEx();
        return 1;
    }
#endif
    PyObject* module = PyImport_ImportModule("roswasm_app");
    if (module) {
        PyObject* result = PyObject_CallMethod(module, "main", NULL);
        Py_DECREF(module);
        if (result) {
            if (result == Py_None) code = 0;
            else {
                long value = PyLong_AsLong(result);
                if (!PyErr_Occurred() && value >= 0 && value <= 255) code = (int)value;
                else if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError, "entry return must be 0..255 or None");
            }
            Py_DECREF(result);
        }
    }
    if (PyErr_Occurred()) PyErr_Print();
#if UWVM_PYTHON_DEBUG_SNAPSHOT
    if (uwvm_python_debug_uninstall() < 0) {
        PyErr_Print();
        if (code == 0) code = 1;
    }
#endif
    if (Py_FinalizeEx() < 0) code = 120;
    return code;
}
