#include "systemc.h"
#include "Const.h"

SC_MODULE(LinearModule)
{
   sc_in_clk     clock ;   

   //sterowanie LinearModule
   sc_in<bool> enable;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie ContextModule 
   sc_out <sc_uint<COORD_WIDTH>>  i;
   sc_out   <sc_uint<COORD_WIDTH>> j;
   sc_out    <bool>                 readSingleFlag;
   //sc_out <bool>                 readContextFlag;

   //wyniki ContextModule
   sc_in <bool> singleOutFlag;
   sc_in <bool> valSingle;
   //sc_in<bool> contextOutFlag;
   //sc_in<sc_uint<4>>	cntContextOut;
   //sc_in<bool> context[9]


   //sterowanie ContextModule / VISITED
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readSingleFlag_vis;
   sc_out <bool>				 writeFlag_vis;
   sc_out <bool>		dataOut_vis;

   //wyniki ContextModuule / VISITED
   sc_in <bool> singleOutFlag_vis;
   sc_in <bool> val_vis;

   //sterowanie MemoryAccessModule / RESULTS
   sc_out <sc_uint<COORD_WIDTH>>  i_res;
   sc_out   <sc_uint<COORD_WIDTH>> j_res;
   sc_out <bool>				 writeFlag_res;
   sc_out <bool>		dataOut_res;
   

   //komunikacja ze SmartModule
   sc_out<bool> enableSmart;
   sc_out <sc_uint<COORD_WIDTH>>  i_smart;
   sc_out   <sc_uint<COORD_WIDTH>> j_smart;
   sc_in<bool> finished_smart;
  
	sc_signal <bool> enabled;
	sc_signal <bool> paused;


 void nextStep()	{
 		 if(i.read()+1 == IMG_SIZE && j.read()+1 == IMG_SIZE)	{
			//cout<<endl;
			writeFlag_res.write(false);
			writeFlag_vis.write(false);
			readSingleFlag.write(false);
			readSingleFlag_vis.write(false);
			finished.write(true);
			//cout<<"@" << sc_time_stamp()<<endl;
		}
		else if(j.read()+1 == IMG_SIZE)	{
				i_vis.write(i.read()+1);
				i_res.write(i.read()+1);
				i.write(i.read()+1);

				j_vis.write(0);
				j_res.write(0);
				j.write(0);

				writeFlag_res.write(false);
				writeFlag_vis.write(false);
				//cout<<endl; //COUT LINIA
				readSingleFlag.write(true);
				readSingleFlag_vis.write(true);
		}
		else	{
				//cout<<"@" << sc_time_stamp()<<endl;
				j_vis.write(j.read()+1);
				j_res.write(j.read()+1);
				j.write(j.read()+1);

				writeFlag_res.write(false);
				writeFlag_vis.write(false);
				readSingleFlag.write(true);
				readSingleFlag_vis.write(true);
		}
 }

 void linearProcess()	{
	while(true)	{
	 wait();
	 /*cout<<i.read()<<"x"<<j.read()<<endl<<"ENABLED: "<<enabled.read() << " WRITEFLAGVIS: "<<writeFlag_vis.read() << " READCONTEXTFLAG: "<<
		 readContextFlag.read()<<endl<<" READSINGLEFLAG: "<<readSingleFlag.read() <<" READVISFLAG: "<< readSingleFlag_vis.read()<<
		 " FINISHED: "<<finished.read() <<endl<< " SINGLEOUT: "<<singleOutFlag.read() << "VISOUTFLAG: "<< singleOutFlag_vis.read()<<endl<<
		 " CNTCONTEXT: "<<cntContextOut.read()<<" CONTEXTOUTFLAG: "<<contextOutFlag.read()<<endl;*/
	 if(paused.read())	{
		 if(finished_smart.read())	{
			 enableSmart.write(false);
			 paused.write(false);
			 nextStep();
		 }
	 }
	 else if(enabled.read() && writeFlag_vis.read() && writeFlag_res.read() /*&& !readContextFlag.read()*/ && !readSingleFlag.read() && !finished.read())
	 {
		 nextStep();
	 } 
	 else if(enabled.read() && !writeFlag_vis.read() && !writeFlag_res.read() /*&& !readContextFlag.read()*/ && !finished.read() && readSingleFlag.read() && singleOutFlag.read() && singleOutFlag_vis.read())	{
		 if(!val_vis.read())	{	 
			if(valSingle.read())	{
				 readSingleFlag.write(false);
				 readSingleFlag_vis.write(false);

				i_smart.write(i.read());
				j_smart.write(j.read());

				paused.write(true);

				enableSmart.write(true);
			 }
			 else	{
				 readSingleFlag.write(false);
				 readSingleFlag_vis.write(false);
				 dataOut_res.write(false); //WYNIK cout<<0;
				 dataOut_vis.write(true);
				 writeFlag_res.write(true);
				 writeFlag_vis.write(true);
			 }
		}
		else	{
			nextStep();
		}
	 }
	/*else if(enabled.read() && !readSingleFlag.read() && !writeFlag_vis.read() && !finished.read() && readContextFlag.read() && contextOutFlag.read())	{
		 if(!(context[0].read() & context[1].read() & context[2].read() & context[3].read() & context[5].read() & context[6].read() & context [7].read() & context [8].read()))	{
				 //dataOut_res.write(true); //WYNIK cout<<1;
				readContextFlag.write(false);

				i_smart.write(i.read());
				j_smart.write(j.read());

				paused.write(true);

				enableSmart.write(true);
		 }
		 else	{
				 dataOut_res.write(false); //WYNIK cout<<0;
				 dataOut_vis.write(true);
				 readContextFlag.write(false);
				 writeFlag_res.write(true);
				 writeFlag_vis.write(true);
		 }
	 }*/
	 else if(enabled.read() /*&& !readContextFlag.read()*/ && !readSingleFlag.read() && !readSingleFlag_vis.read() && !writeFlag_vis.read())	{
		  cout<<endl<<"@" << sc_time_stamp()<<" "<< "START"<<endl<<endl;
		  finished.write(false);
		  paused.write(false);
		  i.write(0);
		  j.write(0);
		  i_vis.write(0);
		  j_vis.write(0);
		  i_res.write(0);
		  j_res.write(0);
		  writeFlag_res.write(false);
		  writeFlag_vis.write(false);
		  //readContextFlag.write(false);
		  readSingleFlag_vis.write(true);
		  readSingleFlag.write(true);
	 }
	}
 }

  void startStop()	{
		if(enabled.read())	{
			cout<<"@" << sc_time_stamp()<<" KONIEC"<<endl;
			enabled.write(false);
		}
		else	{
			enabled.write(true);
		}
  }

				 
  void testPrintout()	{
	while(true)	{
		wait();
		cout<<"@" << sc_time_stamp()<<" "<<i.read()<<"x"<<j.read()<<" readSingle: "<<readSingleFlag.read()<<" singleIsOut "<<
			singleOutFlag.read()<<" val: "<<valSingle.read()<<endl;
	}
  }


  SC_CTOR(LinearModule)
  {
	SC_CTHREAD(linearProcess, clock.pos());
	
	SC_METHOD(startStop);
		dont_initialize();
		sensitive<<enable.pos()<<finished.pos();

	//SC_CTHREAD(testPrintout, clock.pos());
  }
};