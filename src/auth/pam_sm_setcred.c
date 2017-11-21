/*
** pam_sm_setcred.c for  in /home/levuoj/rendu/pamela/src/auth
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:05:22 2017 Jouvel Anthony
** Last update Tue Nov 21 18:06:52 2017 Jouvel Anthony
*/

#include <grp.h>
#include "authenticate.h"

PAM_EXTERN int		pam_sm_setcred(pam_handle_t *pamh,
				       int flags,
				       int argc,
				       const char **argv)
{
  return (PAM_IGNORE);
}
