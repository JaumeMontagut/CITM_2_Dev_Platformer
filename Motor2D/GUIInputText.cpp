#include "GUIInputText.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"

GUIInputText::GUIInputText(const iPoint& position, const SDL_Rect &bounds, const char* text, SDL_Color color, uint size, const SDL_Rect * outSection = nullptr, const SDL_Rect * inSection = nullptr, GUIElement * parent = nullptr) : GUIElement(position)
{
	if (text != nullptr)
	{
		childText = App->gui->CreateText(position, text, color, bounds.w, nullptr, this);
	}

	//if () {

	//}
	interactable = true;
}

bool GUIInputText::PreUpdate()
{
	return true;
}