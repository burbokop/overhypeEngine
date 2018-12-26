#include "nodeapi.h"


v8::Local<v8::Context> NodeApi::NewContext(v8::Isolate* isolate) {
    auto context = v8::Context::New(isolate);
    if (context.IsEmpty()) return context;
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(context);

    return context;
}
