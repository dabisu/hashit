/*********************************************************************
 *                
 * Copyright (C) 2002,  Dru Lemley
 *                
 * Filename:      crc.h
 * Description:   Definitions for CRC hashing
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
 * $Log: crc.h,v $
 * Revision 1.1.1.1  2002/02/26 18:21:25  simon
 * libghthash MK2
 *
 *                
 ********************************************************************/
#ifndef CRC_H
#define CRC_H

#include "ght_hash_table.h" /* typedefs */

ght_uint32_t __crc32(void *data, int length);

#endif
