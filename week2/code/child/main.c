#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  int fds[2];

  pipe(fds);

  pid_t pid = fork();

  int n;

  if (pid == 0)
  {

    read(fds[0], (void *)(&n), sizeof(int));
    printf("%d", n);
  }
  else
  {
    scanf("%d", &n);
    write(fds[1], (void *)(&n), sizeof(int));
  }
}