#include "engine.h"


std::unique_ptr<v8::Platform> Engine::platform;
v8::Isolate::CreateParams Engine::create_params;
Path Engine::location;
int Engine::argc;
char **Engine::argv;
std::string Engine::runMargin = "";
std::map<std::string, v8::Local<v8::Value>> Engine::scriptsCache;
std::map<std::string, v8::Persistent<v8::Value>> Engine::persistentsCache;


std::vector<v8::Local<v8::Context>> Engine::tmp_contexts;
std::vector<v8::Context::Scope> Engine::tmp_contexScopes;

v8::Local<v8::Context> Engine::tmp_context;
v8::Local<v8::Object> Engine::tmp_global;



v8::Local<v8::Value> Engine::LoadModule(v8::Isolate *isolate, v8::Local<v8::String> module)
{
    v8::Local<v8::Value> result;
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if(module->Equals(context, v8::String::NewFromUtf8(isolate, "fs", v8::NewStringType::kNormal).ToLocalChecked()).ToChecked()) {
        result = FS::Require(isolate);
    } else if(module->Equals(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, "overhype", v8::NewStringType::kNormal).ToLocalChecked()).ToChecked()) {
        result = Overhype::Require(isolate);

        v8::Local<v8::Object> mathObject = v8::Object::New(isolate);
        result->ToObject(isolate)->Set(v8::String::NewFromUtf8(isolate, "math"), mathObject);

        std::cout << "require: overhype lib\n";
        std::cout << "run vector\n";
        v8::Local<v8::Value> vec = Engine::Run(isolate, Overhype::GetVectorSource(isolate));
        std::cout << COLOR_GREEN << Engine::runMargin << "cache: " << vec.operator->() << COLOR_RESET << "\n";
        mathObject->Set(v8::String::NewFromUtf8(isolate, "Vector"), vec);


        std::cout << "run timer\n";
        v8::Local<v8::Value> tim = Engine::Run(isolate, Overhype::GetTimerSource(isolate));
        std::cout << COLOR_GREEN << Engine::runMargin << "cache: " << tim.operator->() << COLOR_RESET << "\n";
        mathObject->Set(v8::String::NewFromUtf8(isolate, "Timer"), tim);


        std::cout << "run spead\n";
        v8::Local<v8::Value> spd = Engine::Run(isolate, Overhype::GetSpeadSource(isolate));
        std::cout << COLOR_GREEN << Engine::runMargin << "cache: " << spd.operator->() << COLOR_RESET << "\n";
        mathObject->Set(v8::String::NewFromUtf8(isolate, "Spead"), spd);
        std::cout << "overhype end\n";
    } else {

        std::cout << "require: " << *v8::String::Utf8Value(isolate, module->ToString(isolate)) << "\n";

        Path wtg(isolate, module->ToString(isolate));
        std::cout << "(_) path created: " << wtg.C_Str() << "\n";
        wtg.Pop();
        std::cout << "(_) path poped: " << wtg.C_Str() << "\n";
        v8::Local<v8::String> file = FS::LoadScript(isolate, module->ToString(isolate));
        wtg.Apply();
        std::cout << "(_) path applied: " << wtg.C_Str() << "\n";
        result = Engine::Run(isolate, file);
        wtg.Back();
        std::cout << "(_) path returned: " << wtg.C_Str() << "\n";
        std::cout << "module require end\n";
    }
    return result;
}

void DbLiteWeakCallback(v8::Persistent<v8::Value> object, void* parameter)
{
    std::cout << "void DbLiteWeakCallback(v8::Persistent<v8::Value> object, void* parameter)\n";
    //Console::LogValue(v8::Isolate::GetCurrent(), object.Get(v8::Isolate::GetCurrent()));
}


class DestroyParam {
public:
    double asyncId;
    v8::Persistent<v8::Value> target;
};

void WeakCallback(const v8::WeakCallbackInfo<DestroyParam>& info) {
    //v8::HandleScope scope(info.GetIsolate());
    //std::unique_ptr<DestroyParam> p{info.GetParameter()};
    std::cout << COLOR_BLUE << "void WeakCallback(const v8::WeakCallbackInfo<DestroyParam>& info)" << COLOR_RESET << "\n";
}

