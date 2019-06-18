#include "matrixLib.hpp"
#include <array>

using matrix::commands, matrix::HT_1632, matrix::matrixWindow;

// ===========================================================================

class drawable {
protected:

   hwlib::window & w;
   hwlib::xy location;
   hwlib::xy size;
   
public:

   drawable( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & size ):
      w( w ),
      location( location ),
      size( size )
   {}      
   
   virtual void draw() = 0;
   virtual void update(){}
   bool overlaps( const drawable & other );   
   virtual void interact( drawable & other ){}
   
   hwlib::ostream & print( hwlib::ostream & out ) const {
      return out << location << " " << ( location + size );
   }      
};

// ===========================================================================

class line : public drawable {
private:
   hwlib::xy end;
public:

   line( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & end ):
      drawable( w, location, end - location),
      end( end )
   {}
   
   void draw() override {
      hwlib::line x( location, end );
      x.draw( w );;
   }
};

// ===========================================================================


int main(){
	
	auto dat = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto wr = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto cs = hwlib::target::pin_out(hwlib::target::pins::d8);
	
	auto m = HT_1632(dat, wr, cs, matrix::commands::HT1632_COMMON_16NMOS);

	m.begin();
	m.setBrightness(0xf);
	hwlib::wait_ms(500);
	
//	m.setPixel(0,0);
//	m.setPixel(15,0);
//	m.setPixel(0,4);
//	m.setPixel(15,4);
//	m.setPixel(7,11);
//	m.setPixel(8,11);
//	m.setPixel(7,12);
//	m.setPixel(8,12);
//	m.setPixel(0,19);
//	m.setPixel(15,19);
//	m.setPixel(0,23);
//	m.setPixel(15,23);
//	m.writeScreen();
	
	auto win = matrix::matrixWindow(16, 24, m);
	auto l0 = line(win, hwlib::xy(0,4), hwlib::xy(7,11));
	auto l1 = line(win, hwlib::xy(15,4), hwlib::xy(8,11));
	auto l2 = line(win, hwlib::xy(0,19), hwlib::xy(7,12));
	auto l3 = line(win, hwlib::xy(15,19), hwlib::xy(8,12));
	
	auto l00 = line(win, hwlib::xy(0,0), hwlib::xy(0,4));
	auto l01 = line(win, hwlib::xy(0,19), hwlib::xy(0,24));
	auto l02 = line(win, hwlib::xy(15,0), hwlib::xy(15,4));
	auto l03 = line(win, hwlib::xy(15,19), hwlib::xy(15,24));
	auto l04 = line(win, hwlib::xy(6,10), hwlib::xy(6,14));
	auto l05 = line(win, hwlib::xy(9,10), hwlib::xy(9,14));
	
	auto l000 = line(win, hwlib::xy(0,0), hwlib::xy(15,0));
	auto l001 = line(win, hwlib::xy(0,23), hwlib::xy(15,23));
	
	std::array< drawable *, 12 > objects = { &l0, &l1, &l2, &l3, &l00, &l01, &l02, &l03, &l04, &l05, &l000, &l001 };

	
	for( auto & p : objects ){
         p->draw();
	}
	
	m.writeScreen();
}


