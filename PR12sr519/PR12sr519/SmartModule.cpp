#include "systemc.h"
#include "Const.h"

SC_MODULE(SmartModule)
{
   sc_in_clk     clock ;   

   //sterowanie SmartModule
   sc_in<bool> enable;
   sc_in <sc_uint<COORD_WIDTH>>  i_start;
   sc_in   <sc_uint<COORD_WIDTH>> j_start;

   //flaga zakoñczenia
   sc_out<bool> finished;

   //sterowanie AccessPictureModule
   sc_out <sc_uint<COORD_WIDTH>>  i;
   sc_out   <sc_uint<COORD_WIDTH>> j;
   sc_out <bool>                 readContextFlag;

   //wyniki AccessPictureModule
   sc_in <bool> contextOutFlag;
   sc_in <bool> context[9];

   //sterowanie VisitedModule
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readContextFlag_vis;
   sc_out <bool>				writeFlag_vis;
   sc_out <bool>		dataOut_vis;

   //wyniki VisitedModule
   sc_in <bool> contextOutFlag_vis;
   sc_in <bool> context_vis[9];

   //sterowanie ResultsModule
   sc_out <sc_uint<COORD_WIDTH>>  i_res;
   sc_out   <sc_uint<COORD_WIDTH>> j_res;
   sc_out <bool>                 writeFlag_res;
   sc_out <bool>		dataOut_res;

   sc_fifo<sc_uint<COORD_WIDTH>> iFifo;
   sc_fifo<sc_uint<COORD_WIDTH>> jFifo;

   int iTmp;
   int jTmp;

   sc_signal<bool> enabled;
 
 void nextStep()	{
			if(iFifo.num_available() !=0 && jFifo.num_available() != 0)	{
				iTmp = iFifo.read();
				jTmp = jFifo.read();
				i.write(iTmp);
				j.write(jTmp);
				i_vis.write(iTmp);
				j_vis.write(jTmp);
				i_res.write(iTmp);
				j_res.write(jTmp);

				writeFlag_vis.write(false);
				writeFlag_res.write(false);

				readContextFlag.write(true);
				readContextFlag_vis.write(true);
			}
			else	{
				readContextFlag_vis.write(false);
				readContextFlag.write(false);
				writeFlag_vis.write(false);
				writeFlag_res.write(false);
				finished.write(true);
			}
 }

 void smartProcess()	{
	while(true)	{
		 wait();
		 finished.write(false);
		 if(enabled.read() && !writeFlag_vis.read() && !writeFlag_res.read() && !readContextFlag.read() && !readContextFlag_vis.read())	{
			i.write(i_start.read());
			j.write(j_start.read());
			i_vis.write(i_start.read());
			j_vis.write(j_start.read());
			i_res.write(i_start.read());
			j_res.write(j_start.read());

			readContextFlag.write(true);
			readContextFlag_vis.write(true);
			writeFlag_vis.write(false);
			writeFlag_res.write(false);
			dataOut_vis.write(false);
			dataOut_res.write(false);
		 } 
		 else if(enabled.read() && writeFlag_vis.read() && writeFlag_res.read() && !readContextFlag.read() && !readContextFlag_vis.read())	{
			nextStep();
		 }
		 else if(enabled.read() && !writeFlag_vis.read() && !writeFlag_res.read() && readContextFlag.read() 
			 && readContextFlag_vis.read() && contextOutFlag.read() && contextOutFlag_vis.read())	{

			 cout<<iFifo.num_available()<<endl;

			 readContextFlag.write(false);
			 readContextFlag_vis.write(false);
			 /*cout<<"@" << sc_time_stamp()<<" got context"<<" ";
			 cout<<i.read()<<i_res.read()<<i_vis.read()<<"x"<<j.read()<<j_res.read()<<j_vis.read()<<endl;*/

			 if(!context_vis[4])	{	
				 //cout<<i.read()<<i_vis.read()<<"x"<<j.read()<<j_vis.read()<<endl;
					 if(!(context[0].read() & context[1].read() & context[2].read() & context[3].read() & context[5].read() 
						 & context[6].read() & context [7].read() & context [8].read()))	{
							/*for(int i=0; i<9; i++)	{
								cout<<"@" << sc_time_stamp()<<"context "<<i<<" "<<context[i].read()<<" visited? "<<context_vis[i].read()<<endl;
							}*/
							if(context[0].read() & !context_vis[0].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()-1<<"x"<<j.read()-1<<endl;
								iFifo.write(i.read()-1);
								jFifo.write(j.read()-1);
							}
							if(context[1].read() & !context_vis[1].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()-1<<"x"<<j.read()<<endl;
								iFifo.write(i.read()-1);
								jFifo.write(j.read());
							}
							if(context[2].read() & !context_vis[2].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()-1<<"x"<<j.read()+1<<endl;
								iFifo.write(i.read()-1);
								jFifo.write(j.read()+1);
							}
							if(context[3].read() & !context_vis[3].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()<<"x"<<j.read()-1<<endl;
								iFifo.write(i.read());
								jFifo.write(j.read()-1);
							}
							if(context[5].read() & !context_vis[5].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()<<"x"<<j.read()+1<<endl;
								iFifo.write(i.read());
								jFifo.write(j.read()+1);
							}
							if(context[6].read() & !context_vis[6].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()+1<<"x"<<j.read()-1<<endl;
								iFifo.write(i.read()+1);
								jFifo.write(j.read()-1);
							}
							if(context[7].read() & !context_vis[7].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()+1<<"x"<<j.read()<<endl;
								iFifo.write(i.read()+1);
								jFifo.write(j.read());
							}
							if(context[8].read() & !context_vis[8].read())	{
								//cout<<"ADD TO FIFO: "<<i.read()+1<<"x"<<j.read()+1<<endl;
								//cout<<iFifo.num_available()<<" "<<iFifo.num_free()<<endl;
								iFifo.write(i.read()+1);
								jFifo.write(j.read()+1);
							}
							dataOut_res.write(true); 
							writeFlag_res.write(true);
					 }
					 else	{
						dataOut_res.write(false);
					 }
				dataOut_vis.write(true);
				writeFlag_res.write(true);
				writeFlag_vis.write(true);
			 }
			 else	{
				nextStep();
			 }
		 }
	 }
 }

 
  void startStop()	{
		if(enabled.read())	{
			enabled.write(false);
		}
		else	{
			enabled.write(true);
		}
  }


				 


  SC_CTOR(SmartModule) : iFifo(100), jFifo(100)
  {

	SC_CTHREAD(smartProcess, clock.pos());

	SC_METHOD(startStop);
		dont_initialize();
		sensitive<<enable.pos()<<finished.pos();

	//SC_CTHREAD(test_printout, clock.pos());
  }
};