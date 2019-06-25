#ifndef MATRIXLIB
#define MATRIXLIB

#include "hwlib.hpp"

namespace spi {
	class bus {
	protected:
		hwlib::pin_direct_from_out_t clk;
		hwlib::pin_direct_from_out_t mosi;
		hwlib::pin_direct_from_out_t cs;
		friend class transaction;
	public:
		bus(hwlib::target::pin_out &clk, hwlib::target::pin_out &mosi, hwlib::target::pin_out &cs):
			clk(hwlib::pin_direct_from_out_t(clk)),
			mosi(hwlib::pin_direct_from_out_t(mosi)),
			cs(hwlib::pin_direct_from_out_t(cs))
			{}
	};

	class transaction{
	protected:
		spi::bus &b;
		hwlib::pin_direct_from_out_t &clk;
		hwlib::pin_direct_from_out_t &mosi;
		hwlib::pin_direct_from_out_t &cs;
		friend class bus;
	public:
		transaction(spi::bus &b) :
			b(b), clk(b.clk), mosi(b.mosi), cs(b.cs)
		{
			cs.write(0);  // start of transmission
		}

		void writeData(uint8_t bits, uint16_t d) {
			for (uint16_t bit = 1<<(bits-1); bit; bit >>= 1) {
				clk.write(0);
				mosi.write((d & bit) ? 1 : 0);
				clk.write(1);
			}
		}

		~transaction() {
			cs.write(1);  // end of transmission
		}

	};
}


namespace matrix {

struct commands {

static constexpr const uint8_t HT1632_READ			= 0x06;

static constexpr const uint8_t HT1632_WRITE			= 0x05;
static constexpr const uint8_t HT1632_COMMAND		= 0x04;

static constexpr const uint8_t HT1632_SYS_DIS		= 0x00;
static constexpr const uint8_t HT1632_SYS_EN		= 0x01;
static constexpr const uint8_t HT1632_LED_OFF		= 0x02;
static constexpr const uint8_t HT1632_LED_ON		= 0x03;
static constexpr const uint8_t HT1632_BLINK_OFF		= 0x08;
static constexpr const uint8_t HT1632_BLINK_ON		= 0x09;
static constexpr const uint8_t HT1632_SLAVE_MODE	= 0x10;
static constexpr const uint8_t HT1632_MASTER_MODE 	= 0x14;
static constexpr const uint8_t HT1632_INT_RC		= 0x18;
static constexpr const uint8_t HT1632_EXT_CLK		= 0x1C;
static constexpr const uint8_t HT1632_PWM_CONTROL	= 0xA0;

static constexpr const uint8_t HT1632_COMMON_8NMOS	= 0x20;
static constexpr const uint8_t HT1632_COMMON_16NMOS	= 0x24;
static constexpr const uint8_t HT1632_COMMON_8PMOS	= 0x28;
static constexpr const uint8_t HT1632_COMMON_16PMOS	= 0x2C;

};


class HT_1632 {
protected:
	spi::bus spi_bus;

void sendCommand(uint16_t cmd){
	spi::transaction(spi_bus).writeData(12, (((uint16_t)commands::HT1632_COMMAND << 8) | cmd) << 1);
}

void writeRam(uint8_t addr, uint8_t data) {

	uint16_t d = commands::HT1632_WRITE;
	d <<= 7;
	d |= addr & 0x7F;
	d <<= 4;
	d |= data & 0xF;
	spi::transaction(spi_bus).writeData(14, d);
}


public:
	uint16_t ledmatrix[24] = {0};
	uint16_t type;

HT_1632(spi::bus spi_bus,
	uint16_t type):
	spi_bus(spi_bus),
	type(type)
	{}

void begin() {

	sendCommand(commands::HT1632_SYS_EN);
	sendCommand(commands::HT1632_LED_ON);
	sendCommand(commands::HT1632_BLINK_OFF);
	sendCommand(commands::HT1632_MASTER_MODE);
	sendCommand(commands::HT1632_INT_RC);
	sendCommand(type);
	sendCommand(commands::HT1632_PWM_CONTROL | 0xf);

	for (int i=0;i<24;i++) {
		ledmatrix[i]=0;
	}

	for (int i=0; i<24*4; i++) {
		writeRam(i, 0x00);
	}

}


// set to true for memory debugging
#define debug false

void dumpMem() {
	if (debug) {
		hwlib::wait_ms(10);
		for (uint16_t i=0; i<24;i++) {
			if(i%2 == 0)
				hwlib::cout << '\n' ;
			hwlib::cout << hwlib::bin << ledmatrix[i] << '|' << hwlib::flush;
			hwlib::wait_ms(5);
		}
		hwlib::cout << '\n';
	} else {
		for (unsigned int i=0; i<24; i++) {
			ledmatrix[i] = 0x00;
		}
		if (debug)
			hwlib::cout << "memory cleared\n";
	}
}

void setPixel(uint16_t x, uint16_t y) {

	if((x < 0) || (x >= 16) || (y < 0) || (y >= 24)) return;
	ledmatrix[y] |= 0x8000 >> x;

}

void clearPixel(uint16_t x, uint16_t y) {

	if((x < 0) || (x >= 16) || (y < 0) || (y >= 24)) return;
	ledmatrix[y] &= ~(0x8000 >> x);

}

void writeScreen() {
	auto spi_transaction = spi::transaction(spi_bus);
	spi_transaction.writeData(3, commands::HT1632_WRITE);
	spi_transaction.writeData(7,0);
	
	for(uint16_t i=0; i<24; i++) {
		spi_transaction.writeData(16, ledmatrix[i]);
	}
	spi_transaction.~transaction();

}

void setBrightness(uint8_t b) {
	sendCommand(commands::HT1632_PWM_CONTROL | b);
}

};

class matrixWindow : public hwlib::window {
	// TODO: merge matrixWindow with ht_1632 class.
private:
	HT_1632 &matrix;
public:
	matrixWindow(int x, int y, HT_1632 &matrix) :
		hwlib::window(hwlib::xy(x,y)),
		matrix(matrix)
		{}

	void write_implementation(hwlib::xy pos, hwlib::color col) override {
		matrix.setPixel(pos.x, pos.y);
	}

	void flush() override {
		matrix.writeScreen();
	}

	void clear()  {
		matrix.dumpMem();
		matrix.writeScreen();
	}

};
};

#endif  // MATRIXLIB
