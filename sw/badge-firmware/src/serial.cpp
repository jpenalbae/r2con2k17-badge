/**********************************************************************
 *           serial.cpp
 *
 * Tue Aug 29 03:26:34 CEST 2017
 * Copyright  2017  Jaime Peñalba Estébanez
 * jpenalbae@gmail.com
 *
 * Serial configuration menu
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

#include <Arduino.h>
#include "config.h"

void wifi_connect(void);

bool serial_config = false;


void serial_stop_config() {
   if (serial_config == true)
      Serial.println("\r\nConfiguration aborted.");
   serial_config = false;
};

void serial_discard_data()
{
   int x, avail, byte;

   delay(100); // Stupid hack
   avail = Serial.available();
   for (x=0; x<avail; x++)
      byte = Serial.read();
}

int serial_read_input(char *buffer)
{
   int byte, x = 0;

   while (serial_config) {
      if (!Serial.available())
         continue;

      byte = Serial.read();

      if (((char)byte == '\r') || ((char)byte == '\n')) {
         serial_discard_data();
         buffer[x] = '\0';
         Serial.print("\r\n");
         return x;
      }

      /* Ignore non printable chars */
      if (((char)byte < 0x20) ||  ((char)byte > 0x7E))
         continue;

      buffer[x++] = (char)byte;
      Serial.print((char)byte);
      //Serial.printf(" (0x%X) ", (char)byte);
   }

   return -1;
}

void serial_loop()
{
   int read;

   if (Serial.available())
   {
      serial_config = true;
      serial_discard_data();

      Serial.println("\nWireless configuration menu");
      Serial.println("Only Open and WPA networks are supported");
      Serial.println("For Open network leave password blank");
      Serial.println("To exit this menu press any button on the badge.\n");

      Serial.print("Enter ESSID: ");
      read = serial_read_input(cfg->essid);
      if (!serial_config)
         return;

      Serial.printf("Got ESSID: %s\r\n", cfg->essid);

      Serial.print("Enter Password: ");
      read = serial_read_input(cfg->password);
      if (!serial_config)
         return;

      if (read <= 0) {
         memset(cfg->password, 0, sizeof(cfg->password));
         Serial.println("No password. Using open network config");
      } else {
         Serial.printf("Got WPA password: %s\r\n", cfg->password);
      }

      config_save();
      Serial.println("Configuration saved to EEPROM.");

      wifi_connect();
   }
}
