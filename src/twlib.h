#ifndef TWLIB
#define TWLIB

char* get_to_number(const char* username);
char* get_code(int count);
char* get_auth_sid(void);
char* get_auth_key(void);
char* get_from_number(void);
int send_sms(char* to_number, char* from_number, char* auth_sid, char* auth_key, char* code, char* codeid);
char* get_conf_line(int linenum, char* filename);

#endif
