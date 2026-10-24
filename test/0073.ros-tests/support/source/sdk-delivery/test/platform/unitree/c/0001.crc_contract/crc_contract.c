/** Mathematical layout contract, not a native Unitree oracle or ROS test.
 * Root may compile this C11 source for each target and run it independently.
 * Official native C++/Python comparison remains a separate actual prerequisite.
 */
#include <platform/unitree/c/roswasm_unitree/crc.h>
#define CHECK(x) do {++checks;if(!(x))return __LINE__;}while(0)
static unsigned checks;
/** Independent xor-first recurrence, byte-order explicit and no table/padding. */
static uint32_t oracle(uint8_t const* bytes,size_t n) {
    uint32_t crc=UINT32_C(0xffffffff);size_t i;unsigned j;
    for(i=0;i<n;i+=4){crc^=(uint32_t)bytes[i]|((uint32_t)bytes[i+1]<<8)|((uint32_t)bytes[i+2]<<16)|((uint32_t)bytes[i+3]<<24);
        for(j=0;j<32;++j)crc=(crc&UINT32_C(0x80000000))?(crc<<1)^UINT32_C(0x04c11db7):crc<<1;}
    return crc;
}
unsigned uwvm_unitree_completed_checks(void){return checks;}
int main(void) {
    uwvm_unitree_go_lowcmd go={0};uwvm_unitree_hg_lowcmd hg={0};
    uint8_t a[UWVM_UNITREE_GO_LOWCMD_BYTES],b[UWVM_UNITREE_HG_LOWCMD_BYTES],saved[UWVM_UNITREE_HG_LOWCMD_BYTES];
    uint8_t words[8]={0x78,0x56,0x34,0x12,0xf0,0xde,0xbc,0x9a};
    union {uint32_t crc;uint8_t bytes[8];} overlap_words={0};
    size_t n=987;uint32_t crc=456,bits;unsigned i;
    CHECK(uwvm_unitree_crc32_words_le(words,sizeof words,&crc)==0&&crc==oracle(words,sizeof words));
    CHECK(uwvm_unitree_crc32_words_le(NULL,0,&crc)==0&&crc==UINT32_C(0xffffffff));
    crc=456;CHECK(uwvm_unitree_crc32_words_le(words,7,&crc)==22&&crc==456);
    CHECK(uwvm_unitree_crc32_words_le(overlap_words.bytes,8,&overlap_words.crc)==22);
    go.head[0]=0xfe;go.head[1]=0xef;go.level_flag=0xff;go.bandwidth=0x1234;
    go.sn[0]=UINT32_C(0x01020304);go.version[1]=UINT32_C(0x11223344);
    go.motor_cmd[0].mode=1;go.motor_cmd[0].q=1.0f;go.motor_cmd[19].kd=-2.0f;
    go.bms_off=7;go.wireless_remote[39]=0x91;go.led[11]=0x92;go.fan[1]=0x93;go.gpio=0x94;
    CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,sizeof a,&n,&crc)==0&&n==812);
    CHECK(a[0]==0xfe&&a[1]==0xef&&a[2]==0xff&&a[3]==0&&a[4]==4&&a[7]==1);
    CHECK(a[16]==0x44&&a[19]==0x11&&a[20]==0x34&&a[21]==0x12&&a[22]==0&&a[23]==0);
    CHECK(a[24]==1&&a[25]==0&&a[26]==0&&a[27]==0&&uwvm_unitree_get32(a+28)==UINT32_C(0x3f800000));
    CHECK(uwvm_unitree_get32(a+24+19*36+20)==UINT32_C(0xc0000000));
    CHECK(a[744]==7&&a[787]==0x91&&a[799]==0x92&&a[801]==0x93&&a[802]==0x94&&a[803]==0);
    CHECK(crc==oracle(a,808)&&uwvm_unitree_get32(a+808)==crc);
    memset(a,0xa5,sizeof a);n=987;crc=456;
    CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,811,&n,&crc)==90&&n==987&&crc==456&&a[0]==0xa5&&a[811]==0xa5);
    go.frame_reserve=1;CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,sizeof a,&n,&crc)==22&&n==987&&crc==456&&a[0]==0xa5);go.frame_reserve=0;
    go.reserve=1;CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,sizeof a,&n,&crc)==22);go.reserve=0;
    go.bms_reserve[2]=1;CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,sizeof a,&n,&crc)==22);go.bms_reserve[2]=0;
    go.motor_cmd[19].reserve[2]=1;CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,sizeof a,&n,&crc)==22);go.motor_cmd[19].reserve[2]=0;
    bits=UINT32_C(0x7fc00001);memcpy(&go.motor_cmd[19].q,&bits,4);CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,a,sizeof a,&n,&crc)==22);go.motor_cmd[19].q=0;
    CHECK(uwvm_unitree_go_lowcmd_pack_crc(&go,&go,812,&n,&crc)==22);
    hg.mode_pr=1;hg.mode_machine=7;hg.motor_cmd[34].mode=2;hg.motor_cmd[34].tau=-3.0f;
    CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,b,sizeof b,&n,&crc)==0&&n==1004);
    CHECK(b[0]==1&&b[1]==7&&b[2]==0&&b[3]==0&&b[4+34*28]==2);
    CHECK(uwvm_unitree_get32(b+4+34*28+12)==UINT32_C(0xc0400000));
    for(i=984;i<1000;++i)if(b[i])return __LINE__;
    CHECK(crc==oracle(b,1000)&&uwvm_unitree_get32(b+1000)==crc);
    memcpy(saved,b,sizeof b);n=987;crc=456;
    for(i=0;i<4;++i){hg.reserve[i]=1;if(uwvm_unitree_hg_lowcmd_pack_crc(&hg,b,sizeof b,&n,&crc)!=22||memcmp(saved,b,sizeof b)||n!=987||crc!=456)return __LINE__;hg.reserve[i]=0;}
    CHECK(n==987&&crc==456); /* all four words, including the upstream discrepancy */
    hg.motor_cmd[34].reserve=1;CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,b,sizeof b,&n,&crc)==22);hg.motor_cmd[34].reserve=0;
    bits=UINT32_C(0x7f800000);memcpy(&hg.motor_cmd[0].kd,&bits,4);CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,b,sizeof b,&n,&crc)==22);hg.motor_cmd[0].kd=0;
    CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,b,1003,&n,&crc)==90&&memcmp(saved,b,sizeof b)==0);
    CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,&n,1004,&n,&crc)==22);
    CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,&crc,1004,&n,&crc)==22);
    CHECK(uwvm_unitree_hg_lowcmd_pack_crc(&hg,b,sizeof b,&n,&crc)==0);
    return 0;
}
