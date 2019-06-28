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
   hwlib::xy end;
   hwlib::xy bounce = hwlib::xy(-1,-1);
public:

   line( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & end, const hwlib::xy &bounce ):
      drawable( w, location, end - location, bounce ),
      end( end )
   {}

   void draw() override {
      hwlib::line x( location, end );
      x.draw( w );;
   }
};

// ===========================================================================

class circle : public drawable {
protected:

   int radius;

public:

   circle( hwlib::window & w, const hwlib::xy & midpoint, int radius, const hwlib::xy &bounce ):
      drawable( w,
         midpoint - hwlib::xy( radius, radius ),
         hwlib::xy( radius, radius ) * 2, bounce ),
      radius( radius )
   {}

   void draw() override {
      hwlib::circle c( location + hwlib::xy( radius, radius ), radius );
      c.draw( w );
   }
};

// ===========================================================================

class ball : public circle {
private:
<<<<<<< HEAD

   hwlib::xy speed;

public:

   ball(
      hwlib::window & w,
      const hwlib::xy & midpoint,
      int radius = 0,
      const hwlib::xy & speed = hwlib::xy(0,1),
	  const hwlib::xy &bounce = hwlib::xy(1,1)
   ):
      circle( w, midpoint, radius, bounce ),
      speed( speed )
   {}

   void draw() override {
	   w.write(hwlib::xy(location.x-1, location.y-1));
   }

   void update() override {
      location = location + speed;
   }

   void interact( drawable & other ) override {
      if( this != & other){
         if( overlaps( other )){
            speed.x *= other.bounce.x;
            speed.y *= other.bounce.y;
         }
      }
   }
};

// ===========================================================================

class rectangle : public drawable {
protected:
	hwlib::color color;

public:
	rectangle(hwlib::window &w, const hwlib::xy &location, const hwlib::xy &size, hwlib::color color, const hwlib::xy &bounce) :
		drawable(w, location, size, bounce),
		color(color)
		{}
};

// ===========================================================================

class wall : public rectangle {
private:
	int update_count = 0,
		update_interval;
	bool filled = false;

public:
	wall(hwlib::window &w, const hwlib::xy &location, const hwlib::xy &size, int update_interval=0, hwlib::color color=hwlib::red, const hwlib::xy &bounce=hwlib::xy(0,-1)) :
		rectangle(w, location, size, color, bounce),
		update_interval(update_interval)
		{}

=======
	
	hwlib::xy speed;
	
public:
	
	ball( 
		hwlib::window & w, 
		const hwlib::xy & midpoint, 
		int radius = 0, 
		const hwlib::xy & speed = hwlib::xy(0,1),
		const hwlib::xy &bounce = hwlib::xy(1,1)
	):
		circle( w, midpoint, radius, bounce ),
		speed( speed )  
	{}
	
>>>>>>> 4b28ba6a65e01e9a92b38610ea73ebb5a20b42aa
	void draw() override {
		w.write(hwlib::xy(location.x-1, location.y-1));
	}
<<<<<<< HEAD

	void update() override {
		update_count++;
		if (update_count==update_interval) {
			filled = !(filled);
			update_count=0;
		}

	}

	void drawHollow() {
		for(int i = location.x; i <= (location.x + size.x); i++){
            for(int j = location.y; j <= (location.y + size.y); j++){
				hwlib::xy pixel(i,j);
				if(j == location.y || j == (location.y + size.y) || i == location.x || i == (location.x +size.x)){
                    w.write(pixel, hwlib::color(0,0,0,false));
                }else if(filled){
                    w.write(pixel, hwlib::color(255,0,0,false));
                }
			}
		}
		w.flush();
	}

	void drawFilled() {
		for(int i = location.x; i <= (location.x + size.x); i++){
            for(int j = location.y; j <= (location.y + size.y); j++){
				hwlib::xy pixel(i,j);
				if(j == location.y || j == (location.y + size.y) || i == location.x || i == (location.x +size.x)){
                    w.write(pixel, hwlib::color(0,0,0,false));
                }else if(filled){
                    w.write(pixel, hwlib::color(255,0,0,false));
                }
			}
		}
		w.flush();
	}

=======
   
	void update() override {
		speed = hwlib::xy(0, 1);
		location = location + speed; 
	}
	
