#ifndef ENGINE_H
#define ENGINE_H


#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <sstream>
#include <openssl/md5.h>

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

#include "console.h"
#include "fs.h"
#include "json.h"
#include "overhype.h"
#include "timecontrol.h"
#include "object.h"
#include "path.h"
#include "nodeapi.h"



class Engine
{
private:
    static std::unique_ptr<v8::Platform> platform;
    static v8::Isolate::CreateParams create_params;
    static Path location;

    static int argc;
    static char **argv;
    static std::string runMargin;
    static std::map<std::string, v8::Local<v8::Value>> scriptsCache;
    static std::map<std::string, v8::Persistent<v8::Value>> persistentsCache;



    static std::vector<v8::Local<v8::Context>> tmp_contexts;
    static std::vector<v8::Context::Scope> tmp_contexScopes;

    static v8::Local<v8::Context> tmp_context;
    static v8::Local<v8::Object> tmp_global;

    static v8::Local<v8::Value> LoadModule(v8::Isolate *isolate, v8::Local<v8::String> module);
protected:
    static void RequireCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void ExitCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void GetGlobalCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
public:
    static v8::Isolate *Init(int argc, char *argv[]);
    static void Mix(v8::Isolate *isolate, v8::Local<v8::Object> global);
    static void RunEntryPoint(v8::Isolate *isolate, const char *source);
    static v8::Local<v8::Value> Run(v8::Isolate *isolate, v8::Local<v8::String> source);
    static void Quit(v8::Isolate *isolate);
};

#endif // ENGINE_H
