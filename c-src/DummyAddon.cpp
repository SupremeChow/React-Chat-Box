#include <napi.h>
#include <iostream>
#include "DummyMain.h"
#include "DummySupport.h"

#include "./Tries/TriesNode.h"
#include "./Tries/Tries.h"


using namespace std;

Napi::Boolean DummyInBetweenMethod(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  //TODO figure out the relative path of where this module will exist, so if it can access files if needs be
  Tries* aDictionary = new Tries("C:/Users/Phillip/Desktop/NodeAddon/c-src/Tries/specificDict.txt");
  
  bool isWord = aDictionary->isWord(info[0].As<Napi::String>().Utf8Value());
  
  delete aDictionary;
  
  return Napi::Boolean::New(env, isWord);
  /* 
  DummyMain* aMain = new DummyMain("IDummy", 2);
  
  int dummyDoThingNumber = aMain->doMainThing(5);
  
  
  
  return Napi::Number::New(env, dummyDoThingNumber);
 */
	//return Napi::String::New(env, "String Method Works, so Addon is Working...");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
	exports.Set(
		Napi::String::New(env, "DummyStringMethod"),
		Napi::Function::New(env, DummyInBetweenMethod));
	return exports;
}

		//Could also use NODE_GYP_MODULE_NAME instead of 'dummyAddon', since it recferences what Gyp creates as target name
NODE_API_MODULE(dummyAddon, Init);