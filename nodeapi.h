#ifndef NODEAPI_H
#define NODEAPI_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

class NodeApi
{
public:
    static v8::Local<v8::Context> NewContext(v8::Isolate *isolate);
};

#endif // NODEAPI_H
