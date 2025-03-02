#include <memory>
#include <iostream>

class Incomplete;

int main() {
    Incomplete *raw;
    /**
    In file included from /usr/include/c++/11/bits/shared_ptr.h:53,
                     from /usr/include/c++/11/memory:77,
                     from incomplete_type.cpp:1:
    /usr/include/c++/11/bits/shared_ptr_base.h: In instantiation of ‘std::__shared_ptr<_Tp, _Lp>::__shared_ptr(_Yp*) [with _Yp = Incomplete; <template-parameter-2-2> = void; _Tp = Incomplete; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’:
    /usr/include/c++/11/bits/shared_ptr.h:160:46:   required from ‘std::shared_ptr<_Tp>::shared_ptr(_Yp*) [with _Yp = Incomplete; <template-parameter-2-2> = void; _Tp = Incomplete]’
    incomplete_type.cpp:8:46:   required from here
    /usr/include/c++/11/bits/shared_ptr_base.h:1102:26: error: invalid application of ‘sizeof’ to incomplete type ‘Incomplete’
     1102 |           static_assert( sizeof(_Yp) > 0, "incomplete type" );
          |                          ^~~~~~~~~~~
    In file included from /usr/include/c++/11/memory:76,
                     from incomplete_type.cpp:1:
    /usr/include/c++/11/bits/unique_ptr.h: In instantiation of ‘void std::default_delete<_Tp>::operator()(_Tp*) const [with _Tp = Incomplete]’:
    /usr/include/c++/11/bits/unique_ptr.h:361:17:   required from ‘std::unique_ptr<_Tp, _Dp>::~unique_ptr() [with _Tp = Incomplete; _Dp = std::default_delete<Incomplete>]’
    incomplete_type.cpp:9:47:   required from here
    /usr/include/c++/11/bits/unique_ptr.h:83:23: error: invalid application of ‘sizeof’ to incomplete type ‘Incomplete’
       83 |         static_assert(sizeof(_Tp)>0,
          |                       ^~~~~~~~~~~
    */

    /**
     * The following code will not compile because the type Incomplete is incomplete.
     * But std::shared_ptr and std::unique_ptr fail for different reasons.
     * As for std::shared_ptr, it requires the complete type of the template argument in constructor.
     *
     * As for std::unique_ptr, it requires the complete type of the template argument in destructor.
     */
    std::shared_ptr<Incomplete> share_ptr(raw);
    std::unique_ptr<Incomplete> unique_ptr(raw);
}