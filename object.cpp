#include "object.h"

#include <iostream>

void Object::AssignCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    //Object.assign(this, api.res.clone(key));
    v8::Isolate* isolate = args.GetIsolate();
    if(args[0]->IsObject() && args[1]->IsObject()) {
        std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++assign CALLED\n";

        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::Object> object = args[1]->ToObject(context).ToLocalChecked();
        v8::Local<v8::Array> props = object->GetOwnPropertyNames(context).ToLocalChecked();
        v8::Local<v8::Object> result = args[0]->ToObject(isolate);


        for(unsigned int i = 0, l = props->Length(); i < l; i++) {
            v8::Local<v8::Value> localKey = props->Get(i);
            v8::Local<v8::Value> localVal = object->Get(context, localKey).ToLocalChecked();
            std::cout << "[][][][][][][][][][][][][][][][][]assign: " << *v8::String::Utf8Value(isolate, localKey) << "\n";
            result->Set(localKey, localVal);
        }
    }
}

void Object::Mix(v8::Isolate *isolate, v8::Local<v8::Object> global)
{
    v8::Local<v8::Object> objectValue = v8::Object::New(isolate);
    v8::Local<v8::String> objectkey = v8::String::NewFromUtf8(isolate, "Object");
    global->Set(objectkey, objectValue);

    v8::Local<v8::FunctionTemplate> assignTemplate = v8::FunctionTemplate::New(isolate,  Object::AssignCallback);
    v8::Local<v8::Function> assignFunction = assignTemplate->GetFunction();
    objectValue->Set( v8::String::NewFromUtf8(isolate, "assign"), assignFunction);
}
