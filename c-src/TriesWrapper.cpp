/** 
	TriesWrapper.cpp
	
	Definition for TriesWrapper, used to wrap the functionality of Tries class into
	Objects readible in Javascript, using Node-addon-api
*/

#pragma once

#include "TriesWrapper.h"



Napi::Object TriesWrapper::Init(Napi::Env env, Napi::Object exports)
{
	Napi::Function func = DefineClass(env, "TriesWrapper", {
		InstanceMethod("AddWord", &TriesWrapper::AddWord),
		InstanceMethod("CheckIfWord", &TriesWrapper::CheckIfWord)
		
	});
	
	Napi::FunctionReference* constructor = new Napi::FunctionReference();
	*constructor = Napi::Persistent(func);
	env.SetInstanceData(constructor);
	
	exports.Set("TriesWrapper", func);
	return exports;
	
}


TriesWrapper::TriesWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TriesWrapper> (info)
{
	Napi::Env env = info.Env();
	
	//Will start with empty dictionary and add word by word instead of constructing by sending in an array of words
	//Since Node_addon_api is too confusing to make a constructor like this work
	
	aDictionary = new Tries();
	
	
}



void TriesWrapper::AddWord(const Napi::CallbackInfo& info)
{
	Napi::Env env = info.Env();
	
	int length = info.Length();
	
	if(length <= 0 || !info[0].IsString())
	{
		Napi::TypeError::New(env, "Expected Word to insert into Tries...").ThrowAsJavaScriptException();
		return;
	}
	
	
	
	
	
	string givenWord = info[0].As<Napi::String>().Utf8Value();
	
	bool insertResult = aDictionary->insertWord(givenWord);
	if(!insertResult)
	{
		Napi::TypeError::New(env, "Insertion encountered some kind of error...").ThrowAsJavaScriptException();
		return;
	}
	
}


Napi::Value TriesWrapper::CheckIfWord(const Napi::CallbackInfo& info)
{
	Napi::Env env = info.Env();
  
	int length = info.Length();
	if(length <= 0 || !info[0].IsString())
	{
		Napi::TypeError::New(env, "Expected Word to search Tries...").ThrowAsJavaScriptException();
		return Napi::Boolean::New(env, true); //send true to as if a filter word to prevent publishing of text
	}
	
	string givenWord = info[0].As<Napi::String>().Utf8Value();
	
	bool isWordResult = aDictionary->isWord(givenWord);

	return Napi::Boolean::New(env, isWordResult);
}



