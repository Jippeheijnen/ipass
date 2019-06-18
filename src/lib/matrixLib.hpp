#ifndef MATRIXLIB
#define MATRIXLIB

#include "hwlib.hpp"

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
	hwlib::pin_direct_from_out_t dat;
	hwlib::pin_direct_from_out_t wr;
	hwlib::pin_direct_from_out_t cs;
	
void writeData(uint8_t bits, uint16_t d) {
	cs.write(false);  // start of transmission
	for (uint16_t bit = 1<<(bits-1); bit; bit >>= 1) {
		wr.write(false);
		dat.write((d & bit) ? true : false);
		wr.write(true);
	}
	cs.write(true);  // end of transmission				
}
	
void sendCommand(uint16_t cmd){
	writeData(12, (((uint16_t)commands::HT1632_COMMAND << 8) | cmd) << 1);
}
	
void writeRam(uint8_t addr, uint8_t data) {
	
	uint16_t d = commands::HT1632_WRITE;
	d <<= 7;
	d |= addr & 0x7F;
	d <<= 4;
	d |= data & 0xF;
	writeData(14, d);
}
	
	
public:
	uint16_t ledmatrix[24] = {0};
	uint16_t type;

HT_1632(hwlib::pin_out &dat,
	hwlib::pin_out &wr,
	hwlib::pin_out &cs,
	uint16_t type):
	dat(hwlib::pin_direct_from_out_t(dat)),
	wr(hwlib::pin_direct_from_out_t(wr)),
	cs(hwlib::pin_direct_from_out_t(cs)),
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
	
	cs.write(false);  // start of transmission
	
	for (uint16_t bit = 1<<2; bit; bit >>= 1) {
		wr.write(false);
		dat.write((commands::HT1632_WRITE & bit) ? true : false);
		wr.write(true);
	}
	for (uint16_t bit = 1<<6; bit; bit >>= 1) {
		wr.write(false);
		dat.write((0 & bit) ? true : false);
		wr.write(true);
	}
	for(uint16_t i=0; i<24; i++) {
		uint16_t data = ledmatrix[i];
		for(uint16_t bit = 0x8000; bit; bit >>= 1) {
			wr.write(false);
			dat.write((data & bit) ? true : false);
			wr.write(true);
		}

	}
	cs.write(true);  // end of transmission	
	
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