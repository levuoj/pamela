/*
** pam_sm_authenticate.c for  in /home/pamela/pamela/src/auth
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 11:01:46 2017 pamela
** Last update Tue Nov 21 21:38:43 2017 Jouvel Anthony
*/

#include <security/pam_ext.h>
#include <sys/wait.h>
#include "authenticate.h"
#include "unused.h"

int		pam_sm_authenticate(pam_handle_t *pamh,
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
