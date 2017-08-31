/**********************************************************************
 *           config.h
 *
 * Tue Aug 29 03:26:34 CEST 2017
 * Copyright  2017  Jaime Peñalba Estébanez
 * jpenalbae@gmail.com
 *
 * EEPROM configuration values
 *
 **********************************************************************/

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

#define EEPROM_SIZE  512
#define MAGIC        0xDEADBEEF

/*
 * Add new fields at the end to keep compatibility
 * Max structure size should be 512 bytes
 */
struct config {
   uint32_t magic;
   char essid[32];
   char password[64];
};


extern struct config *cfg;

int config_init(void);
void config_save(void);


#endif  //_CONFIG_H
