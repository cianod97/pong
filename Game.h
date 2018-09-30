#pragma once
#include <SDL2/SDL.h>

// 2D vector struct
struct Vector2D
{
    float x;
    float y;
};

class Game
{
public:
    //Constructor
    Game();
    //Setup subsystems and game
    bool Initialise();
    //Runs until game is completed
    void RunLoop();
    void Shutdown();
private:
    //Helper functions for RunLoop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    //SDL window
    SDL_Renderer * mRenderer;
    SDL_Window * mWindow;
    bool mIsRunning;

    Vector2D mPaddlePos;
    Vector2D mBallPos;
    int mPaddleDir;

    Uint32 mTicksCount;
};

