/*********************************************************************
 *
 * Copyright (C) 2002,  David Gómez	
 *
 * Filename:      bench_hashit.c
 * Description:   A benchmark for the hashit library
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
 *
 ********************************************************************/
#include <stdio.h> 
#include <string.h>
#include <hashit.h>

hash_t *htable;


void ht_usage(void)
{
  printf("Usage:\n"
	 "bench_hashit dictfile textfile\n");
}

int ht_init(int i_buckets, char *p_opts)
{

    htable=hashit_create(i_buckets, 0, NULL, strcmp, C_OVERFLOW);

    if (htable==NULL) return -1;

    return 0;
}

int ht_insert(char *p_data, char *p_key)
{

    return (hashit_insert(htable, p_key, p_data));

}

char *ht_get(char *p_key)
{

   return (hashit_lookup(htable, p_key));

}

int ht_remove(char *p_key)
{
    
   return (hashit_delete(htable, p_key));
   
}
