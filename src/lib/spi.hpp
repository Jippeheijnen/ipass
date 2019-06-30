#include "hwlib.hpp"

namespace spi {

	/*!
		\addtogroup spi
		@{
	*/

	/*!
		\class bus
		\author jippe
		\date 30/06/19
		\file spi.hpp
		\brief This is my SPI bus implementation.
		 * Other than hwlib this class uses the cs pin as well.
		 * The MISO pin, since no reading is required, is left out.
	*/

	class bus {
	protected:
		hwlib::pin_direct_from_out_t clk;
		hwlib::pin_direct_from_out_t mosi;
		hwlib::pin_direct_from_out_t cs;
		friend class transaction;
	public:

		//! 
		/*!
			\brief The bus constructor.
			 * This is the constructor for the spi bus class.
			 * All params should contain hwlib::target::pin_out's.
			\param clk This is the (write) clock pin.
			\param mosi This is the mosi (data) pin.
			\param cs This is the cs pin.
		*/

		bus(hwlib::target::pin_out &clk, hwlib::target::pin_out &mosi, hwlib::target::pin_out &cs);
	};  // end of bus class.

	/*!
		\class transaction
		\author jippe
		\date 30/06/19
		\file spi.hpp
		\brief SPI transaction class.
		 * This class creates a transaction from a spi::bus.
		 * In the constructor the CS pin is set to high,
		 * and in the destructor the CS pin is set to low to mark the end
		 * of the transaction.
	*/

	class transaction{
	protected:
		spi::bus &b;
		hwlib::pin_direct_from_out_t &clk;
		hwlib::pin_direct_from_out_t &mosi;
		hwlib::pin_direct_from_out_t &cs;
		friend class bus;
	public:

		/*!
			\brief The constructor.
			 * This is the constructor in which the CS pin is set to high.
			 * The transaction starts here.
			
		 * @param b Reference spi::bus goes in here.
		 */

		transaction(spi::bus &b);

		/*!
			\brief WriteData is the function that actually sends data to the receiving chip.
			 * 
			
		 * @param bits This is a uint8_t number that indicates how many bits are being sent.
		 * @param d The data to send, MSBT, (Most Significant Bit First).
		 */

		void writeData(uint8_t bits, uint16_t d);

		/*!
			\brief The destructor.
			 * This is the destructor in which the CS pin is set to low.
			 * The transaction ends here.
			
		 */

		~transaction();

	};  // end of transaction class.

	/*! @} End of Doxygen Groups*/

}  // End namespace spi
