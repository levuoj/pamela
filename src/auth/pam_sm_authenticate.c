/*
** pam_sm_authenticate.c for  in /home/pamela/pamela/src/auth
**
** Made by pamela
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 11:01:46 2017 pamela
** Last update Tue Nov 21 11:31:17 2017 pamela
*/

#include <security/pam_ext.h>
#include <sys/wait.h>
#include "auhthenticate.h"

static int
