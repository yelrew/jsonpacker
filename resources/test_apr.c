#include <json_packer.h>

#include <apr_hash.h>
#include <apr_strings.h>

/* Quick test of the APR library (hash)

    http://dev.ariel-networks.com/apr/apr-tutorial/sample/hashtab-sample.c
    https://resources.oreilly.com/examples/9780596528393/blob/master/mod_ajax.c
*/

int main() {

    apr_pool_t *mp;
    apr_hash_t *ht;
    int val1 = 1, val2 = 2, val3 = 3;
    apr_initialize();
    apr_pool_create(&mp, NULL);

    ht = apr_hash_make(mp);
//    const char *filepath = "../resources/records.txt";
//    int status = JSONp_Packer(filepath);

    /* it is a good idea to allocate strings in the same memory pool in almost cases */
    apr_hash_set(ht, apr_pstrdup(mp, "key1"), APR_HASH_KEY_STRING, (int*)&val1);
    apr_hash_set(ht, apr_pstrdup(mp, "key2"), APR_HASH_KEY_STRING, (int*)&val2);
    apr_hash_set(ht, apr_pstrdup(mp, "key3"), APR_HASH_KEY_STRING, (int*)&val3);
    apr_hash_set(ht, (const void*) "key3", APR_HASH_KEY_STRING, (int*)&val1);
    apr_hash_set(ht, (const void*) "key3", APR_HASH_KEY_STRING, (int*)&val2);

    /* get the value from a key */
   {
       const int *val = apr_hash_get(ht, "key3", APR_HASH_KEY_STRING);
       printf("val for \"key1\" is %d\n", *val);
   }

    apr_hash_index_t *hi;
    for (hi = apr_hash_first(NULL, ht); hi; hi = apr_hash_next(hi)) {
        const char *k;
        const int *v;
        apr_hash_this(hi, (const void**)&k, NULL, (void**)&v);
        printf("ht iteration: key=%s, val=%d\n", k, *v);
    }

    /* the hash table is destroyed when @mp is destroyed */
    apr_pool_destroy(mp);
    apr_terminate();

    return 0;

}
