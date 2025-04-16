// Adapted from https://gist.github.com/vadz/1d7a7c1be4f55fe45b46ea06454349e1

%{
#include <functional>
%}


// Provide simplified declarations of various template classes we use for SWIG.
namespace std {
template <typename T>
class std::reference_wrapper {
 public:

  reference_wrapper(const T& ref );
  reference_wrapper( T&& ref );
  reference_wrapper( reference_wrapper const& );
  reference_wrapper& operator =( reference_wrapper const& x );

  T& get() const;
};
}

#if defined(SWIGPYTHON)
    %include "python/reference_wrapper.i"
#else
    /*#error "No reference_wrapper<> typemaps for this language."*/
#endif

#ifndef DEFINE_REFERENCE_CLASS
%define DEFINE_REFERENCE_CLASS(scope, classname)
DEFINE_REFERENCE_WRAPPER(Optional ## classname, scope::classname)
%enddef
#endif


