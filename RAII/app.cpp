/*!
* RAII和所有权-C++/Rust

* - 堆上的内存通常都直接或者间接的被栈上的变量指向和维护
*
* - C++和Rust都遵循RAII:
*   - C++的move语义不改变原变量的scope：析构函数的调用仍然在生命周期结束时调用
*     区别在于被move的变量析构时内部资源，例如栈内存被转移，不会释放该部分内存
*     这决定C++允许用户自定义资源的处理方式
*   - Rust的所有权改变原变量的scope：原变量的drop不会再调用，资源全部转移到新的
*     scope

* - 栈上和堆上内存所有权转移的区别
*   - 堆上的内存界限清晰，是由所有者最后析构时候释放，这点C++、Rust相同
*   - 栈内存：函数的栈帧是在编译期已经决定，其栈帧的大小，变量的位置等都已确定
*     无论是C++还是Rust，编译器都会在栈上进行优化，具体的栈上变量内存的转移方式
*     由编译器种类和优化级别等相关
*   - 即便是全部内存在栈上的变量，所有权的概念也非常重要，因为：
*     - 对于Rust来说，这决定了drop函数调用的时间，已经编译器栈内存的分布方式
*     - 对于C++来说，会决定编译器的栈内存优化方式，但是C++并没有保证安全性，详见
*       to_outer_as_rvalue()的例子
*     - 最重要的一点：决定了析构代码的执行分支。资源不一定是数据资源，还可能是
*       操作系统资源，比如文件描述符。此时即便是完全在栈上的简单数据也会造成极大
*       的程序运行不同。所以所有权的丢失不仅仅是为了避免数据的拷贝，更重要的是
*       在RAII模式中定义了资源什么时候释放，程序如何运行。例如：
*       @code
          class FileFD{
            public:

            ...
            // 如果被移动，文件描述符资源不会被关闭；否则析构时会关闭文件描述符
            ~FileFD(){
              if (is_valid){
                close(file_descriptor_);
              }
            }
            // other失去对文件描述符的所有权
            FileFD(FileFD&& other){
              this->file_descriptor_ = other.file_descriptor_;
              other.is_valid = false
            }

            ...
            private:
            int file_descriptor_;
            bool is_valid{true};
          };
*       @endcode
*
*
* - C++ unique_ptr和Rust所有权
*   - C++ unique_ptr 相当于Rust的Option<Box<>>
*   - 区别在于C++和Rust在RAII上的区别：C++不改变scope；Rust改变scope
*
*
*/

#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <vector>

template <typename T>
void print_info(const std::vector<T>& value, const std::string& msg)
{
	std::cout << std::setw(40) << std::left << msg << std::setw(30) << &value << std::endl;
	std::cout << std::setw(40) << std::left << msg + ".data" << std::setw(30) << value.data()
			  << std::endl;
}

void to_inner_as_copy(std::vector<int> copy)
{
	print_info(copy, "copy");
}

void to_inner_as_ref(std::vector<int>& ref)
{
	print_info(ref, "ref");
}

void to_inner_as_rvalue(std::vector<int>&& r_value)
{
	print_info(r_value, "r_value");
}

// 因为返回值是右值：
// 在函数返回前会调用ret的析构，且会释放栈和堆上的内存，如果返回值被用来构造新的变量：
// 1. 新变量也会析构，并且释放堆上的内存
// 2. 如果堆上的内存仍然属于进程，则会报double
// free错误，因为runtime已经将该内存标记为free
//    正在等待操作系统回收该片内存
// 3. 如果堆上的内存已经被操作系统回收，则会报segmentation fault
std::vector<int>&& to_outer_as_rvalue()
{
	auto ret = std::vector<int>{1};
	print_info(ret, "ret");
	return std::move(ret);
}

// 在新的栈帧中不会调用from_lvalue的析构函数，而是直接将from_lvalue作为结果返回
// 这里应当时编译器的优化，将返回值放在栈帧的开始，而避免了重复内存操作
std::vector<int> to_outer_as_copy_from_lvalue()
{
	auto from_lvalue = std::vector<int>{1};
	print_info(from_lvalue, "from_lvalue");
	return from_lvalue;
}

// 新的函数栈帧上会调用析构
std::vector<int> to_outer_as_copy_from_rvalue()
{
	auto from_rvalue = std::vector<int>{1};
	print_info(from_rvalue, "from_rvalue");
	return std::move(from_rvalue);
}

int main()
{
	auto outer = std::vector<int>{1};
	print_info(outer, "outer");

	// 变量栈上的内存被拷贝到下一个栈帧；变量堆上部分由变量拷贝构造函数决定处理方式；
	// std::vector会使用元素的拷贝构造，是深拷贝；当该栈帧退出时会调用析构函数
	// 因为新的栈帧上有一个新的变量，当生命周期结束，会自动调用析构
	// 新栈帧上的copy变量的栈上部分的地址和堆上部分的地址都与outer不同
	to_inner_as_copy(outer);
	// 引用传递，没有任何构造和析构；新栈帧上没有新增的变量存储空间；新栈帧上直接使用
	// 外层栈帧上的数据
	to_inner_as_ref(outer);
	// 参数传入是右值，接收方也是右值，中间没有任何构造和析构；新栈帧上的变量直接使用
	// 外层栈帧的数据(行为与使用引用传递一样)；注意：右值传递给右值没有类似左值的构造
	// 过程，右值是直接传递给右值的，如果是左值的话，那要调用拷贝构造，但是这里右值
	// 并没有任何构造过程
	to_inner_as_rvalue(std::move(outer));

	auto outer1 = std::vector<int>{1};
	print_info(outer1, "outer1");

	// copy参数由outer1移动构造而来，outer1栈上的数据会被拷贝到新的栈帧；堆上的数据
	// 被直接转移到新的栈帧上的变量；新栈帧结束会调用copy的析构，同时将堆上的内存释放
	// outer1变量由于被移动，其指向堆的指针为空，在其生命周期结束调用outer1的析构函数
	// 时，不会再次释放，但outer1的析构函数正常调用（这点与Rust的所有权不同，Rust不会
	// 在所有权丢失后调用其析构函数）
	to_inner_as_copy(std::move(outer1));

	std::cout << "\n-----end of to inner test-----\n" << std::endl;

	// double free 错误，或者seg fault，由操作系统决定
	// auto from_inner_as_rvalue = to_outer_as_rvalue();
	// print_info(from_inner_as_rvalue, "from_inner_as_rvalue");

	// 新栈帧和此处的变量无论栈上的数据还是堆上的数据地址相同，说明编译器进行了优化，并没有
	// 做重复的拷贝构造，而是直接将新栈帧的内容合并到了当前栈帧
	auto from_inner_as_copy_lvalue = to_outer_as_copy_from_lvalue();
	print_info(from_inner_as_copy_lvalue, "from_inner_as_copy_lvalue");

	// 当前栈帧变量栈上的地址与内层栈帧上变量地址不同，但是堆上的内存地址一样；编译器也进行
	// 优化：本来应当是一次移动构造+拷贝赋值，但是实际上只做了一次移动构造
	auto from_inner_as_copy_rvalue = to_outer_as_copy_from_rvalue();
	print_info(from_inner_as_copy_rvalue, "from_inner_as_copy_rvalue");
}