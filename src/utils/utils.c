#include "utils.h"

int			execute_command(const char *command)
{
  pid_t			pid;
  int			status;

  pid = fork();
  if (pid == -1)
    return (-1);
  if (pid != 0)
    {
      if (wait(&status) == -1)
	return (-1);
    }
  else
    {
      execlp("/bin/sh", "/bin/sh", "-c", command, NULL);
      return (0);
    }
  return (0);
}
