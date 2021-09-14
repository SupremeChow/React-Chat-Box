#pragma once

#include <iostream>
#include <string>
#include "DummySupport.h"

using namespace std;

class DummyMain
{
	private:
		DummySupport* supportClass;
		
		int aNum;
		string oname;
	
	public:
	
		DummyMain();
		DummyMain(string newName, int newNum);
		~DummyMain();
		
		string getName();
		int getNum();
		
		int doMainThing(int multiplyNum);
		

};