/*********************************************************************
 *                
 * Copyright (C) 2002,  Simon Kagstrom
 *                
 * Filename:      bench.c
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

#include <stdlib.h>    /* malloc */
#include <string.h>    /* strncpy */
#include <unistd.h>    /* ssize_t */
#include <stdio.h>     /* File reading etc. */
#include <sys/types.h> /* stat */
#include <sys/stat.h>  /* stat */
#include <stddef.h>

#include "bench.h"

/* Delimiters between words */
#define DELIMS " \".,;:?!-\'/*()=+&%[]#$\n\r"

/*
 * This is an example program that reads words from a text-file (a
 * book or something like that) and uses those as keys in a hash table
 * (the actual data stored is unimportant). The words are case
 * sensitive.
 *
 * After this, the program opens another text-file and tries to match
 * the words in that with the words stored in the table.
 *
 * The meaning with this program is to test the speed of the table and
 * to provide an example of its use.
 */
int do_benchmark(char *p_dict_name, char *p_text_name, int i_buckets, char *p_data_arg)
{
  struct stat stat_struct;
  FILE *p_file;
  int i_found;
  int i_cnt;
  char *p_buf;
  char *p_tok;

  /* Create a new hash table */
  if (ht_init(i_buckets, p_data_arg))
    {
      return 1;
    }

  /* Open the dictionary file (first check its size) */
  if (stat(p_dict_name, &stat_struct) < 0)
    {
      perror("stat");
      return 1;
    }
  if (!(p_buf = (char*)malloc(stat_struct.st_size)))
    {
      perror("malloc");
      return 1;
    }

  /* Open the dictionary file and read that into the buffer. */
  if (! (p_file = fopen(p_dict_name, "r")) )
    {
      perror("fopen");
      return 1;
    }
  fread(p_buf, sizeof(char), stat_struct.st_size, p_file);
  fclose(p_file);

  /* For each word in the dictionary file, insert it into the hash table. */
  p_tok = strtok(p_buf, DELIMS);
  i_cnt = 0;
  i_found = 0;
  while (p_tok)
    {
      char *p_data;
      
      if ( !(p_data = (char*)malloc(strlen(p_tok)+1)) )
	{
	  perror("malloc");
	  return 1;
	}
      strncpy(p_data, p_tok, strlen(p_tok));
      p_data[strlen(p_tok)]='\0'; /* NULL-terminate the string */
      
      /* Insert the word into the table */
      if (ht_insert(p_data,
		    p_data) < 0)
	free(p_data); /* Could not insert the item (already exists), free it. */
      else
	i_found++;
      p_tok = strtok(NULL, DELIMS);
      i_cnt++;
    }
  printf("Done reading %d unique words from the wordlist.\n"
	 "Total number of words is %d.\n\n", i_found, i_cnt);

  free(p_buf);

  /* Check the size of the text file. */
  if (stat(p_text_name, &stat_struct) < 0)
    {
      perror("stat");
      return 1;
    }
  if (!(p_buf = (char*)malloc(stat_struct.st_size)))
    {
      perror("malloc");
      return 1;
    }

  /* Open the text file and read that into the buffer. */
  if (! (p_file = fopen(p_text_name, "r")) )
    {
      perror("fopen");
      return 1;
    }
  fread(p_buf, sizeof(char), stat_struct.st_size, p_file);
  fclose(p_file);

  /* For each word in the text file, check if it exists in the hash
     table. */
  p_tok = strtok(p_buf, DELIMS);
  i_cnt = 0;
  i_found = 0;
  while (p_tok)
    {
      char *p_data;

      if ((p_data = ht_get(p_tok)))
	{
	  if (strcmp(p_data, p_tok) != 0)
	    printf("ERROR: %s and %s does not match!\n", p_data, p_tok);
	  i_found++;	  
	}
      i_cnt++;
      p_tok = strtok(NULL, DELIMS);
    }
  free(p_buf);

  printf("Found %d words out of %d words\n", i_found, i_cnt);

  return 0;
}
