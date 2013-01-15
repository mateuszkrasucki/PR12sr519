#include "systemc.h"
#include "Const.h"
#include "MemoryModule.cpp"
#include "AccessPictureModule.cpp"
#include "VisitedModule.cpp"
#include "ResultsModule.cpp"
#include "LinearModule.cpp"
#include "SmartModule.cpp"
#include "ImageGenerator.cpp"


SC_MODULE(Edges)
{
	SmartModule smartModule;
	LinearModule linearModule;
	AccessPictureModule contextModule;
	VisitedModule visitedModule;
	ResultsModule resultsModule;
	MemoryModule sourceMemory;
	MemoryModule visitedMemory;
	MemoryModule resultsMemory;

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
  sc_signal   <bool> dataIn_visitedMem;
  sc_signal   <bool> dataOut_visitedMem;

  //komunikacja z resultsMemory
  sc_signal    <sc_uint<ADDR_WIDTH> > address_resultsMem;
  sc_signal    <bool>                 writeFlag_resultsMem;
  sc_signal    <bool>                 readFlag_resultsMem;
  sc_signal   <bool> dataIn_resultsMem;
  sc_signal   <bool> dataOut_resultsMem;

  //komunikacja z MemoryAccessModule
  sc_signal    <sc_uint<COORD_WIDTH>>  i_context_1;
  sc_signal   <sc_uint<COORD_WIDTH>> j_context_1;
  sc_signal <bool> readSingleFlag_context_1;

  sc_signal    <sc_uint<COORD_WIDTH>>  i_context_2;
  sc_signal   <sc_uint<COORD_WIDTH>> j_context_2;
  sc_signal <bool> readContextFlag_context_2;

  sc_signal<bool> singleOutFlag_context;
  sc_signal <bool> dataOut_context;
  sc_signal<bool> contextOutFlag_context;
  sc_signal <bool> context[8];

  //komunikacja z visitedModule
  sc_signal    <sc_uint<COORD_WIDTH>>  i_visitedMemA_1;
  sc_signal   <sc_uint<COORD_WIDTH>> j_visitedMemA_1;
  sc_signal <bool> readSingleFlag_visitedMemA_1;
  sc_signal    <bool>				  writeFlag_visitedMemA_1;
  sc_signal	   <bool>				  dataIn_visitedMemA_1;

  sc_signal    <sc_uint<COORD_WIDTH>>  i_visitedMemA_2;
  sc_signal   <sc_uint<COORD_WIDTH>> j_visitedMemA_2;
  sc_signal <bool> readContextFlag_visitedMemA_2;
  sc_signal    <bool>				  writeFlag_visitedMemA_2;
  sc_signal	   <bool>				  dataIn_visitedMemA_2;

  sc_signal<bool> singleOutFlag_visitedMemA_1;
  sc_signal <bool> single_visitedMemA_1;

  sc_signal<bool> contextVisitedFlag_visitedMemA;
  sc_signal<bool> contextOutFlag_visitedMemA;
  sc_signal <bool> context_visitedMemA[8];

  sc_signal <bool> allowContextFlag_visitedMemA;

  //komunikacja z resultsModule
  sc_signal    <sc_uint<COORD_WIDTH>>  i_resultsMemA;
  sc_signal   <sc_uint<COORD_WIDTH>> j_resultsMemA;
  sc_signal    <bool>				  writeFlag_resultsMemA;
  sc_signal	   <bool>				  dataIn_resultsMemA;


  //komunikacja z SmartModule
  //sterowanie SmartModule
  sc_signal<bool> enableSmart;
  sc_signal <sc_uint<COORD_WIDTH>>  i_smart;
  sc_signal   <sc_uint<COORD_WIDTH>> j_smart;
  //flaga zakoñczenia
  sc_signal<bool> finishedSmart;

  //komunikacja z LinearModule
  //sterowanie LinearModule
  sc_signal<bool> enable;
  //flaga zakoñczenia
  sc_signal<bool> finished;

  sc_signal<bool> displayFlag;

  void finish()	{
	  while(true)	{
		wait();
		if(finished.read()==true)	{
			displayFlag.write(true);
		}
	  }

   }

  void display()	{
	    /*cout<<"VISITED"<<endl;
		visitedMemory.displayMemory();
		cout<<endl<<"RESULTS"<<endl;
		resultsMemory.displayMemory();*/
		//resultsMemory.displayMemory();

		int* tablicaVisit;
		int* tablicaResult;
		tablicaVisit = new int[IMG_SIZE_i*IMG_SIZE_j];
		tablicaResult = new int[IMG_SIZE_i*IMG_SIZE_j];
		for(int i=0; i<IMG_SIZE_i*IMG_SIZE_j; i++)	{
			tablicaVisit[i]=visitedMemory.getMem(i);
			tablicaResult[i]=resultsMemory.getMem(i);
		}

		ImageGenerator *ig = new ImageGenerator();
		string filenameVisit = sc_time_stamp().to_string();
		string filenameResult = sc_time_stamp().to_string();
		ig->generateBoth(tablicaVisit,tablicaResult,filenameVisit);
  }

  void displaying()	{
	  while(true)	{
		wait(1600);
		if(displayFlag.read()==false)	{
			cout<<"STAN @" << sc_time_stamp()<<endl;
			display();
			cout<<endl;
		}
	  }
  }

  void createImage() {
	cout << "KONIEC PRZETWARZANIA. " << endl << "ZAPISYWANIE OBRAZU." << endl;
	
	int* tablica;
	tablica = new int[IMG_SIZE_i*IMG_SIZE_j];
	for(int i=0; i<IMG_SIZE_i*IMG_SIZE_j; i++)	{
		tablica[i]=resultsMemory.getMem(i);
	}
	ImageGenerator *ig = new ImageGenerator();
	ig->generate(tablica, sc_time_stamp().to_string());

	cout << "OBRAZ ZAPISANY." << endl;
  }

  void destroyMemory()	{
	  sourceMemory.destroyMemory();
	  visitedMemory.destroyMemory();
	  resultsMemory.destroyMemory();
  }

  SC_CTOR(Edges) : smartModule("SmartModule"), linearModule("LinearModule"), contextModule("ContextModule"), visitedModule("VisitedModule"), resultsModule("ResultsModule"), sourceMemory("SourceMemory"), visitedMemory("VisitedMemory"), resultsMemory("ResultsMemory")
  {
	sourceMemory.clock(clock);
	visitedMemory.clock(clock);
	resultsMemory.clock(clock);
	visitedModule.clock(clock);
	resultsModule.clock(clock);
	contextModule.clock(clock);
	linearModule.clock(clock);
	smartModule.clock(clock);

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
	visitedMemory.dataIn(dataIn_visitedMem);
	visitedMemory.dataOut(dataOut_visitedMem);

	visitedModule.address(address_visitedMem);
	visitedModule.writeMemFlag(writeFlag_visitedMem);
	visitedModule.readMemFlag(readFlag_visitedMem);
	visitedModule.dataMemOut(dataIn_visitedMem);
	visitedModule.dataMemIn(dataOut_visitedMem);

	resultsMemory.address(address_resultsMem);
	resultsMemory.writeFlag(writeFlag_resultsMem);
	resultsMemory.readFlag(readFlag_resultsMem);
	resultsMemory.dataIn(dataIn_resultsMem);
	resultsMemory.dataOut(dataOut_resultsMem);

	resultsModule.address(address_resultsMem);
	resultsModule.writeMemFlag(writeFlag_resultsMem);
	resultsModule.readMemFlag(readFlag_resultsMem);
	resultsModule.dataMemOut(dataIn_resultsMem);
	resultsModule.dataMemIn(dataOut_resultsMem);

  contextModule.i_1(i_context_1);
  contextModule.j_1(j_context_1);
  contextModule.readSingleFlag_1(readSingleFlag_context_1);

  contextModule.i_2(i_context_2);
  contextModule.j_2(j_context_2);
  contextModule.readContextFlag_2(readContextFlag_context_2);
  contextModule.allowContextFlag(allowContextFlag_visitedMemA);

  contextModule.singleOutFlag(singleOutFlag_context);
  contextModule.dataOut(dataOut_context);
  contextModule.contextOutFlag(contextOutFlag_context);

  linearModule.i(i_context_1);
  linearModule.j(j_context_1);
  linearModule.readSingleFlag(readSingleFlag_context_1);
  linearModule.singleOutFlag(singleOutFlag_context);
  linearModule.single(dataOut_context);

  smartModule.i(i_context_2);
  smartModule.j(j_context_2);
  smartModule.readContextFlag(readContextFlag_context_2);
  smartModule.contextOutFlag(contextOutFlag_context);

  visitedModule.i_1(i_visitedMemA_1);
  visitedModule.j_1(j_visitedMemA_1);
  visitedModule.readSingleFlag_1(readSingleFlag_visitedMemA_1);
  visitedModule.writeFlag_1(writeFlag_visitedMemA_1);
  visitedModule.dataIn_1(dataIn_visitedMemA_1);

  visitedModule.i_2(i_visitedMemA_2);
  visitedModule.j_2(j_visitedMemA_2);
  visitedModule.readContextFlag_2(readContextFlag_visitedMemA_2);
  visitedModule.writeFlag_2(writeFlag_visitedMemA_2);
  visitedModule.dataIn_2(dataIn_visitedMemA_2);

  visitedModule.singleOutFlag_1(singleOutFlag_visitedMemA_1);
  visitedModule.single_1(single_visitedMemA_1);
  visitedModule.contextVisitedFlag(contextVisitedFlag_visitedMemA);
  visitedModule.allowContextFlag(allowContextFlag_visitedMemA);
  visitedModule.contextOutFlag(contextOutFlag_visitedMemA);

  linearModule.i_vis(i_visitedMemA_1);
  linearModule.j_vis(j_visitedMemA_1);
  linearModule.readSingleFlag_vis(readSingleFlag_visitedMemA_1);
  linearModule.writeFlag_vis(writeFlag_visitedMemA_1);
  linearModule.dataOut_vis(dataIn_visitedMemA_1);
  linearModule.singleOutFlag_vis(singleOutFlag_visitedMemA_1);
  linearModule.single_vis(single_visitedMemA_1);

  smartModule.i_vis(i_visitedMemA_2);
  smartModule.j_vis(j_visitedMemA_2);
  smartModule.readContextFlag_vis(readContextFlag_visitedMemA_2);
  smartModule.writeFlag_vis(writeFlag_visitedMemA_2);
  smartModule.dataOut_vis(dataIn_visitedMemA_2);
  smartModule.contextVisitedFlag(contextVisitedFlag_visitedMemA);

  smartModule.contextOutFlag_vis(contextOutFlag_visitedMemA);

	for(int i=0; i<8; i++)	{
		contextModule.context[i](context[i]);
		smartModule.context[i](context[i]);
		visitedModule.context[i](context_visitedMemA[i]);
		smartModule.context_vis[i](context_visitedMemA[i]);
	}

  resultsModule.i(i_resultsMemA);
  resultsModule.j(j_resultsMemA);
  resultsModule.writeFlag(writeFlag_resultsMemA);
  resultsModule.dataIn(dataIn_resultsMemA);

  smartModule.i_res(i_resultsMemA);
  smartModule.j_res(j_resultsMemA);
  smartModule.writeFlag_res(writeFlag_resultsMemA);
  smartModule.dataOut_res(dataIn_resultsMemA);

  smartModule.enable(enableSmart);
  smartModule.i_start(i_smart);
  smartModule.j_start(j_smart);
  smartModule.finished(finishedSmart);

  linearModule.enableSmart(enableSmart);
  linearModule.i_smart(i_smart);
  linearModule.j_smart(j_smart);
  linearModule.finished_smart(finishedSmart);

  linearModule.enable(enable);
  linearModule.finished(finished);

  //sourceMemory.loadImage("../PR12sr519/TestImages/cross.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/slash.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/difficult.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/nested.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/kratownica.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/punkty.png");
  sourceMemory.loadImage("../PR12sr519/TestImages/almostfull.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/100.png");
  //sourceMemory.loadImage("../PR12sr519/TestImages/2028.png");
  resultsMemory.initMemory();
  visitedMemory.initMemory();


  SC_CTHREAD(finish, clock.pos());

 SC_METHOD(display);
	dont_initialize();
	sensitive << displayFlag;

 SC_CTHREAD(displaying,clock.pos());

    displayFlag.write(false);
    enable.write(true);

  }
};