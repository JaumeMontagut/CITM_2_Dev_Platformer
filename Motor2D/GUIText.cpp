#include "GUIText.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"

GUIText::GUIText(const iPoint& centerPos, const char* text, SDL_Color color, _TTF_Font* font) :
	color(color), font(font), GUIElement(centerPos)
{
	if (text != nullptr)
	{
		texture = App->font->Print(text, color, font);
		int texWidth, textHeight;
		SDL_QueryTexture(texture, NULL, NULL, &texWidth, &textHeight);
		localPos.x -= texWidth * 0.5f;
		localPos.y -= textHeight * 0.5f;
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

void GUIText::SetText(const char * text)
{
	texture = App->font->Print(text, color, font);
}
