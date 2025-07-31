#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>

// framerate caps to make consistent system
// framerate is for smoothness
const int FPS = 120;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool isRunning;
  SDL_Window *window;
  SDL_Renderer *renderer;
  int millisecsPreviousFrame;
  Uint64 last;
  Uint64 frameStart;

public:
  Game();
  ~Game();
  void Initialize();
  void Run();
  void Setup();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  int windowWidth;
  int windowHeight;
};

#endif
