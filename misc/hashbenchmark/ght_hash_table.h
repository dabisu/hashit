/*
 * Copyright (C) 2001-2002,  Simon Kagstrom
 *
 * Filename:      ght_hash_table.h.in
 * Description:   The definitions used in the hash table.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * $Log: ght_hash_table.h,v $
 * Revision 1.2  2002/02/27 15:05:43  simon
 * Updated documentation, size is unsigned
 *
 * Revision 1.1.1.1  2002/02/26 18:21:25  simon
 * libghthash MK2
 *
 */

/**
 * @file
 * libghthash is a generic hash table used for storing arbitrary
 * data.
 *
 * Libghthash really stores pointers to data - the hash
 * table knows nothing about the actual type of the data.
 *
 * A simple example to get started can be found in the
 * <TT>simple.c</TT> file found in the distribution.
 * <TT>hash_test.c</TT> provides a more comlpete example.
 *
 * The MK2 version of the library should be more simple to use with
 * fewer exported function calls and fewer structs. The usage have
 * changed a bit since the first version though:
 *
 * Some basic properties of the hash table are:
 *
 * - Both the data stored and the keys are of void type, which
 *   means that you can store any kind of data.
 *
 * - The only functions you probably will need to start is:
 *   - ght_create(), which creates a new hash table
 *   - ght_insert(), which inserts a new entry into a table
 *   - ght_get(), which searches for an entry
 *   - ght_remove(), which removes and entry.
 *     
 * - Inserting entries is done without first creating a key,
 *   i.e. you insert with the data, the datasize, the key and the
 *   key size directly.

 * - The hash table copies the key data when inserting new
 *   entries. This means that you don't have to malloc() the key
 *   before inserting a new entry.
 *
 */
#ifndef GHT_HASH_TABLE_H
#define GHT_HASH_TABLE_H

#define GHT_HEURISTICS_NONE          0
#define GHT_HEURISTICS_TRANSPOSE     1
#define GHT_HEURISTICS_MOVE_TO_FRONT 2
#define GHT_AUTOMATIC_REHASH         4

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/** unsigned 32 bit integer. */
typedef unsigned int ght_uint32_t;

/**
 * The structure for hash keys. You should not care about this
 * structure unless you plan to write your own hash functions.
 */
typedef struct s_hash_key
{
  unsigned int i_size;       /**< The size in bytes of the key p_key */
  void *p_key;               /**< A pointer to the key. */
} ght_hash_key_t;

/*
 * The structure for hash entries.
 */
typedef struct s_hash_entry
{
  void *p_data;
  ght_hash_key_t *p_key;

  struct s_hash_entry *p_next;
  struct s_hash_entry *p_prev;
  unsigned char i_flags;
} ght_hash_entry_t;

/*
 * The structure used in iterations. You should not care about the
 * contents of this, it will be filled and updated by ght_first() and
 * ght_next().
 */
typedef struct
{
  int i_curr_bucket;         /* The current bucket */
  ght_hash_entry_t *p_entry; /* The current entry */
} ght_iterator_t;

/**
 * Definition of the hash function pointers. ght_fn_hash_t should be
 * used when implementing new hash functions. Look at the supplied
 * hash functions, like ght_one_at_a_time_hash(), for examples of hash
 * functions.
 *
 * @param p_key the key to calculate the hash value for.
 *
 * @return a 32 bit hash value.
 *
 * @see ght_one_at_a_time_hash(), ght_rotating_hash(), ght_crc_hash()
 */
typedef ght_uint32_t (*ght_fn_hash_t)(ght_hash_key_t *p_key);

/**
 * The hash table structure.
 */
typedef struct
{
  unsigned int i_items;              /**< The current number of items in the table */
  unsigned int i_size;               /**< The number of buckets */
  ght_fn_hash_t fn_hash;             /**< The hash function used */
  int i_heuristics;                  /**< The type of heuristics used */
  int i_automatic_rehash;            /**< TRUE if automatic rehashing is used */

  /* private: */
  ght_hash_entry_t **pp_entries;
  int *p_nr;                         /* The number of entries in each bucket */
  int i_size_mask;                   /* The number of bits used in the size */
} ght_hash_table_t;

