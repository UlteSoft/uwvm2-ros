/* Actual frozen CPython entry without SDK/ROS registration or ROS imports.
 * Link only this entry, frame_snapshot.c, the matching libpython.a and the
 * actual target closure. debug_frozen.h must come from matching CPython3.14.8.
 * Unexecuted development Source, distinct from the ROS embedding entry. */
#include <Python.h>
#include "debug_frozen.h"
#include "frame_snapshot.h"

int main(int argc, char **argv) {
    PyImport_FrozenModules = debug_frozen;
    PyPreConfig pre; PyPreConfig_InitIsolatedConfig(&pre);
    pre.utf8_mode = 1; pre.configure_locale = 0;
    PyStatus status = Py_PreInitialize(&pre);
    if (PyStatus_Exception(status)) Py_ExitStatusException(status);
    PyConfig config; PyConfig_InitIsolatedConfig(&config);
    config.site_import = config.user_site_directory = config.use_environment = 0;
    config.parse_argv = config.write_bytecode = config.install_signal_handlers = 0;
    config.use_frozen_modules = config.module_search_paths_set = 1;
    config.pathconfig_warnings = config.optimization_level = 0;
    config.use_hash_seed = 1; config.hash_seed = 0;
    status = PyConfig_SetString(&config, &config.program_name, L"uwvm-python-debug");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.home, L"/__uwvm_debug_frozen__");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.filesystem_encoding, L"utf-8");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.stdio_encoding, L"utf-8");
    if (!PyStatus_Exception(status)) status = PyConfig_SetBytesArgv(&config, argc, argv);
    if (!PyStatus_Exception(status)) status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    if (PyStatus_Exception(status)) Py_ExitStatusException(status);
    int code = 1;
    if (uwvm_python_debug_install() == 0) {
        PyObject *module = PyImport_ImportModule("debug_app");
        if (module) {
            PyObject *result = PyObject_CallMethod(module, "main", NULL);
            Py_DECREF(module);
            if (result) {
                if (result == Py_None) code = 0;
                else if (PyLong_CheckExact(result)) {
                    long n = PyLong_AsLong(result);
                    if (!PyErr_Occurred() && n >= 0 && n <= 255) code = (int)n;
                    else if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError, "entry return must be 0..255 or None");
                } else PyErr_SetString(PyExc_TypeError, "entry return must be int or None");
                Py_DECREF(result);
            }
        }
        if (PyErr_Occurred()) PyErr_Print();
        if (uwvm_python_debug_uninstall() < 0) { PyErr_Print(); if (!code) code = 1; }
    } else if (PyErr_Occurred()) PyErr_Print();
    if (Py_FinalizeEx() < 0) code = 120;
    return code;
}
