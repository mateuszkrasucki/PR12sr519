#include "systemc.h"
#include "Const.h"

SC_MODULE(LinearModuleClassic)
{
   sc_in_clk     clock ;   

   //sterowanie LinearModule
   sc_in<bool> enable;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie AccessPictureModule
   sc_out <sc_uint<COORD_WIDTH>>  i_1;
   sc_out   <sc_uint<COORD_WIDTH>> j_1;
   sc_out <sc_uint<COORD_WIDTH>>  i_2;
   sc_out   <sc_uint<COORD_WIDTH>> j_2;
   sc_out    <bool>                 readSingleFlag;
   sc_out <bool>                 readContextFlag;
   sc_out <bool> allowContextFlag;

   //wyniki AccessPictureModule
   sc_in <bool> singleOutFlag;
   sc_in <bool> single;
   sc_in<bool> contextOutFlag;
   sc_in<bool> context[8];

   //sterowanie ResultsModule
   sc_out <sc_uint<COORD_WIDTH>>  i_res;
   sc_out   <sc_uint<COORD_WIDTH>> j_res;
   sc_out <bool>                 writeFlag_res;
   sc_out <bool>		dataOut_res;

  
	sc_signal <bool> enabled;

 void linearProcess()	{
	while(true)	{
	 wait();

	 if(enabled.read() && writeFlag_res.read() && !readContextFlag.read() && !readSingleFlag.read() && !finished.read())
	 {
		 if(i_1.read()+1 == IMG_SIZE_i && j_1.read()+1 == IMG_SIZE_j)	{
			cout<<endl;
			finished.write(true);
			cout<<"@" << sc_time_stamp()<<endl;
		}
		else if(j_1.read()+1 == IMG_SIZE_j)	{
				i_res.write(i_1.read()+1);
				i_2.write(i_1.read()+1);
				i_1.write(i_1.read()+1);
				j_res.write(0);
				j_2.write(0);
				j_1.write(0);
				writeFlag_res.write(false);
				//cout<<endl; //COUT LINIA
				readSingleFlag.write(true);
		}
		else	{
				j_res.write(j_1.read()+1);
				j_2.write(j_1.read()+1);
				j_1.write(j_1.read()+1);
				writeFlag_res.write(false);
				readSingleFlag.write(true);
		}
	 } else if(enabled.read() && !writeFlag_res.read() && !readContextFlag.read() && !finished.read() && readSingleFlag.read() && singleOutFlag.read())	{ 
			if(single.read())	{
				 readSingleFlag.write(false);
				 readContextFlag.write(true);
			 }
			 else	{
				 readSingleFlag.write(false);
				 dataOut_res.write(false); //WYNIK
				 writeFlag_res.write(true);
			 }
	 }
	 else if(enabled.read() && !readSingleFlag.read() && !writeFlag_res.read() && !finished.read() && readContextFlag.read() && contextOutFlag.read())	{
		 if(!(context[0] & context[1] & context[2] & context[3] & context[4] & context[5] & context[6] & context [7]))	{
				 dataOut_res.write(true); //WYNIK
		 }
		 else	{
				 dataOut_res.write(false); //WYNIK
		 }
		 readContextFlag.write(false);
		 writeFlag_res.write(true);
	 }
	 else if(enabled.read() && !readContextFlag.read() && !readSingleFlag.read() && !writeFlag_res.read())	{
		  cout<<"@" << sc_time_stamp()<<" "<< "START"<<endl;
		  finished.write(false);
		  i_1.write(0);
		  j_1.write(0);
		  i_2.write(0);
		  j_2.write(0);
		  i_res.write(0);
		  j_res.write(0);
		  writeFlag_res.write(false);
		  readContextFlag.write(false);
		  readSingleFlag.write(true);
	 }
	}
 }

  void startStop()	{
		if(enabled.read())	{
			enabled.write(false);
		}
		else	{
			enabled.write(true);
			allowContextFlag.write(true);
		}
  }

				 
  SC_CTOR(LinearModuleClassic)
  {
	SC_CTHREAD(linearProcess, clock.pos());
	
	SC_METHOD(startStop);
		dont_initialize();
		sensitive<<enable.pos()<<finished.pos();
  }
};