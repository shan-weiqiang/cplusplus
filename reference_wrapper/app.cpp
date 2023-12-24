#include <functional>
#include <iostream>
#include <vector>

int global = 90;

void print_vector(const std::vector<std::reference_wrapper<int>>& vector)
{

	/// using auto drops const, volatile qualifiers, for elements of vector
	for(auto item : vector)
	{
		/// implicit conversion from std::reference_wrapper<int> to int&
		std::cout << item << '\t';

		/// this changes the argument vector
		item = std::reference_wrapper<int>(global);
	}

	for(auto it = vector.begin(); it != vector.end(); ++it)
	{

		/// NOK, because *it is const
		// *it = std::reference_wrapper<int>(global);

		/// vector is const, so item itself is const, but get() function returns the
		/// reference type, which is int&, meaning that it can be changed!!
		++it->get();
	}
}

void print_const_vector(const std::vector<std::reference_wrapper<const int>>& const_vector)
{
	/// using auto drops const, volatile qualifiers, for elements of vector, not
	/// for template type of reference_wrapper
	for(auto item : const_vector)
	{
		/// implicit conversion from std::reference_wrapper<int> to const int&
		std::cout << item << '\t';

		/// this changes the argument vector
		item = std::reference_wrapper<const int>(global);

		/// NOK, because now it->get() is const
		// ++item.get();
	}

	for(auto it = const_vector.begin(); it != const_vector.end(); ++it)
	{

		/// NOK, because *it is const
		// *it = std::reference_wrapper<int>(global);

		/// NOK, because now it->get() is const
		// ++it->get();
	}
}

int main()
{
	int int_arr[100];

	for(int i = 0; i < 100; ++i)
	{
		int_arr[i] = i;
	}

	std::vector<std::reference_wrapper<int>> ref_int;
	std::vector<std::reference_wrapper<const int>> const_ref_int;

	for(int i = 0; i < 100; ++i)
	{

		/// implicit conversrion form int to std::reference_wrapper<int>
		ref_int.push_back(int_arr[i]);
		const_ref_int.push_back(int_arr[i]);
	}

	print_vector(ref_int);
	std::cout << std::endl;
	print_vector(ref_int);
	std::cout << std::endl;
	print_const_vector(const_ref_int);
}