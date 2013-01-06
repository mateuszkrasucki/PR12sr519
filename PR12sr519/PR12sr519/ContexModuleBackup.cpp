#include "systemc.h"

#include "Const.h"

SC_MODULE (ContextModule) {
  sc_in_clk     clock ; 

  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_in <bool> dataIn;

  //komunikacja z IntelligentModule i LinearModule
  //sterowanie ContextModule
  sc_in    <sc_uint<COORD_WIDTH>>  i;
  sc_in   <sc_uint<COORD_WIDTH>> j;
  sc_in    <bool>                 flagSingle;
  sc_in    <bool>                 flagContext;

  //wyjœcie
  sc_out <sc_uint<4>>	cntContextOut;
  sc_out<bool> flagContextOut;
  sc_out<bool> flagSingleOut;
  sc_out <bool> context[9];
  sc_out <bool> valSingle;


  sc_uint<4> cntContext;
  int cnt;
  sc_uint<10> tmp_address; 
  
  // Odczyt 
  void read () {
    if (flagContext.read() == 1)	{//odczytywanie kontekstu
		switch(cnt)
		{
		case 0:
			flagContextOut.write(false);
			flagSingleOut.write(false);

			if(i.read()>0 && j.read()>0)	{
				tmp_address = (i.read() - 1) * IMG_SIZE + (j.read() - 1);
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 1:
			cnt++;
			break;
		case 2:
			if(i.read()>0 && j.read()>0)
				context[cntContext].write(dataIn.read());
			else	{
				context[cntContext].write(false);
			}
			cntContext++;
			cntContextOut.write(cntContext);

			if(i.read()>0)	{
				tmp_address = (i.read() - 1) * IMG_SIZE + (j.read());
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 3:
			cnt++;
			break;
		case 4:
			if(i.read()>0)
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);
	
			if(i.read()>0 && j.read()<IMG_SIZE-1)	{
				tmp_address = (i.read() - 1) * IMG_SIZE + (j.read() + 1);
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 5:
			cnt++;
			break;
		case 6:
			if(i.read()>0 && j.read()<IMG_SIZE-1)	
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);

			if(j.read()>0)	{
				tmp_address = (i.read()) * IMG_SIZE + (j.read() - 1);
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 7:
			cnt++;
			break;
		case 8:
			if(j.read()>0)
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);

			tmp_address = (i.read()) * IMG_SIZE + (j.read());
			address.write(tmp_address);

			cnt++;
			break;
		case 9:
			cnt++;
			break;
		case 10:
			context[cntContext].write(dataIn.read());

			cntContext++;
			cntContextOut.write(cntContext);

			if(j.read()<IMG_SIZE-1)	{
				tmp_address = (i.read()) * IMG_SIZE + (j.read() + 1);
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 11:
			cnt++;
			break;
		case 12:
			if(j.read()<IMG_SIZE-1)
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);

			if(i.read()<IMG_SIZE-1 && j.read()>0)	{
				tmp_address = (i.read() + 1) * IMG_SIZE + (j.read() - 1);
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 13:
			cnt++;
			break;
		case 14:
			if(i.read()<IMG_SIZE-1 && j.read()>0)
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);

			if(i.read()<IMG_SIZE-1)	{
				tmp_address = (i.read() + 1) * IMG_SIZE + (j.read());
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 15:
			cnt++;
			break;
		case 16:
			if(i.read()<IMG_SIZE-1)
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);

			if(i.read()<IMG_SIZE-1 && j.read()<IMG_SIZE-1)	{
				tmp_address = (i.read() + 1) * IMG_SIZE + (j.read() + 1);
				address.write(tmp_address);
			}

			cnt++;
			break;
		case 17:
			cnt++;
			break;
		case 18:
			if(i.read()<IMG_SIZE-1 && j.read()<IMG_SIZE-1)	
				context[cntContext].write(dataIn.read());
			else
				context[cntContext].write(false);

			cntContext++;
			cntContextOut.write(cntContext);

			flagContextOut.write(true);

			cnt++;
			break;
		case 19:
			cntContext = 0;
			cntContextOut.write(cntContext);

			cnt = 0;
			break;
		}	 
	} 
	else if (flagSingle.read() == 1)
	{
		switch(cnt)
		{
		case 0:
			flagContextOut.write(false);
			flagSingleOut.write(false);

			tmp_address = i.read() * IMG_SIZE + j.read();
			address.write(tmp_address);

			cnt++;
			break;
		case 1:
			cnt++;
			break;
		case 2:
			valSingle.write(dataIn.read());
			flagSingleOut.write(true);

			cnt++;
			break;
		case 3:
			flagSingleOut.write(false);

			cnt = 0;
			break;
		}
	}

  }

  void test_printout()	{
	while(true)	{
		wait();
		if(flagSingle.read() ==1)	{
			cout<<"@" << sc_time_stamp()<<" CONTEXTMOD SINGLE "<<i.read()<<"x"<<j.read()<<" "<<address.read()<<" IN:"<<dataIn.read()<<" FLAGSINGLEOUT: "<<flagSingleOut.read()<<" OUT:"<<valSingle.read()<<endl;
		}
		else if(flagContext.read() == 1)	{
			cout<<"@" << sc_time_stamp()<<" CONTEXTMOD CONTEXT "<<i.read()<<"x"<<j.read()<<" "<<address.read()<<" IN:"<<dataIn.read()<<" CNTXTNUMBOUT: "<<cntContextOut.read()<<" OUT:";
			if(cntContextOut.read() > 0)
				cout<<context[cntContextOut.read()-1];
			cout<<endl;
		}

	}
  }
 

  SC_CTOR(ContextModule) {
	  cnt = 0;
	  cntContext = 0;

    SC_METHOD (read);
	  dont_initialize();
      sensitive << clock.pos();

  //SC_CTHREAD(test_printout, clock.pos());
  }

};