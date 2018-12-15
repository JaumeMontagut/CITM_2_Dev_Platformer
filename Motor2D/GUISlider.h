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
	bool limitX = false;
	bool limitY = false;
	float * xPerCent = nullptr;
	float * yPerCent = nullptr;
	GUIImage * childThumb = nullptr;
	GUIImage * childBox = nullptr;
	//Methods
private:
	GUISlider(iPoint pos, SDL_Rect * boxSection, SDL_Rect* thumbSection, GUISlider::TYPE sliderType, float * multiplier1, float * multiplier2);
	bool PreUpdate() override;
};

#endif
