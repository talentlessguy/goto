#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

#define WORKERS 9
/*

[
  [thread1, thread2, null,
  thread3, null, null,
  null, thread4, null]
]

idx = 4
top = idx - 3
bottom = idx + 3
left = idx - 1
right = idx + 1

*/

std::mutex mutex;

std::vector<bool> matrix(9);

int ALIVE_CELLS = 1;

void routine(size_t i)
{
  int n = matrix.size();
  while (ALIVE_CELLS > 0)
  {
    mutex.lock();

    if (matrix[(i - 3) % n] && matrix[(i + 3) % n] && matrix[(i - 1) % n] && matrix[(i + 1) % n])
    {
      ALIVE_CELLS--;
      matrix[i] = false;
    }

    mutex.unlock();

    for (auto cell : matrix)
    {
      std::cout << cell << " ";
    }

    if (ALIVE_CELLS > 0)
      ALIVE_CELLS--;

    mutex.lock();

    ALIVE_CELLS++;
    matrix[rand() % static_cast<int>(9 - 1)] = true;

    mutex.unlock();
  }
}

int main()
{
  std::vector<std::thread> workers;

  workers.reserve(WORKERS);

  for (size_t i = 0; i < WORKERS; ++i)
  {
    workers.emplace_back(routine, i);
  }

  for (auto &worker : workers)
  {
    worker.join();
  }
}