#include "Game.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
  isRunning = false;
  Logger::Log("Game Constructor Called");
}

Game::~Game() { Logger::Log("Game Destructor Called"); }

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("Error initializing SDL.");
    return;
  }

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = 800;  // displayMode.w;
  windowHeight = 600; // displayMode.h;
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);

  if (!window) {
    Logger::Err("Error creating SDL window.");
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    Logger::Err("Error creating SDL rendered");
    return;
  }
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
        isRunning = false;
      }
      break;
    }
  }
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
  last = SDL_GetPerformanceCounter();
  frameStart = last;
  playerPosition = glm::vec2(10.0, 20.0);
  playerVelocity = glm::vec2(150.0, 120.0);
}

void Game::Update() {
  // High precision current time
  Uint64 now = SDL_GetPerformanceCounter();

  // Difference of ticks, in seconds, since last frame
  // think of pixels per seconds, all objects are moved by delta time
  double deltaTime = (double)(now - last) / SDL_GetPerformanceFrequency();

  // Update last frame time
  last = now;

  // Move player based on deltaTime
  playerPosition.x += playerVelocity.x * static_cast<float>(deltaTime);
  playerPosition.y += playerVelocity.y * static_cast<float>(deltaTime);

  // Calculate how long the frame has taken since frameStart (in milliseconds)
  double frameDurationMs = (SDL_GetPerformanceCounter() - frameStart) /
                           (double)SDL_GetPerformanceFrequency() * 1000.0;

  // Cap frame rate: if frame finished too fast, delay remaining time
  if (frameDurationMs < MILLISECS_PER_FRAME) {
    SDL_Delay(static_cast<Uint32>(MILLISECS_PER_FRAME - frameDurationMs));
  }

  // Reset frameStart for next frame iteration
  frameStart = SDL_GetPerformanceCounter();
}

void Game::Render() {
  // Draw background buffer
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Load png
  SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  // Destination Rectangle to place the texture on
  SDL_Rect dstRect = {static_cast<int>(playerPosition.x),
                      static_cast<int>(playerPosition.y), 32, 32};
  // we copy the whole texture (not a part of it - so NULL)
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);

  // Display all accumulating buffers from back to front
  SDL_RenderPresent(renderer);
}

void Game::Run() {
  Setup();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
