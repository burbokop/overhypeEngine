#ifndef OVERHYPECORE_H
#define OVERHYPECORE_H

#include </home/boris/Sketchbook/c++/v8-GN/v8/include/v8.h>
#include </home/boris/Sketchbook/c++/v8-GN/v8/include/libplatform/libplatform.h>

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class OverhypeCore {
public:
    static const Uint32 MAIN_COLOR;
    static const Uint32 MAIN_BG_COLOR;

private:
    static SDL_Window *window;
    static SDL_Surface *screen;
    static std::map<Uint32, SDL_Surface*> surfaces;
    static Uint32 nextSurfacesKey;
    static bool screenExist;

    static std::map<Uint32, TTF_Font*> fonts;
    static Uint32 nextFontsKey;
    static Uint32 activeFontsKey;

    static bool eventHandlerExitFlag;
    static bool *scancode;
    static bool pressed;
    static bool released;
    static v8::Local<v8::Function> eventCallback;
    static v8::Isolate *eventIsolate;

    static std::map<Uint32, Mix_Chunk*> musicList;
    static Uint32 nextMusicKey;


public:
    static int x;
    static int y;

    static void Init();

    static void Err(v8::Isolate* isolate, const char *message);
    
    static SDL_Window *GetWindow();
    static void SetWindow(SDL_Window *window);

    static Uint32 RegisterSurface (SDL_Surface *surface);
    static Uint32 RegisterSurface (SDL_Surface *surface, bool isScreen);
    static SDL_Surface *GetSurface(Uint32 id);
    static SDL_Surface *GetSurface(v8::Local<v8::Value> id);
    static SDL_Surface *GetScreen();
    static bool HasScreen();

    static Uint32 RegisterFont (TTF_Font *font);
    static void BindFont (Uint32 id);
    static TTF_Font *GetCurrentFont();

    static void Quit();

    static void HandleEvent();

    static bool GetKeyPressed();
    static bool GetKeyState(Uint32 code);
    static bool GetKeyReleased();

    static void SetEventCallback(v8::Isolate *isolate, v8::Local<v8::Function> callback);


    static Uint32 RegisterMusic (Mix_Chunk *music);
    static Mix_Chunk *GetMusic (Uint32 id);
    static Mix_Chunk *GetMusic (v8::Local<v8::Value> id);
};

#endif
