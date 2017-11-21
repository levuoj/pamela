/*
** session.c for  in /home/levuoj/rendu/pamela/src/session
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Tue Nov 21 18:45:54 2017 Jouvel Anthony
** Last update Tue Nov 21 18:48:33 2017 Jouvel Anthony
*/

#include "session.h"

int		pam_sm_open_session(pam_handle_t *pamh,
				    int flags,
				    int argc,
				    const char **argv)
{
  return (PAM_IGNORE);
}

int		pam_sm_close_session(pam_handle_t *pamh,
				     int flags,
				     int argc,
				     const char **argv)
{
  return (PAM_IGNORE);
}
