/*
* This file is part of SkunkDB
* Copyright (c) 2001 Gianni Tedesco
*
*    SkunkDB is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    SkunkDB is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with SkunkDB; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <skunk.h>
#include <skunk_hash.h>

/* Fowler/Noll/Vo hash 
* Its fast to compute, its obvious code, it performs well... */
skunkhash_t inline skunk_hash(u_int8_t *data, skunk_ofs_t len)
{
	skunkhash_t h=0;

	while(len) {
		h = (h*16777619) ^ *data++;
		--len;
	}
	
	return h;
}
