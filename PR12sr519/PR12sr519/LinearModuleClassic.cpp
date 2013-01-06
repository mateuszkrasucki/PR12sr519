#include "systemc.h"
#include "Const.h"
/*
SC_MODULE(LinearModule)
{
   sc_in_clk     clock ;   

   //sterowanie LinearModule
   sc_in<bool> enable;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie MemoryAccessModule
   sc_out <sc_uint<COORD_WIDTH>>  i;
   sc_out   <sc_uint<COORD_WIDTH>> j;
   sc_out <bool>                 flagContext;
   sc_out    <bool>                 flagSingle;

   //wyniki MemoryAccessModule
   sc_in <bool> flagContextOut;
   sc_in <bool> flagSingleOut;
   sc_in <sc_uint<4>>	cntContextOut;
   sc_in <bool> context[9];
   sc_in <bool> valSingle;

   //sterowanie MemoryAccessModule / VISITED AND CHECKED
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readContextFlag_vis;
   //wyniki MemoryAccessModule / VISITED AND CHECKED
   sc_in <bool> contextOutFlag_vis;
   sc_in <sc_uint<4>>	cntContextOut_vis;
   sc_in <bool> context_vis[9];
  
	sc_signal <bool> enabled;
	int cnt;

 void linearProcess()	{
	while(true)	{
	 wait();
	 if(enabled.read() && !finished.read() && flagSingle.read() && flagSingleOut.read())	{
		 if(valSingle.read())	{
			 flagSingle.write(false);
			 flagContext.write(true);
		 }
		 else	{
			cout<<0; 
			if(i.read()+1 == IMG_SIZE && j.read()+1 == IMG_SIZE)	{
				cout<<endl;
				flagSingle.write(false);
				finished.write(true);
			}
			else if(j.read()+1 == 20)	{
				i.write(i.read()+1);
				j.write(0);
				cout<<endl;
			}
			else	{
				j.write(j.read()+1);
			}
		 }

	 }
	 else if(enabled.read() && !finished.read() && flagContext.read() && flagContextOut.read())	{
		 if(!(context[0] & context[1] & context[2] & context[3] & context[4] & context[6] & context [7] & context [8] & context[9]))	{
			 cout<<1;
		 }
		 else	{
			 cout<<0;
		 }

        flagContext.write(false); 
		if(i.read()+1 == IMG_SIZE && j.read()+1 == IMG_SIZE)	{
			cout<<endl;
			finished.write(true);
			cout<<"@" << sc_time_stamp()<<endl;
		}
		else if(j.read()+1 == 20)	{
				i.write(i.read()+1);
				j.write(0);
				flagSingle.write(true);
				cout<<endl;
		}
		else	{
			    flagSingle.write(true);
				j.write(j.read()+1);
		}
	 }
	 else if(enabled.read() && !flagContext.read() && !flagSingle.read())	{
		  cout<<"@" << sc_time_stamp()<<" "<< "START"<<endl;
		  finished.write(false);
		  i.write(0);
		  j.write(0);
		  flagContext.write(false);
		  flagSingle.write(true);
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
};*/