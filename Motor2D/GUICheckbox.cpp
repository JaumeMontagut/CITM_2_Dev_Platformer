#include "GUICheckbox.h"
#include "GUIButton.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Input.h"
#include "SDL/include/SDL_mouse.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Fonts.h"

GUICheckbox::GUICheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr, const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section, const SDL_Rect * checkSection, uint clickSfx) : boolPtr(boolPtr), clickSfx(clickSfx), GUIElement(position) {
	//Create child image
	if (out_section != nullptr) {
		this->outSection = new SDL_Rect(out_section->x, out_section->y, out_section->w, out_section->h);
		childImage = App->gui->CreateImage(position, *out_section, this);
	}
	if (out_section != nullptr) {
		this->inSection = new SDL_Rect(in_section->x, in_section->y, in_section->w, in_section->h);
	}
	if (out_section != nullptr) {
		this->clickSection = new SDL_Rect(click_section->x, click_section->y, click_section->w, click_section->h);
	}
	if (out_section != nullptr) {
		this->checkSection = new SDL_Rect(checkSection->x, checkSection->y, checkSection->w, checkSection->h);
	}
	//Create child Text
	if (text != nullptr) {
		App->gui->CreateText(position, text, WHITE, App->font->default, this);//TODO: Get even another parameter with the font
	}
	this->bounds = bounds;
}

bool GUICheckbox::PreUpdate()
{
	if ((state == MOUSE_STATE::M_ENTER || state == MOUSE_STATE::M_IN) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
		if (clickSection != nullptr && childImage != nullptr && &childImage->section != clickSection) {
			childImage->section = *clickSection;
		}
		if (boolPtr != nullptr) {
			*boolPtr = !(*boolPtr);
		}
		App->audio->PlayFx(clickSfx);
	}
	else if (state == MOUSE_STATE::M_ENTER || (state == MOUSE_STATE::M_IN && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)) {
		if (inSection != nullptr  && childImage != nullptr && &childImage->section != inSection) {
			childImage->section = *inSection;
		}
	}
	else if (state == MOUSE_STATE::M_EXIT) {
		if (outSection != nullptr  && childImage != nullptr && &childImage->section != outSection) {
			childImage->section = *outSection;
		}
	}
	return true;
}

bool GUICheckbox::PostUpdate()
{
	if (boolPtr != nullptr && *boolPtr && checkSection != nullptr) {
		return App->render->BlitGUI(App->gui->GetAtlas(), localPos.x, localPos.y, checkSection);
	}
	return false;
}

bool GUICheckbox::CleanUp()
{
	RELEASE(outSection);
	RELEASE(inSection);
	RELEASE(clickSection);
	RELEASE(checkSection);

	return true;
}
