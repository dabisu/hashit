/*********************************************************************
 *                
 * Copyright (C) 2002,  Simon Kagstrom
 *                
 * Filename:      simple.c
 * Description:   A simple sample program to demonstrate the
 *                usage of the generic hash table.
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
 * $Log: simple.c,v $
 * Revision 1.1.1.1  2002/02/26 18:21:25  simon
 * libghthash MK2
 *
 *                
 ********************************************************************/

#include <string.h>          /* strlen */
#include <stdio.h>           /* printf */
#include <stdlib.h>          /* malloc */

/* Include the hash table (would normally be #include <ght_hash_table> but we
   want to be sure to include the one in this directory here) */
#include "ght_hash_table.h"

/* This is a very short example of the usage of the hash table */
int main(int argc, char *argv[])
{
  ght_hash_table_t *p_table;
  char *p_key_data;
  int *p_data;
  char *p_str;
  int *p_he;

  p_table = ght_create(128, NULL, GHT_HEURISTICS_MOVE_TO_FRONT);

  if ( !(p_data = malloc(sizeof(int))) )
    return -1;
  /* Assign a value to the key */
  p_key_data = "blabla";

  /* Assign the data a value */
  *p_data = 15;

  /* Insert "blabla" into the hash table */
  ght_insert(p_table, 
	     p_data,
	     sizeof(char)*strlen(p_key_data), p_key_data);

  /* Create the search key */
  p_str = "blabla";

  /* Search for "blabla" */
  if ( (p_he = ght_get(p_table, 
		       sizeof(char)*strlen(p_str), p_str)) )
    printf("Found %d\n", *p_he);
  else
    printf("Did not find anything!\n");

  /* Remove the hash table */
  ght_finalize(p_table);

  return 0;
}
