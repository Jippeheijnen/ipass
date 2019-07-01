
/*
##############################################################
#                 (c) Jippe Heijnen 2019                     #
#                                                            #
#   Distributed under the GNU General Public License v3.0.   #
#   (See accompanying file LICENSE.txt or copy at            #
#   https://www.gnu.org/licenses/gpl-3.0.nl.html)            #
##############################################################
*/

#include "matrixLib.hpp"
#include <array>

using matrix::commands, matrix::HT_1632, matrix::matrixWindow;

// ===========================================================================

class drawable {
protected:

	hwlib::window & w;

public:
	hwlib::xy location;
	hwlib::xy size;

	hwlib::xy bounce;
	drawable( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & size, const hwlib::xy bounce ):
		w( w ),
		location( location ),
		size( size ),
		bounce( bounce )
	{}

	virtual void draw() = 0;
	virtual void update(){}
	bool overlaps( const drawable & other );
	virtual void interact( drawable & other ){}

	hwlib::ostream & print( hwlib::ostream & out ) const {
		return out << location << " " << ( location + size );
	}
};

hwlib::ostream & operator<<( hwlib::ostream & lhs, const drawable & rhs ){
	return rhs.print( lhs );
}

bool within( int x, int a, int b ){
	return ( x >= a+1 ) && ( x <= b+1 );
}

bool drawable::overlaps( const drawable & other ){

	bool x_overlap = within(
		location.x,
		other.location.x,
		other.location.x + other.size.x
	) || within(
		other.location.x,
		location.x,
		location.x + size.x
	);

	bool y_overlap = within(
		location.y,
		other.location.y,
		other.location.y + other.size.y
	) || within(
		other.location.y,
		location.y,
		location.y + size.y
	);

	return x_overlap && y_overlap;
}

// ===========================================================================

class line : public drawable {
private:
public:
	hwlib::xy end;
	hwlib::xy bounce;

	line( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & end, const hwlib::xy &bounce  = hwlib::xy(-1,-1)):
		drawable( w, location, end - location, bounce ),
		end( end )
	{}

	void draw() override {
		hwlib::line x( location, end );
		x.draw( w );;
	}

};

// ===========================================================================



