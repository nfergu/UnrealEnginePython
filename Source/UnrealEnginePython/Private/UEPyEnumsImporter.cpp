#include "UnrealEnginePythonPrivatePCH.h"

static PyObject *ue_PyEnumsImporter_getattro(ue_PyEnumsImporter *self, PyObject *attr_name) {
	PyObject *ret = PyObject_GenericGetAttr((PyObject *)self, attr_name);
	if (!ret) {
		if (PyUnicodeOrString_Check(attr_name)) {
			char *attr = PyUnicode_AsUTF8(attr_name);
			if (attr[0] != '_') {
				UEnum *u_enum = FindObject<UEnum>(ANY_PACKAGE, UTF8_TO_TCHAR(attr));
				if (u_enum) {
					// swallow old exception
					PyErr_Clear();
					ue_PyUObject *u_ret = ue_get_python_wrapper(u_enum);
					if (!u_ret)
						return PyErr_Format(PyExc_Exception, "PyUObject is in invalid state");
					Py_INCREF(u_ret);
					return (PyObject *)u_ret;
				}
			}
		}
	}
	return ret;
}

static PyTypeObject ue_PyEnumsImporterType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.EnumsImporter", /* tp_name */
	sizeof(ue_PyEnumsImporter), /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,       /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	(getattrofunc)ue_PyEnumsImporter_getattro,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,        /* tp_flags */
	"Unreal Engine Enums Importer",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	0,             /* tp_methods */
	0,
	0,
};

void ue_python_init_Enumsimporter(PyObject *ue_module) {
	ue_PyEnumsImporterType.tp_new = PyType_GenericNew;

	if (PyType_Ready(&ue_PyEnumsImporterType) < 0)
		return;

	Py_INCREF(&ue_PyEnumsImporterType);
	PyModule_AddObject(ue_module, "EnumsImporter", (PyObject *)&ue_PyEnumsImporterType);
}

PyObject *py_ue_new_enumsimporter() {
	ue_PyEnumsImporter *ret = (ue_PyEnumsImporter *)PyObject_New(ue_PyEnumsImporter, &ue_PyEnumsImporterType);
	return (PyObject *)ret;
}