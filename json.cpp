#include "json.h"


void JSON::TransformObject(v8::Isolate *isolate, v8::Local<v8::Object> object, v8::Local<v8::Function> cb)
{
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Array> props = object->GetOwnPropertyNames(context).ToLocalChecked();
    for(unsigned int i = 0, l = props->Length(); i < l; i++) {
        v8::Local<v8::Value> localKey = props->Get(i);
        v8::Local<v8::Value> localVal = object->Get(context, localKey).ToLocalChecked();

        const unsigned argc = 2;
        v8::Local<v8::Value> argv[argc] = { localKey, localVal };
        v8::Local<v8::Value> newValue = cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
        object->Set(localKey, newValue);

        if(newValue->IsObject()) {
            JSON::TransformObject(isolate, newValue->ToObject(context).ToLocalChecked(), cb);
        }
    }
}

void JSON::ParseCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    v8::Local<v8::Value> result;
    if(args[0]->IsString()) {
        result = v8::JSON::Parse(context, args[0]->ToString(isolate)).ToLocalChecked();
        if(args[1]->IsFunction() && result->IsObject()) {
            v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[1]);
            v8::Local<v8::Object> object = result->ToObject(context).ToLocalChecked();
            JSON::TransformObject(isolate, object, cb);
        }
    } else {
        result = v8::Undefined(isolate);
    }
    args.GetReturnValue().Set(result);
}

void JSON::StringifyCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Value> result;
    if(!args[0]->IsUndefined()) {
        if(args[1]->IsFunction() && args[0]->IsObject()) {
            v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[1]);
            v8::Local<v8::Object> object = args[0]->ToObject(context).ToLocalChecked();
            JSON::TransformObject(isolate, object, cb);
        }
        result = v8::JSON::Stringify(isolate->GetCurrentContext(), args[0]).ToLocalChecked();
    } else {
        result = v8::Undefined(isolate);
    }
    args.GetReturnValue().Set(result);
}

void JSON::Mix(v8::Isolate *isolate, v8::Local<v8::Object> global)
{
    v8::Local<v8::Object> objectValue = v8::Object::New(isolate);
    v8::Local<v8::String> objectkey = v8::String::NewFromUtf8(isolate, "JSON");
    global->Set(objectkey, objectValue);

    v8::Local<v8::FunctionTemplate> parseTemplate = v8::FunctionTemplate::New(isolate,  JSON::ParseCallback);
    v8::Local<v8::Function> parseFunction = parseTemplate->GetFunction();
    objectValue->Set( v8::String::NewFromUtf8(isolate, "parse"), parseFunction);

    v8::Local<v8::FunctionTemplate> stringifyTemplate = v8::FunctionTemplate::New(isolate,  JSON::StringifyCallback);
    v8::Local<v8::Function> stringifyFunction = stringifyTemplate->GetFunction();
    objectValue->Set( v8::String::NewFromUtf8(isolate, "stringify"), stringifyFunction);
}
