#include "timecontrol.h"

v8::Local<v8::Function> TimeControl::cb;

#include <iostream>

void TimeControl::loop(v8::Isolate *isolate)
{
}


void TimeControl::SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate* isolate = args.GetIsolate();
    TimeControl::cb = v8::Local<v8::Function>::Cast(args[0]);
    v8::Local<v8::Integer> interval = args[1]->ToInteger(isolate);

    long i = 1000;
    int a = interval->ToInteger(isolate).operator->()->IntegerValue(isolate->GetCurrentContext()).To(&i);
    while (a) {
        const unsigned argc = 0;
        v8::Local<v8::Value> argv[argc] = {};
        TimeControl::cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
        std::this_thread::sleep_for(std::chrono::milliseconds(i));
    }

    //new std::thread(loop, isolate);
}

void TimeControl::Mix(v8::Isolate *isolate, v8::Local<v8::Object> global)
{
    v8::Local<v8::FunctionTemplate> setIntervalTemplate = v8::FunctionTemplate::New(isolate,  TimeControl::SetIntervalCallback);
    v8::Local<v8::Function> setIntervalFunction = setIntervalTemplate->GetFunction();
    global->Set( v8::String::NewFromUtf8(isolate, "setInterval"), setIntervalFunction);
}
