/*********************************************************************
 *
 * Copyright (C) 2002,  Simon Kagstrom
 *
 * Filename:      bench_glibc.c
 * Description:   A benchmark for the GNU libc hsearch
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
 * $Log: bench_glibc.c,v $
 *
 ********************************************************************/
#include <stdio.h>    /* printf */
#include <search.h>   /* hsearch */

void ht_usage(void)
{
  printf("Usage:\n"
	 "bench_glibc dictfile textfile\n");
}

int ht_init(int i_buckets, char *p_opts)
{
    hcreate(i_buckets);

    return 0;
}

int ht_insert(char *p_data, char *p_key)
{
    ENTRY entry;

    entry.key = p_key;
    entry.data = p_data;

    /* See bench_stl.c for some possible caveats here.
     */
    return (hsearch(entry, ENTER)?0:-1);
}

char *ht_get(char *p_key)
{
    ENTRY entry;
    ENTRY *p_out;

    entry.key = p_key;

    p_out = hsearch(entry, FIND);
    return (p_out?p_out->data:NULL);
}

int ht_remove(char *p_key)
{
    /* How do I remove data? */
    return -1;
}
