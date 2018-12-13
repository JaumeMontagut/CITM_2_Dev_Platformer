#include "GUIButton.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Input.h"
#include "SDL/include/SDL_mouse.h"
#include "j1Audio.h"
#include "p2Log.h"

//GUIButton::GUIButton(GUITemplate, text) {
//
//}

GUIButton::GUIButton(const iPoint & position, const SDL_Rect & bounds, void(*clickFunction)(), const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section, uint clickSfx) : clickFunction(clickFunction), clickSfx(clickSfx), GUIElement(position) {
	//Child image
	if (out_section != nullptr) {
		this->outSection = new SDL_Rect(out_section->x, out_section->y, out_section->w, out_section->h);
		childImage = App->gui->CreateImage(position, *out_section);//TODO: Take into account the adjustment
	}
	if (in_section != nullptr) {
		this->inSection = new SDL_Rect(in_section->x, in_section->y, in_section->w, in_section->h);
	}
	if (click_section != nullptr) {
		this->clickSection = new SDL_Rect(click_section->x, click_section->y, click_section->w, click_section->h);
	}
	//Child text
	if (text != nullptr) {
		App->gui->CreateText(position, text/*,(GUIElement)this*/);//TODO: Take into account the adjustment
	}
	this->bounds = bounds;
}

GUIButton::GUIButton(const iPoint& position, ButtonTemplates& templateType, void(*clickFunction)(), const char* text) : clickFunction(clickFunction), GUIElement(position)
{
	outSection = &templateType.sectionUp;
	childImage = App->gui->CreateImage(position, *outSection);
	inSection = &templateType.sectionHover;
	clickSection = &templateType.sectionDown;

	// assigns sfx
	clickSfx = templateType.clickSfx;
	hoverSfx = templateType.hoverSfx;
	
	if (text != nullptr)
		App->gui->CreateText(position, text, templateType.fontColor, templateType.font);

	bounds = templateType.bounds;

}

bool GUIButton::CleanUp() {
	RELEASE(outSection);
	RELEASE(inSection);
	RELEASE(clickSection);
	return true;
}

bool GUIButton::PreUpdate()
{
	if ((state == MOUSE_STATE::M_ENTER || state == MOUSE_STATE::M_IN) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
		if (clickSection != nullptr && childImage != nullptr && &childImage->section != clickSection) {
			childImage->section = *clickSection;
		}
		if (clickFunction != nullptr) {
			clickFunction();//Call function pointer
		}
		App->audio->PlayFx(clickSfx);
	}
	else if (state == MOUSE_STATE::M_ENTER || (state == MOUSE_STATE::M_IN && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)) {
		if (inSection != nullptr  && childImage != nullptr && &childImage->section != inSection) {
			childImage->section = *inSection;
		}
		if(!hoverSfxPlayed)
			App->audio->PlayFx(hoverSfx);
		hoverSfxPlayed = true;
	}
	else if (state == MOUSE_STATE::M_EXIT) {
		if (outSection != nullptr  && childImage != nullptr && &childImage->section != outSection) {
			childImage->section = *outSection;
		}
		hoverSfxPlayed = false;
	}

	return true;
}