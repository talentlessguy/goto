#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

class stack
{
private:
  vector<int> items_;
  int last_cell;

public:
  stack(int size) : items_(size), last_cell(0) {}

  int push(int x)
  {
    assert(items_.size() >= last_cell + 1);

    last_cell++;

    items_[last_cell] = x;
    return x;
  }
  int pop()
  {
    assert(last_cell - 1 >= 0);

    return items_[last_cell--];
  }
  int top()
  {
    assert(last_cell - 1 >= 0);

    return items_[last_cell];
  }
};

int main()
{
  stack s(3);

  s.push(10);

  cout << s.pop() << endl;

  s.push(128);

  cout << s.pop() << endl;

  s.push(10);
  s.push(10);
  s.push(10);
  s.push(10);

  return 0;
}