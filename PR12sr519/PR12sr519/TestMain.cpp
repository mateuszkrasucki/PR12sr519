#include "Const.h"
#include "Edges.cpp"
#include <iostream>

using namespace cv;
using namespace std;


int sc_main (int argc, char* argv[]) {
  sc_clock clock("clock",10,SC_NS,0.5);

  Edges test("Test");
  test.clock(clock);
  sc_start(1600000,SC_NS);
  int i;
  test.createImage();
  test.destroyMemory();
  cin >> i;

  return 0;
 }