#include "systemc.h"

#include "Const.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

SC_MODULE (SourceMemory) {
  sc_in_clk     clock ; 
  sc_in    <sc_uint<ADDR_WIDTH> > address ;
  sc_out   <sc_logic> dataOut;

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

	for(int i=0; i<20; i++)	{
		for(int j=0; j<20; j++)	{
			mem[i*20+j] = !bool(img_gray.at<unsigned char>(i,j));
		}
	}

	int j = 0;
	for(int i=0; i<400; i++)	{
		cout<<mem[i];
		if(i != 0 && i == 19+j*20)	{
			cout<<" "<<i<<endl;
			j++;
		}
	}
    return 1;
  }


  void readMem () {
	  int a = address.read();
      dataOut.write(sc_logic(mem[a]));
	  //cout<<"MEMORY: "<<a<<" "<<mem[a]<<sc_logic(mem[a])<<dataOut.read()<<endl;
  }

  void test_printout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" MEMMOD "<<address.read()<<" IN:"<<dataOut.read()<<endl;
	}
  }

  SC_CTOR(SourceMemory) {
    SC_METHOD (readMem);
	  dont_initialize();
      sensitive << clock.pos();
	SC_CTHREAD(test_printout, clock.pos());
  }

};