/** @file
 * @brief Bounded UTF-8 intrinsics inside the actual Wasm engine.
 *
 * Canonical SDK decoders need fatal UTF-8 validation and literal BOM retention.
 * These functions use no host codec, file, locale, clock or JavaScript engine.
 * Nonfatal decoding consumes each maximal valid prefix of an invalid sequence;
 * lone UTF-16 surrogates encode as U+FFFD rather than CESU-8.
 */
#include "engine.h"
#include <string.h>
#ifndef UWVM_JS_MAX_TEXT_BYTES
#error "Select the explicit UTF-8 allocation bound"
#endif

static JSValue encode(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self;
    if (argc!=1 || !JS_IsString(argv[0])) return JS_ThrowTypeError(ctx,"encode requires a string");
    JSValue length=JS_GetPropertyStr(ctx,argv[0],"length");
    uint32_t units;
    int good=uwvm_js_checked_u32(ctx,length,&units);
    JS_FreeValue(ctx,length);
    if (good<0) return JS_EXCEPTION;
    if (units>UWVM_JS_MAX_TEXT_BYTES) return JS_ThrowRangeError(ctx,"UTF-8 text bound");
    size_t bytes;
    char const *text=JS_ToCStringLen2(ctx,&bytes,argv[0],0);
    if (!text) return JS_EXCEPTION;
    if (bytes>UWVM_JS_MAX_TEXT_BYTES) { JS_FreeCString(ctx,text); return JS_ThrowRangeError(ctx,"UTF-8 byte bound"); }
    size_t first=0;
    while (first+2<bytes) {
        if ((uint8_t)text[first]==0xed && ((uint8_t)text[first+1]&0xe0)==0xa0) break;
        ++first;
    }
    JSValue result;
    if (first+2>=bytes) result=uwvm_js_new_bytes(ctx,(uint8_t const *)text,bytes);
    else {
        uint8_t *copy=js_malloc(ctx,bytes ? bytes : 1);
        if (!copy) { JS_FreeCString(ctx,text); return JS_EXCEPTION; }
        memcpy(copy,text,bytes);
        for (size_t i=first;i+2<bytes;++i)
            if (copy[i]==0xed && (copy[i+1]&0xe0)==0xa0) {
                copy[i]=0xef; copy[i+1]=0xbf; copy[i+2]=0xbd; i+=2;
            }
        result=uwvm_js_new_bytes(ctx,copy,bytes); js_free(ctx,copy);
    }
    JS_FreeCString(ctx,text); return result;
}

/** Return the valid sequence length, or zero and the consumed invalid prefix.
 * Restricted second-byte ranges reject overlong encodings, surrogate code
 * points and values above U+10FFFF before building an engine string.
 */
static unsigned sequence(uint8_t const *p, size_t size, unsigned *consumed)
{
    *consumed=1;
    uint8_t first=p[0];
    if (first<0x80) return 1;
    unsigned length;
    uint8_t low=0x80, high=0xbf;
    if (first>=0xc2 && first<=0xdf) length=2;
    else if (first>=0xe0 && first<=0xef) {
        length=3;
        if (first==0xe0) low=0xa0;
        if (first==0xed) high=0x9f;
    } else if (first>=0xf0 && first<=0xf4) {
        length=4;
        if (first==0xf0) low=0x90;
        if (first==0xf4) high=0x8f;
    } else return 0;
    if (size<2 || p[1]<low || p[1]>high) return 0;
    *consumed=2;
    for (unsigned i=2;i<length;++i) {
        if (size<=i || p[i]<0x80 || p[i]>0xbf) return 0;
        *consumed=i+1;
    }
    return length;
}
static JSValue decode(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self;
    if (argc!=3 || !JS_IsBool(argv[1]) || !JS_IsBool(argv[2]))
        return JS_ThrowTypeError(ctx,"decode requires bytes and two booleans");
    uint8_t const *bytes; size_t size; JSValue root;
    if (uwvm_js_read_bytes(ctx,argv[0],&bytes,&size,&root)<0) return JS_EXCEPTION;
    if (size>UWVM_JS_MAX_TEXT_BYTES) { JS_FreeValue(ctx,root); return JS_ThrowRangeError(ctx,"UTF-8 decode bound"); }
    int fatal=JS_ToBool(ctx,argv[1]), ignore_bom=JS_ToBool(ctx,argv[2]);
    size_t offset=(!ignore_bom && size>=3 && bytes[0]==0xef && bytes[1]==0xbb && bytes[2]==0xbf) ? 3 : 0;
    size_t i=offset;
    for (;i<size;) {
        unsigned consumed, good=sequence(bytes+i,size-i,&consumed);
        if (!good) break;
        i+=good;
    }
    JSValue value;
    if (i==size) value=JS_NewStringLen(ctx,size ? (char const *)bytes+offset : "",size-offset);
    else if (fatal) value=JS_ThrowTypeError(ctx,"Malformed UTF-8");
    else {
        if (size-offset>(SIZE_MAX-1)/3) { JS_FreeValue(ctx,root); return JS_ThrowRangeError(ctx,"UTF-8 replacement bound"); }
        uint8_t *copy=js_malloc(ctx,(size-offset)*3+1);
        if (!copy) { JS_FreeValue(ctx,root); return JS_EXCEPTION; }
        size_t used=0;
        for (i=offset;i<size;) {
            unsigned consumed, good=sequence(bytes+i,size-i,&consumed);
            if (good) { memcpy(copy+used,bytes+i,good); used+=good; i+=good; }
            else { copy[used++]=0xef; copy[used++]=0xbf; copy[used++]=0xbd; i+=consumed; }
        }
        value=JS_NewStringLen(ctx,(char const *)copy,used); js_free(ctx,copy);
    }
    JS_FreeValue(ctx,root); return value;
}
static JSCFunctionListEntry const functions[]={JS_CFUNC_DEF("encode",1,encode),JS_CFUNC_DEF("decode",3,decode)};
static int initialize(JSContext *ctx, JSModuleDef *module)
{ return JS_SetModuleExportList(ctx,module,functions,sizeof functions/sizeof *functions); }
JSModuleDef *uwvm_js_register_utf8(JSContext *ctx,char const *name)
{
    JSModuleDef *module=JS_NewCModule(ctx,name,initialize);
    if (!module || JS_AddModuleExportList(ctx,module,functions,sizeof functions/sizeof *functions)<0) return NULL;
    return module;
}
