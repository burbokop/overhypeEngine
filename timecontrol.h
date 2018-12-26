#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

#include <thread>

class TimeControl
{
private:
    static v8::Local<v8::Function> cb;
    static void loop(v8::Isolate *isolate);
protected:
    static void SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
public:
    static void Mix(v8::Isolate *isolate, v8::Local<v8::Object> global);
};

#endif // TIMECONTROL_H
