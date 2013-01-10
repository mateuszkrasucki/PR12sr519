#include "systemc.h"

#include "Const.h"

SC_MODULE (SaveResultsModule) {
  sc_in_clk     clock ; 

  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_out    <bool>                 readMemFlag;
  sc_out    <bool>                 writeMemFlag;
  sc_in <bool> dataMemIn;
  sc_out <bool> dataMemOut;

  //sterowanie MemoryAccessModule 1
  sc_in    <sc_uint<COORD_WIDTH>>  i_1;
  sc_in   <sc_uint<COORD_WIDTH>> j_1;
  sc_in    <bool>				  writeFlag_1;
  sc_in	   <bool>				  dataIn_1;

  //sterowanie MemoryAccessModule 2
  sc_in    <sc_uint<COORD_WIDTH>>  i_2;
  sc_in   <sc_uint<COORD_WIDTH>> j_2;
  sc_in    <bool>				  writeFlag_2;
  sc_in	   <bool>				  dataIn_2;

  sc_uint<10> tmp_address; 
  
  void process () {
	while(true)	{
		wait();
		writeMemFlag.write(false);
		if(writeFlag_1.read() && !readSingleFlag_1.read())
			{
					 address.write(i_1.read() * IMG_SIZE_j + j_1.read());
					 dataMemOut.write(dataIn_1.read());
					 writeMemFlag.write(true);
			}
		//jeœli sterowanie_2
		} else if(readSingleFlag_2.read() || writeFlag_2.read())	{
			//odczyt ze sterowania_2
			 if (readSingleFlag_2.read() && !writeFlag_2.read())
			 {
					singleOutFlag.write(false);
					readMemFlag.write(true);
					tmp_address = i_2.read() * IMG_SIZE_j + j_2.read();
					address.write(tmp_address);
					wait(1);
					readMemFlag.write(false);
					wait(1);
					dataOut.write(dataMemIn.read());
					singleOutFlag.write(true);
					wait(1);
					singleOutFlag.write(false);
			}	
			//zapis ze sterowania _2
			else if(writeFlag_2.read() && !readSingleFlag_2.read())
			{
					 address.write(i_2.read() * IMG_SIZE_j + j_2.read());
					 dataMemOut.write(dataIn_2.read());
					 writeMemFlag.write(true);
			}
		}
	}
  }


  SC_CTOR(MemoryAccessModule) {
    SC_CTHREAD(process,clock.pos());


  //SC_CTHREAD(test_printout, clock.pos());
  }

};