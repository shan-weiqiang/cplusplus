Demonstrate that:

- Destruction order can significantly affect resource allocation and deallocation order and can cause errors like segmentation fault
- When using asio `steady_timer`, it has following behavior:
  - When the timer object is canceled or destructed, if the timer is running in io context, the waiting handlers WILL be called, with `asio::error::operation_aborted` as parameter
  - The canceled handlers WILL be called even after the timer is completely destructed
- It's crucial to properly handle the handler for `steady_timer` in asio C++ when the timer object is destructed:
  - Call `cancel()` before the `steady_timer` object is destructed
    - `cancel()` will only cancel already waiting expiries BEFORE the call, will NOT de-register the timer from io context, after the call new `async_wait` call be called on the same `steady_timer` object
    - destructor of `steady_timer` will call `cancel()` AND de-register the timer from io context
  - Handle `asio::error::operation_aborted` inside handler
  - Note, it's not enough to just call `cancel()`, since this action just trigger the `asio::error::operation_aborted` to be passed as argument to the handler. If the handler ignores the `asio::error::operation_aborted` and goes on to restart the already destructed timer, it's undefined behavior. This because the handler is stored inside `steady_timer` object as callable objects and all the required parameters are stored insided the callable object, which is stored inside the timer. If `steady_timer` is destructed, it means that all the context required to execute the handler is corrupted, for example, if the handler is registered using lamda function, all the captured value by copy will be deallocated, so trying to executed this lamda handler(callable object) results in undefined behavior.
