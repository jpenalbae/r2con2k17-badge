/**********************************************************************
 *           stats.cpp
 *
 * Wed Aug 30 02:15:24 CEST 2017
 * Copyright  2017  Jaime Peñalba Estébanez
 * jpenalbae@gmail.com
 *
 * Download and handle code for github statistics
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
#include <ESP8266WiFi.h>

#include "stats.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


struct _commits *commits;
struct _prs *prs;
struct _issues *issues;
struct _status *status;
struct _top_contribs *top_contribs;

uint8_t buffer[128];


int get_stats(char *path, uint8_t *dest, size_t len)
{
   int left, offset, pending;
   size_t sz;
   char *start;
   WiFiClient client;
   unsigned long timeout;

   left = len;

   Serial.printf("Getting stats path: %s\r\n", path);

   if (!client.connect(HOST, PORT)) {
      Serial.println("Connection failed");
      goto close;
   }

   sprintf((char *)buffer, "GET %s HTTP/1.0\r\n\r\n", path);
   if (client.write((char *)buffer) < 1) {
      Serial.println("Http request write failed");
      goto close;
   }

   timeout = millis();
   while (client.available() == 0) {
      if (millis() - timeout > TIMEOUT) {
         Serial.println("Client Timeout");
         goto close;
      }
   }

   //This is totally wrong, but headers should always fit in first 512 bytes
   sz = client.read(buffer, sizeof(buffer));
   if (!(start = strstr((char *)buffer, "\r\n\r\n"))) {
      Serial.println("Could not find headers start");
      goto close;
   }

   offset = start - (char *)buffer + 4;
   pending = sz - offset;
   //Serial.printf("First read was: %d. offset %d\r\n", sz, offset);

   memset(dest, '\0', len);
   memcpy(dest, &buffer[offset], MIN(pending, left));
   offset = MIN(pending, left);
   left -= offset;

   /* Read pending data if any */
   if ((left > 0) && client.available()) {
      sz = client.read(&dest[offset], left);
      //Serial.printf("Another read was: %d\r\n", sz);
      if (sz != left)
         Serial.printf("Only read %d of %d pending bytes\r\n", sz, left);
   }

   //Serial.println("Got data:");
   //Serial.write(dest, len);

   client.stop();
   return 0;

close:
   client.stop();
   return 1;
}


void stats_init()
{
   top_contribs = (struct _top_contribs *)malloc(sizeof(struct _top_contribs));
   if (!top_contribs)
      Serial.println("Error allocating top_contribs buffer");
   else
      memset(top_contribs, '\0', sizeof(struct _top_contribs));

   status = (struct _status *)malloc(sizeof(struct _status));
   if (!status)
      Serial.println("Error allocating status buffer");
   else
      memset(status, '\0', sizeof(struct _status));

   issues = (struct _issues *)malloc(sizeof(struct _issues));
   if (!issues)
      Serial.println("Error allocating issues buffer");
   else
      memset(issues, '\0', sizeof(struct _issues));

   commits = (struct _commits *)malloc(sizeof(struct _commits));
   if (!commits)
      Serial.println("Error allocating commits buffer");
   else
      memset(commits, '\0', sizeof(struct _commits));

   prs = (struct _prs *)malloc(sizeof(struct _prs));
   if (!prs)
      Serial.println("Error allocating prs buffer");
   else
      memset(prs, '\0', sizeof(struct _prs));
}

void stats_update()
{
   int x;

   if (WiFi.status() != WL_CONNECTED) {
      Serial.println("No wireless connection available to download stats");
      return;
   }

   get_stats((char *)"/r2k17/contribs", (uint8_t *)top_contribs,
         sizeof(struct _top_contribs));
   get_stats((char *)"/r2k17/status", (uint8_t *)status,
         sizeof(struct _status));
   get_stats((char *)"/r2k17/prs", (uint8_t *)prs,
         sizeof(struct _prs));
   get_stats((char *)"/r2k17/issues", (uint8_t *)issues,
         sizeof(struct _issues));
   get_stats((char *)"/r2k17/commits", (uint8_t *)commits,
         sizeof(struct _commits));

}
