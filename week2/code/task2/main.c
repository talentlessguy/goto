#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  struct stat file_stat;
  stat("file.txt", &file_stat);

  off_t file_size = file_stat.st_size;

  int page_size = getpagesize();
  if (file_size % page_size)
  {
    file_size += page_size - (file_size % page_size);
  }

  int file = open("file.txt", O_RDWR, 0);

  void *mapped = mmap(
      NULL,
      file_size,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      file,
      0);

  char *array = mapped;

  int *x = (int *)mapped;

  printf("%d", *x);

  scanf("%d", x);

  munmap(mapped, file_size);
  close(file);
  return 0;
}