#include "path.h"


Path::Path()
{
    this->string = "";
}

Path::Path(char *string)
{
    this->string = string;
}

Path::Path(std::string string)
{
    this->string = string;
}

Path::Path(v8::Isolate *isolate, v8::Local<v8::String> string)
{
    this->string = *v8::String::Utf8Value(isolate, string);
}

void Path::Pop()
{
    if(this->string[this->string.size() - 1] == '/') this->string.pop_back();

    while(this->string[this->string.size() - 1] != '/') {
        this->string.pop_back();
    }
}

void Path::Apply()
{
    //ONLY FOR LONUX
    this->last = Path::Current();
    chdir(this->string.c_str());
}

void Path::Back()
{
    //ONLY FOR LONUX
    chdir(this->last);
}

std::string Path::Str()
{
    return this->string;
}

const char *Path::C_Str()
{
    return this->string.c_str();
}

v8::Local<v8::String> Path::V8_Str(v8::Isolate *isolate)
{
    return v8::String::NewFromUtf8(isolate, this->string.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
}

const char *Path::Current()
{
    char *dir = new char[256];
    getcwd(dir, 256);
    return dir;
}

std::string Path::Current_Str()
{
    char *dir = new char[256];
    getcwd(dir, 256);
    return std::string(dir);
}

std::string Path::InCurrent(v8::Isolate *isolate, v8::Local<v8::String> string)
{
    v8::String::Utf8Value utf(isolate, string);
    char *symlinkpath = *utf;
    char actualpath[4096];
    char *ptr;
    ptr = realpath(symlinkpath, actualpath);
    return actualpath;
}
