/*
** utils.c for  in /home/pamela/pamela
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Wed Nov 22 09:54:26 2017 pamela
** Last update Wed Nov 22 09:54:28 2017 pamela
*/

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
      if (waitpid(pid, &status, 0) == -1)
	return (-1);
    }
  else
    {
      if (execlp("/bin/sh", "/bin/sh", "-c", command, NULL) == -1)
	exit(1);
    }
  return (0);
}
