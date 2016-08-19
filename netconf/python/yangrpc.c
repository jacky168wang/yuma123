#include <Python.h>

#include "ncx.h"
#include "ncxmod.h"
#include "ncxtypes.h"
#include "status.h"

#include "procdefs.h"
#include "dlq.h"
#include "rpc.h"
#include "yangrpc.h"

static int yangrpc_init_done = 0;

static PyObject *
py_yangrpc_connect(PyObject *self, PyObject *args)
{
    status_t res;
    char* server;
    int port;
    char* user;
    char* password;
    char* private_key;
    char* public_key;
    yangrpc_cb_ptr_t yangrpc_cb_ptr;


    if (!PyArg_ParseTuple(args, (char *) "sissss:yangrpc_yangrpc_connect", &server,&port,&user,&password,&public_key,&private_key)) {
        return (NULL);
    }
    if(yangrpc_init_done==0) {
        res=yangrpc_init(NULL);
        assert(res==NO_ERR);
        yangrpc_init_done=1;
    }

    res = yangrpc_connect(server, port, user, password, NULL/*public_key*/, NULL/*private_key*/, NULL, &yangrpc_cb_ptr);

    return Py_BuildValue("O", PyCapsule_New(yangrpc_cb_ptr, "yangrpc_cb_ptr_t", NULL));
}

static PyObject *
py_yangrpc_exec(PyObject *self, PyObject *args)
{
    PyObject *py_retval;

    PyObject *py_yangrpc_cb_ptr;
    PyObject *py_rpc_val;

    int res;
    yangrpc_cb_ptr_t yangrpc_cb_ptr;
    val_value_t  *rpc_val;
    val_value_t  *reply_val;

    if (!PyArg_ParseTuple(args, (char *) "OO:yangrpc_yangrpc_exec", &py_yangrpc_cb_ptr, &py_rpc_val)) {
        return (NULL);
    }

    yangrpc_cb_ptr = (val_value_t*)PyCapsule_GetPointer(py_yangrpc_cb_ptr, "yangrpc_cb_ptr_t");
    rpc_val = (val_value_t*)PyCapsule_GetPointer(py_rpc_val, "val_value_t_ptr");

    res = yangrpc_exec(yangrpc_cb_ptr, rpc_val, &reply_val);

    py_retval = PyTuple_New(2);
    PyTuple_SetItem(py_retval, 0, Py_BuildValue("i", (int)res));
    PyTuple_SetItem(py_retval, 1, Py_BuildValue("O", PyCapsule_New(reply_val, "val_value_t_ptr", NULL)));
    return py_retval;
}

static PyObject *
py_yangrpc_parse_cli(PyObject *self, PyObject *args)
{
    PyObject *py_retval;

    PyObject *py_yangrpc_cb_ptr;
    PyObject *py_rpc_val;

    int res;
    yangrpc_cb_ptr_t *yangrpc_cb_ptr;
    val_value_t  *rpc_val;
    char* cmd;    

    if (!PyArg_ParseTuple(args, (char *) "Os:yangrpc_yangrpc_parse_cli", &py_yangrpc_cb_ptr, &cmd)) {
        return (NULL);
    }

    yangrpc_cb_ptr = (val_value_t*)PyCapsule_GetPointer(py_yangrpc_cb_ptr, "yangrpc_cb_ptr_t");

    res = yangrpc_parse_cli(yangrpc_cb_ptr, cmd, &rpc_val);

    py_retval = PyTuple_New(2);
    PyTuple_SetItem(py_retval, 0, Py_BuildValue("i", (int)res));
    PyTuple_SetItem(py_retval, 1, Py_BuildValue("O", PyCapsule_New(rpc_val, "val_value_t_ptr", NULL)));
    return py_retval;
}

/*  define functions in module */
static PyMethodDef yang_rpc_methods[] =
{
     {"connect", py_yangrpc_connect, METH_VARARGS, "connects to client"},
     {"rpc", py_yangrpc_exec, METH_VARARGS, "executes rpc by sending the rpc data contents and blocking until reply is received"},
     {"parse_cli", py_yangrpc_parse_cli, METH_VARARGS, "converts cli string to rpc data contents"},
     {NULL, NULL, 0, NULL}
};

/* module initialization */
PyMODINIT_FUNC
inityangrpc(void)
{
    (void) Py_InitModule("yangrpc", yang_rpc_methods);
}
