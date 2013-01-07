#include "systemc.h"

#include "Const.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

SC_MODULE (MemoryModule) {
  sc_in_clk     clock ; 

  sc_in    <sc_uint<ADDR_WIDTH> > address ;
  sc_in    <bool>                 writeFlag;
  sc_in    <bool>                 readFlag;
  sc_in   <bool> dataIn;
  sc_out   <bool> dataOut;

  bool mem [IMG_SIZE*IMG_SIZE];

  int loadImage(string filepath)	{
	
	Mat img;
    img = imread(filepath);

    if (img.empty()) 
    {
        cout << "Cannot load image!" << endl;
        return -1;
    }
	Mat img_gray;
	cvtColor(img,img_gray,CV_RGB2GRAY);

	for(int i=0; i<IMG_SIZE; i++)	{
		for(int j=0; j<IMG_SIZE; j++)	{
			mem[i*IMG_SIZE+j] = !bool(img_gray.at<unsigned char>(i,j));
		}
	}
    return 1;
  }

  void displayMemory()	{
  	int j = 0;
	for(int i=0; i<IMG_SIZE*IMG_SIZE; i++)	{
		cout<<mem[i];
		if(i != 0 && i == IMG_SIZE-1+j*IMG_SIZE)	{
			cout<<" "<<j+1<<endl;;
			j++;
		}
	}
  
  }

  void writeMem () {
    if (writeFlag.read() && !readFlag.read()) {
	  int a = address.read();
      mem[a] = dataIn.read();
    }
  }

  void readMem () {
    if (readFlag.read() && !writeFlag.read())  {
	  int a = address.read();
      dataOut.write(mem[a]);
    }
  }

  void test_printout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" MEMORY WRITEFLAG: "<<writeFlag.read()<<" READFLAG: "<<readFlag.read()<<" ADD: "<<address.read()<<" IN:"<<dataOut.read()<<endl;
	}
  }

  SC_CTOR(MemoryModule) {
    SC_METHOD (readMem);
	  dont_initialize();
      sensitive << clock.pos();
    SC_METHOD (writeMem);
	  dont_initialize();
      sensitive << clock.pos();

	//SC_CTHREAD(test_printout, clock.pos());

	for(int i=0; i<IMG_SIZE*IMG_SIZE; i++)	{
		mem[i] = false;
	}


  }

};