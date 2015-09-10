#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>

#include "twlib.h"

/* SMS sending logic */

/* we will parse output, once we add additional logging */
/* useless piece of code at the moment */

/* 
struct MemoryStruct
{
    char   *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t              realsize = size * nmemb;
    struct MemoryStruct *mem     = (struct MemoryStruct *) userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size             += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
*/

int send_sms(char* to_number, char* from_number, char* auth_sid, char* auth_key, char* code, char* codeid)
{
    /* SMS sending code is here */

    /* building URL */
    char base_url[] = "https://api.twilio.com/2010-04-01/Accounts/";
    char url_with_params[ strlen(base_url) + strlen(auth_sid) + strlen("/Messages") + 1 ];
    strcpy(url_with_params, base_url);
    strcat(url_with_params, auth_sid);
    strcat(url_with_params, "/Messages");

    /* building basic auth. string */
    char basic_auth[ strlen(auth_sid) + strlen(":") + strlen(auth_key) + 1 ];
    sprintf(basic_auth, "%s:%s", auth_sid, auth_key);

    /* building BODY message */
    char body[64];
    body[0] = '\0';
    sprintf(body,"OTP token #%s: %s", codeid, code);

    /* building POST data */
    char
    post_data[ strlen("From=") + strlen(from_number) + strlen("&To=") + strlen(to_number) + strlen("&Body=") + strlen(body) + 1 ];
    strcpy(post_data, "From=");
    strcat(post_data, from_number);
    strcat(post_data, "&To=");
    strcat(post_data, to_number);
    strcat(post_data, "&Body=");
    strcat(post_data, body);

    CURL                *curl;
    CURLcode            res;

    // we are not parsing HTTP response yet
    // struct MemoryStruct chunk;
    // chunk.memory = malloc(1); /* will be grown as needed by the realloc
    //                               above */
    // chunk.size   = 0;         /* no data at this point */

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_USERPWD, basic_auth);
        curl_easy_setopt(curl, CURLOPT_URL, url_with_params);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

	/* see above - we are not parsing any data coming from the server yet */
	/* 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
	*/

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    // uncomment, if we need to parse server response
    // free(chunk.memory);
    // curl_global_cleanup();

    return res;
}
