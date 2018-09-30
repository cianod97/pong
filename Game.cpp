#include "Game.h"

const int thickness = 15;
const int paddleH = 150.0f;

Game::Game():
    mRenderer(nullptr),
    mWindow(nullptr),
    mIsRunning(true),
    mPaddleDir(0),
    mTicksCount(0)
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
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;

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

    // To determine direction of paddle
    // Add and subtract from mPaddleDir to ensure that direction is zero
    // if both buttons are pressed
    mPaddleDir = 0;
    if(state[SDL_SCANCODE_UP])
    {
        mPaddleDir -= 1;
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        mPaddleDir += 1;
    }
}

void Game::UpdateGame()
{
    while( !SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16) );
    // Delta time is diff in ticks from last frame
    // (converted into seconds)
    float DeltaTime = (SDL_GetTicks() - mTicksCount)/1000.0f;
    
    // Clamp maximum value of DeltaTime
    if(DeltaTime > 0.05f)
        DeltaTime = 0.05f;
    
    // Update ticks count for next frame
    mTicksCount = SDL_GetTicks();

    if(mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * DeltaTime;

        if(mPaddlePos.y < (paddleH/2.0f + thickness))
        {
            mPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if(mPaddlePos.y >  ((768.0f - (paddleH/2.0f + thickness))))
        {
            mPaddlePos.y = (768.0f - (paddleH/2.0f + thickness));
        }
    }

    mBallPos.x += mBallVel.x * DeltaTime; 
    mBallPos.y += mBallVel.y * DeltaTime;

    // To calculate the absolute difference between the ball and paddle y coordinate
    float diff = mBallPos.y - mPaddlePos.y;

    diff = (diff > 0.0f)?diff:-diff;

    if( 
        diff <= paddleH / 2.0f &&
        mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
        mBallVel.x < 0.0f 
        )
    {
        mBallVel.x *= -1;
    }
    else if(mBallPos.x < 0.0f)
    {
        mIsRunning = false;
    }
    else if( (1024.0f - mBallPos.x) <= thickness && mBallVel.x > 0.0f )
    {
        mBallVel.x *= -1;
    }

    // Did ball collide with bottom wall?
    if( ( 768.0f - mBallPos.y ) <= thickness && mBallVel.y > 0.0f)
    {
        mBallVel.y *= -1;
    }
    else if( (mBallPos.y <= thickness) && mBallVel.y < 0.0f )
    {
        mBallVel.y *= -1;
    }
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
