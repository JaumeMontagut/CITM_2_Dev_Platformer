#include "GUIButton.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Input.h"
#include "SDL/include/SDL_mouse.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Fonts.h"
#include "GUIText.h"
#include "SDL/include/SDL_scancode.h"

GUIButton::GUIButton(const iPoint & position, const SDL_Rect & bounds, p2SString functionName, const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section, uint clickSfx) :
	clickSfx(clickSfx), GUIElement(position) {
	//Child image
	if (out_section != nullptr) {
		this->outSection = new SDL_Rect(out_section->x, out_section->y, out_section->w, out_section->h);
		childImage = App->gui->CreateImage(position, *out_section, this);//TODO: Take into account the adjustment
	}
	if (in_section != nullptr) {
		this->inSection = new SDL_Rect(in_section->x, in_section->y, in_section->w, in_section->h);
	}
	if (click_section != nullptr) {
		this->clickSection = new SDL_Rect(click_section->x, click_section->y, click_section->w, click_section->h);
	}
	//Child text
	if (text != nullptr) {
		App->gui->CreateText(position, text, WHITE, App->font->default, this);//TODO: Take into account the adjustment
	}
	clickFunction = App->gui->GetButtonFunction(functionName);
	interactable = true;
	this->bounds = bounds;
}

GUIButton::GUIButton(const iPoint& position, ButtonTemplates& templateType, p2SString functionName, const char* text) :
	outSection(&templateType.sectionUp),
	inSection(&templateType.sectionHover),
	clickSection(&templateType.sectionDown),
	moveTextDown(templateType.moveTextDown),
	clickSfx(templateType.clickSfx),
	hoverSfx(templateType.hoverSfx),
	GUIElement(position)
{
	bounds = templateType.bounds;
	if (outSection != nullptr) {
		childImage = App->gui->CreateImage(position, *outSection, this);
	}	
	if (text != nullptr) {
		iPoint centerPos(bounds.x + position.x + bounds.w * 0.5f, bounds.y + position.y + bounds.h * 0.4f);
		childText = App->gui->CreateText(centerPos, text, templateType.fontColor, templateType.font, this);
	}
	clickFunction = App->gui->GetButtonFunction(functionName);
	interactable = true;
}

bool GUIButton::CleanUp() {
	RELEASE(outSection);
	RELEASE(inSection);
	RELEASE(clickSection);
	return true;
}

bool GUIButton::PreUpdate()
{
	if ((state == FOCUS::GET_FOCUS || state == FOCUS::ON_FOCUS) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)) {
		if (clickSection != nullptr && childImage != nullptr && &childImage->section != clickSection) {
			childImage->section = *clickSection;
		}
		if (childText != nullptr) {
			childText->localPos.y += moveTextDown;
			textMoved = true;
		}
		if (clickFunction != nullptr) {
			clickFunction();//Call function pointer
		}
		App->audio->PlayFx(clickSfx);
	}
	else if (state == FOCUS::GET_FOCUS) {
		if (inSection != nullptr  && childImage != nullptr && &childImage->section != inSection) {
			childImage->section = *inSection;
		}
		App->audio->PlayFx(hoverSfx);
	}
	else if ((state == FOCUS::GET_FOCUS || state == FOCUS::ON_FOCUS) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)) {
		if (inSection != nullptr  && childImage != nullptr && &childImage->section != inSection) {
			childImage->section = *inSection;
		}
		if (textMoved && childText != nullptr) {
			childText->localPos.y -= moveTextDown;
			textMoved = false;
		}
	}
	else if (state == FOCUS::LOSE_FOCUS) {
		if (outSection != nullptr  && childImage != nullptr && &childImage->section != outSection) {
			childImage->section = *outSection;
		}
		if (textMoved && childText != nullptr) {
			childText->localPos.y -= moveTextDown;
			textMoved = false;
		}
	}



	return true;
}