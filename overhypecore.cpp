#include "overhypecore.h"

const Uint32 OverhypeCore::MAIN_COLOR = 0x052F6D;
const Uint32 OverhypeCore::MAIN_BG_COLOR = 0xC97878;




SDL_Window *OverhypeCore::window = nullptr;
SDL_Surface *OverhypeCore::screen = nullptr;
std::map<Uint32, SDL_Surface*> OverhypeCore::surfaces;
Uint32 OverhypeCore::nextSurfacesKey = 1;
bool OverhypeCore::screenExist = false;

std::map<Uint32, TTF_Font*> OverhypeCore::fonts;
Uint32 OverhypeCore::nextFontsKey = 0;
Uint32 OverhypeCore::activeFontsKey = 0;

bool OverhypeCore::eventHandlerExitFlag = false;
bool *OverhypeCore::scancode = new bool[256];
bool OverhypeCore::pressed = false;
bool OverhypeCore::released = false;
v8::Local<v8::Function> OverhypeCore::eventCallback;
v8::Isolate *OverhypeCore::eventIsolate = nullptr;

int OverhypeCore::x = 0;
int OverhypeCore::y = 0;


std::map<Uint32, Mix_Chunk*> OverhypeCore::musicList;
Uint32 OverhypeCore::nextMusicKey = 0;

void OverhypeCore::Init() {
    for(Uint32 i = 0; i < 256; i++) {
        OverhypeCore::scancode[i] = false;
    }
}

void OverhypeCore::Err(v8::Isolate* isolate, const char *message) {
    isolate->ThrowException(
        v8::Exception::TypeError(
            v8::String::NewFromUtf8(
            isolate,
            message,
            v8::NewStringType::kNormal
            ).ToLocalChecked()
        )
    );
}

SDL_Window *OverhypeCore::GetWindow() {
    return OverhypeCore::window;
}

void OverhypeCore::SetWindow(SDL_Window *window) {
    OverhypeCore::window = window;
}

Uint32 OverhypeCore::RegisterSurface(SDL_Surface *surface) {
    surfaces[nextSurfacesKey] = surface;
    nextSurfacesKey++;
    return nextSurfacesKey - 1;
}

Uint32 OverhypeCore::RegisterSurface (SDL_Surface *surface, bool isScreen) {
    if(isScreen && !OverhypeCore::screenExist) {
        OverhypeCore::screen = surface;
        OverhypeCore::screenExist = true;
        return 0;
    } else {
        OverhypeCore::surfaces[OverhypeCore::nextSurfacesKey] = surface;
        OverhypeCore::nextSurfacesKey++;
        return OverhypeCore::nextSurfacesKey - 1;
    }
}

SDL_Surface *OverhypeCore::GetSurface(Uint32 id) {
    if(id == 0) return OverhypeCore::screen;
    return OverhypeCore::surfaces[id];
}

SDL_Surface *OverhypeCore::GetSurface(v8::Local<v8::Value> value) {
    Uint32 id = value.As<v8::Uint32>()->Value();
    if(id == 0) return OverhypeCore::screen;
    return OverhypeCore::surfaces[id];
}

SDL_Surface *OverhypeCore::GetScreen() {
    return OverhypeCore::screen;
}

bool OverhypeCore::HasScreen() {
    return OverhypeCore::screenExist;
}

Uint32 OverhypeCore::RegisterFont (TTF_Font *font) {    
    OverhypeCore::fonts[OverhypeCore::nextFontsKey] = font;
    OverhypeCore::nextFontsKey++;
    return OverhypeCore::nextFontsKey - 1;
}

void OverhypeCore::BindFont (Uint32 id) {
    OverhypeCore::activeFontsKey = id;
}

TTF_Font *OverhypeCore::GetCurrentFont() {
    if(OverhypeCore::nextFontsKey == 0) return nullptr;
    return OverhypeCore::fonts[OverhypeCore::activeFontsKey];
}

void OverhypeCore::Quit() {
    OverhypeCore::eventHandlerExitFlag = true;
    for(std::pair<const unsigned int, SDL_Surface*> pair : OverhypeCore::surfaces) {
        SDL_FreeSurface(pair.second);
    }
    OverhypeCore::nextSurfacesKey = 0;
    OverhypeCore::screenExist = false;
    SDL_FreeSurface(OverhypeCore::screen);
    SDL_DestroyWindow(OverhypeCore::window);
}

void OverhypeCore::HandleEvent() {
    while (!OverhypeCore::eventHandlerExitFlag) {
        SDL_Event event;
        if (SDL_WaitEvent(&event))
        {
            //v8::Isolate* isolate = v8::Isolate::New(); //OverhypeCore::eventIsolate; //v8::Isolate::GetCurrent();
            //v8::Local<v8::Object> data = v8::Object::New(isolate);
            //exports->Set(v8::String::NewFromUtf8(isolate, "event"), event);

            if (event.type == SDL_QUIT)
            {
                //OverhypeCore::eventHandlerExitFlag = true;
                //emit exit;
            }
            else if(event.type == SDL_KEYDOWN) {
                OverhypeCore::pressed = true;
                OverhypeCore::scancode[event.key.keysym.scancode] = true;
                //emit down
                //std::cout << event.key.keysym.scancode;
            }
            else if(event.type == SDL_KEYUP) {
                OverhypeCore::released = true;
                OverhypeCore::scancode[event.key.keysym.scancode] = false;
                //emit up
            }
            else if(event.type == SDL_MOUSEMOTION) {
                //emit motion
                OverhypeCore::x = event.motion.x;
                OverhypeCore::y = event.motion.y;
            }

            //static Isolate * 	New ()
            //< Context > 	GetCurrent ()
            //isolate->GetCurrentContext()->Global()
            
            /*
            const unsigned argc = 0;
            v8::Local<v8::Value> argv[argc]; //= { v8::String::NewFromUtf8(isolate, "hello world"), v8::String::NewFromUtf8(isolate, "hello world") };

            OverhypeCore::eventCallback->Call(isolate->GetCurrentContext()->Global(), argc, argv);
            */
            //std::cout << OverhypeCore::pressed << OverhypeCore::released << "\n";
        }
    }
}



bool OverhypeCore::GetKeyPressed() {
    const bool tmp = OverhypeCore::pressed;
    OverhypeCore::pressed = false;
    return tmp;
}

bool OverhypeCore::GetKeyState(Uint32 code) {
    return OverhypeCore::scancode[code];
}

bool OverhypeCore::GetKeyReleased() {
    const bool tmp = OverhypeCore::released;
    OverhypeCore::released = false;
    return tmp;
}

void OverhypeCore::SetEventCallback(v8::Isolate *isolate, v8::Local<v8::Function> callback) {
    OverhypeCore::eventIsolate = isolate;
    OverhypeCore::eventCallback = callback;
}


Uint32 OverhypeCore::RegisterMusic (Mix_Chunk *music) {
    musicList[nextMusicKey] = music;
    nextMusicKey++;
    return nextMusicKey - 1;
}

Mix_Chunk *OverhypeCore::GetMusic (Uint32 id) {
    return OverhypeCore::musicList[id];
}

Mix_Chunk *OverhypeCore::GetMusic (v8::Local<v8::Value> value) {
    Uint32 id = value.As<v8::Uint32>()->Value();
    return OverhypeCore::musicList[id];
}
