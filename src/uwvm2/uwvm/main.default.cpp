/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @file        main.cc
 * @brief       main function for crt startup
 * @details     This function does nothing but call the cpp boot method of the other module
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-03-20
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/push_macros.h>
// import
# include <uwvm2/uwvm/crtmain/impl.h>
#endif
#if defined(__linux__) && defined(UWVM_WSEA_OPENSSL)
# include <cstdio>
# include <uwvm2/utils/security/trusted_driver.h>
#endif

/// @brief      (crt) main func. The main function is a special function, but there are still meaningless [[gnu::used]].
/// @details    winnt avoid using int main(argc argv), use int main() directly to avoid wasting time parsing and transcoding ACP.
/// @param      argc Argument Count
/// @param      argv Argument Vector
/// @return     exit(3)
/// @see        ::uwvm2::uwvm::uwvm_main()
UWVM_GNU_USED int main(
#if !((defined(_WIN32) && !defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS))  // NOT WINNT
    int argc,
    char** argv
#endif
)
{
#if defined(__linux__) && defined(UWVM_WSEA_OPENSSL)
    if(argc>=2 && ::std::string_view{argv[1]}=="--verify-elf-driver")
    {
        using namespace ::uwvm2::utils::security::trusted_driver;
        if(argc!=5 || (::std::string_view{argv[2]}!="executable" && ::std::string_view{argv[2]}!="shared-library"))
        {
            ::std::fputs("usage: uwvm2-ros --verify-elf-driver executable|shared-library ELF SIGNER.pem\n",stderr);
            return 1;
        }
        if(auto error=process_verifier.select(argv[4],true))
        {
            ::std::fprintf(stderr,"ELF trust policy: %s\n",error);
            return 1;
        }
        image verified;
        refusal why{refusal::policy};
        if(auto error=verified.prepare(argv[3],::std::string_view{argv[2]}=="executable",&why,256u*1024u*1024u))
        {
            ::std::fprintf(stderr,"ELF verification: %s\n",error);
            if(why==refusal::unsigned_elf)return 2;
            if(why==refusal::invalid_signature)return 3;
            return 1;
        }
        return 0;
    }
#endif
#if !((defined(_WIN32) && !defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS))  // NOT WINNT
    return ::uwvm2::uwvm::uwvm_main_non_winnt(argc, argv);
#else
    return ::uwvm2::uwvm::uwvm_main_winnt();
#endif
}

#ifndef UWVM_MODULE
// This cpp may not be the end of the translation unit, it may be included in other cpp files. So it needs to be pop.
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
