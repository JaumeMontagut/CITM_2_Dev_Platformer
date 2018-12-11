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
	SDL_Rect * out_section = nullptr;	//The sprite of the image when the mouse isn't inside the bounds
	SDL_Rect * in_section = nullptr;	//The sprite of the child image when the mouse is inside the bounds
	SDL_Rect * click_section = nullptr;	//The sprite of the child image when the mouse is inside the bounds and clicks the button

//Methods
public:
	//Constructors
	//Only button (invisible)
	GUIButton(const iPoint & position);
	//Butttons that create a child image
	GUIButton(const iPoint & position, const SDL_Rect & out_tex);
	GUIButton(const iPoint & position, const SDL_Rect & out_tex, const SDL_Rect & in_tex, const SDL_Rect & click_tex);
	//Buttons that create a child text
	GUIButton(const iPoint & position, const char * text);
	//Button that creates a child image and text
	GUIButton(const iPoint & position, const SDL_Rect & out_tex, const char * text);
	GUIButton(const iPoint & position, const SDL_Rect & out_tex, const SDL_Rect & in_tex, const SDL_Rect & click_tex, const char * text);

	bool PreUpdate() override;
	bool CleanUp() override;
};

#endif