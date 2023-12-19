// 헤더파일 포함
#include <Python.h>

void initialize(char* module_name, PyObject **pModule)
{
    // Python 인터프리터 초기화
    Py_Initialize();
    // PyUnicode_DecodeFSDefault: 모듈이름을 python 유니코드 객체로 변환
    PyObject *pName = PyUnicode_DecodeFSDefault(module_name);
    // PyImport_Import: 지정된 모듈을 import
    *pModule = PyImport_Import(pName);
    // 참조 카운트 감소
    Py_DECREF(pName);
}

void finalize(PyObject *pModule)
{
    // 참조 카운트 감소
    Py_DECREF(pModule);
    // Python 인터프리터 종료
    Py_FinalizeEx();
}

// Python의 make_string함수를 호출
void make_string(PyObject *pModule, char *arg)
{
    PyObject *pFunc, *pArgs, *pValue;
    // pModule에서 "make_string"이름의 함수를 찾는다.
    pFunc = PyObject_GetAttrString(pModule, "make_string");
    // PyTuple_New(1): 함수 인자를 위한 튜플을 생성
    pArgs = PyTuple_New(1);

    PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(arg));
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    printf("Result of call: %s \n", PyUnicode_AsUTF8(pValue));
    Py_DECREF(pValue);
    Py_DECREF(pFunc);
}


void make_int(PyObject *pModule, int arg)
{
    PyObject *pFunc, *pArgs, *pValue;

    pFunc = PyObject_GetAttrString(pModule, "make_int");
    pArgs = PyTuple_New(1);

    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(arg));
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    printf("Result of call: %d \n", PyLong_AsLong(pValue));
    Py_DECREF(pValue);
    Py_DECREF(pFunc);

}

void make_list(PyObject *pModule)
{
    PyObject *pFunc, *pArgs, *pValue;

    pFunc = PyObject_GetAttrString(pModule, "make_list");

    pValue = PyObject_CallObject(pFunc, NULL);

    printf("Result of call: \n");
    for (int i = 0; i < PyList_Size(pValue); ++i)
    {
        printf("\t%dst item: %s \n", i, PyUnicode_AsUTF8(PyList_GetItem(pValue, i)));
    }
    Py_DECREF(pValue);
    Py_DECREF(pFunc);
}

int main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    initialize(argv[1], &pModule);

    make_string(pModule, "Hello World!");
    make_int(pModule, 99);
    make_list(pModule);

    finalize(pModule);

    return 0;
}
