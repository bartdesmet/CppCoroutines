#include <future>
#include <iostream>

using namespace std;

future<int> f1() {
  cout << "f" << endl;
  co_return 1;
}

future<int> f2() {
  return async([]() { return 2; });
}

future<int> g() {
  cout << "g0" << endl;
  int x = co_await f1();
  cout << "g1 - x = " << x << endl;
  int y = co_await f2();
  cout << "g2 - y = " << y << endl;
  co_return x + y;
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
