/*********************************************************************
 *                
 * Copyright (C) 2002,  Simon Kagstrom
 *                
 * Filename:      bench_main.c
 * Description:   A hash table benchmark program.
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
 * $Log: bench.c,v $
 *                
 ********************************************************************/
#include <stdio.h>     /* printf */
#include "bench.h"

int main(int argc, char *argv[])
{
  char *p_opts;
  char *p_dict_name;
  char *p_text_name;

  /* Parse the arguments */
  if (argc < 3)
    {
      ht_usage();
      return 0;
    }
  else if (argc > 3)
    {
      p_opts = argv[1];
      p_dict_name = argv[2];
      p_text_name = argv[3];      
    }
  else
    {
      /* 2 arguments */
      p_opts = NULL;
      p_dict_name = argv[1];
      p_text_name = argv[2];
    }

  return do_benchmark(p_dict_name, p_text_name, 8000, p_opts);
}
