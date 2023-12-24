#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

/// mutex to synchronize std::cout
std::mutex c_;

class MutexDemo
{
public:
	void cri_section()
	{
		std::lock_guard<std::mutex> lock(m_);
		/// this critical section will be executed by multiple threads that does not
		/// share m_ critical section can only be protected by a specific mutex, if
		/// we have two different instance of MutexDemo, then these instances can
		/// execute this critical setion at the same time
		while(1)
		{
			/// since mutex c_ is shared by all instance of MutexDemo, following code
			/// can only be executed by only one instance. This lock can be used to
			/// sync std::cout
			{
				std::lock_guard<std::mutex> lck(c_);
				std::cout << "from " << std::this_thread::get_id() << '\n';
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

private:
	std::mutex m_;
};

int main()
{
	MutexDemo t1, t2;
	std::thread thread1_ = std::thread(std::bind(&MutexDemo::cri_section, &t1));
	std::thread thread2_ = std::thread(std::bind(&MutexDemo::cri_section, &t2));
	thread1_.join();
	thread2_.join();
}