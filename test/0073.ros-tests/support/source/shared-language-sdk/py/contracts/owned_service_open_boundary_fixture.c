/* UNEXECUTED. Separate target CPython/WASI contract module, not Native proof.
 * Includes the ACTUAL legacy bridge/witness. Only open/close are counted doubles;
 * other81 imports remain actual Runtime imports. An actual OBJ allocator fails
 * after successful full-u64 open, including real primary-attribute attachment.
 * A missing expected fault is an assertion failure, never a qualification PASS.
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <string.h>
#include "ros2/abi.h"
static int faulting, attribute_failed;
static unsigned open_calls, failed_owner_closes, other_closes;
static PyMemAllocatorEx saved_allocator;
static const uint64_t fixture_handle = UINT64_C(0xfedcba9800000007);
static void *fault_malloc(void *c,size_t n) {(void)c;return faulting?NULL:saved_allocator.malloc(saved_allocator.ctx,n);}
static void *fault_calloc(void *c,size_t n,size_t z) {(void)c;return faulting?NULL:saved_allocator.calloc(saved_allocator.ctx,n,z);}
static void *fault_realloc(void *c,void *p,size_t n) {(void)c;return faulting?NULL:saved_allocator.realloc(saved_allocator.ctx,p,n);}
static void fault_free(void *c,void *p) {(void)c;saved_allocator.free(saved_allocator.ctx,p);}
static uint32_t counted_open(uint32_t m,uint64_t a,uint64_t n,uint64_t out) {
    (void)m;(void)a;(void)n;++open_calls;
    for(unsigned i=0;i<8;++i)((unsigned char *)(uintptr_t)out)[i]=(unsigned char)(fixture_handle>>(8*i));
    faulting=1;return 0;
}
static uint32_t counted_close(uint64_t h) {
    if(h==fixture_handle)++failed_owner_closes;else ++other_closes;
    return 90;
}
static int observed_attribute(PyObject *o,const char *name,PyObject *v) {
    int result=PyObject_SetAttrString(o,name,v);
    if(result<0)attribute_failed=1;
    return result;
}
#define uwvm_ros2_capability_open counted_open
#define uwvm_ros2_capability_close counted_close
#define PyObject_SetAttrString observed_attribute
#define PyInit__roswasm_native original_fixture_init
#define ROSWASM_BACKEND 2
#define ROSWASM_OWNED_SERVICE 1
#include "../../../../../../../sdk/origin/python/roswasm/_native.c"
#undef PyInit__roswasm_native
#undef PyObject_SetAttrString
#undef uwvm_ros2_capability_close
#undef uwvm_ros2_capability_open

static PyObject *run_open_boundary(PyObject *self,PyObject *unused) {
    PyObject *alias=NULL,*empty=NULL,*cell=NULL,*prep=NULL,*token=NULL,*args=NULL,*later=NULL;
    PyObject *result=NULL,*primary=NULL,*observed=NULL;int okay=0;
    PyMemAllocatorEx fault;
    (void)self;(void)unused;
    alias=PyBytes_FromStringAndSize("explicit-fixture-alias",22);
    empty=PyBytes_FromStringAndSize(NULL,0);
    cell=Py_BuildValue("[O]",Py_False);
    if(!alias||!empty||!cell)goto done;
    prep=PyTuple_Pack(2,alias,cell);if(!prep)goto done;
    token=roswasm_prepare_open(NULL,prep);if(!token)goto done;
    args=Py_BuildValue("(sKOiKKOO)","capability_open",(unsigned long long)0,alias,0,
        (unsigned long long)0,(unsigned long long)0,Py_None,token);
    later=Py_BuildValue("(sKOiKK)","capability_close",(unsigned long long)UINT64_C(0x100000003),
        empty,0,(unsigned long long)0,(unsigned long long)0);
    if(!args||!later)goto done;
    open_calls=failed_owner_closes=other_closes=0;attribute_failed=faulting=0;
    PyMem_GetAllocator(PYMEM_DOMAIN_OBJ,&saved_allocator);
    fault=(PyMemAllocatorEx){NULL,fault_malloc,fault_calloc,fault_realloc,fault_free};
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ,&fault);
    result=roswasm_invoke(NULL,args);
    primary=PyErr_GetRaisedException();
    faulting=0;PyMem_SetAllocator(PYMEM_DOMAIN_OBJ,&saved_allocator);
    if(result||!primary||!PyObject_TypeCheck(primary,(PyTypeObject *)PyExc_MemoryError)||
        !attribute_failed||open_calls!=1||failed_owner_closes!=1||PyList_GET_ITEM(cell,0)!=Py_True)goto failed;
    /* A subsequent ACTUAL generic invoke clears only the old global. */
    result=roswasm_invoke(NULL,later);if(!result)goto done;
    Py_CLEAR(result);
    if(roswasm_cleanup_known||other_closes!=1)goto failed;
    observed=roswasm_open_saved_primary(NULL,token);
    if(observed!=primary)goto failed;
    Py_CLEAR(observed);
    roswasm_open_witness *w=roswasm_open_get(token);
    if(!w||w->handle!=fixture_handle||!w->entered||!w->returned||w->code||
        !w->cleanup_entered||!w->cleanup_returned||w->cleanup_code!=90)goto failed;
    result=roswasm_close_open(NULL,token);if(!result)goto done;
    Py_CLEAR(result);
    if(failed_owner_closes!=1)goto failed;
    okay=1;goto done;
