#include <future>
#include <iostream>
#include <experimental/coroutine>

using namespace std;

template<class T>
struct future_wrapper {
  struct promise_type {
    void return_value(T value) {
      cout << "return_value" << endl;
      m_promise.set_value(value);
    }
    future_wrapper<T> get_return_object() {
      cout << "get_return_object" << endl;
      return {m_promise.get_future()};
    }
    std::experimental::suspend_never initial_suspend() const {
      cout << "initial_suspend" << endl;
      return {};
    }
    std::experimental::suspend_never final_suspend() const {
      cout << "final_suspend" << endl;
      return {};
    }

  private:
    promise<T> m_promise;
  };

  future_wrapper(future<T>& future) : m_future(std::move(future)) { }

  T get() {
    return m_future.get();
  }

  void wait() const {
    m_future.wait();
  }

private:
  future<T> m_future;
};

future_wrapper<int> f() {
  co_return 42;
}

int main() {
  auto t = f();
  t.wait();
  cout << "Answer = " << t.get() << endl;
}
