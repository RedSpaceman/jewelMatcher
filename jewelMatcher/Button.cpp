#include "Button.h"

#include "SDL.h"
#include "SDL_image.h"

Button::Button()
{
}

Button::Button( int x, int y, int width, int height )
{
	// Set the button's attributes
	box.x = x;
	box.y = y;
	box.w = width;
	box.h = height;
}

void Button::handle_events(SDL_Event event)
{
	// The mouse offsets
	int x = 0, y = 0;

	// If the mouse moved
	if( event.type == SDL_MOUSEMOTION )
	{
		// Get the mouse offsets
		x = event.motion.x;
		y = event.motion.y;

		// If the mouse is hovering over the button...
		if( ( x > box.x ) && ( x < box.x + box.w) && ( y > box.y ) && ( y < box.y + box.h ) )
		{
			// Set the image for mouse-over
		}
		else
		{
			// Set the default button sprite
		}
	}

}