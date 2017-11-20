/*
** test.c for  in /home/levuoj/rendu/pamela
**
** Made by Jouvel Anthony
** Login   <anthony.jouvel@epitech.eu>
**
** Started on  Mon Nov 20 16:43:30 2017 Jouvel Anthony
** Last update Mon Nov 20 17:18:46 2017 Jouvel Anthony
*/

#include "pamela.h"

int			main()
{
  pam_handle_t		*pamh;
  struct pam_conv	pamc;

  pamc.conv = &misc_conv;
  pamc.appdata_ptr = NULL;
  pam_start ("su", getenv ("USER"), &pamc, &pamh);
  if (pam_authenticate (pamh, 0) != PAM_SUCCESS)
    fprintf (stderr, "Échec de l'authentification !\n");
  else
    fprintf (stderr, "Authentification OK.\n");
  pam_end (pamh, 0);
  return 0;
}
