/**********************************************************************
 *           stats.h
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

#ifndef _STATS_H
#define _STATS_H

#include <Arduino.h>


#define TIMEOUT   5000              // HTTP timeout in milliseconds
#define HOST      "176.31.116.94"   // Backend host address
#define PORT      9615              // Backend host port

struct _top_contribs {
   struct {
      char name[12];
      uint32_t commits;
   } list[10];
} __attribute__((packed));


struct _status {
      char global[16];
      char travis[16];
      char appveyor[16];
} __attribute__((packed));

struct _prs {
   struct {
      char title[64];
      uint32_t id;
   } list[10];
} __attribute__((packed));

struct _issues {
   struct {
      char title[64];
      uint32_t id;
   } list[10];
} __attribute__((packed));

struct _commits {
   struct {
      char title[64];
   } list[10];
} __attribute__((packed));


extern struct _commits *commits;
extern struct _prs *prs;
extern struct _issues *issues;
extern struct _status *status;
extern struct _top_contribs *top_contribs;
extern uint8_t buffer[128];


void stats_init();
void stats_update();

#endif //_STATS_H
