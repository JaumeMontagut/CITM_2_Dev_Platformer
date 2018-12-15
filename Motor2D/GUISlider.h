#ifndef _GUI_SLIDER_H_
#define _GUI_SLIDER_H_

#include "j1Gui.h"

//Only supports horizontal sliders
class GUISlider : public GUIElement {
	//Variables
private:
	GUIImage * childBox;
	GUIImage * childThumb;
	//Slider limits
	bool limitX = false;
	bool limitY = false;
	float * multiplier = nullptr;

	//Methods
private:
	bool PreUpdate() override;
};

#endif
