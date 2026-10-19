#ifndef UWVM_PYTHON_FRAME_SNAPSHOT_H
#define UWVM_PYTHON_FRAME_SNAPSHOT_H
/* Single attached GIL thread, matching CPython3.14.8 full C API only.
 * Call install after initialization, before the application; uninstall before
 * finalization. Installation refuses an existing trace function. No ROS or
 * Runtime debug Host import is introduced by these functions. */
int uwvm_python_debug_install(void);
int uwvm_python_debug_uninstall(void);
#endif
