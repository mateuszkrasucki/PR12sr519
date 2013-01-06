#include "systemc.h"
#include "ContextModule.cpp"
/*#include "SourceMemory.cpp"


SC_MODULE(Edges)
{
   sc_in_clk     clock ;   

   sc_signal   <sc_uint<COORD_WIDTH>>  i;
   sc_signal   <sc_uint<COORD_WIDTH>> j;
   sc_signal <sc_uint<4>>	cntContextOut;
   sc_signal <bool> context[9];
   sc_signal <bool> valSingle;
   sc_signal <bool>                 flagContext;
   sc_signal    <bool>                 flagSingle;

   ContextModule contextModule;
   SourceMemory sourceMemory;

   sc_signal <sc_uint<ADDR_WIDTH>> address;
   sc_signal <bool> val;
   sc_signal <bool> nextStepAllowed;

   int i_cnt;
   int j_cnt;
   int contextCntInt;
   bool cnt_switch;
   bool singleContextSwitch;


  void test()	{
	  while(true)	{
		  wait();
		  if(!singleContextSwitch)	{
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
				  flagSingle.write(bool('1'));
			  }
			  else if(cntContextOut.read() == 1)	{
				  flagSingle.write(bool('0'));
				  cout<<"@" << sc_time_stamp() <<
				" i: "<<i.read()<<" j: "<<j.read()<<" address: "<<address.read()<<endl<<
				" flagContext: "<<flagContext.read()<<" flagSingle: "<<flagSingle.read()<<" cntContextOut: "<<cntContextOut.read()<<" VAL: "<<valSingle.read()<<endl;
				  nextStepAllowed.write(true);
				  singleContextSwitch = true;
			  }
		  }
		  else if(singleContextSwitch)	{
			  if(nextStepAllowed.read())	{
				  i.write(sc_uint<ADDR_WIDTH/2>(i_cnt));
				  j.write(sc_uint<ADDR_WIDTH/2>(j_cnt));
				  nextStepAllowed.write(false);
				  flagContext.write(bool('1'));
				  contextCntInt = 1;
			  }
			  else if(cntContextOut.read() == contextCntInt)	{
				  cout<<"@" << sc_time_stamp() <<
				" i: "<<i.read()<<" j: "<<j.read()<<" address: "<<address.read()<<endl<<
				" flagContext: "<<flagContext.read()<<" flagSingle: "<<flagSingle.read()<<" cntContextOut: "<<cntContextOut.read()<<" VAL("<<cntContextOut.read()<<"): "<<context[cntContextOut.read()-1].read()<<endl;
			
				  if(contextCntInt == 9)	{
					flagContext.write(bool('0'));
					nextStepAllowed.write(true);
					contextCntInt = 0;
					singleContextSwitch = false;
				  }
				  else	{
					contextCntInt++;
				  }

			  }
		  }
			 
	  }
  }


  void test_printout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" "<<valSingle.read()<<endl;
	}
  }


  SC_CTOR(Edges) : contextModule("ContextModule"), sourceMemory("SourceMemory")
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

	contextModule.valSingle(valSingle);
	contextModule.clock(clock);

	sourceMemory.address(address);
	sourceMemory.dataOut(val);
	sourceMemory.clock(clock);

	sourceMemory.loadImage("C:/Users/Mateusz/Documents/GitHub/PR12sr519/PR12sr519/TestImages/slash.png");

	i_cnt = 4;
	j_cnt = 2;
	cnt_switch = false;
	singleContextSwitch = false;
	nextStepAllowed.write(true);
	flagSingle.write(bool('0'));
	flagContext.write(bool('0'));

	SC_CTHREAD(test, clock.pos());
	//SC_CTHREAD(test_printout, clock.pos());
  }
};*/