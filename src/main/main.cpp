
/*
##############################################################
#                 (c) Jippe Heijnen 2019                     #
#                                                            #
#   Distributed under the Boost Software License v1.0.       #
#   (See accompanying file LICENSE_1_0.txt or copy at        #
#          https://www.boost.org/LICENSE_1_0.txt)            #
##############################################################


		To easily flash code to the Arduino Due,
		use the following line of code:
		cmd.exe /c @mode COM6:1200; bossac -p ttyS6 -e; make run -j 16 --silent; make clean --silent


*/

// ===========================================================================

#include "matrixLib.hpp"
#include <array>

using matrix::commands, matrix::HT_1632;

// ===========================================================================

int main(){

/*!
	\brief Demo script
	 * 
	 * This is a short demo script which can be used to figure out how the HT1632 library works
 */

	auto dat = hwlib::target::pin_out(hwlib::target::pins::d30);
	auto wr = hwlib::target::pin_out(hwlib::target::pins::d31);
	auto cs1 = hwlib::target::pin_out(hwlib::target::pins::d32);
	auto cs2 = hwlib::target::pin_out(hwlib::target::pins::d33);
	auto spi_bus = spi::bus(wr, dat, cs1);
	auto spi_bus1 = spi::bus(wr, dat, cs2);
	
	cs2.write(1);
	auto m = HT_1632(spi_bus, matrix::commands::HT1632_COMMON_16NMOS);
	auto m1 = HT_1632(spi_bus1, matrix::commands::HT1632_COMMON_16NMOS);
	m.setBrightness(0xFF);
	m1.setBrightness(0xFF);
	
	for (;;) {
		for (int i=0;i<24;i++) {
			for (int j=0;j<16;j++) {
				m.setPixel(j,i);
				m1.clearPixel(j,i);
				m.writeScreen();
				m1.writeScreen();
				hwlib::wait_ms(1);
			}
		}

		m.writeScreen();
		for (int i=0;i<24;i++) {
			for (int j=0;j<16;j++) {
				m.clearPixel(j,i);
				m1.setPixel(j,i);
				m.writeScreen();
				m1.writeScreen();
				hwlib::wait_ms(1);
			}
		}
	}
}


