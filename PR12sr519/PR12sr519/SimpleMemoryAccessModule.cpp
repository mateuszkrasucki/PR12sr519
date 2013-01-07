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

  //sterowanie MemoryAccessModule 1
  sc_in    <sc_uint<COORD_WIDTH>>  i_1;
  sc_in   <sc_uint<COORD_WIDTH>> j_1;
  sc_in <bool> readSingleFlag_1;
  sc_in    <bool>				  writeFlag_1;
  sc_in	   <bool>				  dataIn_1;

  //sterowanie MemoryAccessModule 2
  sc_in    <sc_uint<COORD_WIDTH>>  i_2;
  sc_in   <sc_uint<COORD_WIDTH>> j_2;
  sc_in <bool> readSingleFlag_2;
  sc_in    <bool>				  writeFlag_2;
  sc_in	   <bool>				  dataIn_2;

  //wyjœcie
  sc_out<bool> singleOutFlag;
  sc_out <bool> dataOut;

  int cnt;
  sc_uint<10> tmp_address; 
  
  // Odczyt 
  void process () {
	while(true)	{
		wait();
		writeMemFlag.write(false);
		if(readSingleFlag_1.read() || writeFlag_1.read())	{
			 if (readSingleFlag_1.read() && !writeFlag_1.read())
			 {
				switch(cnt)
				{
				case 0:
					singleOutFlag.write(false);

					readMemFlag.write(true);
					tmp_address = i_1.read() * IMG_SIZE + j_1.read();
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
			else if(writeFlag_1.read() && !readSingleFlag_1.read())
			{
					 address.write(i_1.read() * IMG_SIZE + j_1.read());
					 dataMemOut.write(dataIn_1.read());
					 writeMemFlag.write(true);
			}
		} else if(readSingleFlag_2.read() || writeFlag_2.read())	{
			 if (readSingleFlag_2.read() && !writeFlag_2.read())
			 {
				switch(cnt)
				{
				case 0:
					singleOutFlag.write(false);

					readMemFlag.write(true);
					tmp_address = i_2.read() * IMG_SIZE + j_2.read();
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
			else if(writeFlag_2.read() && !readSingleFlag_2.read())
			{
					 address.write(i_2.read() * IMG_SIZE + j_2.read());
					 dataMemOut.write(dataIn_2.read());
					 writeMemFlag.write(true);
			}
		}
	}
  }


  SC_CTOR(MemoryAccessModule) {
	  cnt = 0;

    SC_CTHREAD(process,clock.pos());


  //SC_CTHREAD(test_printout, clock.pos());
  }

};