void Engine::RequireCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
    v8::Local<v8::Value> result;
    v8::Isolate* isolate = args.GetIsolate();
    std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----< require: " << *v8::String::Utf8Value(isolate, args[0]) << COLOR_RESET << "\n";
    if(args[0]->IsString()) {

        std::string scriptId = Path::InCurrent(isolate, args[0]->ToString(isolate));
        //std::string scriptId = *v8::String::Utf8Value(isolate, args[0]);

        if(Engine::scriptsCache.find(scriptId) != Engine::scriptsCache.end()) {
            std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----< script loaded from cache: " << COLOR_RESET << "\n";


            for(std::map<std::string, v8::Local<v8::Value>>::iterator it = Engine::scriptsCache.begin(); it != Engine::scriptsCache.end(); ++it) {
                std::cout << "#$%: " << it->first << "\n";
            }


            result = Engine::scriptsCache[scriptId];
            //result = Engine::persistentsCache[scriptId];

            std::cout << "gggg\n";

            std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----< debug: " << result.operator->() << COLOR_RESET << "\n";
            std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----< debug: " << *v8::String::Utf8Value(isolate, result) << COLOR_RESET << "\n";
            std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----< debug: " << *v8::String::Utf8Value(isolate, result->TypeOf(isolate)) << COLOR_RESET << "\n";
            Console::LogValue(isolate, result);


        } else {


            std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----< script started: " << COLOR_RESET << "\n";

            result = Engine::LoadModule(isolate, args[0]->ToString(isolate));



            DestroyParam* p = new DestroyParam();
            p->asyncId = result->ToObject(isolate)->GetIdentityHash();
            p->target.Reset(isolate, result);
            p->target.SetWeak(p, WeakCallback, v8::WeakCallbackType::kParameter);




            Engine::scriptsCache[scriptId] = result;
            //Engine::persistentsCache[scriptId] = p->target;
        }
    } else {
        result = v8::Undefined(isolate);
    }


    std::cout << COLOR_GREEN << Engine::runMargin << " ## >-----<" << COLOR_RESET << "\n";
    args.GetReturnValue().Set(result);
}

void Engine::ExitCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    if(args[0]->IsNumber()) {
        int code;
        bool result = args[0]->ToInt32(args.GetIsolate())->Int32Value(args.GetIsolate()->GetCurrentContext()).To(&code);
        if(result) {
            exit(code);
        }
    } else {
        exit(0);
    }
}

void Engine::GetGlobalCallback(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    args.GetReturnValue().Set(args.GetIsolate()->GetCurrentContext()->Global());
}

