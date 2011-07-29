#include <Python.h>
#include <string.h>
#include "spud.h"
#include <stdio.h>


#define MAXLENGTH   2048


static PyObject *SpudError;
static PyObject *SpudTypeError;
static PyObject *SpudKeyError;
static PyObject *SpudFileError;
static PyObject *SpudNewKeyWarning;

void* manager;

static PyObject*
error_checking(int outcome, char *functionname)
{
    char errormessage [MAXLENGTH];
    if (outcome == SPUD_KEY_ERROR){
        snprintf(errormessage, MAXLENGTH, "Error: key error in %s", functionname);
        PyErr_SetString(SpudKeyError, errormessage);
        return NULL;
    }
    if (outcome == SPUD_TYPE_ERROR){
        snprintf(errormessage, MAXLENGTH, "Error: Type error in %s", functionname);
        PyErr_SetString(SpudTypeError, errormessage);
        return NULL;
    }
    if (outcome == SPUD_NEW_KEY_WARNING){
        snprintf(errormessage, MAXLENGTH, "Warning: New key warning in %s", functionname);
        PyErr_SetString(SpudNewKeyWarning, errormessage);
        return NULL;
    }
    if (outcome == SPUD_FILE_ERROR){
        snprintf(errormessage, MAXLENGTH, "Error: File does not exist in %s", functionname);
        PyErr_SetString(SpudFileError, errormessage);
        return NULL;
    }

    Py_RETURN_NONE;
}




static PyObject *
libspud_load_options(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int outcomeLoadOptions;

    if (!PyArg_ParseTuple(args, "s", &key))
        return NULL;
    key_len = strlen(key);
    outcomeLoadOptions = spud_load_options(key,key_len);

    return error_checking(outcomeLoadOptions, "load options");
}

static PyObject*
libspud_print_options(PyObject *self, PyObject *args)
{
    spud_print_options();

    Py_RETURN_NONE;
}

static PyObject *
libspud_get_number_of_children(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int child_count;
    int outcomeGetNumChildren;

    if (!PyArg_ParseTuple(args, "s", &key))
        return NULL;
    key_len = strlen(key);
    outcomeGetNumChildren = spud_get_number_of_children(key, key_len, &child_count);
    if (error_checking(outcomeGetNumChildren, "get number of children") == NULL){
        return NULL;
    }

    return Py_BuildValue("i", child_count);
}

static PyObject *
libspud_get_child_name(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int index;
    char child_name [MAXLENGTH];
    int i;
    int outcomeGetChildName;

    for (i = 0; i < MAXLENGTH; i++){
        child_name[i] = '\0';
    }
    if (!PyArg_ParseTuple(args, "si", &key, &index)){
        return NULL;
    }
    key_len = strlen(key);
    outcomeGetChildName = spud_get_child_name(key, key_len, index, child_name, MAXLENGTH);
    if (error_checking(outcomeGetChildName, "get child name") == NULL){
        return NULL;
    }

    return Py_BuildValue("s", child_name);
}

