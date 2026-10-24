/** Real hosted C90 library/CRT probe; no environment, clock or native ROS use. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <platform/ros/c/roswasm/codec.h>
int main(void) {
    unsigned char wire[16];size_t length=0;char text[32];roswasmc_string decoded;
    strcpy(text,"C stdio profile\n");
    if(strlen(text)!=16||memcmp(text,"C stdio profile\n",16))return 1;
    if(roswasmc_encode_string(roswasmc_string_init("hosted",6),wire,sizeof wire,&length)||length!=10)return 2;
    if(roswasmc_decode_string(wire,length,&decoded)||decoded.size!=6)return 3;
    if(fwrite(text,1,16,stdout)!=16||fflush(stdout))return 4;
    return 0;
}
