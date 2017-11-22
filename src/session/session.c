/*
** session.c for  in /home/levuoj/rendu/pamela/src/session
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:45:54 2017 Jouvel Anthony
** Last update Wed Nov 22 05:55:14 2017 pamela
*/

#include <security/pam_ext.h>
#include "session.h"
#include "unused.h"

int		pam_sm_open_session(pam_handle_t *pamh,
				    int flags,
				    int argc,
				    const char **argv)
{
  UNUSED(flags);
  UNUSED(argc);
  UNUSED(argv);
  const char	*password = NULL;
  int		ret_value;

  if ((ret_value = pam_get_authtok(pamh, PAM_AUTHTOK, &password, NULL)) != PAM_SUCCESS ||
      password == NULL)
    return (ret_value);
  printf("password = %s\n", password);
  return (PAM_SUCCESS);
}

int		pam_sm_close_session(pam_handle_t *pamh,
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
