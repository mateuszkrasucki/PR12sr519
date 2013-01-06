#include "systemc.h"
#include "SimpleMemoryAccessModule.cpp"
#include "MemoryModule.cpp"
#include "ContextModule.cpp"
#include "LinearModule.cpp"


SC_MODULE(Edges)
{
	LinearModule linearModule;
	ContextModule contextModule;
	MemoryAccessModule visitedModule;
	MemoryModule sourceMemory;
	MemoryModule visitedMemory;

   sc_in_clk     clock ;   

  //komunikacja z sourceMemory
  sc_signal    <sc_uint<ADDR_WIDTH> > address_sourceMem;
  sc_signal    <bool>                 writeFlag_sourceMem;
  sc_signal    <bool>                 readFlag_sourceMem;
  sc_signal   <bool> dataIn_sourceMem;
  sc_signal   <bool> dataOut_sourceMem;

  //komunikacja z visitedMemory
  sc_signal    <sc_uint<ADDR_WIDTH> > address_visitedMem;
  sc_signal    <bool>                 writeFlag_visitedMem;
  sc_signal    <bool>                 readFlag_visitedMem;
  sc_signal   <bool> dataIn_vistedMem;
  sc_signal   <bool> dataOut_visitedMem;

  //komunikacja z contextSourceAccessModule
  sc_signal    <sc_uint<COORD_WIDTH>>  i_context;
  sc_signal   <sc_uint<COORD_WIDTH>> j_context;
  sc_signal <bool> readSingleFlag_context;
  sc_signal <bool> readContextFlag_context;
  sc_signal    <bool>				  writeFlag_context;
  sc_signal	   <bool>				  dataIn_context;
  sc_signal<bool> singleOutFlag_context;
  sc_signal <bool> dataOut_context;
  sc_signal <sc_uint<4>>	cntContextOut_context;
  sc_signal<bool> contextOutFlag_context;
  sc_signal <bool> context[9];


  //komunikacja z visitedMemoryAccessModule
  sc_signal    <sc_uint<COORD_WIDTH>>  i_visitedMemA;
  sc_signal   <sc_uint<COORD_WIDTH>> j_visitedMemA;
  sc_signal <bool> readSingleFlag_visitedMemA;
  sc_signal    <bool>				  writeFlag_visitedMemA;
  sc_signal	   <bool>				  dataIn_visitedMemA;
  sc_signal<bool> singleOutFlag_visitedMemA;
  sc_signal <bool> dataOut_visitedMemA;

  //komunikacja z LinearModule
    //sterowanie LinearModule
   sc_signal<bool> enable;

   //flaga zakoñczenia
   sc_signal<bool> finished;


  void finish()	{
	  visitedMemory.displayMemory();
  }



  SC_CTOR(Edges) : linearModule("LinearModule"), contextModule("ContextModule"), visitedModule("VisitedModule"), sourceMemory("SourceMemory"), visitedMemory("VisitedMemory")
  {
	sourceMemory.clock(clock);
	visitedMemory.clock(clock);
	visitedModule.clock(clock);
	contextModule.clock(clock);
	linearModule.clock(clock);

	sourceMemory.address(address_sourceMem);
	sourceMemory.writeFlag(writeFlag_sourceMem);
	sourceMemory.readFlag(readFlag_sourceMem);
	sourceMemory.dataIn(dataIn_sourceMem);
	sourceMemory.dataOut(dataOut_sourceMem);

	contextModule.address(address_sourceMem);
	contextModule.writeMemFlag(writeFlag_sourceMem);
	contextModule.readMemFlag(readFlag_sourceMem);
	contextModule.dataMemOut(dataIn_sourceMem);
	contextModule.dataMemIn(dataOut_sourceMem);

	visitedMemory.address(address_visitedMem);
	visitedMemory.writeFlag(writeFlag_visitedMem);
	visitedMemory.readFlag(readFlag_visitedMem);
	visitedMemory.dataIn(dataIn_vistedMem);
	visitedMemory.dataOut(dataOut_visitedMem);

	visitedModule.address(address_visitedMem);
	visitedModule.writeMemFlag(writeFlag_visitedMem);
	visitedModule.readMemFlag(readFlag_visitedMem);
	visitedModule.dataMemOut(dataIn_vistedMem);
	visitedModule.dataMemIn(dataOut_visitedMem);

  contextModule.i(i_context);
  contextModule.j(j_context);
  contextModule.readSingleFlag(readSingleFlag_context);
  contextModule.readContextFlag(readContextFlag_context);
  contextModule.writeFlag(writeFlag_context); //FALSE
  contextModule.dataIn(dataIn_context);
  contextModule.singleOutFlag(singleOutFlag_context);
  contextModule.dataOut(dataOut_context);
  contextModule.cntContextOut(cntContextOut_context);
  contextModule.contextOutFlag(contextOutFlag_context);

  linearModule.i(i_context);
  linearModule.j(j_context);
  linearModule.readSingleFlag(readSingleFlag_context);
  linearModule.readContextFlag(readContextFlag_context);
  linearModule.singleOutFlag(singleOutFlag_context);
  linearModule.valSingle(dataOut_context);
  linearModule.cntContextOut(cntContextOut_context);
  linearModule.contextOutFlag(contextOutFlag_context);

	for(int i=0; i<9; i++)	{
		contextModule.context[i](context[i]);
		linearModule.context[i](context[i]);
	}


  visitedModule.i(i_visitedMemA);
  visitedModule.j(j_visitedMemA);
  visitedModule.readSingleFlag(readSingleFlag_visitedMemA);
  visitedModule.writeFlag(writeFlag_visitedMemA);
  visitedModule.dataIn(dataIn_visitedMemA);
  visitedModule.singleOutFlag(singleOutFlag_visitedMemA);
  visitedModule.dataOut(dataOut_visitedMemA);

  linearModule.i_vis(i_visitedMemA);
  linearModule.j_vis(j_visitedMemA);
  linearModule.readSingleFlag_vis(readSingleFlag_visitedMemA);
  linearModule.writeFlag_vis(writeFlag_visitedMemA);
  linearModule.dataOut_vis(dataIn_visitedMemA);
  linearModule.singleOutFlag_vis(singleOutFlag_visitedMemA);
  linearModule.val_vis(dataOut_visitedMemA);

  linearModule.enable(enable);
  linearModule.finished(finished);

  sourceMemory.loadImage("C:/Users/Mateusz/Documents/GitHub/PR12sr519/PR12sr519/TestImages/cross.png");
  sourceMemory.displayMemory();

  SC_METHOD(finish);
	dont_initialize();
	sensitive << finished;

   writeFlag_context.write(false);
   dataIn_context.write(false);
   enable.write(true);

  }
};