/*********************************************************************
 *
 * Copyright (C) 2002,  Simon Kagstrom
 *
 * Filename:      mean.c
 * Description:   A simple program that counts the mean from a
 *                number of lines.
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
 * $Log: mean.c,v $
 *
 ********************************************************************/
#include <stdlib.h>    /* malloc, atof etc */
#include <string.h>    /* strncpy */
#include <unistd.h>    /* ssize_t */
#include <stdio.h>     /* File reading etc. */
#include <sys/types.h> /* stat */
#include <sys/stat.h>  /* stat */
#include <stddef.h>

#define DELIMS " \n\r"

int main(int argc, char *argv[])
{
  struct stat stat_struct;
  FILE *p_file;
  double d_res=0;
  char *p_buf;
  char *p_tok;
  int i_cnt=0;

  if (stat(argv[1], &stat_struct) < 0)
    {
      perror("stat");
      return 1;
    }
  if (!(p_buf = (char*)malloc(stat_struct.st_size)))
    {
      perror("malloc");
      return 1;
    }

  if (! (p_file = fopen(argv[1], "r")) )
    {
      perror("fopen");
      return 1;
    }
  fread(p_buf, sizeof(char), stat_struct.st_size, p_file);
  fclose(p_file);

  p_tok = strtok(p_buf, DELIMS);
  while (p_tok)
    {
      d_res += atof(p_tok);
      p_tok = strtok(NULL, DELIMS);
      i_cnt++;
    }
  free(p_buf);
  if (!i_cnt)
    printf("0.0");
  else
    printf("%f\n", (d_res/i_cnt));

  return 0;
}
