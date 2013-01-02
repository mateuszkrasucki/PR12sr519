#include "systemc.h"

#include "Const.h"

SC_MODULE (ContextModule) {
  sc_in_clk     clock ; 
  //uniwersalna porty wspolrzednych
  sc_in    <sc_uint<ADDR_WIDTH/2>>  i;
  sc_in   <sc_uint<ADDR_WIDTH/2> > j;
  //komunikacja z pamiêci¹
  sc_out <sc_uint<ADDR_WIDTH>> address;
  sc_in <sc_logic> dataIn;
  //komunikacja z IntelligentModule i LinearModule
  sc_in    <sc_logic>                 flagContext;
  sc_out <sc_uint<4>>	cntContextOut;
  sc_out <sc_logic> context[9];
  sc_in    <sc_logic>                 flagSingle;
  sc_out <sc_logic> valLinear;

  sc_uint<4> cnt_context;
  sc_uint<10> tmp_address; 
  
  // Odczyt 
  void read () {
    if (flagContext.read() == 1)	{//odczytywanie kontekstu
		switch(cnt_context)
		{
		case 0:
			tmp_address = (i.read() - 1) * 20 + (j.read() - 1);
			address.write(tmp_address);
			cnt_context++;
			break;
		case 1:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read() - 1) * 20 + (j.read());
			address.write(tmp_address);
			cnt_context++;
			break;
		case 2:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read() - 1) * 20 + (j.read() + 1);
			address.write(tmp_address);
			cnt_context++;
			break;
		case 3:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read()) * 20 + (j.read() - 1);
			address.write(tmp_address);
			cnt_context++;
			break;
		case 4:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read()) * 20 + (j.read());
			address.write(tmp_address);
			cnt_context++;
			break;
		case 5:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read()) * 20 + (j.read() + 1);
			address.write(tmp_address);
			cnt_context++;
			break;
		case 6:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read() + 1) * 20 + (j.read() - 1);
			address.write(tmp_address);
			cnt_context++;
			break;
		case 7:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read() + 1) * 20 + (j.read());
			address.write(tmp_address);
			cnt_context++;
			break;
		case 8:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			tmp_address = (i.read() + 1) * 20 + (j.read() + 1);
			address.write(tmp_address);
			cnt_context++;
			break;
		case 9:
			context[cnt_context-1].write(dataIn.read());
			cntContextOut.write(cnt_context);
			cnt_context = 10;
			break;
		case 10:
			cntContextOut.write(cnt_context);
			break;
		}	 
	} else if (flagSingle.read() == 1)
	{
		switch(cnt_context)
		{
		case 0:
			tmp_address = i.read() * 20 + j.read();
			address.write(tmp_address);
			//cout<<"STEP1 i: "<<i.read()<<"j: "<<j.read()<<" "<<tmp_address<<endl;
			cnt_context++;
			break;
		case 1:
			cnt_context++;
			break;
		case 2:
			valLinear.write(dataIn.read());
			//cout<<"STEP2 i: "<<i.read()<<"j: "<<j.read()<<" "<<tmp_address<<" "<<dataIn.read()<<" "<<valLinear.read()<<endl;
			cntContextOut.write(cnt_context);
			cnt_context++;
			break;
		case 3:
			cnt_context = 0;
			cntContextOut.write(cnt_context);
			break;
		}
	}

  }

  void test_printout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" CONTEXTMOD "<<i.read()<<"x"<<j.read()<<" "<<address.read()<<" IN:"<<dataIn.read()<<" OUT:"<<valLinear.read()<<endl;
	}
  }
 

  SC_CTOR(ContextModule) {
    SC_METHOD (read);
	  dont_initialize();
      sensitive << clock.pos();

  SC_CTHREAD(test_printout, clock.pos());
  }

};