/*
** password.c for  in /home/levuoj/rendu/pamela/src/password
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:42:37 2017 Jouvel Anthony
** Last update Wed Nov 22 08:22:01 2017 pamela
*/

#include "password.h"
#include "unused.h"

int		pam_sm_chauthtok(pam_handle_t *pamh,
				 int flags,
				 int argc,
				 const char **argv)
{
  UNUSED(flags);
  UNUSED(argc);
  UNUSED(argv);
  const void	*old_password = NULL;
  const void	*new_password = NULL;
  int		ret_value_new;
  int		ret_value_old;

  if ((ret_value_new = pam_get_item(pamh, PAM_AUTHTOK, &new_password)) != PAM_SUCCESS ||
      new_password == NULL)
    {
      return (ret_value_new);
    }
  if ((ret_value_old = pam_get_item(pamh, PAM_OLDAUTHTOK, &old_password)) != PAM_SUCCESS ||
      old_password == NULL)
    {
      return (ret_value_old);
    }
  printf("old password = %s\n", (char*)old_password);
  printf("new password = %s\n", (char*)new_password);
  return (PAM_SUCCESS);
}
