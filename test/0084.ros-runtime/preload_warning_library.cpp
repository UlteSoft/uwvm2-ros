/** @file @brief Observable benign constructor used to prove loader refusal ordering. */
#include <unistd.h>
#ifndef SCENE_WARNING_LIBRARY_ID
#error The test runner must identify this independently loaded library.
#endif
__attribute__((constructor)) static void observed_constructor()
{
    constexpr char text[]="CONSTRUCTOR_" SCENE_WARNING_LIBRARY_ID "\n";
    if(::write(STDOUT_FILENO,text,sizeof(text)-1)!=sizeof(text)-1)::_exit(91);
}
