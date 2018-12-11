#ifndef _GUI_BUTTON_
#define _GUI_BUTTON_

#include "j1Gui.h"
#include "GUIImage.h"
#include "SDL/include/SDL_rect.h"

class GUIButton : public GUIElement
{
//Variables
private:
	//function pointer
	//Child image
	GUIImage * childImage = nullptr;
	SDL_Rect * outSection = nullptr;	//The sprite of the image when the mouse isn't inside the bounds
	SDL_Rect * inSection = nullptr;		//The sprite of the child image when the mouse is inside the bounds
	SDL_Rect * clickSection = nullptr;	//The sprite of the child image when the mouse is inside the bounds and clicks the button
	void(*clickFunction)() = nullptr;	//A pointer to the function that's going to be called when we click the button
	uint clickSfx = 0u;
//Methods
public:
	//Constructors
	//Only button (invisible)
	GUIButton(const iPoint & position);
	//Butttons that create a child image
	GUIButton(const iPoint & position, const SDL_Rect * out_tex, const SDL_Rect * in_tex, const SDL_Rect * click_tex);
	//Buttons that create a child text
	GUIButton(const iPoint & position, const char * text);
	//Button that creates a child image and text
	GUIButton(const iPoint & position, const char * text, const SDL_Rect * out_tex = nullptr, const SDL_Rect * in_tex = nullptr, const SDL_Rect * click_tex = nullptr);
	//TODO: Add parameters for the sfx

	bool PreUpdate() override;
	bool CleanUp() override;
};

#endif