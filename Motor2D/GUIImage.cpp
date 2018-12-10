#include "j1Gui.h"
#include "GUIImage.h"
#include "j1Render.h"
#include "j1App.h"

GUIImage::GUIImage(const SDL_Rect & section, const iPoint & position) : section(section), GUIElement(position)
{
}

bool GUIImage::PostUpdate() {
	return App->render->BlitGUI(App->gui->GetAtlas(), localPos.x, localPos.y, &section);
}