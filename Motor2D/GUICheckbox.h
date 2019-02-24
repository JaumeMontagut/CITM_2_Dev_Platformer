#ifndef _GUI_CHECKBOX_H
#define _GUI_CHECKBOX_H

#include "j1Gui.h"

class GUIImage;

class GUICheckbox : public GUIElement
{
	enum class CB_STATES : uint {
		CB_OUT = 0,
		CB_IN,
		CB_CLICK,
		CB_MAX
	};

	enum class CHECK : uint {
		UNCHECKED = 0,
		CHECKED,
		MAX
	};
public:
	//Constructors
	//Constructor for GUICheckbox. Leave text as nullptr to not have text and leave out_section as nullptr to not have image.
	GUICheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr = nullptr, const char * text = nullptr, const SDL_Rect * outUncheckSection = nullptr, const SDL_Rect * inUncheckSection = nullptr, const SDL_Rect * clickUncheckSection = nullptr, const SDL_Rect * outCheckSection = nullptr, const SDL_Rect * inCheckSection = nullptr, const SDL_Rect * clickCheckSection = nullptr, uint clickSFX = 0u);
	GUICheckbox(const iPoint& position, CheckboxTemplates& templateType, const char* text = nullptr, bool* boolPtr = nullptr);

	bool PreUpdate();
	bool CleanUp();

private:
	void SetSection(CB_STATES state);

private:
	//General info
	bool clickable = true;
	bool *boolPtr = nullptr;//A pointer to the bool that's going to be changed
	GUIImage* childImage;
	uint* clickSfx = nullptr;
	uint* hoverSfx = nullptr;
	//Sprites
	SDL_Rect * sections[(uint)CB_STATES::CB_MAX][(uint)CHECK::MAX];
};


#endif

