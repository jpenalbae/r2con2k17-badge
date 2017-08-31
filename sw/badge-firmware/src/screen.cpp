/**********************************************************************
 *           screen.cpp
 *
 * Wed Aug 30 02:15:24 CEST 2017
 * Copyright  2017  Jaime Peñalba Estébanez
 * jpenalbae@gmail.com
 *
 * Handle screen drawings, etc...
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
#include "SSD1306.h"

#include "images.h"
#include "stats.h"
#include "screen.h"


int overlay = 0;
extern SSD1306  display;


void (*show_overlay[])() = {
   draw_splash_screen,
   draw_top_commits,
   draw_status,
   draw_issues,
   draw_prs,
   draw_commits
};


void draw_top_commits()
{
   int x;
   char *buff = (char *)buffer;

   display.clear();
   display.drawXbm(0, 1, trophy_width, trophy_height, trophy_bits);

   display.setFont(ArialMT_Plain_16);
   display.drawString(20, 0, "Top Contribs");

   display.setFont(ArialMT_Plain_10);
   for (x=0; x<8; x+=2) {
      sprintf(buff, "%d. %s", x+1, top_contribs->list[x].name);
      display.drawString(1, (10*(x/2)+20), buff);

      sprintf(buff, "%d. %s", x+2, top_contribs->list[x+1].name);
      display.drawString(65, (10*(x/2)+20), buff);
   }

   display.display();
}

void draw_splash_screen()
{
   display.clear();
   display.drawXbm(17, 0, logo_width, logo_height, logo_bits);
   display.display();
}

void draw_issues()
{
   int x;

   display.clear();
   display.drawXbm(0, 1, issue_width, issue_height, issue_bits);

   display.setFont(ArialMT_Plain_16);
   display.drawString(20, 0, "Issues");

   display.setFont(ArialMT_Plain_10);
   for (x=0; x<4; x++) {
      if (issues->list[x].id == 0)
         break;

      issues->list[x].title[30] = '\0';
      display.drawString(1, 10*x+20, issues->list[x].title);
   }

   display.display();
}


void draw_prs()
{
   int x;

   display.clear();
   display.drawXbm(0, 1, pr_width, pr_height, pr_bits);

   display.setFont(ArialMT_Plain_16);
   display.drawString(20, 0, "Pull Requests");

   display.setFont(ArialMT_Plain_10);
   for (x=0; x<4; x++) {
      if (prs->list[x].id == 0)
         break;

      prs->list[x].title[30] = '\0';
      display.drawString(1, 10*x+20, prs->list[x].title);
   }

   display.display();
}


void draw_commits()
{
   int x;

   display.clear();
   display.drawXbm(0, 1, commits_width, commits_height, commits_bits);

   display.setFont(ArialMT_Plain_16);
   display.drawString(20, 0, "Commits");

   display.setFont(ArialMT_Plain_10);
   for (x=0; x<4; x++) {
      if (commits->list[x].title[0] == '\0')
         break;

      //display.drawStringMaxWidth(1, 10*x+20, 60, commits->list[x].title);
      commits->list[x].title[30] = '\0';
      display.drawString(1, 10*x+20, commits->list[x].title);
   }

   display.display();
}

void draw_status()
{

   display.clear();

   if (!strcmp(status->global, "success"))
      display.drawXbm(0, 1, ok_width, ok_height, ok_bits);
   else if (!strcmp(status->global, "pending"))
      display.drawXbm(0, 1, loading_width, loading_height, loading_bits);
   else
      display.drawXbm(0, 1, bad_width, bad_height, bad_bits);

   display.setFont(ArialMT_Plain_16);
   display.drawString(20, 0, "Master Status");

   display.setFont(ArialMT_Plain_10);
   display.drawString(20, 20, "Travis");
   display.drawString(74, 20, "AppVeyor");


   if (!strcmp(status->travis, "success"))
      display.drawXbm(28, 33, ok_width, ok_height, ok_bits);
   else if (!strcmp(status->travis, "pending"))
      display.drawXbm(28, 33, loading_width, loading_height, loading_bits);
   else
      display.drawXbm(28, 33, bad_width, bad_height, bad_bits);

   if (!strcmp(status->appveyor, "success"))
      display.drawXbm(84, 33, ok_width, ok_height, ok_bits);
   else if (!strcmp(status->appveyor, "pending"))
      display.drawXbm(84, 33, loading_width, loading_height, loading_bits);
   else
      display.drawXbm(84, 33, bad_width, bad_height, bad_bits);

   display.drawString(20, 50, status->travis);
   display.drawString(74, 50, status->appveyor);

   display.display();
}

void screen_refresh()
{
   show_overlay[overlay]();
}
