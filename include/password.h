/*
** password.h for  in /home/levuoj/rendu/pamela/include
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:42:52 2017 Jouvel Anthony
** Last update Tue Nov 21 18:43:26 2017 Jouvel Anthony
*/

#ifndef PASSWORD_H_
# define PASSWORD_H_

#ifndef PAM_SM_PASSWORD
# define PAM_SM_PASSWORD
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>

int		decrypt_passphrase(const char *password, const char *login);
int		encrypt_passphrase(const char *password, const char *login);
int		create_passphrase(const char *login);

  
#endif /* !PASSWORD_H_ */
