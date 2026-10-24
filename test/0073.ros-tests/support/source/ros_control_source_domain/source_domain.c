/* Authored contract SOURCE. Run only after the real admitted generator produces
 * bounded_records.h from the complete control catalog. No native ROS stand-ins.
 * Static storage here makes the large cartesian C layout explicit; the production
 * decoder's complete private automatic temporary still needs an actual stack budget. */
#include "bounded_records.h"
#include <assert.h>
#include <string.h>
#include <stdint.h>
typedef roswasmc_bounded_2_type_description_interfaces_msg_TypeSource Source;
typedef roswasmc_bounded_2_type_description_interfaces_msg_TypeDescription Description;
typedef roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response Response;
#define S roswasmc_bounded_2_type_description_interfaces_msg_TypeSource
#define R roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response
#define CAT_(a,b) a##b
#define CAT(a,b) CAT_(a,b)
#define SF(s) CAT(S,s)
#define RF(s) CAT(R,s)
_Alignas(size_t) static Source source,source_out,source_before;
static Response response,response_out,response_before;
_Alignas(size_t) static unsigned char wire[80000],saved[80000];
static void reset(void){memset(&source,0,sizeof source);memset(&response,0,sizeof response);memset(wire,0xa5,sizeof wire);memcpy(saved,wire,sizeof wire);}
static void raw_counted_bytes(void){
  static unsigned char const raw[]={0,0xff,0xc0,0x80,0xe2,0x82,0};size_t n=999;
  reset();source.raw_file_contents.size=sizeof raw;memcpy(source.raw_file_contents.data,raw,sizeof raw);
  assert(SF(_measure)(&source,&n)==0&&n==12+sizeof raw);
  assert(SF(_encode)(&source,wire,sizeof wire,&n)==0);
  assert(SF(_decode)(wire,n,&source_out)==0);
  assert(source_out.raw_file_contents.size==sizeof raw&&!memcmp(source_out.raw_file_contents.data,raw,sizeof raw));
}
static void text_domains_still_strict(void){
  size_t n=999;reset();source.type_name.size=1;source.type_name.data[0]=0;
  assert(SF(_encode)(&source,wire,sizeof wire,&n)==22&&n==999&&!memcmp(wire,saved,sizeof wire));
  source.type_name.size=0;source.encoding.size=2;source.encoding.data[0]=0xc0;source.encoding.data[1]=0x80;
  assert(SF(_encode)(&source,wire,sizeof wire,&n)==22&&n==999&&!memcmp(wire,saved,sizeof wire));
}
static void raw_boundary(void){
  size_t n=777;reset();source.raw_file_contents.size=6162;memset(source.raw_file_contents.data,0xff,6162);source.raw_file_contents.data[6161]=0;
  assert(SF(_encode)(&source,wire,sizeof wire,&n)==0&&n==6174);
  source.raw_file_contents.size=6163;n=777;memcpy(saved,wire,sizeof wire);
  assert(SF(_encode)(&source,wire,sizeof wire,&n)==22&&n==777&&!memcmp(wire,saved,sizeof wire));
}
static void complete_capacities(void){
  size_t n=777;reset();response.type_description.type_description.fields.size=10;
  response.type_description.referenced_type_descriptions.size=11;response.type_sources.size=12;
  /* Every active nested record is independently valid; all original scalar defaults remain zero here. */
  assert(RF(_measure)(&response,&n)==0&&n<32768);
  assert(RF(_encode)(&response,wire,sizeof wire,&n)==0&&RF(_decode)(wire,n,&response_out)==0);
  assert(response_out.type_description.type_description.fields.size==10&&response_out.type_description.referenced_type_descriptions.size==11&&response_out.type_sources.size==12);
}
static void aggregate_exact_and_over(void){
  size_t n=777,base=0;unsigned i;reset();response.type_sources.size=6;
  assert(RF(_measure)(&response,&base)==0&&base==97);
  for(i=0;i<5;++i){response.type_sources.data[i].raw_file_contents.size=6162;memset(response.type_sources.data[i].raw_file_contents.data,0xff,6162);}
  response.type_sources.data[5].raw_file_contents.size=1861;
  assert(RF(_measure)(&response,&n)==0&&n==32768&&RF(_encode)(&response,wire,sizeof wire,&n)==0);
  assert(RF(_decode)(wire,n,&response_out)==0);
  response.type_sources.data[5].raw_file_contents.size=1862;n=777;memcpy(saved,wire,sizeof wire);
  assert(RF(_measure)(&response,&n)==90&&n==777);
  assert(!RF(_valid)(&response)&&RF(_encode)(&response,wire,sizeof wire,&n)==90&&n==777&&!memcmp(wire,saved,sizeof wire));
}
static void oversized_independent_wire(void){
  /* Independent byte construction: bool, failure string, empty TypeDescription,
   * twelve sources, empty extra-information. It does not use the codec writer. */
  size_t at=0;unsigned i;reset();wire[at++]=0;
  uwvm_bounded_put(wire+at,0,4);at+=4; /* failure_reason */
  uwvm_bounded_put(wire+at,0,4);at+=4; /* individual.type_name */
  uwvm_bounded_put(wire+at,0,4);at+=4; /* individual.fields */
  uwvm_bounded_put(wire+at,0,4);at+=4; /* referenced descriptions */
  uwvm_bounded_put(wire+at,12,4);at+=4;
  for(i=0;i<12;++i){uwvm_bounded_put(wire+at,0,4);at+=4;uwvm_bounded_put(wire+at,0,4);at+=4;uwvm_bounded_put(wire+at,6162,4);at+=4;memset(wire+at,0,6162);at+=6162;}
  uwvm_bounded_put(wire+at,0,4);at+=4;assert(at==74113);
  memset(&response_out,0x37,sizeof response_out);response_before=response_out;
  assert(RF(_decode)(wire,at,&response_out)==90&&!memcmp(&response_out,&response_before,sizeof response_out));
}
static void decode_atomic_refusal(void){
  size_t n=777;reset();source.raw_file_contents.size=3;source.raw_file_contents.data[1]=0xff;
  assert(SF(_encode)(&source,wire,sizeof wire,&n)==0);memset(&source_out,0x37,sizeof source_out);source_before=source_out;
  assert(SF(_decode)(wire,n-1,&source_out)==22&&!memcmp(&source_out,&source_before,sizeof source_out));
  wire[n]=0;assert(SF(_decode)(wire,n+1,&source_out)==22&&!memcmp(&source_out,&source_before,sizeof source_out));
  uwvm_bounded_put(wire+8,6163,4);assert(SF(_decode)(wire,n,&source_out)==22&&!memcmp(&source_out,&source_before,sizeof source_out));
}
static void overlap_and_capacity(void){
  size_t n=777;reset();source.raw_file_contents.size=1;source.raw_file_contents.data[0]=0;
  assert(SF(_encode)(&source,wire,12,&n)==90&&n==777&&!memcmp(wire,saved,sizeof wire));
  assert(SF(_encode)(&source,&source,sizeof source,&n)==22&&n==777);
  assert(SF(_measure)(&source,(size_t*)&source)==22);
  assert(SF(_encode)(&source,wire,sizeof wire,(size_t*)wire)==22&&!memcmp(wire,saved,sizeof wire));
}
int main(void){raw_counted_bytes();text_domains_still_strict();raw_boundary();complete_capacities();aggregate_exact_and_over();oversized_independent_wire();decode_atomic_refusal();overlap_and_capacity();return 0;}
