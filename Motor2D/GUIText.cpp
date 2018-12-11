#include "GUIText.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"

GUIText::GUIText(const iPoint& position, const char* text, SDL_Color color) : GUIElement(position)
{
	if (text != nullptr)
	{
		texture = App->font->Print(text, color, NULL);
	}
}

bool GUIText::PostUpdate()
{
	if (texture != nullptr) {
		return App->render->BlitGUI(texture, localPos.x, localPos.y, NULL);
	}

	return false;
}