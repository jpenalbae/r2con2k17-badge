/**********************************************************************
 *           config.cpp
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

#include <EEPROM.h>
#include "config.h"

struct config *cfg;

int config_init(void)
{
   EEPROM.begin(EEPROM_SIZE);
   cfg = (struct config *)EEPROM.getDataPtr();

   // Check for valid EEPROM data
   if (cfg->magic != MAGIC) {
      memset(cfg, 0, EEPROM_SIZE);
      cfg->magic = MAGIC;
      config_save();
      return 1;
   }

   return 0;
}

void config_save(void)
{
   EEPROM.getDataPtr(); // Hack for this stupid API to mark data as dirty
   EEPROM.commit();
}
