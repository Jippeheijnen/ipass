#include "../../lib/matrixLib.hpp"
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

TEST_CASE("TESTS") {
	auto clk = hwlib::pin_out_dummy;
	auto mosi = hwlib::pin_out_dummy;
	auto cs = hwlib::pin_out_dummy;
	auto cs_direct = hwlib::direct(cs);
	auto b = spi::bus(clk, mosi, cs);
	bool chipSelect = false;
	
	SECTION("Single transaction") {
		spi::test_transaction trans = spi::test_transaction(b, chipSelect);

		trans.writeData(3, matrix::commands::HT1632_WRITE);
		REQUIRE(trans.transaction_amount==1);
		REQUIRE(trans.size_array[0]==3);
		REQUIRE(trans.data_array[0]==matrix::commands::HT1632_WRITE);
	}
	
	SECTION("3 transactions") {
		auto t = spi::test_transaction(b, chipSelect);
		t.writeData(3, matrix::commands::HT1632_WRITE);
		REQUIRE(t.transaction_amount==1);
		REQUIRE(t.size_array[0]==3);
		REQUIRE(t.data_array[0]==matrix::commands::HT1632_WRITE);
		
		t.writeData(7, 0b1000);
		REQUIRE(t.transaction_amount==2);
		REQUIRE(t.size_array[1]==7);
		REQUIRE(t.data_array[1]==8);
		
		t.writeData(15, 0xEF);
		REQUIRE(t.transaction_amount==3);
		REQUIRE(t.size_array[2]==15);
		REQUIRE(t.data_array[2]==0xEF);
	}
	
	SECTION("cs pin check") {
		bool test = false;
		{
		   REQUIRE(!test);
		   auto t = spi::test_transaction(b, test);
		   REQUIRE(test);
		}
		REQUIRE(!test);
	}

}