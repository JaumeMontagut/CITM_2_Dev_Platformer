#ifndef _GUI_SLIDER_H_
#define _GUI_SLIDER_H_

#include "j1Gui.h"

//Only supports horizontal sliders
class GUISlider : public GUIElement {
	//Variables
private:
	GUIImage * childBox;
	GUIImage * childDrag;
	int minX = 0, maxX = 0;
	int fixedY;

	//Methods
private:
	bool PreUpdate() override;
};

#endif // !
