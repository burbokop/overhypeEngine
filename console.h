#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <vector>
#include <algorithm>

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

#define COLOR_RESET "\u001b[0m"
#define COLOR_BLACK "\u001b[30m"
#define COLOR_RED "\u001b[31m"
#define COLOR_GREEN "\u001b[32m"
#define COLOR_YELLOW "\u001b[33m"
#define COLOR_BLUE "\u001b[34m"
#define COLOR_MAGENTA "\u001b[35m"
#define COLOR_CYAN "\u001b[36m"
#define COLOR_WHITE "\u001b[37m"



class Console
{
private:
    static std::string margin;
    static unsigned int level;
    static std::vector<int> hashes;
protected:
    static void LogCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
public:
    static void LogValue(v8::Isolate* isolate, v8::Local<v8::Value> value);
    static void Mix(v8::Isolate *isolate, v8::Local<v8::Object> global);
};

#endif // CONSOLE_H
