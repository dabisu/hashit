/*********************************************************************
 *                
 * Copyright (C) 2002,  Simon Kagstrom
 *                
 * Filename:      dict_example.c
 * Description:   A program that stores words from a dictionary
 *                and looks them up in a text file.
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
 * $Log: dict_example.c,v $
 * Revision 1.4  2002/02/27 15:06:05  simon
 * Flags are 0 from the start
 *
 * Revision 1.3  2002/02/27 13:28:42  simon
 * We should not open non-existing files...
 *
 * Revision 1.2  2002/02/26 18:25:54  simon
 * Remove automatic rehashing
 *
 * Revision 1.1.1.1  2002/02/26 18:21:25  simon
 * libghthash MK2
 *
 *                
 ********************************************************************/

#include <stdlib.h>    /* malloc */
#include <string.h>    /* strncpy */
#include <unistd.h>    /* ssize_t */
#include <stdio.h>     /* File reading etc. */
#include <sys/types.h> /* stat */
#include <sys/stat.h>  /* stat */
#include <stddef.h>

#include "ght_hash_table.h"

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
int main(int argc, char *argv[])
{
  FILE *p_file;
  char *p_dict_name;
  char *p_text_name;
  ght_hash_table_t *p_table;
  ght_iterator_t iterator;
  int i_flags=0; 
  struct stat stat_struct;
  int i_found;
  int i_cnt;
  char *p_buf;
  char *p_tok;
  void *p_e;

  /* Parse the arguments */
  if (argc < 3)
    {
      printf("Usage: dict_example [-m|-t] dictfile textfile\n");
      return 0;
    }
  else if (argc > 3)
    {
      if(strcmp(argv[1], "-m"))
	i_flags |= GHT_HEURISTICS_MOVE_TO_FRONT;
      else if(strcmp(argv[1], "-t"))
	i_flags |= GHT_HEURISTICS_TRANSPOSE;
      p_dict_name = argv[2];
      p_text_name = argv[3];      
    }
  else
    {
      /* 2 arguments */
      p_dict_name = argv[1];
      p_text_name = argv[2];
    }

  /* Create a new hash table */
  if ( !(p_table = ght_create(1000, NULL, i_flags|GHT_AUTOMATIC_REHASH)) )
    {
      return 1;
    }

  /* Open the dictionary file (first check its size) */
  if (stat(p_dict_name, &stat_struct) < 0)
    {
      perror("stat");
      return 1;
    }
  if (!(p_buf = malloc(stat_struct.st_size)))
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
      int *p_data;
      
      if ( !(p_data = malloc(sizeof(int))) )
	{
	  perror("malloc");
	  return 1;
	}
      *p_data = i_cnt++;
      
      /* Insert the word into the table */
      if (ght_insert(p_table,
		     p_data,
		     strlen(p_tok), p_tok) < 0)
	free(p_data); /* Could not insert the item (already exists), free it. */
      else
	i_found++;
      p_tok = strtok(NULL, DELIMS);
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
  if (!(p_buf = malloc(stat_struct.st_size)))
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
      if (ght_get(p_table,
		  strlen(p_tok), p_tok))
	{
	  i_found++;
	}
      i_cnt++;
      p_tok = strtok(NULL, DELIMS);
    }
  free(p_buf);

  printf("Found %d words out of %d words\n", i_found, i_cnt);

  /* Free the entry data in the table */
  for(p_e = ght_first(p_table, &iterator); p_e; p_e = ght_next(p_table, &iterator))
    {
      free(p_e);
    }

  /* Free the table */
  ght_finalize(p_table);

  return 0;
}
