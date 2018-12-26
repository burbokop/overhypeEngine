#ifndef OVERHYPE_H
#define OVERHYPE_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

#include <thread>

#include "overhypecore.h"
#include "spm.h"

#define NODE_SET_METHOD(ISOLATE, OBJECT, KEY, CB) ((OBJECT)->Set(v8::String::NewFromUtf8((ISOLATE), (KEY)), v8::FunctionTemplate::New((ISOLATE),  (CB))->GetFunction()))


class Overhype
{
public:
    static v8::Local<v8::Object> Require(v8::Isolate *isolate);

    static v8::Local<v8::String> GetVectorSource(v8::Isolate *isolate);
    static v8::Local<v8::String> GetTimerSource(v8::Isolate *isolate);
    static v8::Local<v8::String> GetSpeadSource(v8::Isolate *isolate);
};

#endif // OVERHYPE_H
