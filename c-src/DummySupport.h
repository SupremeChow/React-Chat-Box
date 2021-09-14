#pragma once

#include <iostream>
#include <string>

using namespace std;

class DummySupport
{
	private:
		
		int aNum;
		string oname;
	
	public:
	
		DummySupport();
		DummySupport(string newName, int newNum);
		
		string getName();
		int getNum();
		
		int doSupportThing(int multiplyNum);
		

};