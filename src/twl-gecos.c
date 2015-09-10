#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <assert.h>

#include "twlib.h"

struct Tokens
{
    char   **tokens;
    size_t size;
};

/* split arbitrary string into an array of strings by separating the string into substrings */
struct Tokens * str_split(char* a_str, const char a_delim)
{
    char   *tmp, *token, *string;
    char   **array;
    size_t idx   = 0;
    size_t count = 0;

    // char   delim[2];
    // delim[0] = a_delim;
    // delim[1] = 0;

    tmp = string = strdup(a_str);

/* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
        }
        tmp++;
    }

    count++;
    array = malloc(sizeof(char*) * count + 1);

    while ((token = strsep(&string, ",")) != NULL)
    {
        assert(idx < count);
        *(array + idx++) = strdup(token);
    }

    *(array + idx) = 0;

    struct Tokens *tokens = (struct Tokens *) malloc(sizeof(struct Tokens));

    tokens->tokens = array;
    tokens->size   = idx;

    return tokens;
}

/* extract phone record from GECOS passwd field */
char* get_to_number(const char *username)
{
    char          delim = ',';
    struct Tokens *tokens;

    struct passwd *pw;
    pw = getpwnam(username);
    if (pw == NULL || strlen(pw->pw_gecos) <= 3)
    {
        return 0;
    }

    tokens = str_split(pw->pw_gecos, delim);

    if (tokens->size > 1)
        return *(tokens->tokens + 2);

    return NULL;
}
