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

  bool* mem;

  int loadImage(string filepath)	{
	
	Mat img;
    img = imread(filepath);

    if (img.empty()) 
    {
        cout << "B³¹d!" << endl;
        return -1;
    }
	Mat img_gray;
	cvtColor(img,img_gray,CV_RGB2GRAY);
	IMG_SIZE_i = img.rows;
	IMG_SIZE_j = img.cols;

	mem = new bool[IMG_SIZE_i*IMG_SIZE_j];
	
	for(int i=0; i<IMG_SIZE_i; i++)	{
		for(int j=0; j<IMG_SIZE_j; j++)	{
			mem[i*IMG_SIZE_j+j] = !bool(img_gray.at<unsigned char>(i,j));
		}
	}

    return 1;
  }

  void initMemory()	{
	mem = new bool[IMG_SIZE_i*IMG_SIZE_j];
	for(int i=0; i<IMG_SIZE_i*IMG_SIZE_j; i++)	{
		mem[i] = false;
	}
  }

  void destroyMemory()	{
	 delete [] mem;
  }

  void displayMemory()	{
  	int j = 0;
	for(int i=0; i<IMG_SIZE_i*IMG_SIZE_j; i++)	{
		cout<<mem[i];
		if(i != 0 && i == IMG_SIZE_j-1+j*IMG_SIZE_j)	{
			cout<<" "<<j+1<<endl;;
			j++;
		}
	}

  }

  int getMem(int i)	{
	return mem[i];
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

  SC_CTOR(MemoryModule) {
    SC_METHOD (readMem);
	  dont_initialize();
      sensitive << clock.pos();
    SC_METHOD (writeMem);
	  dont_initialize();
      sensitive << clock.pos();

  }

};