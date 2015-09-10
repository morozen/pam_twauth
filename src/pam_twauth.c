#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include <security/pam_appl.h>
#include <security/pam_modules.h>

#include <pwd.h>
#include <sys/types.h>

#include <syslog.h>

#include "twlib.h"

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return PAM_SUCCESS;
}

int pam_converse(pam_handle_t *pamh, int nargs, struct pam_message **message, struct pam_response **response)
{
    int             retval;
    struct pam_conv *conv;

    retval = pam_get_item(pamh, PAM_CONV, (const void * *) &conv);

    if (retval == PAM_SUCCESS)
    {
        retval = conv->conv(nargs, (const struct pam_message * *) message, response, conv->appdata_ptr);
    }
    return retval;
}


PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    int retval;

    /* these guys will be used by pam_converse() */
    char                *input;
    struct pam_message  msg[1], *pmsg[1];
    struct pam_response *resp;

    char                *code;
    char		*codeid;
    char                *from_number;
    char                *auth_sid;
    char                *auth_key;
    char                *to_number;

    const char          *username;

    /* getting the username that was used in the previous authentication */
    if ((retval = pam_get_user(pamh, &username, "login: ")) != PAM_SUCCESS)
    {
        return retval;
    }

    /* get telephone number for SMS sending  */
    to_number = get_to_number(username);

    openlog("pam_twauth", LOG_PID|LOG_CONS, LOG_AUTH);
    char logmsg[1024];

    /* we should return SUCCESS if user's number is not defined  */
    if (!to_number) {
        sprintf(logmsg, "ToNumber for user %s is not defined. Skipping SMS authentication", username);
        syslog(LOG_INFO, logmsg);
        closelog();
 	return PAM_SUCCESS;
    }

    /* generating a random one-time code and codeid for this code in order to distinguish them */
    code = get_code(6);
    codeid = get_code(3);

    /* getting Twilio API Sid, Key and FromNumber from configuration file */
    auth_sid    = get_auth_sid();
    auth_key    = get_auth_key();
    from_number = get_from_number();

    sprintf(logmsg, "ToNumber: %s, FromNumber: %s, AuthSid: %s, AuthKey: %s, code: %s, codeid: %s\n", to_number, from_number, auth_sid, auth_key, code, codeid );
    syslog(LOG_INFO, logmsg);

    int exit_status;

    /* sending code via Twilio SMS API */
    /* would be nice to implement sanity checks against arguments against NULL values */
    exit_status = send_sms(to_number, from_number, auth_sid, auth_key, code, codeid);
    sprintf(logmsg, "send_sms() exit status: %d", exit_status);
    syslog(LOG_INFO, logmsg);
    closelog();

    /* setting up conversation call prompting for one-time code */
    pmsg[0]          = &msg[0];
    msg[0].msg_style = PAM_PROMPT_ECHO_ON;
    char prompt[20];
    prompt[0] = '\0';
    sprintf(prompt, "OTP token #%s: ", codeid);
    msg[0].msg       = prompt;
    resp             = NULL;
    if ((retval = pam_converse(pamh, 1, pmsg, &resp)) != PAM_SUCCESS)
    {
        // if this function fails, make sure that ChallengeResponseAuthentication in sshd_config is set to yes
        return retval;
    }

    /* retrieving user input */
    if (resp)
    {
        if ((flags & PAM_DISALLOW_NULL_AUTHTOK) && resp[0].resp == NULL)
        {
            free(resp);
            return PAM_AUTH_ERR;
        }
        input = resp[ 0 ].resp;
        resp[ 0 ].resp = NULL;
    }
    else
    {
        return PAM_CONV_ERR;
    }

    /* comparing user input with known code */
    if (strcmp(input, code) == 0)
    {
	/* good to go! */
 	free(input);
 	free(code);
 	free(codeid);

	free(from_number);
	free(auth_sid);
	free(auth_key);
	free(to_number);

 	return PAM_SUCCESS;
    }
    else
    {
        /* wrong code */
        free(input);
        free(code);
        free(codeid);

	free(from_number);
	free(auth_sid);
	free(auth_key);
	free(to_number);

 	return PAM_AUTH_ERR;
    }

    /* we shouldn't read this point, but if we do, we might as well return something bad */
	return PAM_AUTH_ERR;
}
