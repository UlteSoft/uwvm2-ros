/* Pure complete-record CPython/WASI command: actual frozen SDK codecs,
 * no built-in ROS registration, bridge object or ROS imports. */
#include <Python.h>
#include "roswasm_frozen.h"

int main(int argc, char **argv) {
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
    config.use_hash_seed = 1;
    config.hash_seed = 0;
    status = PyConfig_SetString(&config, &config.program_name, L"uwvm-python-records");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.home, L"/__roswasm_frozen__");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.filesystem_encoding, L"utf-8");
    if (!PyStatus_Exception(status)) status = PyConfig_SetString(&config, &config.stdio_encoding, L"utf-8");
    if (!PyStatus_Exception(status)) status = PyConfig_SetBytesArgv(&config, argc, argv);
    if (!PyStatus_Exception(status)) status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    if (PyStatus_Exception(status)) Py_ExitStatusException(status);
    int code = 1;
    PyObject *module = PyImport_ImportModule("roswasm_app");
    if (module) {
        PyObject *result = PyObject_CallMethod(module, "main", NULL);
        Py_DECREF(module);
        if (result) {
            if (result == Py_None) code = 0;
            else if (PyLong_CheckExact(result)) {
                long value = PyLong_AsLong(result);
                if (!PyErr_Occurred() && value >= 0 && value <= 255) code = (int)value;
                else if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError, "entry return must be 0..255 or None");
            } else PyErr_SetString(PyExc_TypeError, "entry return must be exact int or None");
            Py_DECREF(result);
        }
    }
    if (PyErr_Occurred()) PyErr_Print();
    if (Py_FinalizeEx() < 0) code = 120;
    return code;
}
