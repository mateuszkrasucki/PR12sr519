#include "systemc.h"
#include "Const.h"

SC_MODULE(LinearModule)
{
   sc_in_clk     clock ;   

   //sterowanie LinearModule
   sc_in<bool> enable;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie ContextModule 
   sc_out <sc_uint<COORD_WIDTH>>  i;
   sc_out   <sc_uint<COORD_WIDTH>> j;
   sc_out    <bool>                 readSingleFlag;
   sc_out <bool>                 readContextFlag;

   //wyniki ContextModule
   sc_in <bool> singleOutFlag;
   sc_in <bool> valSingle;
   sc_in<bool> contextOutFlag;
   sc_in<sc_uint<4>>	cntContextOut;
   sc_in<bool> context[9];


   //sterowanie MemoryAccessModule / VISITED
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readSingleFlag_vis;
   sc_out <bool>				 writeFlag_vis;
   sc_out <bool>		dataOut_vis;

   //wyniki MemoryAccessModule / VISITED AND CHECKED
   sc_in <bool> singleOutFlag_vis;
   sc_in <bool> val_vis;
  
	sc_signal <bool> enabled;
	int cnt;

 void linearProcess()	{
	while(true)	{
	 wait();
	 /*cout<<i.read()<<"x"<<j.read()<<endl<<"ENABLED: "<<enabled.read() << " WRITEFLAGVIS: "<<writeFlag_vis.read() << " READCONTEXTFLAG: "<<
		 readContextFlag.read()<<endl<<" READSINGLEFLAG: "<<readSingleFlag.read() <<" READVISFLAG: "<< readSingleFlag_vis.read()<<
		 " FINISHED: "<<finished.read() <<endl<< " SINGLEOUT: "<<singleOutFlag.read() << "VISOUTFLAG: "<< singleOutFlag_vis.read()<<endl<<
		 " CNTCONTEXT: "<<cntContextOut.read()<<" CONTEXTOUTFLAG: "<<contextOutFlag.read()<<endl;*/
	 if(enabled.read() && writeFlag_vis.read() && !readContextFlag.read() && !readSingleFlag.read() && !finished.read())
	 {
		 if(i.read()+1 == IMG_SIZE && j.read()+1 == IMG_SIZE)	{
			cout<<endl;
			finished.write(true);
			cout<<"@" << sc_time_stamp()<<endl;
		}
		else if(j.read()+1 == IMG_SIZE)	{
				i_vis.write(i.read()+1);
				i.write(i.read()+1);
				j_vis.write(0);
				j.write(0);
				writeFlag_vis.write(false);
				cout<<endl; //COUT LINIA
				readSingleFlag.write(true);
				readSingleFlag_vis.write(true);
		}
		else	{
				j_vis.write(j.read()+1);
				j.write(j.read()+1);
				writeFlag_vis.write(false);
				readSingleFlag.write(true);
				readSingleFlag_vis.write(true);
		}
	 } else if(enabled.read() && !writeFlag_vis.read() && !readContextFlag.read() && !finished.read() && readSingleFlag.read() && singleOutFlag.read() && singleOutFlag_vis.read())	{
		 if(!val_vis.read())	{	 
			if(valSingle.read())	{
				 readSingleFlag.write(false);
				 readSingleFlag_vis.write(false);
				 readContextFlag.write(true);
			 }
			 else	{
				 readSingleFlag.write(false);
				 readSingleFlag_vis.write(false);
				 dataOut_vis.write(false); //WYNIK
				 cout<<0;
				 writeFlag_vis.write(true);
			 }
		}
		else	{
			//NASTÊPNY
		}
	 }
	 else if(enabled.read() && !readSingleFlag.read() && !writeFlag_vis.read() && !finished.read() && readContextFlag.read() && contextOutFlag.read())	{
		 if(!(context[0] & context[1] & context[2] & context[3] & context[4] & context[6] & context [7] & context [8] & context[9]))	{
				 dataOut_vis.write(true); //WYNIK
				 cout<<1;
		 }
		 else	{
				 dataOut_vis.write(false); //WYNIK
				 cout<<0;
		 }
		 readContextFlag.write(false);
		 writeFlag_vis.write(true);
	 }
	 else if(enabled.read() && !readContextFlag.read() && !readSingleFlag.read() && !readSingleFlag_vis.read() && !writeFlag_vis.read())	{
		  cout<<"@" << sc_time_stamp()<<" "<< "START"<<endl;
		  finished.write(false);
		  i.write(0);
		  j.write(0);
		  i_vis.write(0);
		  j_vis.write(0);
		  writeFlag_vis.write(false);
		  readContextFlag.write(false);
		  readSingleFlag_vis.write(true);
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
		}
  }

				 
  void testPrintout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" "<<valSingle.read()<<endl;
	}
  }


  SC_CTOR(LinearModule)
  {
	cnt = 0;
	SC_CTHREAD(linearProcess, clock.pos());
	
	SC_METHOD(startStop);
		dont_initialize();
		sensitive<<enable.pos()<<finished.pos();

	//SC_CTHREAD(test_printout, clock.pos());
  }
};