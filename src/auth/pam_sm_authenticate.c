/*
** pam_sm_authenticate.c for  in /home/pamela/pamela/src/auth
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 11:01:46 2017 pamela
** Last update Tue Nov 21 18:25:31 2017 Jouvel Anthony
*/

#include <security/pam_ext.h>
#include <sys/wait.h>
#include "authenticate.h"

PAM_EXTERN int		pam_sm_authenticate(pam_handle_t *pamh,
					    int flags,
					    int argc,
					    const char **argv)
{
  return (PAM_IGNORE);
}
