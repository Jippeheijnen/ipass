#include "matrixLib.hpp"

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

matrix::HT_1632::HT_1632(spi::bus spi_bus,
	uint16_t type):
	spi_bus(spi_bus),
	type(type)
	{
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

void matrix::HT_1632::sendCommand(uint16_t cmd){
	spi::transaction(spi_bus).writeData(12, (((uint16_t)commands::HT1632_COMMAND << 8) | cmd) << 1);
}

void matrix::HT_1632::writeRam(uint8_t addr, uint8_t data) {

	uint16_t d = commands::HT1632_WRITE;
	d <<= 7;
	d |= addr & 0x7F;
	d <<= 4;
	d |= data & 0xF;
	spi::transaction(spi_bus).writeData(14, d);
}

void matrix::HT_1632::dumpMem() {

// set to true for memory debugging
#define debug false

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
		if (debug) {
			hwlib::cout << "memory cleared\n";
		}
	}
}

void matrix::HT_1632::setPixel(uint16_t x, uint16_t y) {
	if((x < 0) || (x >= 16) || (y < 0) || (y >= 24)) return;
	ledmatrix[y] |= 0x8000 >> x;
}

void matrix::HT_1632::clearPixel(uint16_t x, uint16_t y) {
	if((x < 0) || (x >= 16) || (y < 0) || (y >= 24)) return;
	ledmatrix[y] &= ~(0x8000 >> x);
}

void matrix::HT_1632::blink(uint16_t seconds) {
	for (int i=0; i<=2*seconds; i++) {
		for (int i=0;i<24;i++) {
			ledmatrix[i]=0xFFFF;
		}
		writeScreen();
		hwlib::wait_ms(250);
		for (int i=0;i<24;i++) {
			ledmatrix[i]=0;
		}
		writeScreen();
		hwlib::wait_ms(250);
	}
}

void matrix::HT_1632::writeScreen() {
	auto spi_transaction = spi::transaction(spi_bus);
	spi_transaction.writeData(3, commands::HT1632_WRITE);
	spi_transaction.writeData(7,0);

	for(uint16_t i=0; i<24; i++) {
		spi_transaction.writeData(16, ledmatrix[i]);
	}
	spi_transaction.~transaction();
}

void matrix::HT_1632::setBrightness(uint8_t b) {
	sendCommand(commands::HT1632_PWM_CONTROL | b);
}

matrix::matrixWindow::matrixWindow(int x, int y, matrix::HT_1632 &matrix) :
	hwlib::window(hwlib::xy(x,y)),
	matrix(matrix)
	{}
	
void matrix::matrixWindow::write_implementation(hwlib::xy pos, hwlib::color col) {
	matrix.setPixel(pos.x, pos.y);
}

void matrix::matrixWindow::flush() {
	matrix.writeScreen();
}

void matrix::matrixWindow::clear()  {
	matrix.dumpMem();
	matrix.writeScreen();
}