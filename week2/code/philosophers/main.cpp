#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#define WORKERS 4

std::vector<int> forks(WORKERS, true);

std::mutex mutex;

int macarons = 100;

void routine(size_t i)
{
  int n = forks.size();

  while (macarons > 0)
  {
    mutex.lock();

    if (forks[i] && forks[(i + 1) % n])
    {
      forks[i] = false;
      forks[(i + 1) % n] = false;
    }

    std::cout << "Philosopher " << i << " took a fork" << std::endl;

    mutex.unlock();

    if (macarons > 0)
    {
      macarons--;
    }

    std::cout << macarons << std::endl;

    mutex.lock();

    forks[i] = true;
    forks[(i + 1) % n] = true;

    std::cout << "Philosopher " << i << " dropped a fork" << std::endl;

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