#include "Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
  isRunning = false;
  registry = std::make_unique<Registry>();
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
  windowWidth = displayMode.w;
  windowHeight = displayMode.h;
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
  if (!window) {
    Logger::Err("Error creating SDL window.");
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    Logger::Err("Error creating SDL renderer.");
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

void Game::Setup() {
  // Create an entity
  Entity tank = registry->CreateEntity();

  // Add some components to that entity
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0),
                                        glm::vec2(1.0, 1.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 50.0));

  // Remove a component from the entity
  tank.RemoveComponent<TransformComponent>();

  last = SDL_GetPerformanceCounter();
  frameStart = last;
}

void Game::Update() {
  // High precision current time
  Uint64 now = SDL_GetPerformanceCounter();

  // Difference of ticks, in seconds, since last frame
  // think of pixels per seconds, all objects are moved by delta time
  double deltaTime = (double)(now - last) / SDL_GetPerformanceFrequency();

  // Update last frame time
  last = now;

  // TODO:
  // MovementSystem.Update();
  // CollisionSystem.Update(); ...
  // playerPosition.x += playerVelocity.x * static_cast<float>(deltaTime);

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

  // TODO: Render game objects...

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
