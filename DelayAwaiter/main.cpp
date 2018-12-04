#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include <experimental/coroutine>

using namespace std;
using namespace std::chrono;

static vector<thread> threads; // NB: Demo leak, avoids need to call join.

template<typename _Rep, typename _Period>
struct delay {
  explicit delay(duration<_Rep, _Period> duration) : m_duration(duration) {}

  bool await_ready() const { return false; }
  void await_suspend(std::experimental::coroutine_handle<> h) const {
    cout << "suspend0" << endl;
    threads.push_back(thread([=](){
      cout << "~" << this_thread::get_id() << " before sleep " << endl;
      this_thread::sleep_for(m_duration);
      cout << "~" << this_thread::get_id() << " after sleep " << endl;
      h.resume();
    }));
    cout << "suspend1" << endl;
  }
  void await_resume() const {
    cout << "resume" << endl;
  }

private:
  duration<_Rep, _Period> m_duration;
};

template<typename _Rep, typename _Period>
delay<_Rep, _Period> make_delay(duration<_Rep, _Period> duration) {
  return delay<_Rep, _Period>{duration};
}

future<void> async() {
  cout << "g0" << endl;
  for (int i = 0; i < 10; i++) {
    cout << "g1 - i = " << i << endl;
    auto d = make_delay(1s);
    co_await d;
  }
  cout << "g2 " << endl;
}

int main() {
  cout << "main0" << endl;
  future<void> t = async();
  cout << "main1" << endl;
  t.wait();
  cout << "main2" << endl;

  for (auto& thread : threads){
    thread.join();
  }
}
