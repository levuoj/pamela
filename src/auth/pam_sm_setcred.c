/*
** pam_sm_setcred.c for  in /home/levuoj/rendu/pamela/src/auth
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:05:22 2017 Jouvel Anthony
** Last update Tue Nov 21 21:39:20 2017 Jouvel Anthony
*/

#include <grp.h>
#include "authenticate.h"
#include "unused.h"

int		pam_sm_setcred(pam_handle_t *pamh,
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
