
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

	auto dat = hwlib::target::pin_out(hwlib::target::pins::d50);
	auto wr = hwlib::target::pin_out(hwlib::target::pins::d51);
	auto cs = hwlib::target::pin_out(hwlib::target::pins::d52);
	auto spi_bus = spi::bus(wr, dat, cs);
	
	auto m = HT_1632(spi_bus, matrix::commands::HT1632_COMMON_16NMOS);
	m.setBrightness(0x00);
	
	for (;;) {
		for (int i=0;i<24;i++) {
			for (int j=0;j<16;j++) {
				m.setPixel(j,i);
				m.writeScreen();
				hwlib::wait_ms(2);
			}
		}
		
		for (int i=0;i<24;i++) {
			for (int j=0;j<16;j++) {
				m.setPixel(j,i);
			}
		}

		m.writeScreen();
		hwlib::wait_ms(1000);
		for (int i=0;i<24;i++) {
			for (int j=0;j<16;j++) {
				m.clearPixel(j,i);
				m.writeScreen();
				hwlib::wait_ms(2);
			}
		}
		hwlib::wait_ms(1000);
	}
}


