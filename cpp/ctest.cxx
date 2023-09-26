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
#include <chrono>
#include <ctime>    
#include <tgmath.h>


#include <wiringPi.h>

using namespace std;		// No need to keep using “std”

class GPIO_assignments {       
  public:             // GPIO pin assignments
	int A_USound_WR = 18;
	int A_USound_RD = 23;
	int B_USound_WR = 24;
	int B_USound_RD = 25;
	int C_USound_WR = 4;
	int C_USound_RD = 27;
	int D_USound_WR = 5;
	int D_USound_RD = 6;

};

class ToneGenerator {
	public:
		void generate_tone(int frequency, int duration_uSec, int gpio_pin) {
			double switchdelay = 1.0 / frequency;  // switchdelay now in seconds
			double uSec_delay = switchdelay * 1000000.0;
			double halfdelay = uSec_delay / 2.0;
			int num_iterations = round(duration_uSec / (halfdelay * 2.0));
			cout << "frequency " << frequency / 1000.0 << "kHz" << std::endl;
			cout << "switchdelay " << switchdelay << " Sec = " << uSec_delay << " uSec" << std::endl;
			cout << "halfdelay " << halfdelay << " iterations " << num_iterations << std::endl;
			for (int i = 0; i < num_iterations; i++) {
				digitalWrite(gpio_pin, 1);
				delayMicroseconds(halfdelay);
				digitalWrite(gpio_pin, 0);
				delayMicroseconds(halfdelay);
			}
		}
};
			

	
int main() {
	GPIO_assignments gpio;
	ToneGenerator tg;
	wiringPiSetupGpio();			// Setup the library
	pinMode(gpio.A_USound_WR, OUTPUT);		// Configure GPIOxx as an output
	cout << gpio.A_USound_WR << "\n";
	
	auto start = std::chrono::high_resolution_clock::now();
	
	tg.generate_tone(40000, 5000000, gpio.A_USound_WR);

	
	// Main program loop
	/*
	for (int i = 0; i < 200000; i++) {
		// Toggle the LED &
		digitalWrite(gpio.A_USound_WR, 1);
		delayMicroseconds(12); 
		digitalWrite(gpio.A_USound_WR, 0);
		delayMicroseconds(13);
	}
	*/
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    auto elapsed = end - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "elapsed time: " << microseconds << " uSec"
              << std::endl;

	return 0;
}
