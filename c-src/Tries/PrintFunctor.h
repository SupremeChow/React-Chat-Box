#pragma once

/**
Functor class for handling printing out input

For now, this is a helper class for the Tires/TriesNode classes, where words are sent to this functor to  be printed to designated stream

*/

#include <string>
#include <fstream> 
#include <iostream>

#include <mutex> //handling concurency


using namespace std;
class PrintFunctor
{
private:
	mutex printMutex; //Locking the printing process when trying to use threads to print to file or console
	ostream outputStream;

public:
	//											 _isstd (?) --> v 
	//PrintFunctor() : printMutex(), outputStream(cout.rdbuf, false) {};

	PrintFunctor() : printMutex(), outputStream(cout.rdbuf()) {};
	/*
	PrintFunctor(ostream& newOutputStream = cout)
	{
		//outputStream = newOutputStream.rdbuf();
		outputStream.rdbuf(newOutputStream.rdbuf());
	};
	*/

	PrintFunctor(ostream& newOutputStream) : printMutex(), outputStream(newOutputStream.rdbuf()) {};

	void operator ()(string printWord)
	{
		printMutex.lock();
		outputStream << printWord << "\n";
		printMutex.unlock();
	};


};
