#include "systemc.h"
#include "ContextModule.cpp"
#include "SourceMemory.cpp"


SC_MODULE(Edges)
{
   sc_in_clk     clock ;   

   sc_signal   <sc_uint<ADDR_WIDTH/2>>  i;
   sc_signal   <sc_uint<ADDR_WIDTH/2> > j;
   sc_signal <sc_uint<4>>	cntContextOut;
   sc_signal <sc_logic> context[9];
   sc_signal <sc_logic> valLinear;
   sc_signal <sc_logic>                 flagContext;
   sc_signal    <sc_logic>                 flagSingle;

   ContextModule contextModule;
   SourceMemory sourceMemory;

   sc_signal <sc_uint<ADDR_WIDTH>> address;
   sc_signal <sc_logic> val;
   sc_signal <bool> nextStepAllowed;

   int i_cnt;
   int j_cnt;
   bool cnt_switch;

  void test()	{
	  while(true)	{
		  wait();
		  if(nextStepAllowed.read())	{
			  if(j_cnt+1 == 20)	{
				  i_cnt++;
				  j_cnt = 0;
			  }
			  else	{
				j_cnt++;
			  }

			  if(i_cnt == 20)	{
				  i_cnt = 0;
			  }

			  i.write(sc_uint<ADDR_WIDTH/2>(i_cnt));
			  j.write(sc_uint<ADDR_WIDTH/2>(j_cnt));
			  nextStepAllowed.write(false);
			  flagSingle.write(sc_logic('1'));
		  }
		  else if(cntContextOut.read() > 0)	{
			  flagSingle.write(sc_logic('0'));
			  cout<<"@" << sc_time_stamp() <<
			" i: "<<i.read()<<" j: "<<j.read()<<" address: "<<address.read()<<endl<<
			" flagContext: "<<flagContext.read()<<" flagSingle: "<<flagSingle.read()<<" cntContextOut: "<<cntContextOut.read()<<" VAL: "<<valLinear.read()<<endl;
			  nextStepAllowed.write(true);
		  }
			 
	  }
  }


  void test_printout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" "<<valLinear.read()<<endl;
	}
  }


  SC_CTOR(Edges) : contextModule("Context Module"), sourceMemory("Source Memory")
  {
    contextModule.i(i);
	contextModule.j(j);
	contextModule.address(address);
	contextModule.dataIn(val);
	contextModule.flagContext(flagContext);
	contextModule.flagSingle(flagSingle);
	contextModule.cntContextOut(cntContextOut);

	for(int i=0; i<9; i++)	{
		contextModule.context[i](context[i]);
	}

	contextModule.valLinear(valLinear);
	contextModule.clock(clock);

	sourceMemory.address(address);
	sourceMemory.dataOut(val);
	sourceMemory.clock(clock);

	sourceMemory.loadImage("C:/Users/Mateusz/Documents/GitHub/PR12sr519/PR12sr519/TestImages/slash.png");

	i_cnt = 3;
	j_cnt = 0;
	cnt_switch = false;
	nextStepAllowed.write(true);
	flagSingle.write(sc_logic('0'));
	flagContext.write(sc_logic('0'));

	SC_CTHREAD(test, clock.pos());
	//SC_CTHREAD(test_printout, clock.pos());
  }
};