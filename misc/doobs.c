
#define hashsize(n) ((unsigned long)1&lt;&lt;(n))
#define hashmask(n) (hashsize(n)-1)

/*
--------------------------------------------------------------------
mix -- mix 3 32-bit values reversibly.
For every delta with one or two bits set, and the deltas of all three
  high bits or all three low bits, whether the original value of a,b,c
  is almost all zero or is uniformly distributed,
* If mix() is run forward or backward, at least 32 bits in a,b,c
  have at least 1/4 probability of changing.
* If mix() is run forward, every bit of c will change between 1/3 and
  2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
mix() was built out of 36 single-cycle latency instructions in a 
  structure that could supported 2x parallelism, like so:
      a -= b; 
      a -= c; x = (c&gt;&gt;13);
      b -= c; a ^= x;
      b -= a; x = (a&lt;&lt;8);
      c -= a; b ^= x;
      c -= b; x = (b&gt;&gt;13);
      ...
  Unfortunately, superscalar Pentiums and Sparcs can't take advantage 
  of that parallelism.  They've also turned some of those single-cycle
  latency instructions into multi-cycle latency instructions.  Still,
  this is the fastest good hash I could find.  There were about 2^^68
  to choose from.  I only looked at a billion or so.
--------------------------------------------------------------------
*/
#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c&gt;&gt;13); \
  b -= c; b -= a; b ^= (a&lt;&lt;8); \
  c -= a; c -= b; c ^= (b&gt;&gt;13); \
  a -= b; a -= c; a ^= (c&gt;&gt;12);  \
  b -= c; b -= a; b ^= (a&lt;&lt;16); \
  c -= a; c -= b; c ^= (b&gt;&gt;5); \
  a -= b; a -= c; a ^= (c&gt;&gt;3);  \
  b -= c; b -= a; b ^= (a&lt;&lt;10); \
  c -= a; c -= b; c ^= (b&gt;&gt;15); \
}

/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
  k       : the key (the unaligned variable-length array of bytes)
  len     : the length of the key, counting by bytes
  initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 6*len+35 instructions.

The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
  h = (h &amp; hashmask(10));
In which case, the hash table should have hashsize(10) elements.

If you are hashing n strings (unsigned char **)k, do it like this:
  for (i=0, h=0; i&lt;n; ++i) h = hash( k[i], len[i], h);

By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

See http://burtleburtle.net/bob/hash/evahash.html
Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/

unsigned long hash( k, length, initval)
 unsigned char *k;        /* the key */
 unsigned long  length;   /* the length of the key */
 unsigned long  initval;  /* the previous hash, or an arbitrary value */
{
   unsigned long a,b,c,len;

   /* Set up the internal state */
   len = length;
   a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
   c = initval;         /* the previous hash value */

   /*---------------------------------------- handle most of the key */
   while (len &gt;= 12)
   {
      a += (k[0] +((unsigned long)k[1]&lt;&lt;8) +((unsigned long)k[2]&lt;&lt;16) +((unsigned long)k[3]&lt;&lt;24));
      b += (k[4] +((unsigned long)k[5]&lt;&lt;8) +((unsigned long)k[6]&lt;&lt;16) +((unsigned long)k[7]&lt;&lt;24));
      c += (k[8] +((unsigned long)k[9]&lt;&lt;8) +((unsigned long)k[10]&lt;&lt;16)+((unsigned long)k[11]&lt;&lt;24));
      mix(a,b,c);
      k += 12; len -= 12;
   }

   /*------------------------------------- handle the last 11 bytes */
   c += length;
   switch(len)              /* all the case statements fall through */
   {
   case 11: c+=((unsigned long)k[10]&lt;&lt;24);
   case 10: c+=((unsigned long)k[9]&lt;&lt;16);
   case 9 : c+=((unsigned long)k[8]&lt;&lt;8);
      /* the first byte of c is reserved for the length */
   case 8 : b+=((unsigned long)k[7]&lt;&lt;24);
   case 7 : b+=((unsigned long)k[6]&lt;&lt;16);
   case 6 : b+=((unsigned long)k[5]&lt;&lt;8);
   case 5 : b+=k[4];
   case 4 : a+=((unsigned long)k[3]&lt;&lt;24);
   case 3 : a+=((unsigned long)k[2]&lt;&lt;16);
   case 2 : a+=((unsigned long)k[1]&lt;&lt;8);
   case 1 : a+=k[0];
     /* case 0: nothing left to add */
   }
   mix(a,b,c);
   /*-------------------------------------------- report the result */
   return c;
}
