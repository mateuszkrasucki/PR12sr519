#include "systemc.h"

#include "Const.h"

SC_MODULE (AccessPictureModule) {
  sc_in_clk     clock ; 

  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_out    <bool>                 readMemFlag;
  sc_out    <bool>                 writeMemFlag;
  sc_in <bool> dataMemIn;
  sc_out <bool> dataMemOut;

  //sterowanie AccessPictureModule 1
  sc_in    <sc_uint<COORD_WIDTH>>  i_1;
  sc_in   <sc_uint<COORD_WIDTH>> j_1;
  sc_in <bool> readSingleFlag_1;

  //sterowanie AccessPictureModule_2
  sc_in    <sc_uint<COORD_WIDTH>>  i_2;
  sc_in   <sc_uint<COORD_WIDTH>> j_2;
  sc_in <bool> readContextFlag_2;

  //wyjœcie
  sc_out<bool> singleOutFlag;
  sc_out <bool> dataOut;
  sc_out<bool> contextOutFlag;
  sc_out <bool> context[9];

  sc_uint<ADDR_WIDTH> tmp_address; 
  
  void process () {
	while(true)	{
		wait();
		writeMemFlag.write(false);
			if (readSingleFlag_1.read() && !readContextFlag_2.read())
			{
					singleOutFlag.write(false);
					readMemFlag.write(true);
					tmp_address = i_1.read() * IMG_SIZE_j + j_1.read();
					address.write(tmp_address);
					wait(1);
					readMemFlag.write(false);
					wait(1);
					dataOut.write(dataMemIn.read());
					singleOutFlag.write(true);
					wait(1);
					singleOutFlag.write(false);
			}
			else if (!readSingleFlag_1.read() && readContextFlag_2.read())
			{
						if(i_2.read()>0 && j_2.read()>0)	{
							tmp_address = (i_2.read() - 1) * IMG_SIZE_j + (j_2.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[0].write(dataMemIn.read());
						}
						else	{
							context[0].write(false);
							wait(1);
						}
						if(i_2.read()>0)	{
							tmp_address = (i_2.read() - 1) * IMG_SIZE_j + (j_2.read());
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[1].write(dataMemIn.read());
						}
						else	{
							context[1].write(false);
						}
						if(i_2.read()>0 && j_2.read()<IMG_SIZE_j-1)	{
							tmp_address = (i_2.read() - 1) * IMG_SIZE_j + (j_2.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[2].write(dataMemIn.read());
						}
						else	{
							context[2].write(false);
						}
						if(j_2.read()>0)	{
							tmp_address = (i_2.read()) * IMG_SIZE_j + (j_2.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[3].write(dataMemIn.read());
						}
						else	{
							context[3].write(false);
						}
						tmp_address = (i_2.read()) * IMG_SIZE_j + (j_2.read());
						address.write(tmp_address);
						readMemFlag.write(true);
						wait(2);

						context[4].write(dataMemIn.read());

						if(j_2.read()<IMG_SIZE_j-1)	{
							tmp_address = (i_2.read()) * IMG_SIZE_j + (j_2.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[5].write(dataMemIn.read());
						}
						else	{
							context[5].write(false);
						}

						if(i_2.read()<IMG_SIZE_i-1 && j_2.read()>0)	{
							tmp_address = (i_2.read() + 1) * IMG_SIZE_j + (j_2.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[6].write(dataMemIn.read());
						}
						else	{
							context[6].write(false);
						}

						if(i_2.read()<IMG_SIZE_i-1)	{
							tmp_address = (i_2.read() + 1) * IMG_SIZE_j + (j_2.read());
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[7].write(dataMemIn.read());
						}
						else	{
							context[7].write(false);
						}

						if(i_2.read()<IMG_SIZE_i-1 && j_2.read()<IMG_SIZE_j-1)	{
							tmp_address = (i_2.read() + 1) * IMG_SIZE_j + (j_2.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
							wait(2);
							context[8].write(dataMemIn.read());
							contextOutFlag.write(true);
							readMemFlag.write(false);
						}
						else	{
							context[8].write(false);
							contextOutFlag.write(true);
							readMemFlag.write(false);
						}
						wait(1);
						contextOutFlag.write(false);
			}
	}
  }

  SC_CTOR(AccessPictureModule) {
    SC_CTHREAD(process,clock.pos());
  }

};