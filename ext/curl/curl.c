#include <curl/curl.h>
#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_curl_init( variable *ths ) {
    CURL *curl;
    curl = curl_easy_init();
    if( ! curl ) {
        printf("warning : problem in curl init\n");
    }
    return doint(curl);
}

do_curl_setopt( variable *ths, variable *curl, variable *opt, variable *opt_val ) {
    int opt1 = opt->val;

    curl_easy_setopt(curl->val, opt1, string_val( opt_val->val) );
}

struct string_curl {
    char *ptr;
    size_t len;
};

void init_string_curl(struct string_curl *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(0);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string_curl *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(0);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}


do_curl_exec( variable *ths, variable *curl ) {
    CURLcode res;
    struct string_curl s;
    init_string_curl(&s);

    curl_easy_setopt(curl->val, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl->val, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl->val);

    return dostring(s.ptr);
}

do_curl_close( variable *ths, variable *curl ) {
    curl_easy_cleanup(curl->val);
    //curl_global_cleanup();
}

extern load() {
    array *arr = malloc( sizeof( array *) );
    array_init( arr );
    array_set1( arr, "curl_init", &do_curl_init);
    array_set1( arr, "curl_setopt", &do_curl_setopt);
    array_set1( arr, "curl_exec", &do_curl_exec);
    array_set1( arr, "curl_close", &do_curl_close);


    return arr;

}
