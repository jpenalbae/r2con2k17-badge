/**********************************************************************
 *           screen.h
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

#ifndef _SCREEN_H
#define _SCREEN_H


void draw_top_commits();
void draw_issues();
void draw_prs();
void draw_commits();
void draw_status();
void draw_splash_screen();

void screen_refresh();


extern int overlay;
extern void (*show_overlay[])();

#endif //_SCREEN_H
