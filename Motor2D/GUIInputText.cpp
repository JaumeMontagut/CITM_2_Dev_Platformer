#include "GUIInputText.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"

GUIInputText::GUIInputText(const iPoint& position, const SDL_Rect &bounds, const char* text, SDL_Color color, uint size) : GUIElement(position)
{
	if (text != nullptr)
	{
		texture = App->font->Print(text, color, NULL);
	}
}

bool GUIInputText::PreUpdate()
{
	return true;
}

bool GUIInputText::PostUpdate()
{
	if (texture != nullptr) {
		return App->render->BlitGUI(texture, localPos.x, localPos.y, NULL);
	}

	return false;
}