failed:
    if(!PyErr_Occurred())PyErr_SetString(PyExc_AssertionError,"actual open/OOM/attribute/onceclose witness failed");
done:
    Py_XDECREF(result);Py_XDECREF(primary);Py_XDECREF(observed);Py_XDECREF(args);Py_XDECREF(later);
    Py_XDECREF(prep);Py_XDECREF(token);Py_XDECREF(alias);Py_XDECREF(empty);Py_XDECREF(cell);
    if(!okay)return NULL;
    Py_RETURN_NONE;
}
/* The Python caller retains an actual _OpenCapture/token in its frame. The
 * real propagated MemoryError therefore creates the traceback backedge; this
 * is not a manually fabricated exception or direct-C-only lifetime test. */
static PyObject *raise_open_with_traceback(PyObject *self, PyObject *token) {
    roswasm_open_witness *w = roswasm_open_get(token);
    PyObject *args, *result;
    PyMemAllocatorEx fault;
    (void)self;
    if (!w) return NULL;
    args = Py_BuildValue("(sKOiKKOO)", "capability_open", (unsigned long long)0,
        w->input, 0, (unsigned long long)0, (unsigned long long)0, Py_None, token);
    if (!args) return NULL;
    faulting = 0; attribute_failed = 0;
    PyMem_GetAllocator(PYMEM_DOMAIN_OBJ, &saved_allocator);
    fault = (PyMemAllocatorEx){NULL, fault_malloc, fault_calloc, fault_realloc, fault_free};
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ, &fault);
    result = roswasm_invoke(NULL, args);
    /* Restore before the VM receives the exact exception and constructs its
     * actual Python traceback. No replacement exception/traceback is made. */
    faulting = 0; PyMem_SetAllocator(PYMEM_DOMAIN_OBJ, &saved_allocator);
    Py_DECREF(args);
    return result;
}
static PyObject *open_counts(PyObject *self, PyObject *unused) {
    (void)self; (void)unused;
    return Py_BuildValue("(III)", open_calls, failed_owner_closes, other_closes);
}
static PyMethodDef extras[]={
    {"fixture_open_boundary",run_open_boundary,METH_NOARGS,"Actual C post-open OOM witness path."},
    {"fixture_open_traceback",raise_open_with_traceback,METH_O,"Real bridge OOM propagated to the actual Python frame."},
    {"fixture_open_counts",open_counts,METH_NOARGS,"Actual counted-C open/cleanup totals, observation may allocate."},
    {NULL,NULL,0,NULL}};
PyMODINIT_FUNC PyInit__roswasm_native(void) {
    PyObject *module=original_fixture_init();
    if(module&&PyModule_AddFunctions(module,extras)<0){Py_DECREF(module);return NULL;}
    return module;
}
