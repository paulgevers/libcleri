/*
 * kwcache.c - holds keyword regular expression result while parsing.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *
 */
#include <stdlib.h>
#include <pcre.h>
#include <string.h>
#include <cleri/kwcache.h>

static int KWCACHE_kw_match(cleri_parse_t * pr, const char * str);

/*
 * Returns 0 when no kw_match is found, -1 when an error has occurred, or the
 * new kwcache->len value.
 */
ssize_t cleri__kwcache_match(
        cleri_parse_t * pr,
        const char * str)
{
    cleri_kwcache_t * kwcache;
    for (uint32_t i = 0; i < pr->kwcache_->n; i++)
    {
        kwcache = (cleri_kwcache_t *) cleri_vec_get(pr->kwcache_, i);
        if (kwcache->str == str) return kwcache->len;

    }
    kwcache = (cleri_kwcache_t *) malloc(sizeof(cleri_kwcache_t));
    if (!kwcache) return -1;

    cleri_vec_t * tmp = cleri_vec_push(pr->kwcache_, kwcache);
    if (!tmp)
    {
        free(kwcache);
        return -1;
    }
    pr->kwcache_ = tmp;

    kwcache->str = str;
    kwcache->len = KWCACHE_kw_match(pr, str);
    return kwcache->len;
}


/*
 * This function will set kwcache->len if a match is found.
 */
static int KWCACHE_kw_match(cleri_parse_t * pr, const char * str)
{
    int pcre_exec_ret;
    int sub_str_vec[2];

    pcre_exec_ret = pcre_exec(
                pr->re_keywords_,
                pr->re_kw_extra_,
                str,
                strlen(str),
                0,                     // start looking at this point
                0,                     // OPTIONS
                sub_str_vec,
                2);                    // length of sub_str_vec
    if (pcre_exec_ret != 1)
    {
        return 0;
    }

    return sub_str_vec[1];
}
