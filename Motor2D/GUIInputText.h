#ifndef _INPUT_TEXT_H_
#define _INPUT_TEXT_H_

#include "j1Gui.h"

struct SDL_Texture;

class GUIInputText : public GUIElement {
	//Variables
public:
	GUIImage * childImage;//The box image behind the input  text
	GUIText * childText;//The text that's going to be changing
	//Methods
public:
	GUIInputText(const iPoint& position, const SDL_Rect &bounds, const char* text, SDL_Color color, uint size, const SDL_Rect * outSection, const SDL_Rect * inSection, GUIElement * parent);
	bool PreUpdate() override;

};

#endif // !_INPUT_TEXT_H_
