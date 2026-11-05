/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
// Bind the selected product ABI to the actual compiler target. Architecture
// aliases or a bootstrap compiler must not silently produce another product.
#if defined(UWVM_ROS_PRODUCT_ABI_X86_64) && (!defined(__x86_64__) || __SIZEOF_POINTER__ != 8)
# error "The selected x86_64 ROS product requires the x86_64 LP64 compiler target."
#endif
#if defined(UWVM_ROS_PRODUCT_ABI_I686) && (!defined(__i386__) || __SIZEOF_POINTER__ != 4)
# error "The selected i686 ROS product requires the i686 ILP32 compiler target."
#endif
#if defined(UWVM_ROS_PRODUCT_ABI_AARCH64) && (!defined(__aarch64__) || !defined(__AARCH64EL__) || __SIZEOF_POINTER__ != 8)
# error "The selected AArch64 ROS product requires the little-endian AArch64 LP64 compiler target."
#endif
#if defined(UWVM_ROS_PRODUCT_ABI_ARM) || defined(UWVM_ROS_PRODUCT_ABI_ARMHF)
# if !defined(__arm__) || !defined(__ARMEL__) || !defined(__ARM_EABI__) || !defined(__ARM_ARCH) || __ARM_ARCH < 7 || __SIZEOF_POINTER__ != 4
#  error "The selected ARM ROS product requires little-endian ARMv7 EABI ILP32."
# endif
# if defined(UWVM_ROS_PRODUCT_ABI_ARM) && defined(__ARM_PCS_VFP)
#  error "The selected ARM soft-float product cannot use the VFP argument ABI."
# endif
# if defined(UWVM_ROS_PRODUCT_ABI_ARMHF) && !defined(__ARM_PCS_VFP)
#  error "The selected ARM hard-float product requires the VFP argument ABI."
# endif
#endif
