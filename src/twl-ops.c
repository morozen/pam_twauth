#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "twlib.h"

char* get_from_number(void)
{
    return get_conf_line(1, "/etc/twauth.conf");
}

char* get_auth_sid(void)
{
    return get_conf_line(2, "/etc/twauth.conf");
}

char* get_auth_key(void)
{
    return get_conf_line(3, "/etc/twauth.conf");
}

/* do not foget to free() all the resources returned by this function */
char* get_conf_line(int linenum, char* filename)
{
    FILE    * fp;
    char    * line = NULL;
    size_t  len    = 0;
    ssize_t read;
    int     counter = 1;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return NULL;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (counter == linenum)
        {
            // let's remove trailing newline character
            line[strlen(line) - 1] = '\0';
            break;
        }
        counter++;
    }

    fclose(fp);
    return line;
}
