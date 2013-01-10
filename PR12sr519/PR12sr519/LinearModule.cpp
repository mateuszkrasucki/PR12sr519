#include "systemc.h"
#include "Const.h"

SC_MODULE(LinearModule)
{
   sc_in_clk     clock ;   

   //sterowanie LinearModule
   sc_in<bool> enable;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie AccessPictureModule 
   sc_out <sc_uint<COORD_WIDTH>>  i;
   sc_out   <sc_uint<COORD_WIDTH>> j;
   sc_out    <bool>                 readSingleFlag;

   //wyniki AccessPictureModule
   sc_in <bool> singleOutFlag;
   sc_in <bool> valSingle;

   //sterowanie VisitedModule
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readSingleFlag_vis;
   sc_out <bool>				 writeFlag_vis;
   sc_out <bool>		dataOut_vis;

   //wyniki VisitedModule / VISITED
   sc_in <bool> singleOutFlag_vis;
   sc_in <bool> val_vis;

   //komunikacja ze SmartModule
   sc_out<bool> enableSmart;
   sc_out <sc_uint<COORD_WIDTH>>  i_smart;
   sc_out   <sc_uint<COORD_WIDTH>> j_smart;
   sc_in<bool> finished_smart;
  
   sc_signal <bool> enabled;
   sc_signal <bool> paused;


 void nextStep()	{
 		 if(i.read()+1 == IMG_SIZE_i && j.read()+1 == IMG_SIZE_j)	{
			writeFlag_vis.write(false);
			readSingleFlag.write(false);
			readSingleFlag_vis.write(false);
			finished.write(true);
		}
		else if(j.read()+1 == IMG_SIZE_j)	{
				i_vis.write(i.read()+1);
				i.write(i.read()+1);

				j_vis.write(0);
				j.write(0);

				writeFlag_vis.write(false);
				readSingleFlag.write(true);
				readSingleFlag_vis.write(true);
		}
		else	{
				j_vis.write(j.read()+1);
				j.write(j.read()+1);

				writeFlag_vis.write(false);
				readSingleFlag.write(true);
				readSingleFlag_vis.write(true);
		}
 }

 void linearProcess()	{
	while(true)	{
	 wait();
	 finished.write(false);
	 if(paused.read())	{
		 if(finished_smart.read())	{
			 enableSmart.write(false);
			 paused.write(false);
			 nextStep();
		 }
	 }
	 else if(enabled.read() && writeFlag_vis.read() && !readSingleFlag.read() && !finished.read())
	 {
		 nextStep();
	 } 
	 else if(enabled.read() && !writeFlag_vis.read() && !finished.read() && readSingleFlag.read() && singleOutFlag.read() && singleOutFlag_vis.read())	{
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
				 dataOut_vis.write(true);
				 writeFlag_vis.write(true);
			 }
		}
		else	{
			nextStep();
		}
	 }
	 else if(enabled.read() && !readSingleFlag.read() && !readSingleFlag_vis.read() && !writeFlag_vis.read())	{
		  cout<<endl<<"@" << sc_time_stamp()<<" "<< "START"<<endl<<endl;
		  finished.write(false);
		  paused.write(false);
		  i.write(0);
		  j.write(0);
		  i_vis.write(0);
		  j_vis.write(0);
		  writeFlag_vis.write(false);
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
	 cout<<"@" << sc_time_stamp()<<" "<<i.read()<<"x"<<j.read()<<endl<<"ENABLED: "<<enabled.read() << " WRITEFLAGVIS: "<<writeFlag_vis.read() <<
		endl<<" READSINGLEFLAG: "<<readSingleFlag.read() <<" READVISFLAG: "<< readSingleFlag_vis.read()<<
		 " FINISHED: "<<finished.read() <<endl<< " SINGLEOUT: "<<singleOutFlag.read() << "VISOUTFLAG: "<< singleOutFlag_vis.read()<<endl;
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