/*********************************************************************
 *
 * Copyright (C) 2002,  Simon Kagstrom
 *
 * Filename:      bench_ght.c
 * Description:   A benchmark for libghthash
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
 * $Log: bench_ght.c,v $
 *
 ********************************************************************/
#include <stdlib.h>          /* NULL */
#include <string.h>          /* strcmp */
#include <stdio.h>           /* printf */
#include "ght_hash_table.h"

#include "bench.h"

ght_hash_table_t *p_table;

void ht_usage(void)
{
  printf("Usage:\n"
	 "bench_ght [-1|-2|-3|-4|-5|-6] dictfile textfile\n"
	 "\n"
	 "where options are:\n"
	 "-1        Run without heuristics, one-at-a-time hash (default)\n"
	 "-2        Run with transpose heuristics, one-at-a-time hash\n"
	 "-3        Run with move-to-front heuristics, one-at-a-time hash\n"
	 "\n"
	 "-4        Run without heuristics, CRC hash\n"
	 "-5        Run with transpose heuristics, CRC hash\n"
	 "-6        Run with move-to-front heuristics, CRC hash\n");
}

int ht_init(int i_buckets, char *p_opts)
{
  ght_fn_hash_t fn = ght_one_at_a_time_hash;
  int i_flags=0;

  /* Parse the options */
  if (p_opts)
    {
      if (strcmp(p_opts, "-2") == 0)
	i_flags |= GHT_HEURISTICS_TRANSPOSE;
      else if (strcmp(p_opts, "-3") == 0)
	i_flags |= GHT_HEURISTICS_MOVE_TO_FRONT;
      else if (strcmp(p_opts, "-4") == 0)
	fn = ght_crc_hash;
      else if (strcmp(p_opts, "-5") == 0)
	{
	  i_flags |= GHT_HEURISTICS_TRANSPOSE;
	  fn = ght_crc_hash;
	}
      else if (strcmp(p_opts, "-6") == 0)
	{
	  i_flags |= GHT_HEURISTICS_MOVE_TO_FRONT;
	  fn = ght_crc_hash;
	}
    }

  if (!(p_table = ght_create(i_buckets, fn, i_flags | GHT_AUTOMATIC_REHASH)))
    return -1;

  return 0;
}

int ht_insert(char *p_data, char *p_key)
{
  return ght_insert(p_table,
		    p_data,
		    strlen(p_key), p_key);
}

char *ht_get(char *p_key)
{
  return ght_get(p_table,
		 strlen(p_key), p_key);
}

int ht_remove(char *p_key)
{
  if (ght_get(p_table,
	      strlen(p_key), p_key))
    return 0;
  return -1;
}
