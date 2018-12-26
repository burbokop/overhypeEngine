#include "fs.h"

v8::Local<v8::String> FS::LoadFile(v8::Isolate *isolate, v8::Local<v8::String> path) {
    v8::String::Utf8Value utfpath(isolate, path);
    std::ifstream ifile(utfpath.operator*());
    std::string string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    return v8::String::NewFromUtf8(isolate, string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
}

v8::Local<v8::String> FS::LoadFile(v8::Isolate *isolate, const char *path) {
    std::ifstream ifile(path);
    std::string string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    return v8::String::NewFromUtf8(isolate, string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
}

v8::Local<v8::String> FS::LoadScript(v8::Isolate *isolate, v8::Local<v8::String> path) {
    v8::String::Utf8Value utfpath(isolate, path);
    std::string fullPath(utfpath.operator*());
    fullPath += ".js";
    std::ifstream ifile(fullPath);
    std::string string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    return v8::String::NewFromUtf8(isolate, string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
}

v8::Local<v8::String> FS::LoadScript(v8::Isolate *isolate, const char *path) {
    std::string fullPath(path);
    fullPath += ".js";
    std::ifstream ifile(fullPath.c_str());
    std::string string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    return v8::String::NewFromUtf8(isolate, string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
}





void FS::ReadFileSyncCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Value> result;
    if(args[0]->IsString()) {
        result = FS::LoadFile(isolate, args[0]->ToString(isolate));
    } else {
        result = v8::Undefined(isolate);
    }
    args.GetReturnValue().Set(result);
}

#include <stdio.h>
#include <string.h>


void FS::ReaddirSyncCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::String::Utf8Value valutf8(isolate, args[0]);
    v8::Local<v8::Array> result = v8::Array::New(isolate);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(*valutf8)) != nullptr) {
        unsigned int i = 0;
        while ((ent = readdir (dir)) != nullptr) {
            if(strcmp(ent->d_name, "..") && strcmp(ent->d_name, ".")) {
                printf ("yes %s\n", ent->d_name);
                result->Set(i, v8::String::NewFromUtf8(isolate, ent->d_name));
                i++;
            } else {
                printf ("no  %s\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    args.GetReturnValue().Set(result);
}

void FS::StatSyncCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::String::Utf8Value valutf8(isolate, args[0]);
    v8::Local<v8::Object> result = v8::Object::New(isolate);


    bool isdir;
    struct stat st;
    if(stat(*valutf8, &st) == 0 && S_ISDIR(st.st_mode)) {
        //std::cout << *valutf8 << " is dir\n";
        isdir = true;
    } else {
        //std::cout << *valutf8 << " is file\n";
        isdir = false;
    }


    result->Set(v8::String::NewFromUtf8(isolate, "isdir"), v8::Boolean::New(isolate, isdir));


    v8::Local<v8::FunctionTemplate> isDirectoryTemplate = v8::FunctionTemplate::New(isolate,  FS::IsDirectoryCallback);
    v8::Local<v8::Function> isDirectoryFunction = isDirectoryTemplate->GetFunction();
    result->Set(v8::String::NewFromUtf8(isolate, "isDirectory"), isDirectoryFunction);

    args.GetReturnValue().Set(result);
}


void FS::IsDirectoryCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(args.This()->Get(v8::String::NewFromUtf8(isolate, "isdir")));
}

void FS::Mix(v8::Isolate *isolate, v8::Local<v8::Object> global)
{

}

v8::Local<v8::Object> FS::Require(v8::Isolate *isolate)
{
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::Object> exports = v8::Object::New(isolate);

    v8::Local<v8::FunctionTemplate> readFileTemplate = v8::FunctionTemplate::New(isolate,  FS::ReadFileSyncCallback);
    v8::Local<v8::Function> readFileFunction = readFileTemplate->GetFunction();
    exports->Set( v8::String::NewFromUtf8(isolate, "readFileSync"), readFileFunction);

    v8::Local<v8::FunctionTemplate> readdirSyncTemplate = v8::FunctionTemplate::New(isolate,  FS::ReaddirSyncCallback);
    v8::Local<v8::Function> readdirSyncFunction = readdirSyncTemplate->GetFunction();
    exports->Set( v8::String::NewFromUtf8(isolate, "readdirSync"), readdirSyncFunction);

    v8::Local<v8::FunctionTemplate> statSyncTemplate = v8::FunctionTemplate::New(isolate,  FS::StatSyncCallback);
    v8::Local<v8::Function> statSyncFunction = statSyncTemplate->GetFunction();
    exports->Set( v8::String::NewFromUtf8(isolate, "statSync"), statSyncFunction);

    return handle_scope.Escape(exports);
}

