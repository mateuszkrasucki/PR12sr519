#include "systemc.h"

#include "Const.h"

SC_MODULE (MemoryAccessModule) {
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
  sc_in <bool> readSingleFlag;
  sc_in    <bool>				  writeFlag;
  sc_in	   <bool>				  dataIn;

  //wyjœcie
  sc_out<bool> singleOutFlag;
  sc_out <bool> dataOut;

  int cnt;
  sc_uint<10> tmp_address; 
  
  // Odczyt 
  void process () {
	while(true)	{
		wait();
		 if (readSingleFlag.read() && !writeFlag.read())
		 {
			writeMemFlag.write(false);
			switch(cnt)
			{
			case 0:
				singleOutFlag.write(false);

				readMemFlag.write(true);
				tmp_address = i.read() * IMG_SIZE + j.read();
				address.write(tmp_address);

				cnt++;
				break;
			case 1:
				readMemFlag.write(false);
				cnt++;
				break;
			case 2:
				dataOut.write(dataMemIn.read());
				singleOutFlag.write(true);
				cnt++;
				break;
			case 3:
				singleOutFlag.write(false);
				cnt = 0;
				break;
			}
		}	
		else if(writeFlag.read() && !readSingleFlag.read())
		{
				 address.write(i.read() * IMG_SIZE + j.read());
				 dataMemOut.write(dataIn.read());
				 writeMemFlag.write(true);
		}
	}
  }


  SC_CTOR(MemoryAccessModule) {
	  cnt = 0;

    SC_CTHREAD(process,clock.pos());


  //SC_CTHREAD(test_printout, clock.pos());
  }

};