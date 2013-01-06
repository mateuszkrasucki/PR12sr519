#include "Const.h"
#include "Edges.cpp"
#include <iostream>

using namespace cv;
using namespace std;



int sc_main (int argc, char* argv[]) {
  sc_clock clock("clock",10,SC_NS,0.5);

  Edges test("TEST");
    test.clock(clock);
    sc_start(60000,SC_NS);



  return 0;
 }