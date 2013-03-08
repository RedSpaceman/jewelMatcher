#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"
#include "SDL_image.h"

// Class of screen items that can be clicked
class Button
{
	private:
		// Position and dimensions of button stored in rectangle
		SDL_Rect box;
		SDL_Surface* clip;
	public:
		Button::Button();
		Button::Button( int x, int y, int width, int height );

		// Handle button events
		void handle_events(SDL_Event event);

		// Show button on screen
		void show();
};

#endif