#include "systemc.h"

#include "Const.h"

SC_MODULE (ResultsModule) {
  sc_in_clk     clock ; 

  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_out    <bool>                 readMemFlag;
  sc_out    <bool>                 writeMemFlag;
  sc_in <bool> dataMemIn;
  sc_out <bool> dataMemOut;

  //sterowanie MemoryAccessModule
  sc_in    <sc_uint<COORD_WIDTH>>  i;
  sc_in   <sc_uint<COORD_WIDTH>> j;
  sc_in    <bool>				  writeFlag;
  sc_in	   <bool>				  dataIn;

  sc_uint<10> tmp_address; 
  
  void process () {
	while(true)	{
		wait();
		writeMemFlag.write(false);
		if(writeFlag.read())
			{
					 address.write(i.read() * IMG_SIZE_j + j.read());
					 dataMemOut.write(dataIn.read());
					 writeMemFlag.write(true);
			}
	}
  }


  SC_CTOR(ResultsModule) {
    SC_CTHREAD(process,clock.pos());


  //SC_CTHREAD(test_printout, clock.pos());
  }

};