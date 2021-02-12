# Hashit

Hashit is just another generic hash library ;). First versions had some ugly mistakes, but with the help and bug reports of all the people that used and tested hashit in its first versions, it has become a almost bug-free library. Be aware that this library has nothing special about it and it is quite simple, I just wrote for the sake of it a lot of years ago (2005, maybe?)

Thanks to Bob Jenkins for his great hash functions published in his web page: http://burtleburtle.net/bob/hash.

My intentions at the time of writing this hash library was to learn to a bit more about hashing while implementing a library with different collision handling methods, which could be selected at the hash table creation time and with one common interface for all the methods in the library. Also I had in mind to make it easy to add new collision policies without much hassle ;). The best method for using this library with practical purposes is the chaining hash, no doubt. Most of the other methods are not sufficiently tested, so they're just added with a didactic purpose. I'll thank any feedback from anyone who uses this library, about any bugs or performance problems you'd noticed when using hashit.


David Gómez <[david@pleyades.net](mailto:david@pleyades.net)>


## Hashit API Documentation

Hashit library is very simple to use. Here is the description of the functions exported by the library and that must be used in your application in order to use the library: 

## hashit_create

```c
 hash_t
 hashit_create (uint32_t sizehint, size_t keysize, 
                uint32_t(*) hfunc(), int(*) cfunc(), unsigned int flags);
```

This function creates a hash table which is returned as a hash_t type. 'errno' is set to ENOMEM if there wasn't enough memory avaliable for creating the table.

Return value is a reference to the table just created or NULL when the table creation has failed.

**Parameters**

* sizehint: It's used to give an aproximated desired value for the hash table, 
the function calculates the real size of the table using this parameter as a hint to the final value.
* keysize: This is the size for the data type used as a key in the hash table.
Keys are void *, so hashit needs to know how long they are. If key size is 0, is assumed that keys are strings.
* hfunc: hfunc is a pointer to the hash function for the hash table. Typically
you want to put here the value NULL to use the default hash function. Avaliable hash functions are BOB_HASH(Default) and ONEAT_HASH. Of course you can add your own hash function.
* cfunc: cfunc is the comparation function for the keys data. Semantics are the
same that in strcmp function. The function should return 0 if both keys are equal, and a value different from zero if they are not.
* flags: flags can have the following values:

    CHAIN_H: Hash table uses chaining for handling collisions

    OADDRESS_H: Hash table uses open address hashing for handling collisions

    OVERFLOW_H: Hasht table uses an overflow area for handling collisions
    
    If you're not sure about what collision handling your hash table should have, then use CHAIN_H.

## hashit_insert

```c
int
hashit_insert (hash_t htable, void *key, void *data)
```

Insert an key/data pair in the hash table. If the key is already inserted in the hash table this function will fail and 'errno' will be set to EINVAL. All the keys inserted in the table must have the size given at creation time, and all must have the same data type, if not, behavior is undefined. If there is not enough memory to allocate a new element, ENOMEM is returned in 'errno'. 

Return value is 0 if insert succeeded and -1 if insert fails.

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.
* key: Pointer to the key value. It must have been previously allocated by the application that is using the library.
* value: Pointer to the data value. It must have been previously allocated by the application that is using the library.

## hashit_delete

```c
int
hashit_delete (hasht_t htable, void *key)
```

Delete a key/data pair that is inside the hash table. If the element is not found, EFAULT is returned.

Return value is 0 if `hashit_delete` succeeded and -1 if delete fails. 

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.
* key: Pointer to the key value for the element that should be deleted from the hash table. The application has the duty to free the data associated to this key.

## hashit_lookup

```c
void
*hashit_lookup (hash_t htable, void *key)
```

Search an element in the hash table, using the key passed as parameter and return the data associated to that key. If the element is not found, functions fails and 'errno' will be set to EFAULT.

Return value is a pointer to element data for the searched key and NULL if the search operation fails.

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.
* key: Pointer to a key value for the searched element

## hashit_replace

```c
int
hashit_replace (hash_t htable, void *key, void *newvalue)
```
	
Replace the data for the element associated to the key 'key' with the data pointer by 'newvalue'. If the key is not found 'errno' will be set to EFAULT.

Return value is 0 if hashit_replace succeeded or -1 if replace failed. 

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the
hashit_create function.
* key: Pointer to a key value whose assocciated value will be changed.
* newdata: Pointer to new data to be assigned to this key, if exists.

## hashit_getkeys

```c
void **
hashit_getkeys (hash_t htable)
```

This functions walks through the hash table and returns all the key values inside it. This way there is no need to keep track of all the keys we've inserted in our table.

Return value is a null terminated array containing all the keys in the hash table, or NULL if a error has ocurred. The returned array must be freed by the caller.

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.

## hashit_getvalues

```c
void **
hashit_getvalues (hash_t htable)
```

This functions walks through the hash table and returns all the data values inside it.

Return value is a null terminated array containing all the keys in the hash table, or NULL if a error has ocurred. The returned array must be freed by the caller.

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.

## hashit_size

```c
size_t
hashit_size (hash_t htable)
```

Returns the number of elements in the hash table

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.

## hashit_tablesize

```c
size_t
hashit_tablesize (hash_t htable)
```

Returns the hash table size. This has different meanings depending on the hash collision method used. For example, for chaining hash, it returns the number of buckets in the hash table, for open address its equal to the hash table size (maximum number of elements), for overflow hash it's the size of the main table without the overflow area.

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.

## hashit_destroy

```c
int
hashit_destroy(hash_t htable)
```

Destroy the hash table and free previously allocated memory. This function
should be called when all the work with the hash table is finished and it
won't be needed again.

**Parameters**

* htable: Pointer to the hash table, this must be the value returned by the hashit_create function.


