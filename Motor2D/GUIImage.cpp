#include "j1Gui.h"
#include "GUIImage.h"
#include "j1Render.h"
#include "j1App.h"

GUIImage::GUIImage(const iPoint & position, const SDL_Rect & section) : section(section), GUIElement(position)
{
	interactable = false;
}

bool GUIImage::PostUpdate() {

	//Iteration
	iPoint globalPos(0, 0);
	for (GUIElement * iterator = this; iterator != nullptr; iterator = iterator->parent) {
		globalPos += iterator->localPos;
	}

	return App->render->BlitGUI(App->gui->GetAtlas(), globalPos.x, globalPos.y, &section, 2);
}