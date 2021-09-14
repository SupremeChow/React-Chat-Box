/** 
//May Be Redundant, everything is actually handled in TriesAddon.cpp


	TriesAddonr.h
	
	Header File for Initiating the TriesWrapper class, used to wrap the functionality of Tries class into
	Objects readible in Javascript, using Node-addon-api
*/

#include <napi.h>
#include "TriesWrapper.cpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return TriesWrapper::Init(env, exports);
}

NODE_API_MODULE(triesAddon, InitAll)