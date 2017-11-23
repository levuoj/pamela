/*
** session.c for  in /home/levuoj/rendu/pamela/src/session
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:45:54 2017 Jouvel Anthony
** Last update Wed Nov 22 05:55:14 2017 pamela
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

static int	passphrase_section(const char *password)
{
  if (access("~/.pass.enc", F_OK) == -1)
    {
      if (create_passphrase() == -1)
	return (PAM_SESSION_ERR);
      if (encrypt_passphrase(password) == -1)
	return (PAM_SESSION_ERR);
    }
  if (decrypt_passphrase(password) == -1)
    return (PAM_SESSION_ERR);
  return (PAM_SUCCESS);
}

static int	luks_creation(const char *password, const char *login)
{
  char		*command = NULL;

  (void)password;
  (void)login;
  if (asprintf(&command, "fallocate -l 2G ~/encrypted_volume") == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (asprintf(&command,
	       "cat ~/.pass |cryptsetup luksFormat ~/encrypted_volume") == -1)
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

  (void)login;
  if (asprintf(&command,
	       "cat ~/.pass |cryptsetup luksOpen ~/encrypted_volume login_volume") == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (stat("~/secure_data-rw", &sb) == -1)
    mkdir("~/secure_data-rw", 0700);
  if (asprintf(&command,
	       "mkfs.ext4 /dev/mapper/login_volume") == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (asprintf(&command,
	       "mount -t ext4 /dev/mapper/login_volume ~/secure_data-rw") == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
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
  const char	*password = NULL;
  const char	*login = NULL;
  int		ret_value;
  
  if ((ret_value = pam_get_authtok(pamh, PAM_AUTHTOK, &password, NULL)) != PAM_SUCCESS ||
      password == NULL)
    return (ret_value);
  printf("password = %s\n", password);
  if (passphrase_section(password) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  if (access("~/encrypted_volume", F_OK) == -1)
    {
      if (luks_creation(password, login) == PAM_SESSION_ERR)
	return (PAM_SESSION_ERR);
    }
  if (luks_open(login) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
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