std::vector<std::string> split(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

std::vector<std::string> pop(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}


void MessageCallback (v8::Local<v8::Message> message, v8::Local<v8::Value> value) {
    std::cout << COLOR_BLUE << " MESSAGE: " << *v8::String::Utf8Value(v8::Isolate::GetCurrent(), message->Get()) << COLOR_RESET << "\n";
    std::cout << COLOR_BLUE << "   VALUE: " << *v8::String::Utf8Value(v8::Isolate::GetCurrent(), value) << COLOR_RESET << "\n";
}

void FatalErrorCallback(const char *arg0, const char *arg1) {
    std::cout << COLOR_BLUE << "FatalError: " << arg0 << " " << arg1 << "\n";
}


void cc(const char *arg0, int arg1, const char *arg2) {
    std::cout << "gogadoda v8::V8::SetDcheckErrorHandler(cc) called\nargs: " << arg0 << " " << arg1 << " " << arg2 << "\n";
}

v8::Isolate *Engine::Init(int argc, char *argv[])
{
    Engine::location = argv[0];
    Engine::location.Pop();
    Engine::argc = argc;
    Engine::argv = argv;

    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    Engine::platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(Engine::platform.get());
    v8::V8::Initialize();
    v8::V8::SetDcheckErrorHandler(cc);
    std::cout << "v8 version: " << v8::V8::GetVersion() << "\n";

    // Create a new Isolate and make it the current one.
    Engine::create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(Engine::create_params);

    isolate->AddMessageListener(MessageCallback);
    isolate->SetFatalErrorHandler(FatalErrorCallback);

    return isolate;
}

void Engine::Mix(v8::Isolate *isolate, v8::Local<v8::Object> global)
{
    v8::Local<v8::FunctionTemplate> requireFunctionTemplate = v8::FunctionTemplate::New(isolate, Engine::RequireCallback);
    v8::Local<v8::Function> requireFunction = requireFunctionTemplate->GetFunction();
    global->Set(v8::String::NewFromUtf8(isolate, "require"), requireFunction);
    v8::Handle<v8::Object> moduleObjectValue = v8::Object::New(isolate);
    v8::Handle<v8::String> moduleObjectkey = v8::String::NewFromUtf8(isolate, "module");
    global->Set(moduleObjectkey, moduleObjectValue);

    v8::Handle<v8::Object> exportsObjectValue = v8::Object::New(isolate);
    v8::Handle<v8::String> exportsObjectkey = v8::String::NewFromUtf8(isolate, "exports");
    moduleObjectValue->Set(exportsObjectkey, exportsObjectValue);

    v8::Handle<v8::String> dirnameKey = v8::String::NewFromUtf8(isolate, "__dirname");
    v8::Local<v8::String> dirnameValue = Engine::location.V8_Str(isolate);
    global->Set(dirnameKey, dirnameValue);

    v8::Handle<v8::Object> processObjectValue = v8::Object::New(isolate);
    v8::Handle<v8::String> processObjectkey = v8::String::NewFromUtf8(isolate, "process");
    global->Set(processObjectkey, processObjectValue);

    v8::Local<v8::String> argcKey = v8::String::NewFromUtf8(isolate, "argc");
    v8::Local<v8::Integer> argcValue = v8::Integer::New(isolate, Engine::argc);
    processObjectValue->Set(argcKey, argcValue);

    v8::Local<v8::String> argvKey = v8::String::NewFromUtf8(isolate, "argv");
    v8::Local<v8::Array> argvValue = v8::Array::New(isolate, Engine::argc);

    for(int i = 0; i < Engine::argc; i++) {
        argvValue->Set(static_cast<uint32_t>(i), v8::String::NewFromUtf8(isolate, Engine::argv[i]));
    }

    processObjectValue->Set(argvKey, argvValue);

    NODE_SET_METHOD(isolate, processObjectValue, "exit", Engine::ExitCallback);
    NODE_SET_METHOD(isolate, global, "getGlobal", Engine::GetGlobalCallback);
}


void Engine::RunEntryPoint(v8::Isolate *isolate, const char *source)
{
    Engine::runMargin += "e";
    std::cout << COLOR_GREEN << Engine::runMargin << " ## entry started" << COLOR_RESET << "\n";
    //init
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);

    //mixing
    v8::Local<v8::Object> global = context->Global();
    Object::     Mix(isolate, global);
    Console::    Mix(isolate, global);
    JSON::       Mix(isolate, global);
    TimeControl::Mix(isolate, global);
    Engine::     Mix(isolate, global);
    //running
    v8::Local<v8::Script> script = v8::Script::Compile(context, v8::String::NewFromUtf8(isolate, source, v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

    //test
    //v8::String::Utf8Value utf8(isolate, result);
    std::cout << COLOR_GREEN << Engine::runMargin << " ## skript ended: " << !result->IsUndefined() << COLOR_RESET << "\n";
    Engine::runMargin.pop_back();
}


v8::Local<v8::Value> Engine::Run(v8::Isolate *isolate, v8::Local<v8::String> source)
{
    Engine::runMargin += "=";
    std::cout << COLOR_GREEN << Engine::runMargin << " ## script started" << COLOR_RESET << "\n";
//init

    //v8::Local<v8::Context> context = isolate->GetCurrentContext();


    v8::EscapableHandleScope escapeble(isolate);
    v8::Local<v8::Context> context = v8::Context::New(isolate);
    context->Enter();
    v8::Context::Scope context_scope(context);


    //v8::Local<v8::Context> context = NodeApi::NewContext(isolate);
    //v8::Context::Scope context_scope(context);

//mixing
    v8::Local<v8::Object> global = context->Global();
    Object::Mix(isolate, global);
    Console::Mix(isolate, global);
    JSON::Mix(isolate, global);
    TimeControl::Mix(isolate, global);
    Engine::Mix(isolate, global);
//running
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
//test
    std::cout << COLOR_GREEN << Engine::runMargin << " ## script ended: " << !result->IsUndefined() << COLOR_RESET << "\n";
    Engine::runMargin.pop_back();
    context->Exit();
    return escapeble.Escape(result);
    //return result;
}


void Engine::Quit(v8::Isolate *isolate)
{
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete Engine::create_params.array_buffer_allocator;
}
