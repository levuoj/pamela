/*
** account.c for  in /home/pamela/pamela/src/account
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 10:10:42 2017 pamela
** Last update Fri Nov 24 14:20:43 2017 Jouvel Anthony
*/

#include "account.h"
#include "unused.h"

PAM_EXTERN int		pam_sm_acct_mgmt(pam_handle_t *pamh,
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
