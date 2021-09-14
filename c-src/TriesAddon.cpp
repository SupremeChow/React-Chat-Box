/** 
	TriesAddon.cpp
	
	This class acts as the entry point for initiating TriesWrapper creation for JavaScript using Node-addon-api
	(AFAIK this just calls TriesWrapper's Init() function, so it could be redundant for this instance)
*/

#include <napi.h>
#include "TriesWrapper.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return TriesWrapper::Init(env, exports);
}

NODE_API_MODULE(triesAddon, InitAll)