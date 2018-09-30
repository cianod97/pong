#include "Game.h"

const int thickness = 15;
const int paddleH = 150.0f;

Game::Game():
    mRenderer(nullptr),
    mWindow(nullptr),
    mIsRunning(true),
	mTicksCount(0);
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

	// Can't initialise in constructor so initialise here
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 728.0f/2.0f;
	mBallPos.x = 1024.0f/2.0f;
	mBallPos.y = 728.0f/2.0f;

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
    SDL_SetRenderDrawColor(
        mRenderer,
        0,      //R
        128,      //G
        128,    //B
        255		//A
    );

	SDL_RenderClear(mRenderer);

	// Draw entire game scene here
	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);

	// Create and draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};

	SDL_RenderFillRect(mRenderer, &wall);

    // Draw bottom wall
    wall.y = 768 - thickness;

    SDL_RenderFillRect(mRenderer, &wall);

    // Draw RHS wall
    wall.y = 0;
    wall.x = 1024 - thickness;
    wall.w = thickness;
    wall.h = 1024;

    SDL_RenderFillRect(mRenderer, &wall);

    SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2),			// Top left x
		static_cast<int>(mBallPos.y - thickness/2),			// Top left y
		thickness,		// Width
		thickness	// Height
	};

    SDL_RenderFillRect(mRenderer, &ball);

    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH/2.0f),
        thickness,
        static_cast<int>(paddleH)
    };

    SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
