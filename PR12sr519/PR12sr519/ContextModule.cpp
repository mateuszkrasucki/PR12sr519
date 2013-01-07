#include "systemc.h"

#include "Const.h"

SC_MODULE (ContextModule) {
  sc_in_clk     clock ; 


  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_out    <bool>                 readMemFlag;
  sc_out    <bool>                 writeMemFlag;
  sc_in <bool> dataMemIn;
  sc_out <bool> dataMemOut;

  //sterowanie ContextModule 1
  sc_in    <sc_uint<COORD_WIDTH>>  i_1;
  sc_in   <sc_uint<COORD_WIDTH>> j_1;
  sc_in <bool> readSingleFlag_1;
  sc_in <bool> readContextFlag_1;
  sc_in    <bool>				  writeFlag_1;
  sc_in	   <bool>				  dataIn_1;

  //sterowanie ContextModule_2
   sc_in    <sc_uint<COORD_WIDTH>>  i_2;
  sc_in   <sc_uint<COORD_WIDTH>> j_2;
  sc_in <bool> readSingleFlag_2;
  sc_in <bool> readContextFlag_2;
  sc_in    <bool>				  writeFlag_2;
  sc_in	   <bool>				  dataIn_2;

  //wyjœcie
  sc_out<bool> singleOutFlag;
  sc_out <bool> dataOut;
  sc_out <sc_uint<4>>	cntContextOut;
  sc_out<bool> contextOutFlag;
  sc_out <bool> context[9];

  int cnt;
 
  sc_uint<10> tmp_address; 
  
  // Odczyt 
  void process () {
	while(true)	{
		wait();
		writeMemFlag.write(false);
		if(readSingleFlag_1.read() || readContextFlag_1.read() || writeFlag_1.read())	{
				 if (readSingleFlag_1.read() && !readContextFlag_1.read() && !writeFlag_1.read())
				 {
					switch(cnt)
					{
					case 0:
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
				} else if (readContextFlag_1.read() && !readSingleFlag_1.read() && !writeFlag_1.read())	{//odczytywanie 
					switch(cnt)
					{
					case 0:
						cntContextOut.write(0);
						if(i_1.read()>0 && j_1.read()>0)	{
							tmp_address = (i_1.read() - 1) * IMG_SIZE + (j_1.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							context[cnt].write(false);
							cntContextOut.write(1);
							cnt++;
						}
						
						cnt++;
						break;
					case 1:
						cnt++;
						break;
					case 2:
						if(i_1.read()>0 && j_1.read()>0)	{
							context[0].write(dataMemIn.read());
							cntContextOut.write(1);
						}

						if(i_1.read()>0)	{
							tmp_address = (i_1.read() - 1) * IMG_SIZE + (j_1.read());
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							context[1].write(false);
							cntContextOut.write(2);
							cnt++;
						}

						cnt++;
						break;
					case 3:
						cnt++;
						break;
					case 4:
						if(i_1.read()>0)	{
							context[1].write(dataMemIn.read());
							cntContextOut.write(2);
						}
	
						if(i_1.read()>0 && j_1.read()<IMG_SIZE-1)	{
							tmp_address = (i_1.read() - 1) * IMG_SIZE + (j_1.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[2].write(false);
							cntContextOut.write(3);
						}

						cnt++;
						break;
					case 5:
						cnt++;
						break;
					case 6:
						if(i_1.read()>0 && j_1.read()<IMG_SIZE-1)	{
							context[2].write(dataMemIn.read());
							cntContextOut.write(3);
						}

						if(j_1.read()>0)	{
							tmp_address = (i_1.read()) * IMG_SIZE + (j_1.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[3].write(false);
							cntContextOut.write(4);
						}

						cnt++;
						break;
					case 7:
						cnt++;
						break;
					case 8:
						if(j_1.read()>0)	{
							context[3].write(dataMemIn.read());
							cntContextOut.write(4);
						}

						tmp_address = (i_1.read()) * IMG_SIZE + (j_1.read());
						address.write(tmp_address);
						readMemFlag.write(true);

						cnt++;
						break;
					case 9:
						cnt++;
						break;
					case 10:
						context[4].write(dataMemIn.read());
						cntContextOut.write(5);

						if(j_1.read()<IMG_SIZE-1)	{
							tmp_address = (i_1.read()) * IMG_SIZE + (j_1.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[5].write(false);
							cntContextOut.write(6);
						}

						cnt++;
						break;
					case 11:
						cnt++;
						break;
					case 12:
						if(j_1.read()<IMG_SIZE-1)	{
							context[5].write(dataMemIn.read());
		 					cntContextOut.write(6);
						}

						if(i_1.read()<IMG_SIZE-1 && j_1.read()>0)	{
							tmp_address = (i_1.read() + 1) * IMG_SIZE + (j_1.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[6].write(false);
							cntContextOut.write(7);
						}

						cnt++;
						break;
					case 13:
						cnt++;
						break;
					case 14:
						if(i_1.read()<IMG_SIZE-1 && j_1.read()>0)	{
							context[6].write(dataMemIn.read());
							cntContextOut.write(7);
						}

						if(i_1.read()<IMG_SIZE-1)	{
							tmp_address = (i_1.read() + 1) * IMG_SIZE + (j_1.read());
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[7].write(false);
							cntContextOut.write(8);
						}

						cnt++;
						break;
					case 15:
						cnt++;
						break;
					case 16:
						if(i_1.read()<IMG_SIZE-1)	{
							context[7].write(dataMemIn.read());
							cntContextOut.write(8);
						}

						if(i_1.read()<IMG_SIZE-1 && j_1.read()<IMG_SIZE-1)	{
							tmp_address = (i_1.read() + 1) * IMG_SIZE + (j_1.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt+=2;
							context[8].write(false);
							cntContextOut.write(9);
							contextOutFlag.write(true);
							readMemFlag.write(false);
						}

						cnt++;
						break;
					case 17:
						cnt++;
						break;
					case 18:
						if(i_1.read()<IMG_SIZE-1 && j_1.read()<IMG_SIZE-1)	{
							context[8].write(dataMemIn.read());
							cntContextOut.write(9);
							contextOutFlag.write(true);
							readMemFlag.write(false);
						}
						cnt++;
						break;
					case 19:
						cntContextOut.write(0);
						contextOutFlag.write(false);

						cnt = 0;
						break;
					}	 
				}
				else if(writeFlag_1.read() && !readSingleFlag_1.read() && !readContextFlag_1.read())
				{
					 address.write(i_1.read() * IMG_SIZE + j_1.read());
					 dataMemOut.write(dataIn_1.read());
					 writeMemFlag.write(true);
				}
			}
			else if(readSingleFlag_2.read() || readContextFlag_2.read() || writeFlag_2.read())	{
				 if (readSingleFlag_2.read() && !readContextFlag_2.read() && !writeFlag_2.read())
				 {
					switch(cnt)
					{
					case 0:
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
				} else if (readContextFlag_2.read() && !readSingleFlag_2.read() && !writeFlag_2.read())	{//odczytywanie kontekstu
					switch(cnt)
					{
					case 0:
						if(i_2.read()>0 && j_2.read()>0)	{
							tmp_address = (i_2.read() - 1) * IMG_SIZE + (j_2.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cnt].write(false);
							cntContextOut.write(cnt+1);
						}

						cnt++;
						break;
					case 1:
						cnt++;
						break;
					case 2:
						if(i_2.read()>0 && j_2.read()>0)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
						}

						if(i_2.read()>0)	{
							tmp_address = (i_2.read() - 1) * IMG_SIZE + (j_2.read());
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
						}

						cnt++;
						break;
					case 3:
						cnt++;
						break;
					case 4:
						if(i_2.read()>0)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
						}
	
						if(i_2.read()>0 && j_2.read()<IMG_SIZE-1)	{
							tmp_address = (i_2.read() - 1) * IMG_SIZE + (j_2.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
						}

						cnt++;
						break;
					case 5:
						cnt++;
						break;
					case 6:
						if(i_2.read()>0 && j_2.read()<IMG_SIZE-1)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
						}

						if(j_2.read()>0)	{
							tmp_address = (i_2.read()) * IMG_SIZE + (j_2.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
						}

						cnt++;
						break;
					case 7:
						cnt++;
						break;
					case 8:
						if(j_2.read()>0)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
						}

						tmp_address = (i_2.read()) * IMG_SIZE + (j_2.read());
						address.write(tmp_address);
						readMemFlag.write(true);

						cnt++;
						break;
					case 9:
						cnt++;
						break;
					case 10:
						context[cntContextOut.read()].write(dataMemIn.read());
						cntContextOut.write(cntContextOut.read()+1);

						if(j_2.read()<IMG_SIZE-1)	{
							tmp_address = (i_2.read()) * IMG_SIZE + (j_2.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
						}

						cnt++;
						break;
					case 11:
						cnt++;
						break;
					case 12:
						if(j_2.read()<IMG_SIZE-1)	{
							context[cntContextOut.read()].write(dataMemIn.read());
		 					cntContextOut.write(cntContextOut.read()+1);
						}

						if(i_2.read()<IMG_SIZE-1 && j_2.read()>0)	{
							tmp_address = (i_2.read() + 1) * IMG_SIZE + (j_2.read() - 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
						}

						cnt++;
						break;
					case 13:
						cnt++;
						break;
					case 14:
						if(i_2.read()<IMG_SIZE-1 && j_2.read()>0)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
						}

						if(i_2.read()<IMG_SIZE-1)	{
							tmp_address = (i_2.read() + 1) * IMG_SIZE + (j_2.read());
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt++;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
						}

						cnt++;
						break;
					case 15:
						cnt++;
						break;
					case 16:
						if(i_2.read()<IMG_SIZE-1)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
						}

						if(i_2.read()<IMG_SIZE-1 && j_2.read()<IMG_SIZE-1)	{
							tmp_address = (i_2.read() + 1) * IMG_SIZE + (j_2.read() + 1);
							address.write(tmp_address);
							readMemFlag.write(true);
						}
						else	{
							cnt+=2;
							context[cntContextOut.read()].write(false);
							cntContextOut.write(cntContextOut.read()+1);
							contextOutFlag.write(true);
							readMemFlag.write(false);
						}

						cnt++;
						break;
					case 17:
						cnt++;
						break;
					case 18:
						if(i_2.read()<IMG_SIZE-1 && j_2.read()<IMG_SIZE-1)	{
							context[cntContextOut.read()].write(dataMemIn.read());
							cntContextOut.write(cntContextOut.read()+1);
							contextOutFlag.write(true);
							readMemFlag.write(false);
						}
						cnt++;
						break;
					case 19:
						cntContextOut.write(0);
						contextOutFlag.write(false);

						cnt = 0;
						break;
					}	 
				}
				else if(writeFlag_2.read() && !readSingleFlag_2.read() && !readContextFlag_2.read())
				{
					 address.write(i_2.read() * IMG_SIZE + j_2.read());
					 dataMemOut.write(dataIn_2.read());
					 writeMemFlag.write(true);
				}
			}

	}
  }



  SC_CTOR(ContextModule) {
	  cnt = 0;

    SC_CTHREAD(process,clock.pos());

  //SC_CTHREAD(test_printout, clock.pos());
  }

};