#include "overhype.h"





//OVERHYPE COMMON CALLBACKS

void InitCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Uint32 flags = SDL_INIT_EVERYTHING;
  if(args[0]->IsUint32()) {
    flags = args[0].As<v8::Uint32>()->Value();
  }

  if (SDL_Init(flags) < 0) {
    // Unrecoverable error, exit here.
    printf("SDL_Init failed: %s\n", SDL_GetError());
  }

  TTF_Init();
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  OverhypeCore::Init();
}

void DelayCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsUint32()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory )");
    return;
  }

  SDL_Delay(args[0].As<v8::Uint32>()->Value());
}


#include <iostream>
void CreateWindowCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsObject()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( obj obligatory )");
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Object> obj = args[0]->ToObject(context).ToLocalChecked();
  v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

  std::string title = "";
  long x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED, w = 0, h = 0;
  Uint32 flags = 0;

  bool isWidthInitialized = 0;
  bool isHeghtInitialized = 0;

  Uint32 j = 0;
  for(Uint32 i = 0, l = props->Length(); i < l; i++) {
    v8::Local<v8::Value> localKey = props->Get(i);
    v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
    std::string key = v8::String::Utf8Value(isolate, localKey).operator*();

    if(key == "title") { //const char*
      v8::String::Utf8Value valutf8(isolate, localVal);
      title = *valutf8;
      j++;
    } else if (key == "x") { //int
      localVal->IntegerValue(context).To(&x);
      j++;
    } else if (key == "y") { //int
      localVal->IntegerValue(context).To(&y);
      j++;
    } else if (key == "w") { //int
      localVal->IntegerValue(context).To(&w);
      isWidthInitialized = 1;
      j++;
    } else if (key == "h") { //int
      localVal->IntegerValue(context).To(&h);
      isHeghtInitialized = 1;
      j++;
    } else if (key == "flags") { //Uint32
      localVal->Uint32Value(context).To(&flags);
      j++;
    }

    if(j == 6) {
      break;
    }
  }

  if(!isWidthInitialized || !isHeghtInitialized) {
    OverhypeCore::Err(isolate, "Invalid object.\n\tRight args: ( { title: string,\n\t\tx: int,\n\t\ty: int,\n\t\tw: int obligatory,\n\t\th: int obligatory,\n\t\tflags: uint,\n\t} )");
    return;
  }

  std::cout << title.c_str() << " " << x << " " << y << " " << w << " " << h << " " << flags << "\n";

  OverhypeCore::SetWindow(SDL_CreateWindow(title.c_str(), x, y, w, h, flags));

  std::cout << OverhypeCore::GetWindow() << "\n";

  Uint32 surfaceId = OverhypeCore::RegisterSurface(SDL_GetWindowSurface(OverhypeCore::GetWindow()), true);

  v8::Local<v8::Integer> result = v8::Uint32::New(isolate, surfaceId);
  args.GetReturnValue().Set(result);
}

void FillRectCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsUint32()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, uint )");
    return;
  }
  Uint32 color = 0xC97878;
  if(args[1]->IsUint32()) {
    color = args[1].As<v8::Uint32>()->Value();
  }

  SDL_FillRect(OverhypeCore::GetSurface(args[0]) , nullptr, color);
}

void UpdateWindowSurfaceCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  SDL_UpdateWindowSurface(OverhypeCore::GetWindow());
}

void LoadFontCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsString()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( str obligatory, int )");
    return;
  }
  int size = 14;
  if(args[1]->IsInt32()) {
    size = args[1].As<v8::Integer>()->Value();
  }

  v8::String::Utf8Value str(isolate, args[0]);
  std::string path(*str);

  TTF_Font *font = TTF_OpenFont(path.c_str(), size);
  if(font == nullptr) OverhypeCore::Err(isolate, ("Can not find the file: " + path).c_str());

  Uint32 fontId = OverhypeCore::RegisterFont(font);
  v8::Local<v8::Integer> result = v8::Uint32::New(isolate, fontId);
  args.GetReturnValue().Set(result);
}

void BindFontCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsUint32()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory )");
    return;
  }
  OverhypeCore::BindFont(args[0].As<v8::Uint32>()->Value());
}

void LoadSurfaceCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsString()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( str obligatory )");
    return;
  }

  v8::String::Utf8Value str(isolate, args[0]);
  std::string path(*str);

  std::cout << "+++loadind       : " << path.c_str() << "\n";
  SDL_Surface *surface = IMG_Load(path.c_str());
  std::cout << "+++loadind result: " << surface << "\n";

  if(surface == nullptr) OverhypeCore::Err(isolate, ("Can not find the file: " + path).c_str());

  Uint32 surfaceId = OverhypeCore::RegisterSurface(surface);
  v8::Local<v8::Integer> result = v8::Uint32::New(isolate, surfaceId);
  args.GetReturnValue().Set(result);
}

void BlitSurfaceCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsUint32()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, obj, uint obligatory, obj )");
    return;
  }

  SDL_Rect rect1; rect1.x = 0; rect1.y = 0; rect1.w = 0; rect1.h = 0;
  bool rect1undef = false;
  if(args[1]->IsObject()) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> obj = args[1]->ToObject(context).ToLocalChecked();
    v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

    for(Uint32 i = 0, l = props->Length(); i < l; i++) {
      v8::Local<v8::Value> localKey = props->Get(i);
      v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
      std::string key = *v8::String::Utf8Value(isolate, localKey);

      if(key == "x") {
        localVal->IntegerValue(context).To((long*)&rect1.x);
      } else if (key == "y") { //int
        localVal->IntegerValue(context).To((long*)&rect1.y);
      } else if (key == "w") { //int
        localVal->IntegerValue(context).To((long*)&rect1.w);
      } else if (key == "h") { //int
        localVal->IntegerValue(context).To((long*)&rect1.h);
      }
    }
  } else {
    rect1undef = true;
  }

  if(!args[2]->IsUint32()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, obj, uint obligatory, obj )");
    return;
  }

  SDL_Rect rect2; rect2.x = 0; rect2.y = 0; rect2.w = 0; rect2.h = 0;
  bool rect2undef = false;
  if(args[3]->IsObject()) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> obj = args[3]->ToObject(context).ToLocalChecked();
    v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

    for(Uint32 i = 0, l = props->Length(); i < l; i++) {
      v8::Local<v8::Value> localKey = props->Get(i);
      v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
      std::string key = *v8::String::Utf8Value(isolate, localKey);

      if(key == "x") {
        localVal->IntegerValue(context).To((long*)&rect2.x);
      } else if (key == "y") { //int
        localVal->IntegerValue(context).To((long*)&rect2.y);
      } else if (key == "w") { //int
        localVal->IntegerValue(context).To((long*)&rect2.w);
      } else if (key == "h") { //int
        localVal->IntegerValue(context).To((long*)&rect2.h);
      }
    }
  } else {
    rect2undef = true;
  }

  SDL_BlitSurface(
    OverhypeCore::GetSurface(args[0]),
    rect1undef ? 0 : &rect1,
    OverhypeCore::GetSurface(args[2]),
    rect2undef ? 0 : &rect2
  );
}




void QuitCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    OverhypeCore::Quit();
    TTF_Quit();
    SDL_Quit();
}

void EventStartCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  std::thread *eventThread = new std::thread(OverhypeCore::HandleEvent);
}


void EventMouseCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "x"), v8::Integer::New(isolate, OverhypeCore::x));
  result->Set(v8::String::NewFromUtf8(isolate, "y"), v8::Integer::New(isolate, OverhypeCore::y));
  args.GetReturnValue().Set(result);
}

void EventPressedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Boolean> result = v8::Boolean::New(isolate, OverhypeCore::GetKeyPressed());
  args.GetReturnValue().Set(result);
}

void EventCodeCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsUint32()) {
    v8::Local<v8::Boolean> result = v8::Boolean::New(isolate, false);
    args.GetReturnValue().Set(result);
    return;
  }

  Uint32 code = args[0].As<v8::Uint32>()->Value();
  v8::Local<v8::Boolean> result = v8::Boolean::New(isolate, OverhypeCore::GetKeyState(code));
  args.GetReturnValue().Set(result);
}

void EventReleasedCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Boolean> result = v8::Boolean::New(isolate, OverhypeCore::GetKeyReleased());
  args.GetReturnValue().Set(result);
}

void EventGetCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  //if(!args[0].IsFunction()) {
  //  return;
  //}
  OverhypeCore::SetEventCallback(isolate, v8::Local<v8::Function>::Cast(args[0]));
}

void SoundLoadCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsString()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( str obligatory )");
    return;
  }

  v8::String::Utf8Value str(isolate, args[0]);
  std::string path(*str);

  Mix_Chunk *music = Mix_LoadWAV(path.c_str());
  if(music == 0) OverhypeCore::Err(isolate, ("Can not find the file: " + path).c_str());

  Uint32 musicId = OverhypeCore::RegisterMusic(music);
  v8::Local<v8::Integer> result = v8::Uint32::New(isolate, musicId);
  args.GetReturnValue().Set(result);
}

void SoundChannelCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "id"), v8::Integer::New(isolate, -1));
  args.GetReturnValue().Set(result);
}

void SoundVolumeCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if(!args[0]->IsObject()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( obj obligatory, int )");
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Object> obj = args[0]->ToObject(context).ToLocalChecked();
  v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

  int channel;
  for(Uint32 i = 0, l = props->Length(); i < l; i++) {
    v8::Local<v8::Value> localKey = props->Get(i);
    v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
    std::string key = *v8::String::Utf8Value(isolate, localKey);

    if(key == "id") { //const char*
      localVal->IntegerValue(context).To((long*)&channel);
      break;
    }
  }

  if(args[1]->IsNumber()) {
    Uint32 volume = args[1].As<v8::Integer>()->Value();
    Mix_Volume(channel, volume);
  }
}

void SoundPlayCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if(!args[0]->IsUint32()) {
      OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, obj obligatory, int )");
      return;
  }

  if(!args[1]->IsObject()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, obj obligatory, int )");
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Object> obj = args[1]->ToObject(context).ToLocalChecked();
  v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

  int channel;
  for(Uint32 i = 0, l = props->Length(); i < l; i++) {
    v8::Local<v8::Value> localKey = props->Get(i);
    v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
    std::string key = *v8::String::Utf8Value(isolate, localKey);

    if(key == "id") {
      localVal->IntegerValue(context).To((long*)&channel);
      break;
    }
  }

  int iterations = -1;
  if(args[2]->IsNumber()) {
      iterations = args[2].As<v8::Integer>()->Value();
  }

  channel = Mix_PlayChannel(channel, OverhypeCore::GetMusic(args[0]), iterations);

  obj->Set(v8::String::NewFromUtf8(isolate, "id"), v8::Integer::New(isolate, channel));
}

void SoundPlayingCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if(!args[0]->IsObject()) {
    OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( obj obligatory )");
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Object> obj = args[0]->ToObject(context).ToLocalChecked();
  v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

  int channel;
  for(Uint32 i = 0, l = props->Length(); i < l; i++) {
    v8::Local<v8::Value> localKey = props->Get(i);
    v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
    std::string key = *v8::String::Utf8Value(isolate, localKey);

    if(key == "id") { //const char*
      localVal->IntegerValue(context).To((long*)&channel);
      break;
    }
  }

  v8::Local<v8::Boolean> result = v8::Boolean::New(isolate, Mix_Playing(channel));
  args.GetReturnValue().Set(result);
}





//OVERHYPE SPM CALLBACKS


void SPMPixelCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, uint )");
        return;
    }
    int x = 0;
    if(args[1]->IsNumber()) {
        x = args[1].As<v8::Integer>()->Value();
    }
    int y = 0;
    if(args[2]->IsNumber()) {
        y = args[2].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[3]->IsUint32()) {
        color = args[3].As<v8::Uint32>()->Value();
    }

    SPM::FillPixel(OverhypeCore::GetSurface(args[0]), x, y, color);
}

void SPMLineCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, num, num, uint )");
        return;
    }
    int x1 = 0;
    if(args[1]->IsNumber()) {
        x1 = args[1].As<v8::Integer>()->Value();
    }
    int y1 = 0;
    if(args[2]->IsNumber()) {
        y1 = args[2].As<v8::Integer>()->Value();
    }
    int x2 = 0;
    if(args[3]->IsNumber()) {
        x2 = args[3].As<v8::Integer>()->Value();
    }
    int y2 = 0;
    if(args[4]->IsNumber()) {
        y2 = args[4].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[5]->IsUint32()) {
        color = args[5].As<v8::Uint32>()->Value();
    }
    if(x1 == x2 && y1 == y2 && x1 == y1) {
        SPM::FillPixel(OverhypeCore::GetSurface(args[0]), 0, 0, color);
    } else if(x1 == x2) {
        SPM::VerticalLine(OverhypeCore::GetSurface(args[0]), x1, y1, y2 - y1, color);
    } else if(y1 == y2) {
        SPM::HorizontalLine(OverhypeCore::GetSurface(args[0]), x1, y1, x2 - x1, color);
    } else {
        SPM::Line(OverhypeCore::GetSurface(args[0]), x1, y1, x2, y2, color);
    }
}

void SPMRectCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, num, num, uint )");
        return;
    }
    int x1 = 0;
    if(args[1]->IsNumber()) {
        x1 = args[1].As<v8::Integer>()->Value();
    }
    int y1 = 0;
    if(args[2]->IsNumber()) {
        y1 = args[2].As<v8::Integer>()->Value();
    }
    int x2 = 0;
    if(args[3]->IsNumber()) {
        x2 = args[3].As<v8::Integer>()->Value();
    }
    int y2 = 0;
    if(args[4]->IsNumber()) {
        y2 = args[4].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[5]->IsUint32()) {
        color = args[5].As<v8::Uint32>()->Value();
    }
    SPM::Rect(OverhypeCore::GetSurface(args[0]), x1, y1, x2, y2, color);
}

void SPMSquareCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, num, uint )");
        return;
    }
    int x = 0;
    if(args[1]->IsNumber()) {
        x = args[1].As<v8::Integer>()->Value();
    }
    int y = 0;
    if(args[2]->IsNumber()) {
        y = args[2].As<v8::Integer>()->Value();
    }
    int l = 0;
    if(args[3]->IsNumber()) {
        l = args[3].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[5]->IsUint32()) {
        color = args[5].As<v8::Uint32>()->Value();
    }
    SPM::Square(OverhypeCore::GetSurface(args[0]), x, y, l, color);
}


void SPMCircleCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, num, uint )");
        return;
    }
    int x = 0;
    if(args[1]->IsNumber()) {
        x = args[1].As<v8::Integer>()->Value();
    }
    int y = 0;
    if(args[2]->IsNumber()) {
        y = args[2].As<v8::Integer>()->Value();
    }
    int r = 0;
    if(args[3]->IsNumber()) {
        r = args[3].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[5]->IsUint32()) {
        color = args[5].As<v8::Uint32>()->Value();
    }
    SPM::Circle(OverhypeCore::GetSurface(args[0]), x, y, r, color);
}

void SPMGridCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, num, num, num, uint )");
        return;
    }
    int x1 = 0;
    if(args[1]->IsNumber()) {
        x1 = args[1].As<v8::Integer>()->Value();
    }
    int y1 = 0;
    if(args[2]->IsNumber()) {
        y1 = args[2].As<v8::Integer>()->Value();
    }
    int x2 = 0;
    if(args[3]->IsNumber()) {
        x2 = args[3].As<v8::Integer>()->Value();
    }
    int y2 = 0;
    if(args[4]->IsNumber()) {
        y2 = args[4].As<v8::Integer>()->Value();
    }
    int i = 0;
    if(args[5]->IsNumber()) {
        i = args[5].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[6]->IsUint32()) {
        color = args[6].As<v8::Uint32>()->Value();
    }
    SPM::Grid(OverhypeCore::GetSurface(args[0]), x1, y1, x2, y2, i, color);
}

void SPMBlendedTextCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, str obligatory, num, num, uint )");
        return;
    }
    if(!args[1]->IsString()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, str obligatory, num, num, uint )");
        return;
    }
    int x = 0;
    if(args[2]->IsNumber()) {
        x = args[2].As<v8::Integer>()->Value();
    }
    int y = 0;
    if(args[3]->IsNumber()) {
        y = args[3].As<v8::Integer>()->Value();
    }
    Uint32 color = OverhypeCore::MAIN_COLOR;
    if(args[4]->IsUint32()) {
        color = args[4].As<v8::Uint32>()->Value();
    }
    v8::String::Utf8Value str(isolate, args[1]);
    std::string text(*str);

    SPM::BlendedText(OverhypeCore::GetSurface(args[0]), text, OverhypeCore::GetCurrentFont(), x, y, color);
}



void SPMRotatedSurfaceCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {

    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, uint obligatory, obj, int, num, int )");
        return;
    }

    if(!args[1]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, uint obligatory, obj, int, num, int )");
        return;
    }

    long x = 0, y = 0;
    if(args[2]->IsObject()) {
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::Object> obj = args[2]->ToObject(context).ToLocalChecked();
        v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

        for(Uint32 i = 0, l = props->Length(); i < l; i++) {
            v8::Local<v8::Value> localKey = props->Get(i);
            v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
            std::string key = *v8::String::Utf8Value(isolate, localKey);

            if(key == "x") {
                localVal->IntegerValue(context).To(&x);
            } else if (key == "y") { //int
                localVal->IntegerValue(context).To(&y);
            }
        }
    }

    int angle = 0;
    if(args[3]->IsNumber()) {
        angle = args[3].As<v8::Integer>()->Value();
    }

    double zoom = 1;
    if(args[4]->IsNumber()) {
        zoom = args[4].As<v8::Number>()->Value();
    }

    int smooth = 1;
    if(args[5]->IsNumber()) {
        smooth = args[5].As<v8::Integer>()->Value();
    }


    SPM::BlitRotatedSurface(
        OverhypeCore::GetSurface(args[0]),
        OverhypeCore::GetSurface(args[1]),
        x, y,
        angle,
        zoom,
        smooth
    );
}


void SPMSliceCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, num, num, num, num )");
        return;
    }
    int x = 0;
    if(args[1]->IsNumber()) {
        x = args[1].As<v8::Integer>()->Value();
    }
    int y = 0;
    if(args[2]->IsNumber()) {
        y = args[2].As<v8::Integer>()->Value();
    }
    int w = 0;
    if(args[3]->IsNumber()) {
        w = args[3].As<v8::Integer>()->Value();
    }
    int h = 0;
    if(args[4]->IsNumber()) {
        h = args[4].As<v8::Integer>()->Value();
    }

    Uint32 surfaceId = OverhypeCore::RegisterSurface(SPM::CutOutSurface(OverhypeCore::GetSurface(args[0]), x, y, w, h));
    v8::Local<v8::Integer> result = v8::Uint32::New(isolate, surfaceId);
    args.GetReturnValue().Set(result);
}

void SPMFlipCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if(!args[0]->IsUint32()) {
        OverhypeCore::Err(isolate, "Invalid arguments.\n\tRight args: ( uint obligatory, bool, bool )");
        return;
    }
    bool x = 1;
    if(args[1]->IsNumber()) {
        x = args[1].As<v8::Integer>()->Value();
    }
    bool y = 1;
    if(args[2]->IsNumber()) {
        y = args[2].As<v8::Integer>()->Value();
    }

    Uint32 surfaceId = OverhypeCore::RegisterSurface(SPM::Flip(OverhypeCore::GetSurface(args[0]), x, y));
    v8::Local<v8::Integer> result = v8::Uint32::New(isolate, surfaceId);
    args.GetReturnValue().Set(result);
}


//OVERHYPE UTIL CALLBACKS

void StructureCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {

}



