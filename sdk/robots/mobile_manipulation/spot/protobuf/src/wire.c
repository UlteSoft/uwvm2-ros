/* SPDX-License-Identifier: Apache-2.0 */
#include "schema.h"
#include <stdbool.h>
/* Does not depend on NUL terminators: protobuf strings may contain U+0000. */
static bool utf8(const uint8_t *p,size_t n) {
    size_t i=0;
    while(i<n) {
        uint32_t c=p[i++],need,minimum;
        if(c<0x80) continue;
        if(c>=0xc2&&c<=0xdf){c&=31;need=1;minimum=0x80;}
        else if(c>=0xe0&&c<=0xef){c&=15;need=2;minimum=0x800;}
        else if(c>=0xf0&&c<=0xf4){c&=7;need=3;minimum=0x10000;}
        else return false;
        if(need>n-i) return false;
        while(need--){uint8_t d=p[i++];if((d&0xc0)!=0x80)return false;c=(c<<6)|(d&63);}
        if(c<minimum||c>0x10ffff||(c>=0xd800&&c<=0xdfff)) return false;
    }
    return true;
}
static bool varint(const uint8_t *p,size_t n,size_t *at,uint64_t *v) {
    uint64_t out=0;
    for(unsigned k=0;k<10;++k) {
        if(*at>=n) return false;
        uint8_t b=p[(*at)++];
        if(k==9&&b>1) return false;
        out|=(uint64_t)(b&127)<<(7*k);
        if(!(b&128)){*v=out;return true;}
    }
    return false;
}
static bool admitted_type(const uwvm_spot_proto_schema *t) {
    for(size_t i=0;i<uwvm_spot_proto_schema_count;++i)
        if(t==&uwvm_spot_proto_schemas[i]) return true;
    return false;
}
typedef struct frame {const uint8_t *p;size_t n,at;const uwvm_spot_proto_schema *type;} frame;
uint32_t uwvm_spot_proto_preflight(const uwvm_spot_proto_schema *root,const void *wire,size_t size) {
    if(!admitted_type(root)||(!wire&&size)) return 22;
    if(size>UWVM_SPOT_PROTO_WIRE_MAX) return 90;
    frame stack[UWVM_SPOT_PROTO_DEPTH_MAX];
    size_t count=1,work=0;
    stack[0]=(frame){(const uint8_t *)wire,size,0,root};
    while(count) {
        frame *f=&stack[count-1];
        if(f->at==f->n){--count;continue;}
        if(++work>32768) return 90;
        uint64_t key;
        if(!varint(f->p,f->n,&f->at,&key)||key<8||(key>>3)>0x1fffffff) return 22;
        uint32_t tag=(uint32_t)(key>>3),wt=(uint32_t)(key&7);
        const uwvm_spot_proto_field *field=NULL;
        for(size_t i=0;i<f->type->field_count;++i)
            if(f->type->fields[i].tag==tag){field=&f->type->fields[i];break;}
        /* Primitive packed repeated fields use length-delimited wire. */
        uint32_t expected=0;
        if(field) {
            switch(field->type) {
            case 1:case 6:case 16:expected=1;break;
            case 2:case 7:case 15:expected=5;break;
            case 9:case 11:case 12:expected=2;break;
            default:expected=0;break;
            }
            if(wt!=expected&&!(wt==2&&field->repeated&&expected!=2)) return 22;
        }
        if(wt==0){uint64_t ignored;if(!varint(f->p,f->n,&f->at,&ignored))return 22;}
        else if(wt==1||wt==5){size_t bytes=wt==1?8:4;if(bytes>f->n-f->at)return 22;f->at+=bytes;}
        else if(wt==2) {
            uint64_t len;
            if(!varint(f->p,f->n,&f->at,&len)||len>f->n-f->at) return 22;
            const uint8_t *data=f->p+f->at;f->at+=(size_t)len;
            if(field&&field->type==9&&!utf8(data,(size_t)len)) return 22;
            if(field&&field->type==11) {
                if(field->child>=uwvm_spot_proto_schema_count) return 22;
                if(count==UWVM_SPOT_PROTO_DEPTH_MAX) return 90;
                stack[count++]=(frame){data,(size_t)len,0,&uwvm_spot_proto_schemas[field->child]};
            }
        } else return 22; /* No proto2 groups, no undefined wire kinds. */
    }
    return 0;
}
