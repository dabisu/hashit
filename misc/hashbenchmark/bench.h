#ifndef BENCH_H
#define BENCH_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Print the usage for the hash table. This should print out the
 * specific options for this hash table.
 */
void ht_usage(void);

/**
 * Initialize the hash table
 *
 * @param i_buckets the number of buckets the hash table should
 *                  have. Use as a hint for the number of elements.
 * @param p_opts options to the hash table. The contents of this
 *               differs between different hash tables.
 *
 * @return 0 if successful, -1 otherwise.
 */
int ht_init(int i_buckets, char *p_opts);

/** 
 * Insert a new element into the hash table.
 *
 * @param p_data the element to insert.
 * @param p_key the key to insert with.
 *
 * @return 0 if successful, -1 otherwise (if the element already exists).
 */
int ht_insert(char *p_data, char *p_key);

/**
 * Get an element from the hash table.
 *
 * @param p_key the key to search for.
 *
 * @return a pointer to the element or NULL if no element was found.
 */
char *ht_get(char *p_key);

/**
 * Remove an element from the hash table.
 *
 * @param p_key the key to remove.
 *
 * @return 0 if successful, -1 otherwise.
 */
int ht_remove(char *p_key);

/* Implemented in bench.c, should not be called from bench_* files. */
int do_benchmark(char *p_dict_name, char *p_text_name, int i_buckets, char *p_data_arg);

#ifdef __cplusplus
}
#endif

#endif /* BENCH_H */
