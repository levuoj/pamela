/*
** password.c for  in /home/levuoj/rendu/pamela/src/password
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:42:37 2017 Jouvel Anthony
** Last update Tue Nov 21 21:40:14 2017 Jouvel Anthony
*/

#include "password.h"
#include "unused.h"

int		pam_sm_chauthtok(pam_handle_t *pamh,
				 int flags,
				 int argc,
				 const char **argv)
{
  UNUSED(pamh);
  UNUSED(flags);
  UNUSED(argc);
  UNUSED(argv);
  return (PAM_IGNORE);
}
