
/*
##############################################################
#                 (c) Jippe Heijnen 2019                     #
#                                                            #
#   Distributed under the Boost Software License v1.0.       #
#   (See accompanying file LICENSE_1_0.txt or copy at        #
#          https://www.boost.org/LICENSE_1_0.txt)            #
##############################################################
*/

#include "spi.hpp"

/*!
	\author jippe
	\date 01/07/19
	\file spi.cpp
 */

spi::bus::bus(hwlib::pin_out &clk, hwlib::pin_out &mosi, hwlib::pin_out &cs) :
	clk(hwlib::direct(clk)),
	mosi(hwlib::direct(mosi)),
	cs(hwlib::direct(cs))
	{}

spi::transaction::transaction(spi::bus &b) :
	b(b), clk(b.clk), mosi(b.mosi), cs(b.cs)
	{
	cs.write(0);  // start of transmission
	}

void spi::transaction::writeData(uint8_t bits, uint16_t d) {
	for (uint16_t bit = 1<<(bits-1); bit; bit >>= 1) {
		clk.write(0);
		mosi.write((d & bit) ? 1 : 0);
		clk.write(1);
	}
}

spi::transaction::~transaction() {
	cs.write(1);  // end of transmission
}