static PyObject *
libspud_option_count(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int numoptions;

    if (!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    numoptions = spud_option_count(key, key_len);

    return Py_BuildValue("i", numoptions);
}

static PyObject *
libspud_have_option(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int haveoption;

    if (!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    haveoption = spud_have_option(key, key_len);

    if (haveoption == 0){
        Py_RETURN_FALSE;
    }
    else{
        Py_RETURN_TRUE;
    }
}

static PyObject *
libspud_add_option(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int outcomeAddOption;

    if (!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    outcomeAddOption = spud_add_option(key, key_len);
    return error_checking(outcomeAddOption, "add option");

}

static PyObject *
libspud_get_option_type(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int type;
    int outcomeGetOptionType;

    if (!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    outcomeGetOptionType = spud_get_option_type(key, key_len, &type);
    if (error_checking(outcomeGetOptionType, "get option type") == NULL){
        return NULL;
    }
    if (type == SPUD_DOUBLE){
        Py_XINCREF(&PyFloat_Type);
        return (PyObject*) &PyFloat_Type;
    }
    else if (type == SPUD_INT){
        Py_XINCREF(&PyInt_Type);
        return (PyObject*) &PyInt_Type;
    }
    else if (type == SPUD_NONE){
        Py_RETURN_NONE;
    }
    else if (type == SPUD_STRING){
        Py_XINCREF(&PyString_Type);
        return (PyObject*) &PyString_Type;
    }

    PyErr_SetString(SpudError,"Error: Get option type function failed");
    return NULL;
}

static PyObject *
libspud_get_option_rank(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int rank;
    int outcomeGetOptionRank;

    if (!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    outcomeGetOptionRank = spud_get_option_rank(key, key_len, &rank);
    if (error_checking(outcomeGetOptionRank, "get option rank") == NULL){
        return NULL;
    }

    return Py_BuildValue("i", rank);
}

static PyObject *
libspud_get_option_shape(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int shape[2];
    int outcomeGetOptionShape;

    if (!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    outcomeGetOptionShape = spud_get_option_shape(key, key_len, shape);
    if (error_checking(outcomeGetOptionShape, "get option shape") == NULL){
        return NULL;
    }

    return Py_BuildValue("(i,i)", shape[0],shape[1]);
}

static PyObject*
spud_get_option_aux_list_ints(const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for getting option when the option is of type a list of ints
    int outcomeGetOption;
    int size = shape[0];
    int val [size];
    int j;

    outcomeGetOption = spud_get_option(key, key_len, val);
    if (error_checking(outcomeGetOption, "get option aux list") == NULL){
        return NULL;
    }
    PyObject* pylist = PyList_New(size);
    if (pylist == NULL){
        printf("New list error.");
        return NULL;
    }
    for (j = 0; j < size; j++){
        PyObject* element = Py_BuildValue("i", val[j]);
        PyList_SetItem(pylist, j, element);
    }

    return pylist;
}

static PyObject*
spud_get_option_aux_list_doubles(const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for getting option when the option is of type a list of doubles
    int outcomeGetOption;
    int size = shape[0];
    double val [size];
    int j;

    outcomeGetOption = spud_get_option(key, key_len, val);
    if (error_checking(outcomeGetOption, "get option aux list") == NULL){
        return NULL;
    }
    PyObject* pylist = PyList_New(size);
    if (pylist == NULL){
        printf("New list error.");
        return NULL;
    }
    for (j = 0; j < size; j++){
        PyObject* element = Py_BuildValue("f", val[j]);
        PyList_SetItem(pylist, j, element);
    }

    return pylist;
}

static PyObject *
spud_get_option_aux_scalar_or_string(const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for getting option when the option is of type a scalar or string
    int outcomeGetOption;

    if (type == SPUD_DOUBLE ){
        float val;
        outcomeGetOption = spud_get_option(key, key_len, &val);
        if (error_checking(outcomeGetOption, "get option aux scalar or string") == NULL){
            return NULL;
        }
        return Py_BuildValue("f", val);
    }
    else if (type == SPUD_INT ){
        int val;
        outcomeGetOption = spud_get_option(key, key_len, &val);
        if (error_checking(outcomeGetOption, "get option aux scalar or string") == NULL){
            return NULL;
        }
        return Py_BuildValue("i", val);
    }
    else if (type == SPUD_STRING) {
        int size = shape[0];
        char val [size];
        outcomeGetOption = spud_get_option(key, key_len, val);
        if (error_checking(outcomeGetOption, "get option aux scalar or string") == NULL){
            return NULL;
        }
        return Py_BuildValue("s", val);
    }

    PyErr_SetString(SpudError,"Error: Get option aux scalar failed");
    return NULL;
}

static PyObject*
spud_get_option_aux_tensor_doubles(const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for getting option when the option is of type a tensor
    int outcomeGetOption;
    int rowsize = shape[0];
    int colsize = shape[1];
    int size = rowsize*colsize;
    double val [size];
    int m;
    int n;
    int counter;

    outcomeGetOption = spud_get_option(key, key_len, val);
    if (error_checking(outcomeGetOption, "get option aux tensor") == NULL){
        return NULL;
    }
    PyObject* pylist = PyList_New(rowsize);
    if (pylist == NULL){
        printf("New list error");
        return NULL;
    }
    counter = 0;
    for (m = 0; m < rowsize; m++){
        PyObject* pysublist = PyList_New(colsize);
        if (pysublist == NULL){
            printf("New sublist error");
            return NULL;
        }
        for (n = 0; n < colsize; n++){
            PyObject* element = Py_BuildValue("f", val[counter]);
            PyList_SetItem(pysublist, n, element);
            counter++;
        }
        PyList_SetItem(pylist, m, pysublist);
    }

    return pylist;
}

static PyObject *
libspud_get_option(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int type;
    int rank = 0;
    int shape[2];
    int outcomeGetOptionType;
    int outcomeGetOptionRank;
    int outcomeGetOptionShape;

    if(!PyArg_ParseTuple(args, "s", &key)){
        return NULL;
    }
    key_len = strlen(key);
    outcomeGetOptionRank = spud_get_option_rank(key, key_len, &rank);
    if (error_checking(outcomeGetOptionRank, "get option") == NULL){
        return NULL;
    }
    outcomeGetOptionType = spud_get_option_type(key, key_len, &type);
    if (error_checking(outcomeGetOptionType, "get option") == NULL){
        return NULL;
    }
    outcomeGetOptionShape = spud_get_option_shape(key, key_len, shape);
    if (error_checking(outcomeGetOptionShape, "get option") == NULL){
        return NULL;
    }
    if (rank == 1 && type == 1){ //a list of ints
        return spud_get_option_aux_list_ints(key, key_len, type, rank, shape);
    }
    else if (rank == 1 && type == 0){ //a list of doubles
        return spud_get_option_aux_list_doubles(key, key_len, type, rank, shape);
    }
    else if (rank == 2 && type == 0){ //a tensor of doubles
        return spud_get_option_aux_tensor_doubles(key, key_len, type, rank, shape);
    }
    else if (rank == 0 || type == 3){ // scalar or string
        return spud_get_option_aux_scalar_or_string(key, key_len, type, rank, shape);
    }
    else if (rank == -1){
        PyErr_SetString(SpudTypeError,"Error: Type error in get option.");
        return NULL;
    }

    PyErr_SetString(SpudError,"Error: Get option failed.");
    return NULL;
}
static PyObject*
set_option_aux_list_ints(PyObject *pylist, const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for setting option when the second argument is of type a list of ints
    int j;
    int psize = PyList_Size(pylist);
    int val [psize];
    int outcomeSetOption;
    int element;

    for (j = 0; j < psize; j++){
        element = -1;
        PyObject* pelement = PyList_GetItem(pylist, j);
        PyArg_Parse(pelement, "i", &element);
        val[j] = element;
    }
    outcomeSetOption = spud_set_option(key, key_len, val, type, rank, shape);
    if (error_checking(outcomeSetOption, "set option aux list ints") == NULL){
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject*
set_option_aux_string(PyObject *pystring, const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for setting option when the second argument is of type string
    char *val = PyString_AsString(pystring);
    int outcomeSetOption = spud_set_option(key, key_len, val, type, rank, shape);
    return error_checking(outcomeSetOption, "set option aux string");
}

static PyObject*
libspud_set_option_attribute(PyObject *self, PyObject *args)
{
    const char*key;
    int key_len;
    PyObject* firstArg;
    PyObject* secondArg;
    const char*val;
    int val_len;
    int outcomeSetOption;

    firstArg = PyTuple_GetItem(args, 0);
    secondArg = PyTuple_GetItem(args, 1);
    PyArg_Parse(firstArg, "s", &key);
    key_len = strlen(key);
    PyArg_Parse(secondArg, "s", &val);
    val_len = strlen(val);
    outcomeSetOption = spud_set_option_attribute(key, key_len, val, val_len);
    return error_checking(outcomeSetOption, "set option attribute");
}
static PyObject*
libspud_delete_option(PyObject *self, PyObject *args)
{
    const char*key;
    int key_len;
    PyObject* firstArg;
    int outcomeDeleteOption;

    firstArg = PyTuple_GetItem(args, 0);
    PyArg_Parse(firstArg, "s", &key);
    printf("%s\n",key);
    key_len = strlen(key);
    outcomeDeleteOption = spud_delete_option(key, key_len);
    return error_checking(outcomeDeleteOption, "delete option");
}



static PyObject*
set_option_aux_tensor_doubles(PyObject *pylist, const char *key, int key_len, int type, int rank, int *shape)
{   // this function is for setting option when the second argument is of type a tensor of doubles
    int i;
    int j;
    int counter = 0;
    int pylistsize = PyList_Size(pylist);
    double val [pylistsize];
    int outcomeSetOption;
    int pysublistsize;
    double element;

    for (i = 0; i < pylistsize; i++){
        PyObject* pysublist = PyList_GetItem(pylist, i);
        pysublistsize = PyList_Size(pysublist);
        for (j = 0; j < pysublistsize; j++){
            element = -1.0;
            PyObject* pysublistElement = PyList_GetItem(pysublist, j);
            PyArg_Parse(pysublistElement, "d", &element);
            val[counter] = element;
            counter ++;
        }
    }

    outcomeSetOption = spud_set_option(key, key_len, val, type, rank, shape);
    return error_checking(outcomeSetOption, "set option aux tensor doubles");
}

static PyObject*
libspud_set_option(PyObject *self, PyObject *args)
{
    const char *key;
    int key_len;
    int type;
    int rank;
    int shape[2];
    int outcomeGetRank;
    int outcomeGetShape;
    int outcomeGetType;
    PyObject* firstArg;
    PyObject* secondArg;

    firstArg = PyTuple_GetItem(args, 0);
    secondArg = PyTuple_GetItem(args, 1);
    PyArg_Parse(firstArg, "s", &key);
    key_len = strlen(key);
    outcomeGetType = spud_get_option_type(key, key_len, &type);
    if (error_checking(outcomeGetType, "set option") == NULL){
        return NULL;
    }
    outcomeGetRank = spud_get_option_rank(key, key_len, &rank);
    if (error_checking(outcomeGetRank, "set option") == NULL){
        return NULL;
    }
    outcomeGetShape = spud_get_option_shape(key, key_len, shape);
    if (error_checking(outcomeGetShape, "set option") == NULL){
        return NULL;
    }
    if (PyList_Check(secondArg)){
        if (rank == 1 && type == 1){ // list of ints
            set_option_aux_list_ints(secondArg, key, key_len, type, rank, shape);
        }
        else if (rank == 2 && type == 0){ //tensor of doubles
            set_option_aux_tensor_doubles(secondArg, key, key_len, type, rank, shape);
        }
    }
    else if (PyString_Check(secondArg)){
        set_option_aux_string(secondArg, key, key_len, type, rank, shape);
    }

    Py_RETURN_NONE;
}

static PyObject*
libspud_write_options(PyObject *self, PyObject *args)
{
    PyObject* firstArg;
    char *filename;
    int filename_len;
    int outcomeWriteOptions;

    firstArg = PyTuple_GetItem(args, 0);
    PyArg_Parse(firstArg, "s", &filename);
    filename_len = strlen(filename);
    outcomeWriteOptions = spud_write_options (filename, filename_len);
    return error_checking(outcomeWriteOptions, "write options");
}

static PyMethodDef libspudMethods[] = {
    {"load_options",  libspud_load_options, METH_VARARGS,
     "load options from xml file."},
    {"print_options",  libspud_print_options, METH_VARARGS,
     "print options from xml file."},
    {"get_number_of_children",  libspud_get_number_of_children, METH_VARARGS,
     "get number of children from xml file."},
    {"get_child_name",  libspud_get_child_name, METH_VARARGS,
     "get child name from xml file."},
    {"option_count",  libspud_option_count, METH_VARARGS,
     "option count from xml file."},
    {"have_option",  libspud_have_option, METH_VARARGS,
     "have option from xml file."},
    {"get_option_type",  libspud_get_option_type, METH_VARARGS,
     "get option type from xml file."},
    {"get_option_rank",  libspud_get_option_rank, METH_VARARGS,
     "get option rank from xml file."},
    {"get_option_shape",  libspud_get_option_shape, METH_VARARGS,
     "get option shape from xml file."},
    {"get_option",  libspud_get_option, METH_VARARGS,
     "get option from xml file."},
    {"set_option",  libspud_set_option, METH_VARARGS,
     "set option from xml file."},
    {"write_options",  libspud_write_options, METH_VARARGS,
     "write options from xml file."},
    {"delete_option",  libspud_delete_option, METH_VARARGS,
     "delete option from xml file."},
    {"set_option_attribute",  libspud_set_option_attribute, METH_VARARGS,
     "set option attribute from xml file."},
    {"add_option",  libspud_add_option, METH_VARARGS,
     "add option from xml file."},
    {NULL, NULL, 0, NULL},
            /* Sentinel */
};

PyMODINIT_FUNC
initlibspud(void)
{
    PyObject *m;

    m = Py_InitModule("libspud", libspudMethods);
    if (m == NULL)
        return;

    SpudError = PyErr_NewException("Spud.error", NULL, NULL);
    SpudNewKeyWarning = PyErr_NewException("SpudNewKey.warning", NULL, NULL);
    SpudKeyError = PyErr_NewException("SpudKey.error", NULL, NULL);
    SpudTypeError = PyErr_NewException("SpudType.error", NULL, NULL);
    SpudFileError = PyErr_NewException("SpudFile.error", NULL, NULL);

    Py_INCREF(SpudError);
    Py_INCREF(SpudNewKeyWarning);
    Py_INCREF(SpudKeyError);
    Py_INCREF(SpudTypeError);
    Py_INCREF(SpudFileError);

    PyModule_AddObject(m, "SpudError", SpudError);
    PyModule_AddObject(m, "SpudNewKeyWarning", SpudNewKeyWarning);
    PyModule_AddObject(m, "SpudKeyError", SpudKeyError);
    PyModule_AddObject(m, "SpudTypeError", SpudTypeError);
    PyModule_AddObject(m, "SpudFileError", SpudFileError);

#if PY_MINOR_VERSION > 6
    manager = PyCapsule_Import("fluidity_api._spud_manager", 0);
    if (manager != NULL) spud_set_manager(manager);
#endif
}

