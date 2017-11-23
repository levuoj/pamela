#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_appl.h>


/* PAM definitions */
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION


/* PAM user data struct */
typedef struct usrData * udataptr;
struct usrData {
  const char * usrname;
  char * password;
} localUser;


/* Log events to syslog */
static void _pam_log(int err, const char *format, ...)
{
  va_list  args;


  va_start(args, format);
  openlog("PAM-<Name_of_module>", LOG_PID | LOG_NDELAY, LOG_SYSLOG);
  vsyslog(err, format, args);
  va_end(args);
  closelog();
}


/* These two functions re-used from pam_pwdfile.c who re-used them from pam_unix.c */
int converse( pam_handle_t *pamh,
	      int nargs,
	      struct pam_message **message,
	      struct pam_response **response )
{
  int retval;
  struct pam_conv *conv;

  // Begin speaking with PAM, flaged with the PAM_CONV argument
  retval = pam_get_item(pamh, PAM_CONV,  (const void **) &conv ) ;
  if( retval == PAM_SUCCESS ) {
    retval = conv->conv( nargs,
			 ( const struct pam_message ** ) message,
			 response,
			 conv->appdata_ptr );
  }
  return retval;
}


int _set_auth_tok( pam_handle_t *pamh,
		   int flags, int argc,
		   const char **argv )
{
  int retval;
  char *p;


  struct pam_message msg[1],*pmsg[1];
  struct pam_response *resp;


  /* set up conversation call */

  pmsg[0] = &msg[0];
  msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
  msg[0].msg = "Password: ";
  resp = NULL;


  // Call the converse function so we know we are speaking with PAM
  if( ( retval = converse( pamh, 1 , pmsg, &resp ) ) != PAM_SUCCESS )
    return retval;


  if( resp )
    {
      if( ( flags & PAM_DISALLOW_NULL_AUTHTOK ) &&
	  resp[0].resp == NULL )
	{
	  free( resp );
	  return PAM_AUTH_ERR;
	}


      p = resp[ 0 ].resp;

      resp[ 0 ].resp = NULL;
    }
  else
    return PAM_CONV_ERR;


  free( resp );
  // Set our authentication arguments to retrieve username & passsword.
  pam_set_item( pamh, PAM_AUTHTOK, p );
  return PAM_SUCCESS;
}


/* Handle our authentication process */
PAM_EXTERN
int pam_sm_authenticate( pam_handle_t *pamh, int flags, int argc,
			 const char **argv )
{
  int retval, i;
  char user[15];
  const void ** password;


  struct pam_conv *conv = NULL;
  struct pam_message message;
  const struct pam_message *msg;
  struct pam_response **response;


  /* Get our username from PAM */
  if( ( retval = pam_get_user( pamh, user, "PAM_RemoteKRB5 login: " ) != PAM_SUCCESS ) ) {
    _pam_log( LOG_ERR, "Error: %s", pam_strerror( pamh, retval ) );
    return 1;
  } else {
    _pam_log( LOG_ERR, "User: %s", user );
  }

  /* PAM conversion stuff just to get to the bloody password */
  /* get password - code from pam_unix_auth.c */
  pam_get_item( pamh, PAM_AUTHTOK, ( void * ) password );
  if( !password ) {
    // next we call our converse() function from within the _set_auth_tok() function
    retval = _set_auth_tok( pamh, flags, argc, argv );
    if( retval != PAM_SUCCESS ) {
      return retval;
    }
  }
  pam_get_item( pamh, PAM_AUTHTOK, ( void * ) password );

  if( ( retval = pam_get_item( pamh, PAM_AUTHTOK, ( void * ) password ) ) != PAM_SUCCESS ) {
    _pam_log( LOG_ERR, "Error: %s", pam_strerror( pamh, retval ) );
    return retval;
  }
}
