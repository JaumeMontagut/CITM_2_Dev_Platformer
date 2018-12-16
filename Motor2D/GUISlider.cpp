#include "GUISlider.h"
#include "GUIImage.h"
#include "j1Gui.h"
#include "j1App.h"

GUISlider::GUISlider(iPoint pos, SDL_Rect * boxSection, SDL_Rect * thumbSection, GUISlider::TYPE sliderType, float * multiplier1, float * multiplier2) :
	GUIElement(pos)
{
	if (boxSection != nullptr) {
		childBox = App->gui->CreateImage(iPoint(0,0), *boxSection, this);
	}
	if (thumbSection != nullptr){
		iPoint boxCenter(0, 0);
		if (boxSection != nullptr) {
			boxCenter.x = boxSection->x * 0.5f;
			boxCenter.y = boxSection->y;
		}
		childThumb = App->gui->CreateImage(boxCenter, *thumbSection, this);
		childThumb->draggable = true;
	}
	switch (sliderType) {
	case GUISlider::TYPE::HORIZONTAL:
		yLimit = new int;
		*yLimit = pos.y;
		break;
	case GUISlider::TYPE::VERTICAL:
		xLimit = new int;
		*xLimit = pos.x;
		break;
	case GUISlider::TYPE::FREE:
		//No limits
		break;
	}
}

bool GUISlider::PreUpdate()
{


	if (xLimit != nullptr) {
		childThumb->localPos.x = *xLimit;
	}
	if (yLimit != nullptr) {
		childThumb->localPos.y = *yLimit;
	}
	return false;
}

bool GUISlider::CleanUp()
{
	RELEASE(xLimit);
	RELEASE(yLimit);
	return true;
}
