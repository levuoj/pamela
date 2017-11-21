/*
** account.c for  in /home/pamela/pamela/src/account
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 10:10:42 2017 pamela
** Last update Tue Nov 21 10:58:42 2017 pamela
*/

#include "account.h"

int		pam_sm_acct_mgmt(pam_handle_t *pamh,
				 int flags,
				 int argc,
				 const char **argv)
{
  return (PAM_IGNORE);
}