v8::Local<v8::Object> Overhype::Require(v8::Isolate *isolate)
{
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::Object> exports = v8::Object::New(isolate);

    //OVERHYPE COMMON REGISTER
    NODE_SET_METHOD(isolate, exports, "init", InitCallback);
    NODE_SET_METHOD(isolate, exports, "delay", DelayCallback);
    NODE_SET_METHOD(isolate, exports, "createWindow", CreateWindowCallback);
    NODE_SET_METHOD(isolate, exports, "fillRect", FillRectCallback);
    NODE_SET_METHOD(isolate, exports, "updateWindowSurface", UpdateWindowSurfaceCallback);
    NODE_SET_METHOD(isolate, exports, "loadFont", LoadFontCallback);
    NODE_SET_METHOD(isolate, exports, "bindFont", BindFontCallback);
    NODE_SET_METHOD(isolate, exports, "loadSurface", LoadSurfaceCallback);
    NODE_SET_METHOD(isolate, exports, "blitSurface", BlitSurfaceCallback);
    NODE_SET_METHOD(isolate, exports, "quit", QuitCallback);

    v8::Local<v8::Object> event = v8::Object::New(isolate);
    exports->Set(v8::String::NewFromUtf8(isolate, "event"), event);

    NODE_SET_METHOD(isolate, event, "start", EventStartCallback);
    NODE_SET_METHOD(isolate, event, "mouse", EventMouseCallback);
    NODE_SET_METHOD(isolate, event, "pressed", EventPressedCallback);
    NODE_SET_METHOD(isolate, event, "code", EventCodeCallback);
    NODE_SET_METHOD(isolate, event, "released", EventReleasedCallback);
    NODE_SET_METHOD(isolate, event, "get", EventGetCallback);

    v8::Local<v8::Object> sound = v8::Object::New(isolate);
    exports->Set(v8::String::NewFromUtf8(isolate, "sound"), sound);

    NODE_SET_METHOD(isolate, sound, "load", SoundLoadCallback);
    NODE_SET_METHOD(isolate, sound, "channel", SoundChannelCallback);
    NODE_SET_METHOD(isolate, sound, "volume", SoundVolumeCallback);
    NODE_SET_METHOD(isolate, sound, "play", SoundPlayCallback);
    NODE_SET_METHOD(isolate, sound, "playing", SoundPlayingCallback);


    //OVERHYPE SPM REGISTER


    v8::Local<v8::Object> spm = v8::Object::New(isolate);
    exports->Set(v8::String::NewFromUtf8(isolate, "spm"), spm);

    NODE_SET_METHOD(isolate, spm, "pixel", SPMPixelCallback);
    NODE_SET_METHOD(isolate, spm, "line", SPMLineCallback);
    NODE_SET_METHOD(isolate, spm, "rect", SPMRectCallback);
    NODE_SET_METHOD(isolate, spm, "square", SPMSquareCallback);
    NODE_SET_METHOD(isolate, spm, "circle", SPMCircleCallback);
    NODE_SET_METHOD(isolate, spm, "grid", SPMGridCallback);
    NODE_SET_METHOD(isolate, spm, "blendedText", SPMBlendedTextCallback);
    NODE_SET_METHOD(isolate, spm, "rotatedSurface", SPMRotatedSurfaceCallback);
    NODE_SET_METHOD(isolate, spm, "slice", SPMSliceCallback);
    NODE_SET_METHOD(isolate, spm, "flip", SPMFlipCallback);


    //OVERHYPE UTIL CALLBACKS

    NODE_SET_METHOD(isolate, exports, "structure", StructureCallback);

    return handle_scope.Escape(exports);
}

