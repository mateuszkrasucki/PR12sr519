#include "systemc.h"
#include "Const.h"
#include "MemoryModule.cpp"
#include "AccessPictureModule.cpp"
#include "ResultsModule.cpp"
#include "LinearModuleClassic.cpp"
#include "ImageGenerator.cpp"


SC_MODULE(EdgesLinear)
{
	LinearModuleClassic linearModule;
	AccessPictureModule contextModule;
	ResultsModule resultsModule;
	MemoryModule sourceMemory;
	MemoryModule resultsMemory;

   sc_in_clk     clock ;   

  //komunikacja z sourceMemory
  sc_signal    <sc_uint<ADDR_WIDTH> > address_sourceMem;
  sc_signal    <bool>                 writeFlag_sourceMem;
  sc_signal    <bool>                 readFlag_sourceMem;
  sc_signal   <bool> dataIn_sourceMem;
  sc_signal   <bool> dataOut_sourceMem;

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
  sc_signal <bool> allowContextFlag;
  
  sc_signal<bool> singleOutFlag_context;
  sc_signal <bool> dataOut_context;
  sc_signal<bool> contextOutFlag_context;
  sc_signal <bool> context[8];

  //komunikacja z resultsModule
  sc_signal    <sc_uint<COORD_WIDTH>>  i_resultsMemA;
  sc_signal   <sc_uint<COORD_WIDTH>> j_resultsMemA;
  sc_signal    <bool>				  writeFlag_resultsMemA;
  sc_signal	   <bool>				  dataIn_resultsMemA;

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
  }

  void displaying()	{
	  while(true)	{
		wait(1600);
		if(displayFlag.read()==false)	{
			//cout<<"STAN @" << sc_time_stamp()<<endl;
			//display();
			//cout<<endl;
		}
	  }
  }

  void createImage() {
	cout << "KONIEC PRZETWARZANIA. " << endl << "...ZAPISYWANIE OBRAZU..." << endl;
	
	int* tablica;
	tablica = new int[IMG_SIZE_i*IMG_SIZE_j];
	for(int i=0; i<IMG_SIZE_i*IMG_SIZE_j; i++)	{
		tablica[i]=resultsMemory.getMem(i);
	}
	ImageGenerator *ig = new ImageGenerator();
	ig->generate(tablica, "_end");

	cout << "OBRAZ ZAPISANY..." << endl;
  }

  void destroyMemory()	{
	  sourceMemory.destroyMemory();
	  resultsMemory.destroyMemory();
  }

  SC_CTOR(EdgesLinear) : linearModule("LinearModule"), contextModule("ContextModule"),  resultsModule("ResultsModule"), sourceMemory("SourceMemory"), resultsMemory("ResultsMemory")
  {
	sourceMemory.clock(clock);
	resultsMemory.clock(clock);
	resultsModule.clock(clock);
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

  contextModule.singleOutFlag(singleOutFlag_context);
  contextModule.dataOut(dataOut_context);
  contextModule.contextOutFlag(contextOutFlag_context);
  contextModule.allowContextFlag(allowContextFlag);

  linearModule.allowContextFlag(allowContextFlag);
  linearModule.i_1(i_context_1);
  linearModule.j_1(j_context_1);
  linearModule.readSingleFlag(readSingleFlag_context_1);
  linearModule.singleOutFlag(singleOutFlag_context);
  linearModule.single(dataOut_context);

  linearModule.i_2(i_context_2);
  linearModule.j_2(j_context_2);
  linearModule.readContextFlag(readContextFlag_context_2);
  linearModule.contextOutFlag(contextOutFlag_context);

	for(int i=0; i<8; i++)	{
		contextModule.context[i](context[i]);
		linearModule.context[i](context[i]);
	}

  resultsModule.i(i_resultsMemA);
  resultsModule.j(j_resultsMemA);
  resultsModule.writeFlag(writeFlag_resultsMemA);
  resultsModule.dataIn(dataIn_resultsMemA);

  linearModule.i_res(i_resultsMemA);
  linearModule.j_res(j_resultsMemA);
  linearModule.writeFlag_res(writeFlag_resultsMemA);
  linearModule.dataOut_res(dataIn_resultsMemA);

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


  SC_CTHREAD(finish, clock.pos());

 SC_METHOD(display);
	dont_initialize();
	sensitive << displayFlag;

 SC_CTHREAD(displaying,clock.pos());

    displayFlag.write(false);
    enable.write(true);

  }
};