#ifndef _GUI_CHECKBOX_H
#define _GUI_CHECKBOX_H

#include "j1Gui.h"

class GUIImage;

class GUICheckbox : public GUIElement
{
public:
	//Constructors
	//Constructor for GUICheckbox. Leave text as nullptr to not have text and leave out_section as nullptr to not have image.
	GUICheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr = nullptr, const char * text = nullptr, const SDL_Rect * out_section = nullptr, const SDL_Rect * in_section = nullptr, const SDL_Rect * click_section = nullptr, const SDL_Rect * checkSection = nullptr, uint clickSFX = 0u);

	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

private:
	//General info
	bool clickable = true;
	bool *boolPtr = nullptr;//A pointer to the bool that's going to be changed
	GUIImage* childImage;
	uint clickSfx = 0u;
	//Sprites
	SDL_Rect* outSection = nullptr;
	SDL_Rect* inSection = nullptr;
	SDL_Rect* clickSection = nullptr;
	SDL_Rect* checkSection = nullptr;
	//SDL_Rect* disabledSection = nullptr;
};


#endif

