#include "DummyMain.h"

using namespace std;


DummyMain::DummyMain()
{
	aNum = 0;
	oname = "None";
	
	supportClass = new DummySupport();
	
}
DummyMain::DummyMain(string newName, int newNum)
{
	aNum = newNum;
	oname = newName;
	
	supportClass = new DummySupport(oname + '2', aNum+1);
	
}
DummyMain::~DummyMain()
{
	delete supportClass;
}

string DummyMain::getName()
{
	return oname;
}
int DummyMain::getNum()
{
	return aNum;
}

int DummyMain::doMainThing(int multiplyNum)
{
	return multiplyNum * (aNum + supportClass->doSupportThing(multiplyNum));
}