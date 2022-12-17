# about

Demonstrate that critical code section can only be protected among threads that share the same mutex instance
Demonstrate that data that protected by different mutex is not protected at all. There is still data race between threads that using different mutex