	void interact( drawable & other ) override {
		if( this != & other){
			if( overlaps( other )){
				speed.x *= other.bounce.x;
				speed.y *= other.bounce.y;
			}
		}
	}   
>>>>>>> 4b28ba6a65e01e9a92b38610ea73ebb5a20b42aa
};

// ===========================================================================


void hourGlass(matrix::HT_1632 &m) {
	auto win = matrixWindow(16, 24, m);
	line(win, hwlib::xy(1,5), hwlib::xy(6,10), hwlib::xy(1,0)).draw();
	line(win, hwlib::xy(15,4), hwlib::xy(9,10), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(0,19), hwlib::xy(7,12), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(15,19), hwlib::xy(8,12), hwlib::xy(0,0)).draw();

	line(win, hwlib::xy(0,0), hwlib::xy(0,5), hwlib::xy(0,1)).draw();
	line(win, hwlib::xy(0,19), hwlib::xy(0,24), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(15,0), hwlib::xy(15,4), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(15,19), hwlib::xy(15,24), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(6,10), hwlib::xy(6,14), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(9,10), hwlib::xy(9,14), hwlib::xy(0,1)).draw();

	line(win, hwlib::xy(0,0), hwlib::xy(15,0), hwlib::xy(0,0)).draw();
	line(win, hwlib::xy(0,23), hwlib::xy(15,23), hwlib::xy(0,0)).draw();
	

	m.clearPixel(0,0);
	m.clearPixel(15,0);
	m.clearPixel(0,23);
	m.clearPixel(15,23);
	
	m.writeScreen();
	
}


int main(){

	auto dat = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto wr = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto cs = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto spi_bus = spi::bus(wr, dat, cs);
	auto m = HT_1632(spi_bus, matrix::commands::HT1632_COMMON_16NMOS);
	
	m.begin();
	m.setBrightness(0x0);
	hwlib::wait_ms(500);
<<<<<<< HEAD

	auto win = matrix::matrixWindow(16, 24, m);
	auto l0 = line(win, hwlib::xy(0,4), hwlib::xy(6,10), hwlib::xy(0,0));
	auto l1 = line(win, hwlib::xy(15,4), hwlib::xy(9,10), hwlib::xy(0,0));
	auto l2 = line(win, hwlib::xy(0,19), hwlib::xy(7,12), hwlib::xy(0,0));
	auto l3 = line(win, hwlib::xy(15,19), hwlib::xy(8,12), hwlib::xy(0,0));

	auto l00 = line(win, hwlib::xy(0,0), hwlib::xy(0,4), hwlib::xy(0,0));
	auto l01 = line(win, hwlib::xy(0,19), hwlib::xy(0,24), hwlib::xy(0,0));
	auto l02 = line(win, hwlib::xy(15,0), hwlib::xy(15,4), hwlib::xy(0,0));
	auto l03 = line(win, hwlib::xy(15,19), hwlib::xy(15,24), hwlib::xy(0,0));
	auto l04 = line(win, hwlib::xy(6,10), hwlib::xy(6,14), hwlib::xy(0,0));
	auto l05 = line(win, hwlib::xy(9,10), hwlib::xy(9,14), hwlib::xy(0,1));

	auto l000 = line(win, hwlib::xy(0,0), hwlib::xy(15,0), hwlib::xy(0,0));
	auto l001 = line(win, hwlib::xy(0,23), hwlib::xy(15,23), hwlib::xy(0,0));

	std::array< drawable *, 18 > objects = { &l0, &l1, &l2, &l3, &l00, &l01, &l02, &l03, &l04, &l05, &l000, &l001 };



	for (;;) {
		for( auto & p : objects ){
			p->draw();
		}
		for( auto & p : objects ){
			p->update();
		}
		for( auto & p : objects ){
			for( auto & other : objects ){
				p->interact( *other );
			}
		}

		m.writeScreen();
		m.clearPixel(0,0);
		m.clearPixel(15,0);
		m.clearPixel(0,23);
		m.clearPixel(15,23);
		m.writeScreen();

		hwlib::wait_ms(100);
		win.clear();
		win.flush();
	}
=======
	
	int i=0;
	for (int x=2; x<14; x++ ) {
		for (int y=2; y<9; y++) {
			m.setPixel(x,y);
			m.clearPixel(2+i,y);
			m.clearPixel(13-i,y);
		}
		i++;
	}
	
	
	hourGlass(m);
	
	m.writeScreen();

>>>>>>> 4b28ba6a65e01e9a92b38610ea73ebb5a20b42aa
}
