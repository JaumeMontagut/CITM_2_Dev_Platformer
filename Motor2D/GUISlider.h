#ifndef _GUI_SLIDER_H_
#define _GUI_SLIDER_H_

#include "j1Gui.h"

class GUIImage;

class GUISlider : public GUIElement {
	enum class TYPE{
		HORIZONTAL,
		VERTICAL,
		FREE
	};

	//Variables
private:
	//Slider limits
	int * xLimit = nullptr;
	int * yLimit = nullptr;
	float * xPerCent = nullptr;
	float * yPerCent = nullptr;
	GUIImage * childThumb = nullptr;
	GUIImage * childBox = nullptr;
	//Methods
private:
	GUISlider(iPoint pos, SDL_Rect * boxSection, SDL_Rect* thumbSection, GUISlider::TYPE sliderType, float * multiplier1 = nullptr, float * multiplier2 = nullptr);
	bool PreUpdate() override;
	bool CleanUp() override;
};

#endif
