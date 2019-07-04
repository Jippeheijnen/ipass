
/*
##############################################################
#                 (c) Jippe Heijnen 2019                     #
#                                                            #
#   Distributed under the Boost Software License v1.0.       #
#   (See accompanying file LICENSE_1_0.txt or copy at        #
#          https://www.boost.org/LICENSE_1_0.txt)            #
##############################################################
*/

#include "hwlib.hpp"

/*!
	\file spi.hpp
	\author jippe
	\date 30/06/19
	\brief
*/

namespace spi {

	/*!
		\addtogroup spi
		@{
	*/

	/*!
		\class bus
		\author jippe
		\date 30/06/19
		\brief This is my SPI bus implementation.
		 * 
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
			 * 
			 * This is the constructor for the spi bus class.
			 * All params should contain hwlib::target::pin_out's.
			@param clk This is the (write) clock pin.
			@param mosi This is the mosi (data) pin.
			@param cs This is the cs pin.
		*/

		bus(hwlib::pin_out &clk, hwlib::pin_out &mosi, hwlib::pin_out &cs);
	};  // end of bus class.

//	class bus_dummy {
//	private:
//		bool clk, mosi, cs;
//		friend class transaction_dummy;
//
//	public:
//		bus_dummy() : 
//			clk(0), mosi(0), cs(0)
//			{}
//		
//	};

	/*!
		\class transaction
		\brief SPI transaction class.
		 * 
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
			 * 
			 * This is the constructor in which the CS pin is set to high.
			 * The transaction starts here.
			
		 * @param b Reference spi::bus goes in here.
		 */

		transaction(spi::bus &b);

		/*!
			\brief WriteData is the function that actually sends data to the receiving chip.
			 * 
			 * 
		 * @param bits This is a uint8_t number that indicates how many bits are being sent.
		 * @param d The data to send, MSBT, (Most Significant Bit First).
		 */

		void writeData(uint8_t bits, uint16_t d);

		/*!
			\brief The destructor.
			 * 
			 * This is the destructor in which the CS pin is set to low.
			 * The transaction ends here.
			
		 */

		~transaction();

	};  // end of transaction class.

//	class transaction_dummy {
//	private:
//		std::string clock;
//		std::string data;
//		std::string chipSelect;
//		bus_dummy &b;
//		bool &clk, &mosi, &cs;
//		friend class bus_dummy;
//	public:
//		transaction_dummy(bus_dummy &b) :
//			b(b), clk(b.clk), mosi(b.mosi), cs(b.cs)
//			{cs}
//		std::string writeData(uint8_t bits, uint16_t d) {
//			for (uint16_t bit = 1<<(bits-1); bit; bit >>= 1) {
//				
//				clock += "0";
//				data += (string)(d & bit) ? 1 : 0);
//				clock += "1";
//			}
//			std::string output = clock += '\n' += data += '\n' += chipSelect;
//			return output;
//		}
//	};

	/*! @} End of Doxygen Groups*/

}  // End namespace spi
