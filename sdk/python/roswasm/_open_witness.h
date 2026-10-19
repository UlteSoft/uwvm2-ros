/* Optional real legacy-open evidence, included by _native.c/.c.in ONLY when
 * ROSWASM_OWNED_SERVICE==1 and backend2. Default-off business paths are intact.
 * A private GC-visible object retains actual C facts, never Host authority.
 * Every strong Python edge is traversed; clearing only releases Guest roots.
 * All fields/storage exist before the capopen import. Cleanup is once-only;
 * the destructor makes no direct ROS calls.
 * Snapshot allocation is an observation and never clears the retained facts.
 */
#ifndef ROSWASM_OPEN_WITNESS_H
#define ROSWASM_OPEN_WITNESS_H
#if !defined(ROSWASM_OWNED_SERVICE) || ROSWASM_OWNED_SERVICE != 1 || ROSWASM_BACKEND != 2
#error "open witnesses belong to the explicit backend2 owned-Service build"
#endif
typedef struct {
    PyObject_HEAD
    PyObject *input, *primary, *retirement;
    uint64_t handle;
    uint32_t code, cleanup_code, cleanup_preparation_code;
    unsigned entered, returned, delivered, adopted, cleanup_entered, cleanup_returned;
    unsigned cleared;
} roswasm_open_witness;
static PyTypeObject roswasm_open_type;
static void roswasm_open_dispose(PyObject *object);
static roswasm_open_witness *roswasm_open_get(PyObject *object) {
    if (!object || !Py_IS_TYPE(object, &roswasm_open_type)) {
        PyErr_SetString(PyExc_TypeError, "actual legacy-open witness required");
        return NULL;
    }
    roswasm_open_witness *w = (roswasm_open_witness *)object;
    if (w->cleared) {
        PyErr_SetString(PyExc_RuntimeError, "cleared legacy-open witness is unavailable");
        return NULL;
    }
    return w;
}
static int roswasm_open_traverse(roswasm_open_witness *w, visitproc visit, void *arg) {
    Py_VISIT(w->input);
    Py_VISIT(w->primary);
    Py_VISIT(w->retirement);
    return 0;
}
static int roswasm_open_clear(roswasm_open_witness *w) {
    /* Make reentry fail closed before DECREF can run any Guest finalizer.
     * GC clear never calls cleanup/adopt/imports or resets the numeric facts.
     * A reachable witness retains its exact primary/traceback unchanged. */
    w->cleared = 1;
    Py_CLEAR(w->input);
    Py_CLEAR(w->primary);
    Py_CLEAR(w->retirement);
    return 0;
}
static void roswasm_open_dispose(PyObject *object) {
    PyObject_GC_UnTrack(object);
    (void)roswasm_open_clear((roswasm_open_witness *)object);
    PyObject_GC_Del(object);
}
static PyTypeObject roswasm_open_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_roswasm_native._OpenWitness",
    .tp_basicsize = sizeof(roswasm_open_witness),
    .tp_dealloc = roswasm_open_dispose,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_DISALLOW_INSTANTIATION,
    .tp_traverse = (traverseproc)roswasm_open_traverse,
    .tp_clear = (inquiry)roswasm_open_clear,
    .tp_free = PyObject_GC_Del
    /* No BASETYPE, tp_new, public type registration or arbitrary constructor. */
};
static int roswasm_open_ready(void) {
    /* Module initialization only, before any ROS capability effect. */
    return PyType_Ready(&roswasm_open_type);
}
static PyObject *roswasm_prepare_open(PyObject *self, PyObject *args) {
    roswasm_open_witness *w; PyObject *input, *retirement; Py_ssize_t n;
    (void)self;
    if (!PyArg_ParseTuple(args, "OO:prepare_open_witness", &input, &retirement)) return NULL;
    if (!PyBytes_CheckExact(input) || (n = PyBytes_GET_SIZE(input)) < 1 || n > 255 ||
        memchr(PyBytes_AS_STRING(input), 0, (size_t)n) ||
        !PyList_CheckExact(retirement) || PyList_GET_SIZE(retirement) != 1 ||
        PyList_GET_ITEM(retirement, 0) != Py_False) {
        PyErr_SetString(PyExc_ValueError, "exact immutable alias bytes1..255 without NUL required");
        return NULL;
    }
    w = PyObject_GC_New(roswasm_open_witness, &roswasm_open_type);
    if (!w) return NULL;
    w->input = Py_NewRef(input);
    w->primary = NULL;
    w->retirement = Py_NewRef(retirement);
    w->handle = 0;
    w->code = w->cleanup_code = w->cleanup_preparation_code = 0;
    w->entered = w->returned = w->delivered = w->adopted = 0;
    w->cleanup_entered = w->cleanup_returned = w->cleared = 0;
    /* Initialize every visited edge before tracking, all before raw entry. */
    PyObject_GC_Track(w);
    return (PyObject *)w;
}
static void roswasm_open_primary(roswasm_open_witness *w, PyObject *primary) {
    /* First exact primary only. INCREF of a real exception allocates nothing. */
    if (w && !w->primary && primary) w->primary = Py_NewRef(primary);
}
static uint32_t roswasm_open_cleanup(roswasm_open_witness *w) {
    if (!w || !w->entered || !w->returned || w->code || !w->handle || w->adopted)
        return 0;
    if (w->cleanup_entered) return w->cleanup_code;
    if (!PyList_CheckExact(w->retirement) || PyList_GET_SIZE(w->retirement) != 1 ||
        PyList_GET_ITEM(w->retirement, 0) != Py_False) {
        w->cleanup_preparation_code = 22; return 22;
    }
    /* Publish into the ACTUAL owner's reserved retirement cell before raw.
     * No later Python observation is needed to hide an attempted/unknown ID.
     * The old value is proven immortal False, so this DECREF cannot callback. */
    Py_INCREF(Py_True); PyList_SET_ITEM(w->retirement, 0, Py_True); Py_DECREF(Py_False);
    w->cleanup_entered = 1; w->cleanup_code = 115;
    /* No allocation, Python callback, result wrapper or root release. */
    w->cleanup_code = uwvm_ros2_capability_close(w->handle);
    w->cleanup_returned = 1;
    return w->cleanup_code;
}
static PyObject *roswasm_close_open(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); (void)self;
    if (!w) return NULL;
    if (w->adopted) {
        PyErr_SetString(PyExc_RuntimeError, "adopted owner requires its original Capability retirement");
        return NULL;
    }
    (void)roswasm_open_cleanup(w);
    Py_RETURN_NONE;
}
static PyObject *roswasm_adopt_open(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); (void)self;
    if (!w) return NULL;
    if (!w->entered || !w->returned || w->code || !w->handle || !w->delivered ||
        w->cleanup_entered || w->adopted) {
        PyErr_SetString(PyExc_RuntimeError, "only one genuine returned uncleaned owner can be adopted");
        return NULL;
    }
    w->adopted = 1; Py_RETURN_NONE;
}
static PyObject *roswasm_open_cleaned(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); (void)self;
    if (!w) return NULL;
    return Py_NewRef(w->cleanup_entered ? Py_True : Py_False);
}
static PyObject *roswasm_open_cleanup_failed(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); (void)self;
    if (!w) return NULL;
    return Py_NewRef((w->cleanup_preparation_code ||
                     (w->cleanup_entered && (!w->cleanup_returned || w->cleanup_code)))
                    ? Py_True : Py_False);
}
static PyObject *roswasm_open_pending(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); (void)self;
    if (!w) return NULL;
    return Py_NewRef((w->entered && w->returned && !w->code && w->handle &&
                      !w->adopted && !w->cleanup_entered) ? Py_True : Py_False);
}
static PyObject *roswasm_open_saved_primary(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); (void)self;
    if (!w) return NULL;
    return Py_NewRef(w->primary ? w->primary : Py_None);
}
static PyObject *roswasm_open_snapshot(PyObject *self, PyObject *object) {
    roswasm_open_witness *w = roswasm_open_get(object); unsigned char out[32] = {0};
    (void)self;
    if (!w) return NULL;
    out[0] = (unsigned char)w->entered; out[1] = (unsigned char)w->returned;
    out[2] = (unsigned char)w->delivered; out[3] = (unsigned char)w->adopted;
    out[4] = (unsigned char)w->cleanup_entered; out[5] = (unsigned char)w->cleanup_returned;
    for (unsigned i = 0; i < 4; ++i) {
        out[8+i] = (unsigned char)(w->code >> (8*i));
        out[12+i] = (unsigned char)(w->cleanup_code >> (8*i));
        out[24+i] = (unsigned char)(w->cleanup_preparation_code >> (8*i));
    }
    for (unsigned i = 0; i < 8; ++i) out[16+i] = (unsigned char)(w->handle >> (8*i));
    /* A COPY only: no mutable/resizable alias of C state escapes. An OOM while
     * observing does not reset the witness or authorize another cleanup. */
    return PyBytes_FromStringAndSize((char const *)out, sizeof(out));
}
#define ROSWASM_OPEN_METHODS \
    {"prepare_open_witness", roswasm_prepare_open, METH_VARARGS, "Reserve actual C open evidence before entry."}, \
    {"cleanup_open_witness", roswasm_close_open, METH_O, "Once close an unadopted actual open; retain C outcome."}, \
    {"adopt_open_witness", roswasm_adopt_open, METH_O, "Transfer a returned actual open into its pre-reserved owner."}, \
    {"open_witness_cleaned", roswasm_open_cleaned, METH_O, "Actual cleanup entry singleton observation."}, \
    {"open_witness_cleanup_failed", roswasm_open_cleanup_failed, METH_O, "Retained cleanup failure singleton observation."}, \
    {"open_witness_pending", roswasm_open_pending, METH_O, "Actual unadopted handle pending cleanup."}, \
    {"open_witness_primary", roswasm_open_saved_primary, METH_O, "Retained exact C primary reference."}, \
    {"open_witness_snapshot", roswasm_open_snapshot, METH_O, "Copied32-byte fullid/status evidence; may allocate."},
#endif
