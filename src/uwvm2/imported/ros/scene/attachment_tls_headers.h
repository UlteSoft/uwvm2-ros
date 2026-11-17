/** @file
 * @brief Global-module declarations for the statically selected attachment TLS library.
 * @details Raw public key support requires OpenSSL 3.2 or newer. A build with
 * an older explicit provider refuses encrypted registrations at configuration.
 */
#pragma once
#if defined(__linux__) && defined(UWVM_ROS_ATTACHMENT_OPENSSL)
# include <openssl/opensslv.h>
# include <openssl/opensslconf.h>
# if OPENSSL_VERSION_NUMBER >= 0x30200000L && !defined(OPENSSL_NO_TLS1_3) && !defined(OPENSSL_NO_DANE) && !defined(OPENSSL_NO_RPK) && (OPENSSL_VERSION_NUMBER < 0x30400000L || defined(OPENSSL_NO_SSLKEYLOG))
#  define UWVM_ROS_HAS_ATTACHMENT_TLS
#  include <openssl/ssl.h>
#  include <openssl/err.h>
#  include <openssl/provider.h>
# endif
#endif