/**
 * Create a new hash table. The number of buckets should be about as
 * big as the number of elements you wish to store in the table for
 * good performance. The number of buckets is rounded to the next
 * higher power of two.
 *
 * The possible flags are:
 *
 * - <TT>GHT_HEURISTICS_TRANSPOSE</TT>: Use transposing heuristics. An accessed
 *   element will move one step up in the bucket-list with this
 *   method. Cannot be combined with GHT_HEURISTICS_MOVE_TO_FRONT.
 * - <TT>GHT_HEURISTICS_MOVE_TO_FRONT</TT>: Use move-to-front heuristics. An
 *   accessed element will be moved the front of the bucket list
 *   with this method. Cannot be combined with GHT_HEURISTICS_TRANSPOSE.
 * - <TT>GHT_AUTOMATIC_REHASH</TT>: Perform automatic rehashing when
 *   the number of elements in the table are twice as many as the
 *   number of buckets. You should note that automatic rehashing
 *   will cause your application to be really slow when the table is
 *   rehashing (which might happen at times when you need speed),
 *   you should * therefore be careful with this in time-constrainted
 *   applications.
 *
 * @param i_size the number of buckets in the hash table. Giving a
 *        non-power of two here will round the size up to the next
 *        power of two.
 * @param fn_hash the hash function to use (NULL for default). You can
 *        define own hash functions to use here, see the
 *        implementation of ght_one_at_a_time_hash() in
 *        <TT>hash_table.c</TT> for an example.
 * @param i_flags specify the flags to use. This should be bitwise or:ed.
 *        note that some options are mutually exclusive.
 *
 * @see ght_one_at_a_time_hash(), ght_rotating_hash(), ght_crc_hash()
 *
 * @return a pointer to the hash table or NULL upon error.  */
ght_hash_table_t *ght_create(unsigned int i_size, ght_fn_hash_t fn_hash, int i_flags);

/**
 * Insert an entry into the hash table. Prior to inserting anything,
 * make sure that the table is created with ght_create(). If an
 * element with the same key as this one already exists in the table,
 * the insertion will fail and -1 is returned.
 *
 * A typical example is shown below, where the string "blabla" is used
 * as a key for the integer 15.
 *
 * <PRE>
 * ght_hash_table_t *p_table;
 * char *p_key_data;
 * int *p_data;
 *
 * [Create p_table etc...]
 * p_data = malloc(sizeof(int));
 * p_key_data = "blabla";
 * *p_data = 15;
 *
 * ght_insert(p_table,
 *            p_data,
 *            sizeof(char)*strlen(p_key_data), p_key_data);
 * </PRE>
 *
 * @param p_ht the hash table to insert into.
 * @param p_entry_data the data to insert.
 * @param i_key_size the size of the key to associate the data with (in bytes).
 * @param p_key_data the key to use. The value will be copied, and it
 *                   is therefore OK to use a stack-allocated entry here.
 *
 * @return 0 if the element could be inserted, -1 otherwise.
 */
int ght_insert(ght_hash_table_t *p_ht,
	       void *p_entry_data,
	       unsigned int i_key_size, void *p_key_data);

/**
 * Lookup an entry in the hash table. The entry is <I>not</I> removed from
 * the table.
 *
 * @param p_ht the hash table to search in.
 * @param i_key_size the size of the key to search with (in bytes).
 * @param p_key_data the key to search for.
 *
 * @return a pointer to the found entry or NULL if no entry could be found.
 */
void *ght_get(ght_hash_table_t *p_ht,
	      unsigned int i_key_size, void *p_key_data);

/**
 * Remove an entry from the hash table. The entry is removed from the
 * table, but not freed (that is, the data stored is not freed).
 *
 * @param p_ht the hash table to use.
 * @param i_key_size the size of the key to search with (in bytes).
 * @param p_key_data the key to search for.
 *
 * @return a pointer to the removed entry or NULL if the entry could be found.
 */
void *ght_remove(ght_hash_table_t *p_ht,
		 unsigned int i_key_size, void *p_key_data);