v8::Local<v8::String> Overhype::GetVectorSource(v8::Isolate *isolate)
{
    return v8::String::NewFromUtf8(isolate, " \
        'use strict'; \
        class Vector { \
           constructor(...args) { \
               this.coords = []; \
               for(const val of args) { \
                   this.coords.push(val || 0); \
               } \
           } \
        \
           add(term) { \
               const length = this.coords.length; \
               const result = new Vector(); \
               for(let i = 0; i < length; i++) { \
                   if(this.coords[i] === undefined) this.coords[i] = 0; \
                   if(term.coords[i] === undefined) term.coords[i] = 0; \
                   result.coords[i] = this.coords[i] + term.coords[i]; \
               } \
               return result; \
           } \
        \
           subtract(subtrahend) { \
               const length = this.coords.length; \
               const result = new Vector(); \
               for(let i = 0; i < length; i++) { \
                   if(this.coords[i] === undefined) this.coords[i] = 0; \
                   if(subtrahend.coords[i] === undefined) subtrahend.coords[i] = 0; \
                   result.coords[i] = this.coords[i] - subtrahend.coords[i]; \
               } \
               return result; \
           } \
        \
           multiply(multiplier) { \
               const length = this.coords.length; \
               const result = new Vector(); \
               for(let i = 0; i < length; i++) { \
                   if(this.coords[i] === undefined) this.coords[i] = 0; \
                   if(multiplier === undefined) multiplier = 0; \
                   result.coords[i] = this.coords[i] * multiplier; \
               } \
               return result; \
           } \
        \
           module() { \
               const length = this.coords.length; \
               let result = 0; \
               for(let i = 0; i < length; i++) { \
                   result += this.coords[i] * this.coords[i]; \
               } \
               return Math.sqrt(result); \
           } \
        \
           normalized() { \
               const length = this.coords.length; \
               const mod = this.module(); \
               const result = new Vector(); \
               for(let i = 0; i < length; i++) { \
                   result.coords[i] = this.coords[i] / mod; \
               } \
               return result; \
           } \
        \
           scalarProduct(term) { \
               const length = this.coords.length; \
               let result = 0; \
               for(let i = 0; i < length; i++) { \
                   if(this.coords[i] === undefined) this.coords[i] = 0; \
                   if(term.coords[i] === undefined) term.coords[i] = 0; \
                   result += (this.coords[i] * term.coords[i]); \
               } \
               return result; \
           } \
        \
           relativisticAddition(term, c) { \
               if(c !== undefined ) { \
                   const termLength = term.module(); \
                   if(termLength !== 0) { \
                       const origin = this.add(term); \
                       const originLength = origin.module(); \
        \
                       const k = Math.sqrt(1 + this.module() * termLength / (c * c)); \
                       const u = originLength / k; \
        \
                       const result = new Vector(); \
                       const length = this.coords.length; \
                       for(let i = 0; i < length; i++) { \
                           result.coords[i] = u * origin.coords[i] / originLength; \
                       } \
                       return result; \
                   } \
                   return this; \
               } \
               return this.add(term); \
           } \
        \
           static createByAngle(length, angle) { \
               return new Vector(length * Math.cos(angle), length * Math.sin(angle)); \
           } \
        \
           angle() { \
               const product = this.normalized().scalarProduct(new Vector(1, 0)); \
               if(this.coords[1] >= 0) return Math.acos(product); \
               else return -Math.acos(product); \
           }\
        \
           set x(value) { this.coords[0] = value; } \
           set y(value) { this.coords[1] = value; } \
           set z(value) { this.coords[2] = value; } \
           set t(value) { this.coords[3] = value; } \
        \
           get x() { return this.coords[0]; } \
           get y() { return this.coords[1]; } \
           get z() { return this.coords[2]; } \
           get t() { return this.coords[3]; } \
        } \
        \
        \
        module.exports = Vector; \
    ");
}

v8::Local<v8::String> Overhype::GetTimerSource(v8::Isolate *isolate)
{
    return v8::String::NewFromUtf8(isolate, " \
        'use strict'; \
         \
         \
        class Timer { \
            constructor(interval) { \
                this.interval = interval; \
                this.duration = 0; \
            } \
         \
            count(pressed) { \
                pressed === undefined ? pressed = true : {}; \
                if (this.timePassed() > this.interval && pressed) { \
                    this.duration = this.time(); \
                    return true; \
                } \
                else { \
                    return false; \
                } \
            } \
         \
            time() { \
                const date = new Date(); \
                return date.getTime(); \
            } \
         \
            timePassed() { \
                return this.time() - this.duration; \
            } \
        } \
         \
         \
        module.exports = Timer; \
    ");
}

v8::Local<v8::String> Overhype::GetSpeadSource(v8::Isolate *isolate)
{
    return v8::String::NewFromUtf8(isolate, " \
        'use strict'; \
         \
         \
        class Spead { \
            constructor() { \
                this.i = 0; \
            } \
         \
            record() { \
                const value = process.hrtime.bigint(); \
         \
                let result; \
                if(this.value !== undefined) result = value - this.value; \
                else { \
                    result = 0; \
                    this.value = value; \
                } \
         \
                return result; \
            } \
         \
            log() { \
                console.log('record', this.i,':', this.record()); \
                this.i++; \
            } \
        } \
         \
         \
        module.exports = Spead; \
    ");
}