void hourGlass(matrix::HT_1632 &matrix, uint16_t h, uint16_t m, uint16_t s) {
	auto time = (h*3600)+(m*60)+s;

	auto w = matrix::matrixWindow(16, 24, matrix);

	auto l0 = line(w, hwlib::xy(2,2), hwlib::xy(14,2));
	auto l1 = line(w, hwlib::xy(2,3), hwlib::xy(14,3));
	auto l2 = line(w, hwlib::xy(3,4), hwlib::xy(13,4));
	auto l3 = line(w, hwlib::xy(4,5), hwlib::xy(12,5));
	auto l4 = line(w, hwlib::xy(5,6), hwlib::xy(11,6));
	auto l5 = line(w, hwlib::xy(6,7), hwlib::xy(10,7));
	auto l6 = line(w, hwlib::xy(7,8), hwlib::xy(9,8));

	std::array< drawable *, 7 > lines = {
		&l0, &l1, &l2, &l3, &l4, &l5, &l6
	};


	bool makeItRain = true;
	int rainCounter = 0;
	
	for (float counter=0;counter!=time; counter+=.25) {
		w.clear();
		line(w, hwlib::xy(1,5), hwlib::xy(6,10)).draw();
		line(w, hwlib::xy(15,4), hwlib::xy(9,10)).draw();
		line(w, hwlib::xy(0,19), hwlib::xy(7,12)).draw();
		line(w, hwlib::xy(15,19), hwlib::xy(8,12)).draw();

		line(w, hwlib::xy(0,0), hwlib::xy(0,5)).draw();
		line(w, hwlib::xy(0,19), hwlib::xy(0,24)).draw();
		line(w, hwlib::xy(15,0), hwlib::xy(15,4)).draw();
		line(w, hwlib::xy(15,19), hwlib::xy(15,24)).draw();
		line(w, hwlib::xy(6,10), hwlib::xy(6,14)).draw();
		line(w, hwlib::xy(9,10), hwlib::xy(9,14)).draw();

		line(w, hwlib::xy(0,0), hwlib::xy(15,0)).draw();
		line(w, hwlib::xy(0,23), hwlib::xy(15,23)).draw();

		for (auto &l : lines) {
			l->draw();
		}

		w.flush();

		matrix.clearPixel(0,0);
		matrix.clearPixel(15,0);
		matrix.clearPixel(0,23);
		matrix.clearPixel(15,23);
		matrix.writeScreen();
		hwlib::wait_ms(250);
		if (makeItRain) {
			rainCounter++;
			w.flush();
		}
		if (counter>=7*(time/7)) {
			hwlib::cout << "7/7" << '\n';
			l6.location=hwlib::xy(7, 15);
			l6.end=hwlib::xy(9, 15);
			l5.location=hwlib::xy(6, 16);
			l5.end=hwlib::xy(10, 16);
			l4.location=hwlib::xy(5, 17);
			l4.end=hwlib::xy(11, 17);
			l3.location=hwlib::xy(4, 18);
			l3.end=hwlib::xy(12, 18);
			l2.location=hwlib::xy(3, 19);
			l2.end=hwlib::xy(13, 19);
			l1.location=hwlib::xy(2, 20);
			l1.end=hwlib::xy(14, 20);
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		} else if (counter>=6*(time/7)) {
			makeItRain = false;
			hwlib::cout << "6/7" << '\n';
			l5.location=hwlib::xy(6, 16);
			l5.end=hwlib::xy(10, 16);
			l4.location=hwlib::xy(5, 17);
			l4.end=hwlib::xy(11, 17);
			l3.location=hwlib::xy(4, 18);
			l3.end=hwlib::xy(12, 18);
			l2.location=hwlib::xy(3, 19);
			l2.end=hwlib::xy(13, 19);
			l1.location=hwlib::xy(2, 20);
			l1.end=hwlib::xy(14, 20);
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		} else if (counter>=5*(time/7)) {
			hwlib::cout << "5/7" << '\n';
			l4.location=hwlib::xy(5, 17);
			l4.end=hwlib::xy(11, 17);
			l3.location=hwlib::xy(4, 18);
			l3.end=hwlib::xy(12, 18);
			l2.location=hwlib::xy(3, 19);
			l2.end=hwlib::xy(13, 19);
			l1.location=hwlib::xy(2, 20);
			l1.end=hwlib::xy(14, 20);
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		} else if (counter>=4*(time/7)) {
			hwlib::cout << "4/7" << '\n';
			l3.location=hwlib::xy(4, 18);
			l3.end=hwlib::xy(12, 18);
			l2.location=hwlib::xy(3, 19);
			l2.end=hwlib::xy(13, 19);
			l1.location=hwlib::xy(2, 20);
			l1.end=hwlib::xy(14, 20);
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		} else if (counter>=3*(time/7)) {
			hwlib::cout << "3/7" << '\n';
			l2.location=hwlib::xy(3, 19);
			l2.end=hwlib::xy(13, 19);
			l1.location=hwlib::xy(2, 20);
			l1.end=hwlib::xy(14, 20);
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		} else if (counter>=2*(time/7)) {
			hwlib::cout << "2/7" << '\n';
			l1.location=hwlib::xy(2, 20);
			l1.end=hwlib::xy(14, 20);
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		} else if (counter>=(time/7)) {
			hwlib::cout << "1/7" << '\n';
			l0.location=hwlib::xy(2, 21);
			l0.end=hwlib::xy(14, 21);
		}
	}
	matrix.blink(4);
	hwlib::cout << "done";

}

// ===========================================================================

int main(){

	auto dat = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto wr = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto cs = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto spi_bus = spi::bus(wr, dat, cs);
	auto m = HT_1632(spi_bus, matrix::commands::HT1632_COMMON_16NMOS);

	m.setBrightness(0x0);
	hwlib::wait_ms(500);

	auto win = matrix::matrixWindow(16, 24, m);
	hourGlass(m, 0, 0, 10);


}


