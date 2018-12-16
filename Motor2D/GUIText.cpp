#include "GUIText.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"

GUIText::GUIText(const iPoint& centerPos, const char* text, SDL_Color color, int maxWidth, _TTF_Font* font) :
	color(color), font(font), GUIElement(centerPos)
{
	clippingRect.w = maxWidth;

	if (text != nullptr)
	{
		texture = App->font->Print(text, color, (uint32)clippingRect.w, font);
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
		/*clippingRect.x = globalPos.x;
		clippingRect.h = globalPos.y;
		clippingRect.h = 256;
		SDL_RenderSetViewport(App->render->renderer, &clippingRect);*/

		App->render->BlitGUI(texture, globalPos.x, globalPos.y, NULL);

		//SDL_Rect screenR = { 0,0, 1024,768 };
		//SDL_RenderSetViewport(App->render->renderer, &screenR); // NULL
		
		return true;
	}

	return false;
}

void GUIText::SetText(const char * text)
{
	texture = App->font->Print(text, color, (uint32)clippingRect.w,font);
}
