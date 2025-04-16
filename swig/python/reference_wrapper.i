%define DEFINE_REFERENCE_WRAPPER(RefT, T)

%naturalvar std::reference_wrapper<T>;

%template(RefT) std::reference_wrapper<T>;

%typemap(in) std::reference_wrapper<T> {
  $1 = std::ref(*($input));
}

%typemap(out) std::reference_wrapper<T>& {
  $result = &($1->get());
}

/*
%typemap(in) std::vector<std::reference_wrapper<T>> {
    PyObject* seq = PySequence_Fast($input, "Expected a sequence");
    if (!seq) {
        SWIG_exception_fail(SWIG_TypeError, "Expected a sequence");
    }

    Py_ssize_t len = PySequence_Fast_GET_SIZE(seq);
    $1.reserve(len);

    for (Py_ssize_t i = 0; i < len; ++i) {
        PyObject* item = PySequence_Fast_GET_ITEM(seq, i);
        T* elem = 0;
        int res = SWIG_ConvertPtr(item, (void**)&elem, SWIGTYPE_p_$T, 0);
        if (!SWIG_IsOK(res)) {
            Py_DECREF(seq);
            SWIG_exception_fail(SWIG_TypeError, "Expected a sequence of T elements");
        }
        $1.push_back(std::ref(*elem));
    }

    Py_DECREF(seq);
}

%typemap(out) std::reference_wrapper<T> {
  $result = &($1.get());
}

%typemap(out) std::vector<std::reference_wrapper<T>> {
    int i;
    PyObject *pylist = PyList_New($1.size());
    for (i = 0; i < $1.size(); i++) {
        PyObject *pyobj = SWIG_NewPointerObj((void *) &$1[i].get(), SWIGTYPE_p_$T, 0);
        PyList_SetItem(pylist, i, pyobj);
    }
    $result = pylist;
}
*/





%enddef














