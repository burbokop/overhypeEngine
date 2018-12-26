#ifndef PATH_H
#define PATH_H

#include <string>
#include <unistd.h>
#include <stdlib.h>

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>


class Path
{
private:
    std::string string;
    const char *last;
public:
    Path();
    Path(char *string);
    Path(std::string string);
    Path(v8::Isolate *isolate, v8::Local<v8::String> string);

    void Pop();
    void Apply();
    void Back();

    std::string Str();
    const char *C_Str();
    v8::Local<v8::String> V8_Str(v8::Isolate *isolate);

    static const char *Current();
    static std::string Current_Str();

    static std::string InCurrent(v8::Isolate *isolate, v8::Local<v8::String> string);
};

#endif // PATH_H
