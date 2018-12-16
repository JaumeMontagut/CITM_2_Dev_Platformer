#ifndef _GUI_TEXT_
#define _GUI_TEXT_

struct SDL_Texture;
#include "p2Point.h"
#include "SDL/include/SDL_pixels.h"
#include "j1Gui.h"

class GUIText : public GUIElement
{
public:
	GUIText(const iPoint& position, const char* text, SDL_Color color, _TTF_Font* font = nullptr);

public:
	bool PostUpdate();
	void SetText(const char * text);

public:
	SDL_Texture * texture = nullptr;
};


#endif