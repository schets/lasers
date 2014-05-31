#ifndef TYPE_OPERATORS_HPP
#define TYPE_OPERATORS_HPP
#include <complex>
#include <utility>
#include "float_traits.hpp"
template<int bound, int dim>
    struct dim_check{
        constexpr static bool value = dim > 0 && dim <= bound;
    };
//Getter structs to allow for generic and effecient retrieval of various parameters
//If only c++ allowed partial specialization of functions...
template<class T, int dim>
struct get{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    static inline T& pull(T& inval){
        return inval;
    }
};
template<class T, int dim>
struct get<T[], dim>{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    template<size_t N>
        static inline T& pull(T(& array)[N]){
            static_assert(dim <= N, "Dimension higher than array dimension used");
            return array[dim-1];
        }
};
template<class T, int dim>
struct get<T*, dim>{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    //adios bounds-checking...
    static inline T& pull(T* inval){
        return inval[dim-1];
    }
};


//Need to add support for complex types utilizing non-native underlying type
//in an effecient and generic manner
template<class T, int dim>
struct get<std::complex<T>, dim>{
    static_assert(dim_check<2, dim>::value, "Dimension used outside the range of complex values");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with complex getter");
    //having this function cleans up the code, and decreases the number of specializations
     static  inline T& pull(std::complex<T>& inval){
        //This looks like a bithack, but is promised to work by the c++11 standard
        return ((T*)&inval)[dim-1];
    }
};
template<class T, int dim>
struct get_cref{
    typedef typename float_traits<T>::type real_type;
    const static inline real_type& pull(const T& inval){
        return get<T, dim>(const_cast<T&>(inval));
    }
};
template<class T, int dim>
struct get_val{
    typedef typename float_traits<T>::type real_type;
    static inline real_type pull(const T& inval){
        return get<T, dim>(const_cast<T&>(inval));
    }
};
//applies a function to each dimension along the way down
template<class T, size_t dim = float_traits<T>::dim>
struct apply{
    static_assert(dim >= 1, "A dimension less then one has been given to the apply structure");
    //no dimension hecking here, that should be done by the getter function
    template<class Lambda>
        static inline void map(T& inval, Lambda&& func){
            std::forward<Lambda>(func)(get<T, dim>::pull(inval), dim);
            apply<T, dim-1>::map(inval, std::forward<Lambda>(func));
        }
};
template<class T>
struct apply<T, 1>{
    template<class Lambda>
        static inline void map(T& inval, Lambda&& func){
            std::forward<Lambda>(func)(get<T, 1>::pull(inval), 1);
        }
};
#endif
