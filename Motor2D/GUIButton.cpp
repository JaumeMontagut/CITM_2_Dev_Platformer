#include "GUIButton.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Input.h"
#include "SDL/include/SDL_mouse.h"

GUIButton::GUIButton(const iPoint & position, const SDL_Rect & out_section, const SDL_Rect & in_section, const SDL_Rect & click_section) : GUIElement(position) {
	this->out_section = new SDL_Rect(out_section.x, out_section.y, out_section.w, out_section.h);
	this->in_section = new SDL_Rect(in_section.x, in_section.y, in_section.w, in_section.h);
	this->click_section = new SDL_Rect(click_section.x, click_section.y, click_section.w, click_section.h);
}

bool GUIButton::CleanUp() {
	RELEASE(out_section);
	RELEASE(in_section);
	RELEASE(click_section);
}

bool GUIButton::PreUpdate()
{
	if (state == MOUSE_STATE::M_ENTER || state == MOUSE_STATE::M_IN) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			if (click_section != nullptr && childImage != nullptr && &childImage->section != click_section) {
				childImage->section = *click_section;
			}
			//Call function pointer
			//Make sound
		}
		else {
			if (in_section != nullptr  && childImage != nullptr && &childImage->section != in_section) {
				childImage->section = *in_section;
			}
		}
	}
	else {
		if (out_section != nullptr  && childImage != nullptr && &childImage->section != in_section) {
			childImage->section = *in_section;
		}
	}

	return true;
}