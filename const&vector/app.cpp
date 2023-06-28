// https://stackoverflow.com/questions/6954906/does-c11-allow-vectorconst-t
//
// No, I believe the allocator requirements say that T can be a "non-const, non-reference object type".
//
// You wouldn't be able to do much with a vector of constant objects. And a const vector<T> would be almost the same anyway.
//
// Many years later this quick-and-dirty answer still seems to be attracting comments and votes. Not always up. :-)
//
// So to add some proper references:
//
// For the C++03 standard, which I have on paper, Table 31 in section [lib.allocator.requirements] says:
//
// T, U         any type
// Not that any type actually worked.
//
// So, the next standard, C++11, says in a close draft in [allocator.requirements] and now Table 27:
//
// T, U, C      any non-const, non-reference object type
// which is extremely close to what I originally wrote above from memory. This is also what the question was about.
//
// However, in C++14 (draft N4296) Table 27 now says:
//
// T, U, C      any non-const object type
// Possibly because a reference perhaps isn't an object type after all?
//
// And now in C++17 (draft N4659) it is Table 30 that says:
//
// T, U, C       any cv-unqualified object type (6.9)
// So not only is const ruled out, but also volatile. Probably old news anyway, and just a clarification.
//
#include <iostream>
#include <vector>

int main()
{
  /// C++ do not support vector<const T>
  /// const vector<T> almost have the same effect, the amount of the element is const and the returned element is also const
  const std::vector<int> const_arr;
  /// both of the following code are not compiling
  // const_arr.push_back(4);
  // const_arr.back() = 10;
}
