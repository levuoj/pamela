/*
** converse.c for  in /home/levuoj/rendu/pamela/src
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:38:07 2017 Jouvel Anthony
** Last update Fri Nov 24 14:21:41 2017 Jouvel Anthony
*/

#include "converse.h"
#include "unused.h"

PAM_EXTERN int		converse(int n,
				 const struct pam_message **msg,
				 struct pam_response **resp, void *data)
{
  UNUSED(n);
  UNUSED(msg);
  UNUSED(resp);
  UNUSED(data);
  return (PAM_CONV_ERR);
}
