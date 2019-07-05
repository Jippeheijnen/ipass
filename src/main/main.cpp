
/*
##############################################################
#                 (c) Jippe Heijnen 2019                     #
#                                                            #
#   Distributed under the Boost Software License v1.0.       #
#   (See accompanying file LICENSE_1_0.txt or copy at        #
#          https://www.boost.org/LICENSE_1_0.txt)            #
##############################################################
*/

// ===========================================================================

#include "matrixLib.hpp"
#include <array>

using matrix::commands, matrix::HT_1632, matrix::matrixWindow;

// ===========================================================================

int main(){

/*!
	\brief Demo script
	 * 
	 * This is a short demo script which can be used to figure out how the HT1632 library works
 */

	auto dat = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto wr = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto cs = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto spi_bus = spi::bus(wr, dat, cs);
	
	auto m = HT_1632(spi_bus, matrix::commands::HT1632_COMMON_16NMOS);
	m.setBrightness(0xf);
	
	for (;;) {
		for (int i=0;i<24;i++) {
			for (int j=0;j<16;j++) {
				m.setPixel(j,i);
				m.writeScreen();
				hwlib::wait_ms(1);
			}
		}
			for (int i=15; i>=0;i--) {
			m.setBrightness(i);
			hwlib::wait_ms(50);
		}
		for (int i=0; i<=15;i++) {
			m.setBrightness(i);
			hwlib::wait_ms(50);
		}
		m.blink(2);
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
				hwlib::wait_ms(1);
			}
		}
		hwlib::wait_ms(1000);
	}
}


