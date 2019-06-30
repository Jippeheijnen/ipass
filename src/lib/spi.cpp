#include "spi.hpp"

/*!
	\author jippe
	\date 01/07/19
	\file spi.cpp
 */

spi::bus::bus(hwlib::target::pin_out &clk, hwlib::target::pin_out &mosi, hwlib::target::pin_out &cs) :
	clk(hwlib::pin_direct_from_out_t(clk)),
	mosi(hwlib::pin_direct_from_out_t(mosi)),
	cs(hwlib::pin_direct_from_out_t(cs))
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