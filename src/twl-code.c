#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "twlib.h"

/* let's generate random numeric characters ;-) */

char* get_code(int count)
{
    char * buf;
    char random_byte;
    FILE *urandom;

    urandom = fopen("/dev/urandom", "r");
    buf     = malloc(sizeof(char) * count + 1);
    buf[0]  = '\0';

    while (count > 0)
    {
        fread(&random_byte, sizeof(char), 1, urandom);
        if (random_byte < 48 || random_byte > 57)
            continue;
        strncat(buf, &random_byte, 1);
        count--;
    }

    fclose(urandom);
    return buf;
}
