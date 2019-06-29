#ifndef MATRIXLIB
#define MATRIXLIB


/*!	\file matrixLib.hpp
		\brief This file contains SPI and Library classes.

		For my iPass project, the SPI implementation from hwlib wasn't enough.
		I had to make my own because not all features were supported in hwlib.
		Regarding my idea for this project, I have created a library which suoports
		a matrix of LEDs with a size of 24x16, controlled by a HT1632 chip.
*/

#include "hwlib.hpp"

namespace spi {

	/*!
		\addtogroup spi
		@{
	*/

	//! SPI bus class.
	/*!
		This class stores the clk mosi and cs pins.
	*/

	class bus {
	protected:
		hwlib::pin_direct_from_out_t clk;
		hwlib::pin_direct_from_out_t mosi;
		hwlib::pin_direct_from_out_t cs;
		friend class transaction;
	public:
		//! The bus constructor.
		/*!
			This is the constructor for the spi bus class.
			All params should contain hwlib::target::pin_out's.
			\param clk This is the (write) clock pin.
			\param mosi This is the mosi (data) pin.
			\param cs This is the cs pin.
		*/
		bus(hwlib::target::pin_out &clk, hwlib::target::pin_out &mosi, hwlib::target::pin_out &cs);
	};

	//! SPI transaction class.
	/*!
		This class is called upon every time something needs to be written to the
		HT_1632 chip. Unlike hwlib this class doesn't need a cs pin.
	*/
	class transaction{
	protected:
		spi::bus &b;
		hwlib::pin_direct_from_out_t &clk;
		hwlib::pin_direct_from_out_t &mosi;
		hwlib::pin_direct_from_out_t &cs;
		friend class bus;
	public:
		transaction(spi::bus &b);

		void writeData(uint8_t bits, uint16_t d);

		~transaction();

	};

	/*! @} End of Doxygen Groups*/

}  // End namespace spi


namespace matrix {

	/*!
		\addtogroup matrix
		@{
	*/

struct commands {

/*!
	\addtogroup commands
	@{
*/

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

static constexpr const uint8_t HT1632_READ			= 0x06;
static constexpr const uint8_t HT1632_WRITE			= 0x05;

/*! @} End of Doxygen Groups*/

};


class HT_1632 {
protected:

uint16_t ledmatrix[24] = {0};
spi::bus spi_bus;
uint16_t type;
void sendCommand(uint16_t cmd);
void writeRam(uint8_t addr, uint8_t data);
void dumpMem();
friend class matrixWindow;

public:

HT_1632(spi::bus spi_bus, uint16_t type);
void setPixel(uint16_t x, uint16_t y);
void clearPixel(uint16_t x, uint16_t y);
void blink(uint16_t seconds);
void writeScreen();
void setBrightness(uint8_t b);

};

class matrixWindow : public hwlib::window {

private:

	HT_1632 &matrix;

public:

	matrixWindow(int x, int y, HT_1632 &matrix);
	void write_implementation(hwlib::xy pos, hwlib::color col) override;
	void flush() override;
	void clear();

};

	/*! @} End of Doxygen Groups*/

}  // end of namespace matrix

#endif  // MATRIXLIB
