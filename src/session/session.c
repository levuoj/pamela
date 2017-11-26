/*
** session.c for  in /home/levuoj/rendu/pamela/src/session
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:45:54 2017 Jouvel Anthony
** Last update Fri Nov 24 14:23:00 2017 Jouvel Anthony
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <security/pam_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <stdbool.h>
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
      if (create_passphrase(login) == -1)
	return (PAM_SESSION_ERR);
      if (encrypt_passphrase(password, login) == -1)
	return (PAM_SESSION_ERR);
    }
  if (decrypt_passphrase(password, login) == -1)
    return (PAM_SESSION_ERR);
  free(path);
  return (PAM_SUCCESS);
}

static int	luks_creation(const char *login,
			      const char *path)
{
  char		*command = NULL;
  struct passwd	*pass = NULL;
  
  if (asprintf(&command, "fallocate -l 2G %s",
	       path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  pass = getpwnam(login);
  if (asprintf(&command, "chown %s:%d %s", login, pass->pw_gid, path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (asprintf(&command,
	       "cat /home/%s/.pass |cryptsetup luksFormat -c aes -h sha256 %s",
	       login, path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  return (PAM_SUCCESS);
}

static int	luks_open(const char *login,
			  const char *path,
			  bool first)
{
  char		*command = NULL;
  char		*login_volume = NULL;
  char		*second_path = NULL;
  struct passwd	*pass = NULL;
  struct stat	sb;

  if (asprintf(&login_volume, "%s_volume", login) == -1)
    return (PAM_SESSION_ERR);
  if (asprintf(&second_path, "/home/%s/.pass", login) == -1)
    return (PAM_SESSION_ERR);
  if (asprintf(&command,
	       "cat %s |cryptsetup luksOpen %s %s",
	       second_path, path, login_volume) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  remove(second_path);
  free(command);
  free(second_path);
  if (asprintf(&second_path, "/home/%s/secure_data-rw", login) == -1)
    return (PAM_SESSION_ERR);
  if (stat(second_path, &sb) == -1)
    mkdir(second_path, 0700);
  if (first == true)
    {
      if (asprintf(&command,
		   "mkfs.ext4 -F /dev/mapper/%s", login_volume) == -1)
	return (PAM_SESSION_ERR);
      if (execute_command(command) == -1)
	return (PAM_SESSION_ERR);
      free(command);
    }
  if (asprintf(&command,
	       "mount -t ext4 /dev/mapper/%s %s", login_volume, second_path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  pass = getpwnam(login);
  if (asprintf(&command, "chown %s:%d %s", login, pass->pw_gid, second_path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  free(second_path);
  return (PAM_SUCCESS);
}


PAM_EXTERN int		pam_sm_open_session(pam_handle_t *pamh,
					    int flags,
					    int argc,
					    const char **argv)
{
  UNUSED(flags);
  UNUSED(argc);
  UNUSED(argv);
  const void	*password = NULL;
  const void	*login = NULL;
  char		*path = NULL;
  int		ret_value;

  if ((ret_value = pam_get_data(pamh, "PASSWORD", &password)) != PAM_SUCCESS)
    return (ret_value);
  if ((ret_value = pam_get_item(pamh, PAM_USER, &login)) != PAM_SUCCESS)
    return (ret_value);
  if (passphrase_section(password, login) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  if (asprintf(&path, "/home/%s/encrypted_volume", (char*)login) == -1)
    return (PAM_SESSION_ERR);
  if (access(path, F_OK) == -1)
    {
      if (luks_creation(login, path) == PAM_SESSION_ERR)
	return (PAM_SESSION_ERR);
      if (luks_open(login, path, true) == PAM_SESSION_ERR)
	return (PAM_SESSION_ERR);
    }
  else
    {
      if (luks_open(login, path, false) == PAM_SESSION_ERR)
	return (PAM_SESSION_ERR);
    }
  return (PAM_SUCCESS);
}

static int	luks_close(const char *login)
{
  char		*path = NULL;
  char		*command = NULL;

  if (asprintf(&path, "/home/%s/secure_data-rw/", login) == -1)
    return (PAM_SESSION_ERR);
  if (asprintf(&command, "umount %s", path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  remove(path);
  if (asprintf(&command, "cryptsetup luksClose %s_volume", login) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  free(path);
  return (PAM_SUCCESS);
}

PAM_EXTERN int		pam_sm_close_session(pam_handle_t *pamh,
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
  if (luks_close(login) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  return (PAM_IGNORE);
}
