#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

#define WORKERS 4

#define MAX_READERS 5

std::vector<int> numbers;

std::mutex mutex;

int CURRENT_READERS = 0;

bool IS_WRITER_ACTIVE = false;

void writer_routine()
{
  IS_WRITER_ACTIVE = true;
  for (int i = 0; i < 10; i++)
  {
    mutex.lock();
    numbers.push_back(1);
    std::cout << "add one" << std::endl;
    mutex.unlock();
  }
  IS_WRITER_ACTIVE = false;
}

void reader_routine()
{
  CURRENT_READERS++;
  for (int i = 0; i < 10; i++)
  {
    mutex.lock();
    int sum = 0;
    for (int j = 0; j < numbers.size(); ++j)
    {
      sum += numbers[i];
    }
    std::cout << "sum is " << sum << std::endl;
    mutex.unlock();
  }
}

int main()
{
  std::vector<std::thread> workers;

  workers.reserve(WORKERS);

  for (size_t i = 0; i < WORKERS; ++i)
  {
    if (!IS_WRITER_ACTIVE)
    {
      workers.emplace_back(writer_routine);
    }

    if (CURRENT_READERS < 5)
    {
      workers.emplace_back(reader_routine);
    }
  }

  for (auto &worker : workers)
  {
    worker.join();
  }
}