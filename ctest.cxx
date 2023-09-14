/*
 * ctest.cxx
 * 
 * Copyright 2023 Lars Vatne <lvatne@piServer01>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>

#include <wiringPi.h>

using namespace std;		// No need to keep using “std”

int main() {
  wiringPiSetupGpio();			// Setup the library
  pinMode(18, OUTPUT);		// Configure GPIO0 as an output

  // Main program loop
  for (int i = 0; i < 200000; i++) {
  	  // Toggle the LED
	  digitalWrite(18, 1);
      delayMicroseconds(12); 	// Delay 500ms
      digitalWrite(18, 0);
      delayMicroseconds(13);
  }
  return 0;
}
