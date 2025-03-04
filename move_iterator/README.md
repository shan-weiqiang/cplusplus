# Move of the element and move of the vector is different

- Using move iterator does not change the state of the vector itself, including size() and it's element. The vector does not feel that it's element being moved at all, even thoese element now are in valid but not specifed state
- Move of the vector itself changes the vector itself to valid but unspecified state. size() is 0. The elements does not feel the move, meaning that the elements does not know that they are now belonging to a new vector
