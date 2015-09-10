# pam_twauth

twauth PAM module enables authentication over Twilio SMS API

## Building and Installation
libcurl (GnuTLS flavour) > 7.38.0 and PAM development headers are required to compile the plugin 

Run:
`$cd src && make `
to compile pam_twauth.so and copy it to /lib/security/, where PAM modules reside by default

## Configuration
Twilio SMS API mandatory data fields like FromNumber, AuthSid and AuthKey must be set in **/etc/twauth.conf** configuration file.

twauth.conf format:
```
line 1:FromNumber
line 2:AuthSid
line 3:AuthKey
````
Spaces or comments are not allowed.

Example:
```
+1234567890
AC03cfd743661f07975fa2f1220c5194cb
a8fdc205a9f19cc1c7507a60c4f01b13
```

Set proper owner and permission on the /etc/twauth.conf configuration file:
```
chown root:root /etc/twauth.conf
chmod 0600 /etc/twauth.conf
```

ToNumber is user specific and must be set in GECOS' telephone field (within /etc/passwd). 

Example:
```
john:x:1000:1000:,,+37255121314,:/home/john:/bin/bash
vasya:x:1001:1001:Vasya Pupkin,,+37255111213,:/home/vasya:/bin/bash
wolf:x:1002:1002:Tambov Wolf,Yosemite,+37255101112,:/home/wolf:/bin/bash
```

Make sure that `ChallengeResponseAuthentication` in sshd_config is set to `yes`

## Enabling
In the per-application configuration (`/etc/pam.d/application`) add:
```
auth	required	pam_twauth.so
```

## Disclaimer
This module was tested on Linux/Debian 8.0. Other Linux flavors and non-Linux systems like Solaris are not verified.

## Troubleshooting
Make sure or double check:

1. api.twilio.com is accessible from your server (nc -zv api.twilio.com 443)
1. AuthSid, AuthKey and FromNumber fields in `/etc/twauth.confs` are correct
1. ToNumber has been specified for your user in /etc/passwd

## TODO

1. Improve Makefile
1. Implement proper logging (via syslog), otherwise it's quite difficult to debug
1. Introduce DEBUG compile option for verbose mode
1. Memory handling (revise memory allocation and releasing)
1. PAM module could accept arguments for better customization (options to enable/disable logging)
1. Improve error handling and sanity checks 
1. Create separate header file for each C file
1. Add meaningful comments

## References
[Twilio REST API: Sending SMS or MMS](https://www.twilio.com/docs/api/rest/sending-messages)

[GECOS](http://en.wikipedia.org/wiki/Gecos_field)
