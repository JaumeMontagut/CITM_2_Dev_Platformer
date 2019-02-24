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
#include "SDL/include/SDL_scancode.h"

GUICheckbox::GUICheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr, const char * text, const SDL_Rect * outUncheckSection, const SDL_Rect * inUncheckSection, const SDL_Rect * clickUncheckSection, const SDL_Rect * outCheckSection, const SDL_Rect * inCheckSection, const SDL_Rect * clickCheckSection, uint clickSfx) : boolPtr(boolPtr), clickSfx(&clickSfx), GUIElement(position) {
	//Create child image
	if (outUncheckSection != nullptr) {
		sections[(uint)CB_STATES::CB_OUT][(uint)CHECK::UNCHECKED] = new SDL_Rect(outUncheckSection->x, outUncheckSection->y, outUncheckSection->w, outUncheckSection->h);
		childImage = App->gui->CreateImage(position, *outUncheckSection, this);
	}
	if (inUncheckSection != nullptr) {
		sections[(uint)CB_STATES::CB_IN][(uint)CHECK::UNCHECKED] = new SDL_Rect(inUncheckSection->x, inUncheckSection->y, inUncheckSection->w, inUncheckSection->h);
	}
	if (clickUncheckSection != nullptr) {
		sections[(uint)CB_STATES::CB_CLICK][(uint)CHECK::UNCHECKED] = new SDL_Rect(clickUncheckSection->x, clickUncheckSection->y, clickUncheckSection->w, clickUncheckSection->h);
	}
	if (outCheckSection != nullptr) {
		sections[(uint)CB_STATES::CB_OUT][(uint)CHECK::CHECKED] = new SDL_Rect(outCheckSection->x, outCheckSection->y, outCheckSection->w, outCheckSection->h);
	}
	if (inCheckSection != nullptr) {
		sections[(uint)CB_STATES::CB_IN][(uint)CHECK::CHECKED] = new SDL_Rect(inCheckSection->x, inCheckSection->y, inCheckSection->w, inCheckSection->h);
	}
	if (clickCheckSection != nullptr) {
		sections[(uint)CB_STATES::CB_CLICK][(uint)CHECK::CHECKED] = new SDL_Rect(clickCheckSection->x, clickCheckSection->y, clickCheckSection->w, clickCheckSection->h);
	}
	//Create child Text
	if (text != nullptr) {
		App->gui->CreateText(position, text, WHITE, bounds.w, App->font->default, this);//TODO: Get even another parameter with the font
	}
	if (boolPtr == nullptr) {
		LOG("Error while associating checkbox with a bool, crash incoming");
	}
	if (childImage == nullptr) {
		LOG("Error while creating checkbox image, crash incoming");
	}
	assert(boolPtr != nullptr || childImage != nullptr);

	interactable = true;

	this->bounds = bounds;
}

GUICheckbox::GUICheckbox(const iPoint& position, CheckboxTemplates& templateType, const char* text, bool* boolPtr) : 
	boolPtr(boolPtr),
	clickSfx(templateType.clickSfx),
	hoverSfx(templateType.hoverSfx),
	GUIElement(position)
{
	// set sections rects
	sections[(uint)CB_STATES::CB_OUT][(uint)CHECK::UNCHECKED] = &templateType.sectionUp;
	sections[(uint)CB_STATES::CB_OUT][(uint)CHECK::CHECKED] = &templateType.sectionUpCheck;
	sections[(uint)CB_STATES::CB_IN][(uint)CHECK::UNCHECKED] = &templateType.sectionHover;
	sections[(uint)CB_STATES::CB_IN][(uint)CHECK::CHECKED] = &templateType.sectionHoverCheck;
	sections[(uint)CB_STATES::CB_CLICK][(uint)CHECK::UNCHECKED] = &templateType.sectionDown;
	sections[(uint)CB_STATES::CB_CLICK][(uint)CHECK::CHECKED] = &templateType.sectionDownCheck;

	// add child image
	if(sections[(uint)CB_STATES::CB_OUT][(uint)CHECK::UNCHECKED] != nullptr)
		childImage = App->gui->CreateImage(position, templateType.sectionUp, this);

	//Create child Text
	if (text != nullptr) {
		App->gui->CreateText(position, text, templateType.fontColor, bounds.w, templateType.font, this);
	}
	if (boolPtr == nullptr) {
		LOG("Error while associating checkbox with a bool, crash incoming");
	}
	if (childImage == nullptr) {
		LOG("Error while creating checkbox image, crash incoming");
	}
	assert(boolPtr != nullptr || childImage != nullptr);

	interactable = true;

	bounds = templateType.bounds;

}

bool GUICheckbox::PreUpdate()
{
	if ((state == FOCUS::GET_FOCUS || state == FOCUS::ON_FOCUS) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)) {
		*boolPtr = !(*boolPtr);
		SetSection(CB_STATES::CB_CLICK);
		if(clickSfx != nullptr)
			App->audio->PlayFx(*clickSfx);
	}
	else if (state == FOCUS::GET_FOCUS || (state == FOCUS::ON_FOCUS && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP))) {
		SetSection(CB_STATES::CB_IN);
	}
	else if (state == FOCUS::LOSE_FOCUS) {
		SetSection(CB_STATES::CB_OUT);
	}
	return true;
}

bool GUICheckbox::CleanUp()
{
	for (int i = 0; i < (uint)CB_STATES::CB_MAX; ++i) {
		for (int j = 0; j < (uint)CHECK::MAX; ++j) {
			RELEASE(sections[i][j]);
		}
	}
	return true;
}

void GUICheckbox::SetSection(CB_STATES state)
{
	if (*boolPtr) {
		if (sections[(uint)state][(uint)CHECK::CHECKED] != nullptr  && &childImage->section != sections[(uint)state][(uint)CHECK::CHECKED]) {
			childImage->section = *sections[(uint)state][(uint)CHECK::CHECKED];
		}
	}
	else {
		if (sections[(uint)state][(uint)CHECK::UNCHECKED] != nullptr  && &childImage->section != sections[(uint)state][(uint)CHECK::UNCHECKED]) {
			childImage->section = *sections[(uint)state][(uint)CHECK::UNCHECKED];
		}
	}
}