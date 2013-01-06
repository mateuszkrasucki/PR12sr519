#include "systemc.h"
#include "Const.h"
/*
SC_MODULE(SmartModule)
{
   sc_in_clk     clock ;   

   //sterowanie SmartModule
   sc_in<bool> enable;
   sc_out <sc_uint<COORD_WIDTH>>  i_start;
   sc_out   <sc_uint<COORD_WIDTH>> j_start;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie MemoryAccessModule
   sc_out <sc_uint<COORD_WIDTH>>  i;
   sc_out   <sc_uint<COORD_WIDTH>> j;
   sc_out <bool>                 readContextFlag;
   //wyniki MemoryAccessModule
   sc_in <bool> contextOutFlag;
   sc_in <sc_uint<4>>	cntContextOut;
   sc_in <bool> context[9];

   //sterowanie MemoryAccessModule / VISITED AND CHECKED
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readContextFlag_vis;
   //wyniki MemoryAccessModule / VISITED AND CHECKED
   sc_in <bool> contextOutFlag_vis;
   sc_in <sc_uint<4>>	cntContextOut_vis;
   sc_in <bool> context_vis[9];

   //sterowanie MemoryAccessModule / RESULTS
   sc_out <sc_uint<COORD_WIDTH>>  i_rslt;
   sc_out   <sc_uint<COORD_WIDTH>> j_rslt;
   sc_out <bool>                 writeFlag;
   //wyniki MemoryAccessModule / RESULTS
   sc_in <bool> flagContextOut_vis;
   sc_in <sc_uint<4>>	cntContextOut_vis;
   sc_in <bool> context_vis[9];

   sc_fifo<sc_uint<COORD_WIDTH>> iFifo;
   sc_fifo<sc_uint<COORD_WIDTH>> jFifo;

   int cnt;
   sc_signal<bool> enabled;

 void smartProcess()	{
	while(true)	{
	 wait();
	}
 }

 
  void startStop()	{
		if(enabled.read())	{
			enabled.write(false);
		}
		else	{
			iFifo.write(i_start.read());
			jFifo.write(j_start.read());
			enabled.write(true);
		}
  }


				 
  void testPrintout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" "<<valSingle.read()<<endl;
	}
  }


  SC_CTOR(SmartModule)
  {
	cnt = 0;
	SC_CTHREAD(smartProcess, clock.pos());

	SC_METHOD(startStop);
		dont_initialize();
		sensitive<<enable.pos()<<finished.pos();

	sc_fifo<sc_uint<COORD_WIDTH>[2]> fifo (8);
	//SC_CTHREAD(test_printout, clock.pos());
  }
};*/