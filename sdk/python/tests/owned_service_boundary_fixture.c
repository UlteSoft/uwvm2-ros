/* UNEXECUTED source. Link ONLY into a separate CPython/WASI contract fixture.
 * Selected counted C callees are doubles, never Native/Host qualification.
 * Other imports remain actual Runtime imports. Production has no test flag/API.
 * A real PYMEM_DOMAIN_OBJ allocator fails every object allocation AFTER the
 * counted import and is restored inside C before returning to Python.
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <string.h>
#include "roswasm/rmw_service_v1.h"
static int failing;
static uint32_t fixture_code;
static unsigned calls, allocation_attempts;
static PyMemAllocatorEx original_allocator;
static void *fault_malloc(void *ctx, size_t size) {
    (void)ctx; if (failing) { ++allocation_attempts; return NULL; }
    return original_allocator.malloc(original_allocator.ctx,size);
}
static void *fault_calloc(void *ctx, size_t count, size_t size) {
    (void)ctx; if (failing) { ++allocation_attempts; return NULL; }
    return original_allocator.calloc(original_allocator.ctx,count,size);
}
static void *fault_realloc(void *ctx, void *ptr, size_t size) {
    (void)ctx; if (failing) { ++allocation_attempts; return NULL; }
    return original_allocator.realloc(original_allocator.ctx,ptr,size);
}
static void fault_free(void *ctx, void *ptr) {
    (void)ctx; original_allocator.free(original_allocator.ctx,ptr);
}
static uint32_t counted_submit(uint64_t h,uint64_t inc,uint32_t m,uint64_t in,uint64_t n,uint64_t out) {
    (void)h;(void)inc;(void)m;(void)in;(void)n;
    if (!fixture_code) memset((void *)(uintptr_t)out,0xff,8); /* legal full-u64 ticket */
    ++calls; failing=1; return fixture_code;
}
static uint32_t counted_commit(uint64_t h,uint64_t inc,uint64_t seq,uint64_t nonce) {
    (void)h;(void)inc;(void)seq;(void)nonce;
    ++calls; failing=1; return fixture_code;
}
#define uwvm_ros2_rmw_srv_v1_client_submit counted_submit
#define uwvm_ros2_rmw_srv_v1_endpoint_commit counted_commit
#define PyInit__roswasm_owned_service fixture_original_init
#define ROSWASM_BACKEND 2
#define ROSWASM_OWNED_SERVICE 1
#include "../roswasm/_owned_service.c"
#undef PyInit__roswasm_owned_service
#undef uwvm_ros2_rmw_srv_v1_endpoint_commit
#undef uwvm_ros2_rmw_srv_v1_client_submit

static PyObject *run_boundary(PyObject *self,PyObject *args) {
    unsigned index, code; PyObject *arguments=NULL,*token=NULL,*empty=NULL,*value=NULL,*data=NULL,*status_root=NULL;
    int okay=0; PyMemAllocatorEx fault;
    (void)self;
    if (!PyArg_ParseTuple(args,"II:fixture_run_boundary",&index,&code)) return NULL;
    if (index!=4 && index!=8) {PyErr_SetString(PyExc_ValueError,"fixture index must be4 or8");return NULL;}
    empty=PyBytes_FromStringAndSize(NULL,0);
    if(!empty)return NULL;
    arguments=Py_BuildValue("(iKKKKOn)",(int)index,(unsigned long long)UINT64_MAX,
        (unsigned long long)1,(unsigned long long)(index==8?1:0),
        (unsigned long long)(index==8?1:0),empty,(Py_ssize_t)(index==4?8:0));
    Py_DECREF(empty);
    if(!arguments)return NULL;
    token=prepare(NULL,arguments);Py_DECREF(arguments);
    if(!token)return NULL;
    fixture_code=code;calls=allocation_attempts=0;failing=0;
    PyMem_GetAllocator(PYMEM_DOMAIN_OBJ,&original_allocator);
    fault=(PyMemAllocatorEx){NULL,fault_malloc,fault_calloc,fault_realloc,fault_free};
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ,&fault);
    value=execute(NULL,token);
    /* These are the SAME actual builtin observation functions. All must return
     * existing roots/singletons under the still-failing actual Obj allocator. */
    if(value==Py_None) {
        Py_DECREF(value);value=success(NULL,token);
        if(value==(code?Py_False:Py_True)) {
            Py_DECREF(value);value=attempted(NULL,token);
            if(value==Py_True) {
                status_root=status(NULL,token);data=output(NULL,token);
                okay=status_root&&data&&calls==1&&allocation_attempts==0;
                if(okay)for(unsigned i=0;i<4;++i)if((unsigned char)PyBytes_AS_STRING(status_root)[i]!=(unsigned char)(code>>(i*8)))okay=0;
                if(okay&&index==4)for(unsigned i=0;i<8;++i)if((unsigned char)PyBytes_AS_STRING(data)[i]!=(code?0:255))okay=0;
            }
        }
    }
    failing=0;PyMem_SetAllocator(PYMEM_DOMAIN_OBJ,&original_allocator);
    Py_XDECREF(value);Py_XDECREF(status_root);Py_XDECREF(data);Py_DECREF(token);
    if(PyErr_Occurred())return NULL;
    if(!okay){PyErr_SetString(PyExc_AssertionError,"actual post-effect allocator/retained-result boundary failed");return NULL;}
    Py_RETURN_NONE;
}
static PyMethodDef extras[]={{"fixture_run_boundary",run_boundary,METH_VARARGS,"Run the real C result path under failing Obj allocations."},{NULL,NULL,0,NULL}};
PyMODINIT_FUNC PyInit__roswasm_owned_service(void) {
    PyObject *module=fixture_original_init();
    if(module&&PyModule_AddFunctions(module,extras)<0){Py_DECREF(module);return NULL;}
    return module;
}
