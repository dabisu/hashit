/*********************************************************************
 *
 * Copyright (C) 2002,  Simon Kagstrom
 *
 * Filename:      bench_stl.cc
 * Description:   A benchmark for the C++ STL hash_map
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
 * $Log: bench_stl.cc,v $
 *
 ********************************************************************/
#include <string.h>     /* strcmp */
#include <stdio.h>      /* printf */
#include <hash_map.h>

struct eqstr
{
  bool operator()(char* s1, char* s2)
  {
    return strcmp(s1, s2) == 0;
  }
};

hash_map<char*, char*, hash<char*>, eqstr> *p_table;

extern "C" void ht_usage(void)
{
  printf("Usage:\n"
	 "bench_stl dictfile textfile\n");
}

extern "C" int ht_init(int i_buckets, char *p_opts)
{
  p_table = new hash_map<char*, char*, hash<char*>, eqstr>(i_buckets);
  return 0;
}

extern "C" int ht_insert(char *p_data, char *p_key)
{
  /* Does hash_map store the data, or just a pointer to it?
   * My guess is that it "stores" the pointer here since the
   * template uses char* here. Otherwise we have a memory leak
   * here.
   *
   * I'm also not sure what STL does when the item is already
   * in the table - does it replace it or does the insertion
   * fail? The returned value here is not indicative of if
   * the insertion failed or not.
   */
  ((*p_table)[p_key] = p_data);
  return 0;
}

extern "C" char *ht_get(char *p_key)
{
  return (*p_table)[p_key];
}

extern "C" int ht_remove(char *p_key)
{
  return -1;
}
