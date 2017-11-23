/*
** session.c for  in /home/levuoj/rendu/pamela/src/session
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:45:54 2017 Jouvel Anthony
** Last update Thu Nov 23 10:27:22 2017 pamela
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <security/pam_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "session.h"
#include "password.h"
#include "unused.h"
#include "utils.h"

static int	passphrase_section(const char *password, const char *login)
{
  char		*path = NULL;

  if (asprintf(&path, "/home/%s/.pass.enc", login) == -1)
    return (PAM_SESSION_ERR);  
  if (access(path, F_OK) == -1)
    {
      if (create_passphrase() == -1)
	return (PAM_SESSION_ERR);
      if (encrypt_passphrase(password) == -1)
	return (PAM_SESSION_ERR);
    }
  if (decrypt_passphrase(password) == -1)
    return (PAM_SESSION_ERR);
  free(path);
  return (PAM_SUCCESS);
}

static int	luks_creation(const char *password, const char *login)
{
  char		*command = NULL;

  (void)password;
  (void)login;
  if (asprintf(&command, "fallocate -l 2G /home/%s/encrypted_volume",
	       login) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (asprintf(&command,
	       "cat /home/%s/.pass |cryptsetup luksFormat /home/%s/encrypted_volume",
	       login, login) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  return (PAM_SUCCESS);
}

static int	luks_open(const char *login)
{
  char		*command = NULL;
  struct stat	sb;

  if (asprintf(&command,
	       "cat /home/%s/.pass |cryptsetup luksOpen /home/%s/encrypted_volume %s_volume",
	       login, login, login) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);

  char		*path = NULL;
  if (asprintf(&path, "/home/%s/secure_data-rw", login) == -1)
    return (PAM_SESSION_ERR);
  if (stat(path, &sb) == -1)
    mkdir(path, 0700);
  if (asprintf(&command,
	       "mkfs.ext4 /dev/mapper/login_volume") == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (asprintf(&command,
	       "mount -t ext4 /dev/mapper/login_volume %s", path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  free(path);
  return (PAM_SUCCESS);
}


int		pam_sm_open_session(pam_handle_t *pamh,
				    int flags,
				    int argc,
				    const char **argv)
{
  UNUSED(flags);
  UNUSED(argc);
  UNUSED(argv);
  const void	*password = NULL;
  const void	*login = NULL;
  int		ret_value;

  if ((ret_value = pam_get_data(pamh, "PASSWORD", &password)) != PAM_SUCCESS)
    return (ret_value);
  if ((ret_value = pam_get_item(pamh, PAM_USER, &login)) != PAM_SUCCESS)
    return (ret_value);
  printf("login = %s\n", (char*)login);
  printf("password = %s\n", (char*)password);
  fflush(stdin);
  if (passphrase_section(password, login) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  /*  if (access("~/encrypted_volume", F_OK) == -1)
    {
      if (luks_creation(password, login) == PAM_SESSION_ERR)
	return (PAM_SESSION_ERR);
    }
  if (luks_open(login) == PAM_SESSION_ERR)
  return (PAM_SESSION_ERR); */
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
