#include "Game.h"

Game::Game():
    mRenderer(nullptr),
    mWindow(nullptr),
    mIsRunning(true)
    {}

bool Game::Initialise()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    if(sdlResult != 0)
    {
        SDL_Log("Failed to initialise SDL subsystems: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "MMKO",
        100,    //Top left x-coordinate of window
        100,    //Top left y-coordinate of window
        1024,   //Width of window
        768,    //Height of window
        SDL_WINDOW_RESIZABLE      //Flags (0 = no flags)
    );

    if(!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,    //Window
        -1,         //Index
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC    //Flags
    );

    if(!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}
void Game::RunLoop()
{
    while(mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    //While there are still events queued
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            //handle different events here
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    //Get keyboard state
    const Uint8 * state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
}

void Game::UpdateGame()
{

}

void Game::GenerateOutput()
{

}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
