/*
** password.c for  in /home/levuoj/rendu/pamela/src/password
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:42:37 2017 Jouvel Anthony
** Last update Thu Nov 23 10:50:29 2017 pamela
*/

#define _GNU_SOURCE
#include <stdio.h>
#include "password.h"
#include "utils.h"
#include "unused.h"


int		create_passphrase(const char *login)
{
  char		*command = NULL;
  char		*path = NULL;

  if (asprintf(&path, "/home/%s/.pass", login) == -1)
    return (-1);
  if (asprintf(&command, "head -c 4000 < /dev/urandom > %s", path) == -1)
    return (-1);
  if (execute_command(command) == -1)
    return (-1);
  free(command);
  return (0);
}

int		encrypt_passphrase(const char *password, const char *login)
{
  char		*command = NULL;
  char		*path = NULL;

  if (asprintf(&path, "/home/%s", login) == -1)
    return (-1);
  if (asprintf(&command,
	       "echo %s |openssl aes-256-cbc -a -salt -in %s/.pass -out %s/.pass.enc -pass stdin",
	       password, path, path) == -1)
    return (-1);
  if (execute_command(command) == -1)
    return (-1);
  free(path);
  if (asprintf(&path, "/home/%s/.pass", login) == -1)
    return (-1);  
  remove(path);
  free(command);
  free(path);
  return (0);
}

int		decrypt_passphrase(const char *password, const char *login)
{
  char		*command = NULL;
  char		*path = NULL;

  if (asprintf(&path, "/home/%s", login) == -1)
    return (-1);
  if (asprintf(&command,
	       "echo %s |openssl aes-256-cbc -d -a -in %s/.pass.enc -out %s/.pass -pass stdin",
	       password, path, path) == -1)
    return (-1);
  if (execute_command(command) == -1)
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
  const void	*login = NULL;
  char		*path = NULL;
  int		ret_value;

  if ((ret_value = pam_get_item(pamh, PAM_AUTHTOK, &new_password)) != PAM_SUCCESS ||
      new_password == NULL)
    return (ret_value);
  if ((ret_value = pam_get_item(pamh, PAM_OLDAUTHTOK, &old_password)) != PAM_SUCCESS ||
      old_password == NULL)
    return (ret_value);
  if ((ret_value = pam_get_item(pamh, PAM_USER, &login)) != PAM_SUCCESS)
    return (ret_value);
  printf("login = %s\n", (char*)login);
  if (asprintf(&path, "/home/%s/.pass", (char *)login) == -1)
    return (PAM_SESSION_ERR);  
  if (access(path, F_OK) == -1)
    {
      if (create_passphrase((char *)login) == -1)
	return (PAM_AUTHTOK_ERR);
      if (decrypt_passphrase(old_password, (char *)login) == -1)
	return (PAM_AUTHTOK_ERR);
      if (encrypt_passphrase(new_password, (char *)login) == -1)
	return (PAM_AUTHTOK_ERR);
    }
  free(path);
  return (PAM_SUCCESS);
}
