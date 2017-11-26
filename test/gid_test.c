/*
** test.c for  in /home/levuoj/rendu/pamela
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Fri Nov 24 12:25:32 2017 Jouvel Anthony
** Last update Fri Nov 24 14:24:53 2017 Jouvel Anthony
*/

#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>

int		main()
{
  struct passwd	*tmp;
  uid_t		user_id;
  gid_t		group_id;

  tmp = getpwnam("levuoj");
  group_id = tmp->pw_gid;
  user_id = tmp->pw_uid;
  printf("%d - %d\n", group_id, user_id);
}