/**
 * Return the first entry in the hash table. This function should be
 * used for iteration and is used together with ght_next(). Note that
 * you cannot assume anything about the order in which the entries are
 * accessed. If an entry is inserted during an iteration, the entry
 * might or might not occur in the iteration.
 *
 * The use of the ght_iterator_t allows for several concurrent
 * iterations, where you would use one ght_iterator_t for each
 * iteration. In threaded environments, you should still lock access
 * to the hash table for insertion and removal.
 *
 * A typical example might look as follows:
 * <PRE>
 * ght_hash_table_t *p_table;
 * ght_iterator_t iterator;
 * void *p_e;
 *
 * [Create table etc...]
 * for(p_e = ght_first(p_table, &iterator); p_e; p_e = ght_next(p_table, &iterator))
 *   {
 *      [Do something with the current entry p_e]
 *   }
 * </PRE>
 *
 * @param p_ht the hash table to iterate through.
 *
 * @param p_iterator the iterator to use. The value of the structure
 * is filled in by this function and may be stack allocated.
 *
 * @return a pointer to the first entry in the table or NULL if there
 * are no entries.
 *
 *
 * @see ght_next()
 */
void *ght_first(ght_hash_table_t *p_ht, ght_iterator_t *p_iterator);

/**
 * Return the next entry in the hash table. This function should be
 * used for iteration, and must be called after ght_first().
 *
 * @warning calling this without first having called ght_first will
 * give undefined results (probably a crash), since p_iterator isn't
 * filled correctly.
 *
 * @param p_ht the hash table to iterate through.
 *
 * @param p_iterator the iterator to use.
 *
 * @return a pointer to the next entry in the table or NULL if there
 * are no more entries in the table.
 *
 * @see ght_first()
 */
void *ght_next(ght_hash_table_t *p_ht, ght_iterator_t *p_iterator);

/**
 * Rehash the hash table.
 *
 * Rehashing will change the size of the hash table, retaining all
 * elements. This is very costly and should be avoided unless really
 * needed. If <TT>GHT_AUTOMATIC_REHASH</TT> is specified in the flag
 * parameter when ght_create() is called, the hash table is
 * automatically rehashed when the number of stored elements exceeds
 * two times the number of buckets in the table (making calls to this
 * function unessessary).
 *
 * @param p_ht the hash table to rehash.
 * @param i_size the new size of the table.
 *
 * @see ght_create()
 */
void ght_rehash(ght_hash_table_t *p_ht, unsigned int i_size);

/**
 * Free the hash table. ght_finalize() should typically be called
 * at the end of the program. Note that only the metadata and the keys
 * of the table is freed, not the entries. If you want to free the
 * entries when removing the table, the entries will have to be
 * manually freed before ght_finalize() is called like:
 *
 * <PRE>
 * ght_iterator_t iterator;
 * void *p_e;
 *
 * for(p_e = ght_first(p_table, &iterator); p_e; p_e = ght_next(p_table, &iterator))
 *   {
 *     free(p_e);
 *   }
 *
 * ght_finalize(p_table);
 * </PRE>
 *
 * @param p_ht the table to remove.
 */
void ght_finalize(ght_hash_table_t *p_ht);

/* exported hash functions */

/**
 * One-at-a-time-hash. One-at-a-time-hash is a good hash function, and
 * is the default when ght_create() is called with NULL as the
 * fn_hash parameter. This was found in a DrDobbs article, see
 * http://burtleburtle.net/bob/hash/doobs.html
 *
 * @warning Don't call this function directly, it is only meant to be
 * used as a callback for the hash table.
 *
 * @see ght_fn_hash_t
 * @see ght_rotating_hash(), ght_crc_hash()
 */
ght_uint32_t ght_one_at_a_time_hash(ght_hash_key_t *p_key);

/**
 * Rotating hash. Not so good hash function. This was found in a
 * DrDobbs article, see http://burtleburtle.net/bob/hash/doobs.html
 *
 * @warning Don't call this function directly, it is only meant to be
 * used as a callback for the hash table.
 *
 * @see ght_fn_hash_t
 * @see ght_one_at_a_time_hash(), ght_crc_hash()
 */
ght_uint32_t ght_rotating_hash(ght_hash_key_t *p_key);

/**
 * CRC32 hash. CRC32 hash is a good hash function. This came from Dru
 * Lemley <spambait@lemley.net>.
 *
 * @warning Don't call this function directly, it is only meant to be
 * used as a callback for the hash table.
 *
 * @see ght_fn_hash_t
 * @see ght_one_at_a_time_hash(), ght_rotating_hash()
 */
ght_uint32_t ght_crc_hash(ght_hash_key_t *p_key);

#ifdef USE_PROFILING
/**
 * Print some statistics about the table. Only available if the
 * library was compiled with <TT>USE_PROFILING</TT> defined.
 */
void ght_print(ght_hash_table_t *p_ht);
#endif

#endif /* GHT_HASH_TABLE_H */
