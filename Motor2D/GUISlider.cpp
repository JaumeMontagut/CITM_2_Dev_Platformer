#include "GUISlider.h"
#include "GUIImage.h"
#include "j1Gui.h"
#include "j1App.h"

GUISlider::GUISlider(iPoint pos, SDL_Rect * boxSection, SDL_Rect * thumbSection, GUISlider::TYPE sliderType, float * multiplier1, float * multiplier2) :
	GUIElement(pos)
{
	if (boxSection != nullptr) {
		childBox = App->gui->CreateImage(iPoint(0,0), *boxSection, this);
		SDL_Rect boxBounds(0, 0, boxSection->w * 2, boxSection->h * 2);
		childBox->bounds = boxBounds;
	}
	if (thumbSection != nullptr){
		iPoint boxCenter(0, 0);
		if (boxSection != nullptr) {
			boxCenter.x = boxSection->x;
			boxCenter.y = boxSection->y;
		}
		childThumb = App->gui->CreateImage(boxCenter, *thumbSection, this);
		childThumb->draggable = true;
		childThumb->interactable = true;
		SDL_Rect thumbBounds(0, 0, thumbSection->w * 2, thumbSection->h * 2);
		childThumb->bounds = thumbBounds;
		//bounds
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
	//Don't exit the box
	if (childThumb != nullptr && childBox != nullptr) {
		if (childThumb->localPos.x < childBox->localPos.x) {
			childThumb->localPos.x = childBox->localPos.x;
		}
		else if (childThumb->localPos.x + childThumb->bounds.w > childBox->localPos.x + childBox->bounds.w) {
			childThumb->localPos.x = childBox->localPos.x + childBox->bounds.w - childThumb->bounds.w;
		}
		if (childThumb->localPos.y < childBox->localPos.y) {
			childThumb->localPos.y = childBox->localPos.y;
		}
		else if (childThumb->localPos.y > childBox->localPos.y + childBox->bounds.h) {
			childThumb->localPos.y = childBox->localPos.y + childBox->bounds.h;
		}
	}

	//TODO: Give the percent
	//TODO: global pos instead of local pos

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
