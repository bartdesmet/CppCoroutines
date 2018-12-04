#include <future>
#include <iostream>
#include <experimental/coroutine>

using namespace std;

template<class T>
struct async_value {
  explicit async_value(T value) : m_value(value) {}

  bool await_ready() const { return true; }
  void await_suspend(std::experimental::coroutine_handle<>) const {}
  T await_resume() const { return m_value; }

private:
  T m_value;
};

async_value<int> f() {
  cout << "f" << endl;
  return async_value<int>{41};
}

future<int> g() {
  cout << "g0" << endl;
  int x = co_await f();
  cout << "g1 - x = " << x << endl;
  co_return x + 1;
}

int main() {
  cout << "main0" << endl;
  future<int> t = g();
  cout << "main1" << endl;
  t.wait();
  cout << "main2" << endl;
  int res = t.get();
  cout << "main3 - res = " << res << endl;
}
