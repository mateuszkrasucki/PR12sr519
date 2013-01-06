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

  //sterowanie ContextModule
  sc_in    <sc_uint<COORD_WIDTH>>  i;
  sc_in   <sc_uint<COORD_WIDTH>> j;
  sc_in <bool> readSingleFlag;
  sc_in <bool> readContextFlag;
  sc_in    <bool>				  writeFlag;
  sc_in	   <bool>				  dataIn;

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
		 if (readSingleFlag.read() && !readContextFlag.read() && !writeFlag.read())
		 {
			switch(cnt)
			{
			case 0:
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
		} else if (readContextFlag.read() && !readSingleFlag.read() && !writeFlag.read())	{//odczytywanie kontekstu
			switch(cnt)
			{
			case 0:
				if(i.read()>0 && j.read()>0)	{
					tmp_address = (i.read() - 1) * IMG_SIZE + (j.read() - 1);
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
				if(i.read()>0 && j.read()>0)	{
					context[cntContextOut.read()].write(dataMemIn.read());
					cntContextOut.write(cntContextOut.read()+1);
				}

				if(i.read()>0)	{
					tmp_address = (i.read() - 1) * IMG_SIZE + (j.read());
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
				if(i.read()>0)	{
					context[cntContextOut.read()].write(dataMemIn.read());
					cntContextOut.write(cntContextOut.read()+1);
				}
	
				if(i.read()>0 && j.read()<IMG_SIZE-1)	{
					tmp_address = (i.read() - 1) * IMG_SIZE + (j.read() + 1);
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
				if(i.read()>0 && j.read()<IMG_SIZE-1)	{
					context[cntContextOut.read()].write(dataMemIn.read());
					cntContextOut.write(cntContextOut.read()+1);
				}

				if(j.read()>0)	{
					tmp_address = (i.read()) * IMG_SIZE + (j.read() - 1);
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
				if(j.read()>0)	{
					context[cntContextOut.read()].write(dataMemIn.read());
					cntContextOut.write(cntContextOut.read()+1);
				}

				tmp_address = (i.read()) * IMG_SIZE + (j.read());
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

				if(j.read()<IMG_SIZE-1)	{
					tmp_address = (i.read()) * IMG_SIZE + (j.read() + 1);
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
				if(j.read()<IMG_SIZE-1)	{
					context[cntContextOut.read()].write(dataIn.read());
		 			cntContextOut.write(cntContextOut.read()+1);
				}

				if(i.read()<IMG_SIZE-1 && j.read()>0)	{
					tmp_address = (i.read() + 1) * IMG_SIZE + (j.read() - 1);
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
				if(i.read()<IMG_SIZE-1 && j.read()>0)	{
					context[cntContextOut.read()].write(dataMemIn.read());
					cntContextOut.write(cntContextOut.read()+1);
				}

				if(i.read()<IMG_SIZE-1)	{
					tmp_address = (i.read() + 1) * IMG_SIZE + (j.read());
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
				if(i.read()<IMG_SIZE-1)	{
					context[cntContextOut.read()].write(dataMemIn.read());
					cntContextOut.write(cntContextOut.read()+1);
				}

				if(i.read()<IMG_SIZE-1 && j.read()<IMG_SIZE-1)	{
					tmp_address = (i.read() + 1) * IMG_SIZE + (j.read() + 1);
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
				if(i.read()<IMG_SIZE-1 && j.read()<IMG_SIZE-1)	{
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
		else if(writeFlag.read() && !readSingleFlag.read() && !readContextFlag.read())
		{
			if(cnt==0)	{
				 cnt++;
				 address.write(i.read() * IMG_SIZE + j.read());
				 dataMemOut.write(dataIn.read());
				 writeMemFlag.write(true);
			}
			else	{
				cnt = 0;
				writeMemFlag.write(false);
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