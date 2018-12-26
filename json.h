#ifndef JSON_H
#define JSON_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

class JSON
{
private:
    static void TransformObject(v8::Isolate *isolate, v8::Local<v8::Object> object, v8::Local<v8::Function> cb);

protected:

    static void ParseCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
public:
    static void StringifyCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void Mix(v8::Isolate *isolate, v8::Local<v8::Object> global);
};

#endif // JSON_H
