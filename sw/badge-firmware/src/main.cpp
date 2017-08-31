/**********************************************************************
 *           main.cpp
 *
 * Tue Aug 29 03:26:34 CEST 2017
 * Copyright  2017  Jaime Peñalba Estébanez
 * jpenalbae@gmail.com
 *
 * Basic firmware for the r2con2k17 badge
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
#include <WiFiClient.h>

#include "SSD1306.h"

#include "config.h"
#include "serial.h"
#include "stats.h"
#include "screen.h"
#include "images.h"


#define DEBOUNCE_DELAY  100
#define UPDATE_DELAY    5*60*1000 // Time beween updates in miliseconds

#define LED       2
#define BTN_LEFT  5
#define BTN_RIGHT 12
#define BTN_ENTER 0

// display(0x3C, SDA, SCL);
SSD1306  display(0x3C, 14, 4);
volatile long debounce[16] = { 0 };
volatile long last_millis;


void wifi_connect()
{
   int counter = 0;

   WiFi.disconnect(true);
   delay(500);
   WiFi.mode(WIFI_OFF);
   delay(500);
   WiFi.mode(WIFI_STA);

   if (cfg->essid[0] == '\0') {
      Serial.println("No wireless config found. Please configure.");
      WiFi.mode(WIFI_OFF);
      return;
   } else if (cfg->password[0] == '\0') {
      Serial.printf("Connecting to network %s without password.", cfg->essid);
      WiFi.begin(cfg->essid);
   } else {
      Serial.printf("Connecting to network %s with password.", cfg->essid);
      WiFi.begin(cfg->essid, cfg->password);
   }

   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");

      if (counter++ > 20) {
         Serial.println("");
         Serial.println("Timeout connecting to the network");
         return;
      }
   }

   Serial.print("Connected with IP: ");
   Serial.println(WiFi.localIP());
}


void handle_button(int button)
{
   if ((millis() - debounce[button]) <= DEBOUNCE_DELAY)
     return;

   debounce[button] = millis();

   serial_stop_config();
   Serial.printf("Button %d is: %d\r\n", button, digitalRead(12));

   if (button == BTN_LEFT) {
      overlay = (overlay <= 0)? 5 : overlay-1;
   } else if (button == BTN_RIGHT) {
      overlay = (overlay >= 5)? 0 : overlay+1;
   }
}


void push_left()  { handle_button(BTN_LEFT); }
void push_right() { handle_button(BTN_RIGHT); }
void push_enter() { handle_button(BTN_ENTER); }


void buttons_init()
{
   //pinMode(4, INPUT);
   pinMode(BTN_LEFT, INPUT_PULLUP);
   pinMode(BTN_RIGHT, INPUT_PULLUP);
   pinMode(BTN_ENTER, INPUT_PULLUP);

   // LED
   pinMode(LED, OUTPUT);
   digitalWrite(LED, HIGH);

   attachInterrupt(BTN_LEFT, push_left, HIGH);
   attachInterrupt(BTN_RIGHT, push_right, HIGH);
   attachInterrupt(BTN_ENTER, push_enter, HIGH);
}

void setup()
{
   /* Initi serial console */
   Serial.begin(115200);
   Serial.println("\r\n\r\nBooting...");

   /* Init the screen */
   display.init();
   display.flipScreenVertically();
   draw_splash_screen();


   /* Init EEPROM config and wireless */
   if (config_init())
      Serial.println("EEPROM config corrupted. Resetting config.");

   /* Connect to wifi and download stats */
   wifi_connect();
   stats_init();
   stats_update();

   buttons_init();
   last_millis = millis();

   Serial.println("Setup done");
}


void loop() {

   /* Check wireless connection */
   if (WiFi.status() != WL_CONNECTED)
      digitalWrite(LED, HIGH);
   else
      digitalWrite(LED, LOW);

   screen_refresh();
   serial_loop();

   /* Check if we need to update stats */
   if ((millis() - last_millis) >= UPDATE_DELAY) {
      stats_update();
      last_millis = millis();
   }

   // Fake 30 fps delay
   delay(20);

   return;

}
