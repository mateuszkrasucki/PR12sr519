#include "systemc.h"

#include "Const.h"

SC_MODULE (MemoryAccessModule) {
  sc_in_clk     clock ; 

  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_out    <bool>                 writeMemFlag;
  sc_out    <bool>                 readMemFlag;
  sc_in <bool> dataMemIn;
  sc_out <bool> dataMemOut;

  //komunikacja z IntelligentModule i LinearModule
  //sterowanie MemoryAccessModule
  sc_in    <sc_uint<COORD_WIDTH>>  i;
  sc_in   <sc_uint<COORD_WIDTH>> j;
  sc_in    <bool>                 readSingleFlag;
  sc_in    <bool>                 readContextFlag;
  sc_in    <bool>				  writeFlag;
  sc_in	   <bool>				  dataIn;

  //wyjœcie
  sc_out <sc_uint<4>>	cntContextOut;
  sc_out<bool> contextOutFlag;
  sc_out<bool> singleOutFlag;
  sc_out <bool> context[9];
  sc_out <bool> dataOut;
  sc_out <bool> writtenFlag;

  int cnt;
  int cnt_write;
  sc_uint<10> tmp_address; 
  
  // Odczyt 
  void read () {
    if (readContextFlag.read() && !readSingleFlag.read() && !writeFlag.read())	{//odczytywanie kontekstu
		switch(cnt)
		{
		case 0:
			contextOutFlag.write(false);
			singleOutFlag.write(false);

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

			cnt = 0;
			break;
		}	 
	} 
	else if (readSingleFlag.read() && !readContextFlag.read() && !writeFlag.read())
	{
		switch(cnt)
		{
		case 0:
			contextOutFlag.write(false);
			singleOutFlag.write(false);

			readMemFlag.write(true);
			tmp_address = i.read() * IMG_SIZE + j.read();
			address.write(tmp_address);

			cnt++;
			break;
		case 1:
			cnt++;
			break;
		case 2:
			dataOut.write(dataMemIn.read());
			singleOutFlag.write(true);
			readMemFlag.write(false);

			cnt++;
			break;
		case 3:
			singleOutFlag.write(false);

			cnt = 0;
			break;
		}
	}

  }

  void write()	{
	  while(true)	{
		  wait();
		  if(writeFlag.read() && !readContextFlag.read() && !readSingleFlag.read())	{
			  if(cnt == 0)	{
				 address.write(i.read() * IMG_SIZE + j.read());
				 dataMemOut.write(dataIn.read());
				 writeMemFlag.write(true);
			  }
			  else	{
				writeMemFlag.write(false);
			  }
		  }
	  }
  }

  void test_printout()	{
	while(true)	{
		wait();
		if(readSingleFlag.read() ==1)	{
			cout<<"@" << sc_time_stamp()<<" CONTEXTMOD SINGLE "<<i.read()<<"x"<<j.read()<<" "<<address.read()<<" IN:"<<dataMemIn.read()<<" FLAGSINGLEOUT: "<<singleOutFlag.read()<<" OUT:"<<dataOut.read()<<endl;
		}
		else if(readContextFlag.read() == 1)	{
			cout<<"@" << sc_time_stamp()<<" CONTEXTMOD CONTEXT "<<i.read()<<"x"<<j.read()<<" "<<address.read()<<" IN:"<<dataMemIn.read()<<" CNTXTNUMBOUT: "<<cntContextOut.read()<<" OUT:";
			if(cntContextOut.read() > 0)
				cout<<context[cntContextOut.read()-1];
			cout<<endl;
		}

	}
  }
 

  SC_CTOR(MemoryAccessModule) {
	  cnt = 0;
	  cnt_write = 0;

    SC_METHOD (read);
	  dont_initialize();
      sensitive << clock.pos();

  //SC_CTHREAD(test_printout, clock.pos());
  }

};