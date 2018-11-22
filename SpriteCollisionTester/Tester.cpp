#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

int SCREEN_WIDTH = 500;
int SCREEN_HEIGHT = 500;

using namespace std;

//Input Variables
string filePath = "Images\\Flipper.png";

//Program Variables
int mPitch;
int mWidth;
int mHeight;
SDL_Renderer* theRenderer = NULL;
SDL_Texture* theTexture;
SDL_Window* theWindow = NULL;
void* mPixels;

//Output Variables
bool** spritePixels;

int main(int argc, char* args[])
{
	/*
	===================================================
	- Creates a Basic Window and Renderer
	===================================================
	*/
	theWindow = SDL_CreateWindow("Sprite Collision Tester", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	theRenderer = SDL_CreateRenderer(theWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	/*
	===================================================
	- Loads up the Sprite for Manipulation
	===================================================
	*/
	//Load the sprite from the specified path
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
	//Convert surface to display format
	SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(theWindow), NULL);
	//Create blank streamable texture
	theTexture = SDL_CreateTexture(theRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);

	//Gets the sprite's dimensions
	mWidth = formattedSurface->w;
	mHeight = formattedSurface->h;

	//Locks the texture for manipulation
	SDL_LockTexture(theTexture, NULL, &mPixels, &mPitch);

	//Copy loaded/formatted surface pixels
	memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);


	/*
	===================================================
	- Gets the Pixel Information from the Loaded Sprite (RGBA Values)
	===================================================
	*/
	//Gets the pixel data
	Uint32* pixels = (Uint32*)mPixels;
	int pixelCount = (mWidth * mHeight);

	cout << "Width: " << mWidth << endl;
	cout << "Height: " << mHeight << endl;

	int transparent = 0, other = 0;

	//Creating 2D Array to store sprite details
	spritePixels = new bool*[mHeight];
	for (int i = 0; i < mHeight; i++)
	{
		spritePixels[i] = new bool[mWidth];
	}

	int h = 0, w = 0;

	for (int i = 0; i < pixelCount; ++i)
	{
		if (w >= mWidth)
		{
			w = 0;
			h++;
		}

		//Getting the pixel's RGBA information
		Uint8 r, g, b, a;
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
		SDL_GetRGBA(pixels[i], mappingFormat, &r, &g, &b, &a);

		//Converts the values to a number between 0 & 255
		int rValue = (int)r, gValue = (int)g, bValue = (int)b, aValue = (int)a;

		//Acting on what colour the pixel is
		if (aValue == 0) //Transparent pixels
		{
			transparent++;
			spritePixels[h][w] = false;
		}
		else
		{
			other++;
			spritePixels[h][w] = true;
		}
		w++;
	}

	//Displaying the number of each pixel colour
	std::cout << "Transparent: " << transparent << std::endl;
	std::cout << "Other: " << other << std::endl << std::endl;


	/*
	===================================================
	- Displaying the Collision Points in the Console
	===================================================
	*/
	for (int h = 0; h < mHeight; h++)
	{
		std::string s;

		for (int w = 0; w < mWidth; w++)
		{
			if (spritePixels[h][w] == true)
			{
				s += 'X';
			}
			else
			{
				s += '.';
			}
		}

		std::cout << s << std::endl;
	}


	/*
	===================================================
	- Program Cleanup
	===================================================
	*/
	//Free texture if it exists
	if (theTexture != NULL)
	{
		SDL_DestroyTexture(theTexture);
		theTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}

	//Destroy window	
	SDL_DestroyRenderer(theRenderer);
	SDL_DestroyWindow(theWindow);
	theWindow = NULL;
	theRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	//Pausing to see output
	cout << endl;
	system("Pause");
	return 0;
}