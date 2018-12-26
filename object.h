#ifndef OBJECT_H
#define OBJECT_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>


class Object
{
protected:
    static void AssignCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
public:
    static void Mix(v8::Isolate *isolate, v8::Local<v8::Object> global);
};

#endif // OBJECT_H
