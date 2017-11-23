/*
** password.c for  in /home/levuoj/rendu/pamela/src/password
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:42:37 2017 Jouvel Anthony
** Last update Wed Nov 22 08:22:01 2017 pamela
*/

#define _GNU_SOURCE
#include <stdio.h>
#include "password.h"
#include "utils.h"
#include "unused.h"


int		create_passphrase()
{
  char		*command = NULL;

  if (asprintf(&command, "head -c 4000 < /dev/urandom > ~/.pass") == -1)
    return (-1);
  if (execute_command(command) == -1)
    return (-1);
  free(command);
  return (0);
}

int		encrypt_passphrase(const char *password)
{
  char		*command = NULL;

  if (asprintf(&command,
	       "echo %s |openssl aes-256-cbc -a -salt -in ~/.pass -out ~/.pass.enc -pass stdin",
	       password) == -1)
    return (-1);
  remove("~/.pass");
  free(command);
  return (0);
}

int		decrypt_passphrase(const char *password)
{
  char		*command = NULL;
  
  if (asprintf(&command,
	       "echo %s |openssl aes-256-cbc -d -a -in ~/.pass.enc -out ~/.pass -pass stdin",
	       password) == -1)
    return (-1);
  free(command);
  return (0);
}

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
  int		ret_value;

  if ((ret_value = pam_get_item(pamh, PAM_AUTHTOK, &new_password)) != PAM_SUCCESS ||
      new_password == NULL)
    {
      return (ret_value);
    }
  if ((ret_value = pam_get_item(pamh, PAM_OLDAUTHTOK, &old_password)) != PAM_SUCCESS ||
      old_password == NULL)
    {
      return (ret_value);
    }
  if (access("~/.pass", F_OK) == -1)
    {
      if (create_passphrase() == -1)
	return (PAM_AUTHTOK_ERR);
      if (decrypt_passphrase(old_password) == -1)
	return (PAM_AUTHTOK_ERR);
      if (encrypt_passphrase(new_password) == -1)
	return (PAM_AUTHTOK_ERR);
    }
  return (PAM_SUCCESS);
}
