## C++ named requirements: Container

https://en.cppreference.com/w/cpp/named_req/Container

## comparsion matrix

|              | `std::array`                                                 | `std::string`                                                | `std::vector`                                                | `std::deque`                                                 | `std::queue`                 | `std::list`                                                  | `std::forward_list`                                          | `std::map`/`std::set`                                        | `std::unordered_map`/`std::unordered_set`                    |
| ------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| memory model | - fixed size, no dynamical memory allocation<br />- elements memory allocated together with container itself | - automatically manage memory dynamically<br />- memory is contiguous | - automatically manage memory dynamically<br />- memory is contiguous<br />- all memroy might be reallocated if size is out of current memory allocation | - memory is not contiguous, and is managed as blocks of fixed-sized arrays<br />- memory blocks is allocated dynamically<br />- constant time at insertion and deletion at both beginning or ending<br />- like `std::vector`, can be accessed using `[]` operator | - FIFO based on `std::deque` | - dynamically allocate memory for every node<br />- every node has pointers to both previous and next node<br />- no memory reallocation in any insertion for other nodes | - nodes only have pointer to next node, no pointer to previous node | - memory is dynamically allocated for every ndoe<br />- the internal data structure is red-black tree<br /><br />- for `set`, the key is the element itself | - memory is dynamically allocated for every node<br />- the internal data stucture is hash table(bucket), every key of map and element of set is hashed into one bucket, inside the bucket, the element is stored without order<br />- bucket is nto ordered, element inside bucket is alos not ordered<br />- for `set`, the key is element itself |
| usage notice |                                                              | - modification can possibly invalide any previously created iterators and references<br />- iterator supports `++`, `--`, `+n`,`-n` | - modification can possibly invalide any previously created iterators and references<br />- iterator supports `++`, `--`, `+n`,`-n` | - modification can possibly invalide any previously created iterators and references<br />- iterator supports `++`, `--`, `+n`,`-n` |                              | - iterator supports `++`, `--`, `+n`,`-n`                    | - iterator supports `++`, `+n`                               | - even though the element is ordered, but is not the same as which the element is emplaced<br />- iterator supports `++`,`--`, but not `+n`,`-n`<br />- since the key decides the postion and will change the overal structure of the red-black tree,  user can not change key with iterator, but value can be changed<br />- `set` only have const iterator(iterator and const_iterator are the same type) | - key-value in map and element in set is unordered<br />- iterator supports `++`, not `--`, not `+n`, `-n`, so no `crbegin()/crend()` available<br />- since the key decides the postion and will change the overal structure of the red-black tree,  user can not change key with iterator, but value can be changed<br />- `set` only have const iterator(iterator and const_iterator are the same type) |
|              |                                                              |                                                              |                                                              |                                                              |                              |                                                              |                                                              |                                                              |                                                              |






## difference between deque and list

[What's the difference between deque and list STL containers?](https://stackoverflow.com/questions/1436020/whats-the-difference-between-deque-and-list-stl-containers#:~:text=std%3A%3Alist%20is%20basically,performance%20characteristics%20than%20a%20list.)