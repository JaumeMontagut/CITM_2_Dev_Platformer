#ifndef _GUI_IMAGE_
#define _GUI_IMAGE_

#include "j1Gui.h"
#include "SDL/include/SDL_rect.h"
#include "p2Point.h"

class GUIImage : public GUIElement // image class that supports optional text
{
public:
	SDL_Rect section;// rect of the target "atlas" texture
public:
	GUIImage(const iPoint & position, const SDL_Rect & section);
	bool PostUpdate();
};

//typedef Panel Image

#endif