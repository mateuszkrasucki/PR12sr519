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
   sc_in <bool> context[8];

   //sterowanie VisitedModule
   sc_out <sc_uint<COORD_WIDTH>>  i_vis;
   sc_out   <sc_uint<COORD_WIDTH>> j_vis;
   sc_out <bool>                 readContextFlag_vis;
   sc_out <bool>				writeFlag_vis;
   sc_out <bool>		dataOut_vis;

   //wyniki VisitedModule
   sc_in<bool> contextVisitedFlag;
   sc_in <bool> contextOutFlag_vis;
   sc_in <bool> context_vis[8];

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
		 if(enabled.read() && !writeFlag_vis.read() && !readContextFlag.read() && !readContextFlag_vis.read())	{
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
		 else if(enabled.read() && writeFlag_vis.read() && !readContextFlag.read() && !readContextFlag_vis.read())	{
				writeFlag_vis.write(false);
				writeFlag_res.write(false);
				nextStep();
		 }
		 else if(enabled.read() && !writeFlag_vis.read() && readContextFlag.read() && readContextFlag_vis.read() && contextVisitedFlag.read())	{
					 readContextFlag.write(false);
					 readContextFlag_vis.write(false);
					 nextStep();
		 }
		 else if(enabled.read() && !writeFlag_vis.read() && readContextFlag.read() 
			 && readContextFlag_vis.read() && contextOutFlag.read() && contextOutFlag_vis.read())	{

			 readContextFlag.write(false);
			 readContextFlag_vis.write(false);

					 if(!(context[0].read() & context[1].read() & context[2].read() & context[3].read() & context[4].read() 
						 & context[5].read() & context [6].read() & context [7].read()))	{

							if(context[0].read() & !context_vis[0].read())	{
								iFifo.write(i.read()-1);
								jFifo.write(j.read()-1);
							} else if(!context_vis[0].read())	{
								if(i.read()>0 && j.read() > 0)	{
									i_vis.write(i.read()-1);
									j_vis.write(j.read()-1);
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[1].read() & !context_vis[1].read())	{
								iFifo.write(i.read()-1);
								jFifo.write(j.read());
							} else if(!context_vis[1].read())	{
								if(i.read()>0)	{
									i_vis.write(i.read()-1);
									j_vis.write(j.read());
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[2].read() & !context_vis[2].read())	{
								iFifo.write(i.read()-1);
								jFifo.write(j.read()+1);
							} else if(!context_vis[2].read())	{
								if(i.read()>0 && j.read() < IMG_SIZE_j-1)	{
									i_vis.write(i.read()-1);
									j_vis.write(j.read()+1);
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[3].read() & !context_vis[3].read())	{
								iFifo.write(i.read());
								jFifo.write(j.read()-1);
							} else if(!context_vis[3].read())	{
								if(j.read() > 0)	{
									i_vis.write(i.read());
									j_vis.write(j.read()-1);
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[4].read() & !context_vis[4].read())	{
								iFifo.write(i.read());
								jFifo.write(j.read()+1);
							} else if(!context_vis[4].read())	{
								if(j.read() < IMG_SIZE_j-1)	{
									i_vis.write(i.read());
									j_vis.write(j.read()+1);
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[5].read() & !context_vis[5].read())	{
								iFifo.write(i.read()+1);
								jFifo.write(j.read()-1);
							} else if(!context_vis[5].read())	{
								if(i.read()<IMG_SIZE_i-1 && j.read() > 0)	{
									i_vis.write(i.read()+1);
									j_vis.write(j.read()-1);
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[6].read() & !context_vis[6].read())	{
								iFifo.write(i.read()+1);
								jFifo.write(j.read());
							} else if(!context_vis[6].read())	{
								if(i.read()>IMG_SIZE_i-1)	{
									i_vis.write(i.read()+1);
									j_vis.write(j.read());
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							if(context[7].read() & !context_vis[7].read())	{
								iFifo.write(i.read()+1);
								jFifo.write(j.read()+1);
							} else if(!context_vis[7].read())	{
								if(i.read()>IMG_SIZE_i-1 && j.read() > IMG_SIZE_j-1)	{
									i_vis.write(i.read()+1);
									j_vis.write(j.read()+1);
									dataOut_vis.write(true);
									writeFlag_vis.write(true);
									wait(1);
									writeFlag_vis.write(false);
								}
							}
							i_vis.write(i.read());
							j_vis.write(j.read());
							dataOut_res.write(true); 
							writeFlag_res.write(true);
					 }
				dataOut_vis.write(true);
				writeFlag_vis.write(true);
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
  }
};