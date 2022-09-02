// Adapted from https://gist.github.com/vadz/1d7a7c1be4f55fe45b46ea06454349e1

%{
#include <optional>
%}

// Provide simplified declarations of various template classes we use for SWIG.
template <typename T>
class std::optional
{
public:
  optional();
  optional(const T& value);
  optional(const optional& other);
  optional& operator=(const optional& other);

  bool has_value() const;

  const T& value() const;
  T& emplace(const T&);
  void reset();
};

#if defined(SWIGPYTHON)
    %include "python/optional.i"
#else
    #error "No OptionalValue<> typemaps for this language."
#endif

// If there is no language-specific DEFINE_OPTIONAL_CLASS() definition, assume
// that we only have "simple" optional values.
#ifndef DEFINE_OPTIONAL_CLASS
%define DEFINE_OPTIONAL_CLASS(scope, classname)
DEFINE_OPTIONAL_SIMPLE(Optional ## classname, scope::classname)
%enddef
#endif
