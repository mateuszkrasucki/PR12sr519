#include "Const.h"
#include "Edges.cpp"
#include <iostream>

using namespace cv;
using namespace std;



int sc_main (int argc, char* argv[]) {
  sc_clock clock("clock",10,SC_NS,0.5);

  Edges test("TEST");
    test.clock(clock);
    sc_start(1600,SC_NS);
  /*for (i=0;i<100;i++) {
	sc_start(5,SC_NS);
	cout<<clock.read();
    sc_start(5, SC_NS); 
	cout<<" "<<address.read()<<" "<<data_out.read()<<endl;
    address.write(i);  
  }*/

  return 0;
 }