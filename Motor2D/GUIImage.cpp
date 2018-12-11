#include "j1Gui.h"
#include "GUIImage.h"
#include "j1Render.h"
#include "j1App.h"

GUIImage::GUIImage(const iPoint & position, const SDL_Rect & section) : section(section), GUIElement(position)
{
}

bool GUIImage::PostUpdate() {
	return App->render->BlitGUI(App->gui->GetAtlas(), localPos.x, localPos.y, &section);
}