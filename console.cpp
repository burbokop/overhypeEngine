#include "console.h"


std::string Console::margin = "";
unsigned int Console::level = 0;
std::vector<int> Console::hashes;

//const char* ToCString(const v8::String::Utf8Value& value) {
//    return *value ? *value : "<string conversion failed>";
//}

bool isInArray(std::vector<int> array, int element) {
    for(std::size_t i = 0, l = array.size(); i < l; i++) {
        if(array[i] == element) return true;
    }
    return false;
}



void Console::LogValue(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    Console::margin += "-";
    Console::level++;

    //std::cout << "\nHASH:" << value->ToObject(isolate)->GetIdentityHash() << "\n";

    if(value->IsFunction()) {
        v8::String::Utf8Value valUtf8(isolate, value);
        std::cout << *valUtf8;
        if(level > 1) std::cout << "\n";
    }
    else if(value->IsObject()) {
        int objectHash = value->ToObject(isolate)->GetIdentityHash();

        if(isInArray(Console::hashes, objectHash)) {
            v8::String::Utf8Value valUtf8(isolate, value);
            std::cout << "[recursive] " << *valUtf8;
            if(level > 1) std::cout << "\n";
        } else {
            v8::Local<v8::Context> context = isolate->GetCurrentContext();
            v8::Local<v8::Object> objectValue = value->ToObject(context).ToLocalChecked();
            v8::Local<v8::Array> props = objectValue->GetOwnPropertyNames(context).ToLocalChecked();
            hashes.push_back(objectHash);
            v8::String::Utf8Value valueUtf8(isolate, value);
            std::cout << *valueUtf8 << " \u2B63\n";
            for(unsigned int i = 0, l = props->Length(); i < l; i++) {
                v8::Local<v8::Value> localKey = props->Get(i);
                v8::Local<v8::Value> localVal = objectValue->Get(context, localKey).ToLocalChecked();

                v8::String::Utf8Value keyUtf8(isolate, localKey);
                std::cout << margin;
                std::cout << *keyUtf8 << ": ";
                Console::LogValue(isolate, localVal);
            }
        }
    } else {
        v8::String::Utf8Value valUtf8(isolate, value);
        std::cout << *valUtf8;
        if(level > 1) std::cout << "\n";
    }
    Console::margin.pop_back();
    Console::level--;
}

void Console::LogCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate* isolate = args.GetIsolate();
    bool first = true;
    std::cout << COLOR_BLUE;
    for (int i = 0; i < args.Length(); i++) {
        v8::HandleScope handle_scope(isolate);
        if (first) {
            first = false;
        } else {
            printf(" ");
        }
        Console::LogValue(isolate, args[i]);
    }
    std::cout << COLOR_RESET << '\n';
    fflush(stdout);
}

void Console::Mix(v8::Isolate *isolate, v8::Local<v8::Object> global)
{
    v8::Handle<v8::Object> objectValue = v8::Object::New(isolate);
    v8::Handle<v8::String> objectkey = v8::String::NewFromUtf8(isolate, "console");
    global->Set(objectkey, objectValue);

    v8::Local<v8::FunctionTemplate> logTemplate = v8::FunctionTemplate::New(isolate,  Console::LogCallback);
    v8::Local<v8::Function> logFunction = logTemplate->GetFunction();
    objectValue->Set( v8::String::NewFromUtf8(isolate, "log"), logFunction);
}
