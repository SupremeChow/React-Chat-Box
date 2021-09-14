/** 
	TriesWrapper.h
	
	Header File for TriesWrapper.cpp, used to wrap the functionality of Tries class into
	Objects readible in Javascript, using Node-addon-api
*/


#pragma once

#include <napi.h>
#include <iostream>


#include "./Tries/TriesNode.h"
#include "./Tries/Tries.h"
//TODO consider implementing a smart pointer for the Tries object, since
//Node_addon_api is too confusing to figure out how to handle memory management



using namespace std;

class TriesWrapper : public Napi::ObjectWrap<TriesWrapper>
{
	public:
		static Napi::Object Init(Napi::Env env, Napi::Object exports);
		TriesWrapper(const Napi::CallbackInfo &info);
	
	private:
		void AddWord(const Napi::CallbackInfo &info);
		Napi::Value CheckIfWord(const Napi::CallbackInfo& info);
		
		Tries* aDictionary;
		
};