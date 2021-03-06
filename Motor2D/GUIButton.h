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
	void(*clickFunction)() = nullptr;	//A pointer to the function that's going to be called when we click the button
	uint* clickSfx = nullptr;//0u;
	uint* hoverSfx = nullptr;// 0u;
	//Child image
	GUIImage * childImage = nullptr;
	GUIText * childText = nullptr;
	int moveTextDown = 0;//The amount of pixels the text will move down when the button is clicked
	bool textMoved = false;
	//Sections
	SDL_Rect * outSection = nullptr;	//The sprite of the image when the mouse isn't inside the bounds
	SDL_Rect * inSection = nullptr;		//The sprite of the child image when the mouse is inside the bounds
	SDL_Rect * clickSection = nullptr;	//The sprite of the child image when the mouse is inside the bounds and clicks the button
	p2SString functionName;				//The name of the function that's going to be called from j1Gui::functionMap
	//SDL_Rect* disabledSection = nullptr;
//Methods
public:
	//Leave text as nullptr to not have text
	//Leave out_section as nullptr to not have image
	GUIButton(const iPoint & position, const SDL_Rect & bounds, p2SString functionName = "\0", const char * text = nullptr, const SDL_Rect * out_section = nullptr, const SDL_Rect * in_section = nullptr, const SDL_Rect * click_section = nullptr, uint clickSFX = 0u);
	GUIButton(const iPoint& position, ButtonTemplates& templateType, p2SString functionName = "\0", const char* text = nullptr);

	bool PreUpdate() override;
	bool CleanUp() override;
};

#endif