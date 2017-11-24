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
      printf("je créer la passphrase\n");
      fflush(stdin);
      if (create_passphrase(login) == -1)
	return (PAM_SESSION_ERR);
      if (encrypt_passphrase(password, login) == -1)
	return (PAM_SESSION_ERR);
    }
  printf("je vais déchiffrer la passphrase\n");
  fflush(stdin);
  if (decrypt_passphrase(password, login) == -1)
    return (PAM_SESSION_ERR);
  free(path);
  return (PAM_SUCCESS);
}

static int	luks_creation(const char *password,
			      const char *login,
			      const char *path)
{
  char		*command = NULL;

  (void)password;
  (void)login;
  if (asprintf(&command, "fallocate -l 2G %s",
	       path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  printf("j'ai créé le conteneur\n");
  fflush(stdin);
  free(command);
  if (asprintf(&command,
	       "cat /home/%s/.pass |sudo cryptsetup luksFormat -c aes -h sha256 %s",
	       login, path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  printf("j'ai chiffrer le conteneur\n");
  fflush(stdin);
  free(command);
  return (PAM_SUCCESS);
}

static int	luks_open(const char *login,
			  const char *path)
{
  char		*command = NULL;
  char		*login_volume = NULL;
  struct stat	sb;

  if (asprintf(&login_volume, "%s_volume", login) == -1)
    return (PAM_SESSION_ERR);
  if (asprintf(&command,
	       "cat /home/%s/.pass |sudo cryptsetup luksOpen %s %s",
	       login, path, login_volume) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  printf("j'ai ouvert le conteneur\n");
  fflush(stdin);
  free(command);

  char		*mount_path = NULL;
  if (asprintf(&mount_path, "/home/%s/secure_data-rw", login) == -1)
    return (PAM_SESSION_ERR);
  if (stat(mount_path, &sb) == -1)
    {
      mkdir(mount_path, 0700);

      struct passwd	*pass = NULL;

      pass = getpwnam(login);
      chown(mount_path, pass->pw_uid, pass->pw_gid);
    }
  if (asprintf(&command,
	       "sudo mkfs.ext4 -F /dev/mapper/%s", login_volume) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  free(command);
  if (asprintf(&command,
	       "sudo mount -t ext4 /dev/mapper/%s %s", login_volume, mount_path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  printf("j'ai mount le conteneur\n");
  fflush(stdin);
  free(command);
  free(mount_path);
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
  printf("login = %s\n", (char*)login);
  printf("password = %s\n", (char*)password);
  fflush(stdin);
  if (passphrase_section(password, login) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  if (asprintf(&path, "/home/%s/encrypted_volume", (char*)login) == -1)
    return (PAM_SESSION_ERR);
  if (access(path, F_OK) == -1)
    {
      if (luks_creation(password, login, path) == PAM_SESSION_ERR)
	return (PAM_SESSION_ERR);
    }
  if (luks_open(login, path) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  return (PAM_SUCCESS);
}

static int	luks_close(const char *login)
{
  char		*path = NULL;
  char		*command = NULL;

  if (asprintf(&path, "/home/%s/secure_data-rw/", login) == -1)
    return (PAM_SESSION_ERR);
  if (asprintf(&command, "sudo umount %s", path) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  printf("j'ai demonter le conteneur\n");
  fflush(stdin);
  free(command);
  remove(path);
  if (asprintf(&command, "sudo cryptsetup luksClose %s_volume", login) == -1)
    return (PAM_SESSION_ERR);
  if (execute_command(command) == -1)
    return (PAM_SESSION_ERR);
  printf("j'ai close le volume\n");
  fflush(stdin);
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
  printf("login = %s\n", (char*)login);
  printf("password = %s\n", (char*)password);
  fflush(stdin);
  if (luks_close(login) == PAM_SESSION_ERR)
    return (PAM_SESSION_ERR);
  return (PAM_IGNORE);
}
