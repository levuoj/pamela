/*
** pam_sm_authenticate.c for  in /home/pamela/pamela/src/auth
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 11:01:46 2017 pamela
** Last update Thu Nov 23 10:29:18 2017 pamela
*/

#include <security/pam_ext.h>
#include <sys/wait.h>
#include "authenticate.h"
#include "unused.h"

static void	_cleanup(pam_handle_t *pamh,
			 void *data,
			 int error_status)
{
  (void)pamh;
  (void)error_status;
  free(data);
}

int		pam_sm_authenticate(pam_handle_t *pamh,
				    int flags,
				    int argc,
				    const char **argv)
{
  UNUSED(flags);
  UNUSED(argc);
  UNUSED(argv);
  const char	*password = NULL;
  int		ret_value;

  if ((ret_value = pam_get_item(pamh, PAM_AUTHTOK, &password)) != PAM_SUCCESS)
    return (ret_value);
  password = strdup(password);
  if (password == NULL)
    return (PAM_AUTH_ERR);
  if (pam_set_data(pamh, "PASSWORD", (char *)password, _cleanup) != PAM_SUCCESS)
    {
      return (PAM_AUTH_ERR);
    }
  return (PAM_SUCCESS);
}
