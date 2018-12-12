#ifndef _INPUT_TEXT_H_
#define _INPUT_TEXT_H_

#include "j1Gui.h"

struct SDL_Texture;

class GUIInputText : public GUIElement {
	//Variables
public:
	SDL_Texture * texture = nullptr;
	//Methods
public:
	GUIInputText(const iPoint& position, const SDL_Rect &bounds, const char* text, SDL_Color color, uint size);
	bool PreUpdate() override;
	bool PostUpdate() override;

};

#endif // !_INPUT_TEXT_H_
