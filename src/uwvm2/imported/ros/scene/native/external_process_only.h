/** @file
 * @brief Refuse native ROS implementations in a VM translation unit.
 * @details Native middleware and vendor-facing adapters are compiled into
 * independently packaged worker executables/libraries. The VM imports only
 * common message contracts, authority and supervision. Its build macros must
 * never accompany these headers, even when deprecated ROS or preload support
 * is explicitly enabled. A platform permission declaration cannot opt out.
 *
 * This compile-time boundary prevents accidental dependency growth and native
 * callbacks in the VM. It is not process confinement, binary attestation or a
 * substitute for the worker's namespace, syscall, resource and lifecycle policy.
 */
#pragma once
#if defined(UWVM) || defined(UWVM_IMPORT_ROSV2) || defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#error "ROSv2 native implementations belong in an isolated external worker, never in the VM."
#endif
