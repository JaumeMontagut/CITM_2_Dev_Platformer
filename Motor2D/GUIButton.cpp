#include "GUIButton.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Input.h"
#include "SDL/include/SDL_mouse.h"
#include "j1Audio.h"

GUIButton::GUIButton(const iPoint & position, const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section) : GUIElement(position) {
	if (out_section != nullptr) {
		this->outSection = new SDL_Rect(out_section->x, out_section->y, out_section->w, out_section->h);
		this->inSection = new SDL_Rect(in_section->x, in_section->y, in_section->w, in_section->h);
		this->clickSection = new SDL_Rect(click_section->x, click_section->y, click_section->w, click_section->h);
		App->gui->CreateImage(position, *out_section);//TODO: Take into account the adjustment
	}
	if (text != nullptr) {
		App->gui->CreateText(position, text);//TODO: Take into account the adjustment
	}

}

bool GUIButton::CleanUp() {
	RELEASE(outSection);
	RELEASE(inSection);
	RELEASE(clickSection);
	return true;
}

bool GUIButton::PreUpdate()
{
	if (state == MOUSE_STATE::M_ENTER || state == MOUSE_STATE::M_IN) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			if (clickSection != nullptr && childImage != nullptr && &childImage->section != clickSection) {
				childImage->section = *clickSection;
			}
			if (clickFunction != nullptr) {
				clickFunction();//Call function pointer
			}
			App->audio->PlayFx(clickSfx);
		}
		else {
			if (inSection != nullptr  && childImage != nullptr && &childImage->section != inSection) {
				childImage->section = *inSection;
			}
		}
	}
	else {
		if (outSection != nullptr  && childImage != nullptr && &childImage->section != inSection) {
			childImage->section = *inSection;
		}
	}

	return true;
}