#ifndef FS_H
#define FS_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>

class FS
{
protected:
    static void ReadFileSyncCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void ReaddirSyncCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void StatSyncCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void IsDirectoryCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
public:
    static v8::Local<v8::String> LoadFile(v8::Isolate *isolate, v8::Local<v8::String> path);
    static v8::Local<v8::String> LoadFile(v8::Isolate *isolate, const char *path);
    static v8::Local<v8::String> LoadScript(v8::Isolate *isolate, v8::Local<v8::String> path);
    static v8::Local<v8::String> LoadScript(v8::Isolate *isolate, const char *path);
    static void Mix(v8::Isolate *isolate, v8::Local<v8::Object> global);
    static v8::Local<v8::Object> Require(v8::Isolate *isolate);
};

#endif // FS_H
