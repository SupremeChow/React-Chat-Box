#include "DummySupport.h"

using namespace std;


DummySupport::DummySupport()
{
	aNum = 0;
	oname = "None";
}
DummySupport::DummySupport(string newName, int newNum)
{
	aNum = newNum;
	oname = newName;
}

/* use default
~DummySupport::DummySupport()
{
}
*/

string DummySupport::getName()
{
	return oname;
}
int DummySupport::getNum()
{
	return aNum;
}

int DummySupport::doSupportThing(int multiplyNum)
{
	return multiplyNum * aNum;
}