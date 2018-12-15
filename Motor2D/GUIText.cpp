#include "GUIText.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"

GUIText::GUIText(const iPoint& position, const char* text, SDL_Color color, _TTF_Font* font) : GUIElement(position)
{
	if (text != nullptr)
	{
		texture = App->font->Print(text, color, font);
	}
	interactable = false;
}

bool GUIText::PostUpdate()
{
	if (texture != nullptr) {
		//Iteration
		iPoint globalPos(0, 0);
		for (GUIElement * iterator = this; iterator != nullptr; iterator = iterator->parent) {
			globalPos += iterator->localPos;
		}

		return App->render->BlitGUI(texture, globalPos.x, globalPos.y, NULL);
	}

	return false;
}

void GUIText::SetText(SDL_Texture * texture)
{

}
