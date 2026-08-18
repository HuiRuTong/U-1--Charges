#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include <math.h>

static void _swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int *_sort(int *arr) {
    // Sorts a 3 element array in ascending order
    if (arr[0] > arr[1]) {
        _swap(arr, arr+1);
    }
    if (arr[1] > arr[2]) {
        _swap(arr+1, arr+2);
        if (arr[0] > arr[1]) {
            _swap(arr, arr+1);
        }
    }
    return arr;     // This is in-place so technically no return is needed
}

static int *_sort_abs(int *arr) {
    // Sorts a 3 element array in order of ascending magnitude
    /*
        The paper used ascending order but ascending magnitude
        makes finding duplicates ensures I don't have to check
        charge inversion independently
    */
    if (abs(arr[0]) > abs(arr[1])) {
        _swap(arr, arr+1);
    }
    if (abs(arr[1]) > abs(arr[2])) {
        _swap(arr+1, arr+2);
        if (abs(arr[0]) > abs(arr[1])) {
            _swap(arr, arr+1);
        }
    }
    return arr;     // Same here
}

static PyObject *get_sorted_charges(PyObject *self, PyObject *args) {
    PyArrayObject *charges;
    PyObject *charges_sorted = PyArray_SimpleNew(2, ((npy_intp[]){6, 3}), NPY_INT32);
    int charge_arr[3];

    if (!PyArg_ParseTuple(args, "O", &charges)) {
        return NULL;
    }

    for (int i = 0; i < 18; i += 3) {
        charge_arr[0] = *((int *) PyArray_DATA(charges) + i);
        charge_arr[1] = *((int *) PyArray_DATA(charges) + i+1);
        charge_arr[2] = *((int *) PyArray_DATA(charges) + i+2);
        
        _sort_abs(charge_arr);
        for (int j = 0; j < 3; j++) {
            *((int *) PyArray_DATA((PyArrayObject *) charges_sorted) + i+j) = charge_arr[j];
        }
    }
    return charges_sorted;      // This is NOT in-place because charge_arr copies the VALUES of charges not its address
}
// The above might not be used at all

static PyObject *get_higgs_charge(PyObject *self, PyObject *args) {
    PyArrayObject *charges_sum;
    int q_sum, v_sum;
    
    if (!PyArg_ParseTuple(args, "O", &charges_sum)) {
        return NULL;
    }

    q_sum = ((int *) PyArray_DATA(charges_sum))[0];
    v_sum = ((int *) PyArray_DATA(charges_sum))[5];

    return PyLong_FromLong((-3*q_sum - v_sum) / 3);
}

static PyObject *get_charges_properties(PyObject *self, PyObject *args) {
    PyArrayObject *charges;
    PyObject *charges_sum = PyArray_SimpleNew(1, (npy_intp[]){6}, NPY_INT32);
    PyObject *charges_bar = PyArray_SimpleNew(1, (npy_intp[]){6}, NPY_INT32);
    PyObject *charges_32 = PyArray_SimpleNew(1, (npy_intp[]){6}, NPY_INT32);
    int q_sum, v_sum;

    if (!PyArg_ParseTuple(args, "O", &charges)) {
        return NULL;
    }

    q_sum = *((int *) PyArray_DATA(charges))
            + *((int *) PyArray_DATA(charges) + 1)
            + *((int *) PyArray_DATA(charges) + 2);
    v_sum = *((int *) PyArray_DATA(charges) + 15)
            + *((int *) PyArray_DATA(charges) + 16)
            + *((int *) PyArray_DATA(charges) + 17);
    
    ((int *) PyArray_DATA((PyArrayObject *) charges_sum))[0] = q_sum;
    ((int *) PyArray_DATA((PyArrayObject *) charges_sum))[1] = -3*q_sum;
    ((int *) PyArray_DATA((PyArrayObject *) charges_sum))[2] = -6*q_sum - v_sum;
    ((int *) PyArray_DATA((PyArrayObject *) charges_sum))[3] = 4*q_sum + v_sum;
    ((int *) PyArray_DATA((PyArrayObject *) charges_sum))[4] = -2*q_sum - v_sum;
    ((int *) PyArray_DATA((PyArrayObject *) charges_sum))[5] = v_sum;

    for (int i = 0; i < 18; i += 3) {
        ((int *) PyArray_DATA((PyArrayObject *) charges_bar))[i / 3] = *((int *) PyArray_DATA(charges) + i + 2)
                                                                       + *((int *) PyArray_DATA(charges) + i + 1)
                                                                       - 2*(*((int *) PyArray_DATA(charges) + i));
        ((int *) PyArray_DATA((PyArrayObject *) charges_32))[i / 3] = *((int *) PyArray_DATA(charges) + i + 2)
                                                                       - *((int *) PyArray_DATA(charges) + i + 1);
    }

    return Py_BuildValue("OOO", charges_sum, charges_bar, charges_32);
}

// List of callable functions
static PyMethodDef callables[] = {
    
    {"get_sorted_charges", get_sorted_charges, METH_VARARGS,
     "Sorts the charges in ascending magnitude"},
                        
    {"get_higgs_charge", get_higgs_charge, METH_VARARGS,
     "Returns the Higgs charge given all the charges"},

    {"get_charges_properties", get_charges_properties, METH_VARARGS,
     "Returns (X_sum, X_bar, X_32)"},

    {NULL, NULL, 0, NULL}
};

// Required API thingy
static struct PyModuleDef charges = {
    PyModuleDef_HEAD_INIT,
    "charges",
    "C implementation of various functions. MIGHT provide minimal speedups idk",
    -1,
    callables
};

// CHANGE THIS FUNCTIONS NAME!!! if renaming the module
PyMODINIT_FUNC PyInit_charges(void) {
    import_array(); // For np
    return PyModule_Create(&charges);
}