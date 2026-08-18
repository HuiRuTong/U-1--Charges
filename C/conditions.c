#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include <math.h>

static PyObject *anomaly_quadratic(PyObject *self, PyObject *args) {
    PyArrayObject *charges;
    int coeff = 0;
    
    if (!PyArg_ParseTuple(args, "O", &charges)) {
        return NULL;
    }

    for (int i = 0; i < 3; i++) {
        int q = *((int *) PyArray_DATA(charges) + i);
        int l = *((int *) PyArray_DATA(charges) + 3+i);
        int e = *((int *) PyArray_DATA(charges) + 6+i);
        int u = *((int *) PyArray_DATA(charges) + 9+i);
        int d = *((int *) PyArray_DATA(charges) + 12+i);

        coeff += q*q - l*l + e*e - 2*u*u + d*d;
    }

    return PyLong_FromLong(coeff);
}

static PyObject *anomaly_cubic(PyObject *self, PyObject *args) {
    PyArrayObject *charges;
    int coeff = 0;
    
    if (!PyArg_ParseTuple(args, "O", &charges)) {
        return NULL;
    }

    for (int i = 0; i < 3; i++) {
        int q = *((int *) PyArray_DATA(charges) + i);
        int l = *((int *) PyArray_DATA(charges) + 3+i);
        int e = *((int *) PyArray_DATA(charges) + 6+i);
        int u = *((int *) PyArray_DATA(charges) + 9+i);
        int d = *((int *) PyArray_DATA(charges) + 12+i);
        int v = *((int *) PyArray_DATA(charges) + 15+i);

        coeff += 6*q*q*q + 2*l*l*l - e*e*e - 3*u*u*u - 3*d*d*d - v*v*v;
    }

    return PyLong_FromLong(coeff);
}

static PyObject *yukawa_check(PyObject *self, PyObject *args) {
    PyArrayObject *charges_sum;
    int q_sum, l_sum, e_sum, u_sum, d_sum, v_sum;
    
    if (!PyArg_ParseTuple(args, "O", &charges_sum)) {
        return NULL;
    }

    q_sum = ((int *) PyArray_DATA(charges_sum))[0];
    l_sum = ((int *) PyArray_DATA(charges_sum))[1];
    e_sum = ((int *) PyArray_DATA(charges_sum))[2];
    u_sum = ((int *) PyArray_DATA(charges_sum))[3];
    d_sum = ((int *) PyArray_DATA(charges_sum))[4];
    v_sum = ((int *) PyArray_DATA(charges_sum))[5];

    if (2*q_sum - u_sum - d_sum != - l_sum + e_sum + 3*q_sum + v_sum) {
        return Py_False;
    }
    return Py_True;
}

static PyObject *multiple_check(PyObject *self, PyObject *args) {
    PyArrayObject *found_charges, *curr_charges;
    int *found_arr; 
    int *curr_arr;

    if (!PyArg_ParseTuple(args, "OO", &found_charges, &curr_charges)) {
        return NULL;
    }

    found_arr = (int *) PyArray_DATA(found_charges);
    curr_arr = (int *) PyArray_DATA(curr_charges);

    for (int i = 0; i < 18; i += 3) {
        // Assumes charges have already been sorted w/ _sort_abs
        // The plan is to check for equal combinations first then multiples
        // Because C hates dynamic arrays, the former check should be done in Python
        int factors[3];
        for (int j = 0; j < 3; j++) {
            if (!(*(found_arr+i+j) || *(curr_arr+i+j))) {    // For 0,0
                factors[j] = 0;
                continue;
            } else if (!(*(found_arr+i+j)) != !(*(curr_arr+i+j))) { // For 0,1 & 1,0: never multiples
                return Py_False;
            } else {
                factors[j] = *(found_arr+i+j) / *(curr_arr+i+j);
            }

            if (j && (factors[j-1] != factors[j])) {
                return Py_False;
            }
        }
    }
    return Py_True;
}

// List of callable functions
static PyMethodDef callables[] = {
    
    {"anomaly_quadratic", anomaly_quadratic, METH_VARARGS,
     "Checks if the individual charges fulfill the quadratic condition"},
        
    {"anomaly_cubic", anomaly_cubic, METH_VARARGS,
     "Checks if the individual charges fulfill the cubic conditions"},
            
    {"yukawa_check", yukawa_check, METH_VARARGS,
     "Checks yukawa stuff"},
                
    {"multiple_check", multiple_check, METH_VARARGS,
     "Checks if a set of charges is equivalent to a known solution"},

    {NULL, NULL, 0, NULL}
};

// Required API thingy
static struct PyModuleDef conditions = {
    PyModuleDef_HEAD_INIT,
    "conditions",
    "C implementation of various functions. MIGHT provide minimal speedups idk",
    -1,
    callables
};

// CHANGE THIS FUNCTIONS NAME!!! if renaming the module
PyMODINIT_FUNC PyInit_conditions(void) {
    import_array(); // For np
    return PyModule_Create(&conditions);
}