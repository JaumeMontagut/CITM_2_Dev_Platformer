#include "GUISlider.h"
#include "GUIImage.h"
#include "j1Gui.h"
#include "j1App.h"
#include "p2Log.h"

GUISlider::GUISlider(iPoint pos, SDL_Rect * boxSection, SDL_Rect * thumbSection, GUISlider::TYPE sliderType, float * multiplier1, float * multiplier2) :
	GUIElement(pos)
{
	if (boxSection != nullptr) {
		childBox = App->gui->CreateImage(iPoint(pos.x, pos.y), *boxSection, this);
		SDL_Rect boxBounds(0, 0, boxSection->w * 2, boxSection->h * 2);
		childBox->bounds = boxBounds;
	}
	if (thumbSection != nullptr){
		iPoint boxCenter(pos.x, pos.y);
		if (childBox != nullptr) {
			boxCenter.x += childBox->bounds.w * 0.5f;
			boxCenter.y += childBox->bounds.h * 0.5f;
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
		xPerCent = multiplier1;
		yLimit = new int;
		*yLimit = pos.y;
		break;
	case GUISlider::TYPE::VERTICAL:
		yPerCent = multiplier1;
		xLimit = new int;
		*xLimit = pos.x;
		break;
	case GUISlider::TYPE::FREE:
		xPerCent = multiplier1;
		yPerCent = multiplier2;
		//No limits
		break;
	}
}

bool GUISlider::PreUpdate()
{
	//Don't exit the box
	if (childThumb != nullptr && childBox != nullptr) {
		if (childThumb->GetGlobalPos().x < childBox->GetGlobalPos().x) {
			childThumb->SetGlobalPos({ childBox->GetGlobalPos().x, childThumb->GetGlobalPos().y });
		}
		else if (childThumb->GetGlobalPos().x + childThumb->bounds.w > childBox->GetGlobalPos().x + childBox->bounds.w) {
			childThumb->SetGlobalPos({ childBox->GetGlobalPos().x + childBox->bounds.w - childThumb->bounds.w , childThumb->GetGlobalPos().y });
		}
		if (childThumb->GetGlobalPos().y < childBox->GetGlobalPos().y) {
			childThumb->SetGlobalPos({ childThumb->GetGlobalPos().x, childBox->GetGlobalPos().y });
		}
		else if (childThumb->GetGlobalPos().y > childBox->GetGlobalPos().y + childBox->bounds.h) {
			childThumb->SetGlobalPos({ childThumb->GetGlobalPos().x, childBox->GetGlobalPos().y + childBox->bounds.h });
		}
	}

	if (xPerCent != nullptr) {
		*xPerCent = (float)(childThumb->GetGlobalPos().x - childBox->GetGlobalPos().x) / (float)(childBox->bounds.w - childThumb->bounds.w);
	}
	if (yPerCent != nullptr) {
		*yPerCent = (float)(childThumb->GetGlobalPos().y - childBox->GetGlobalPos().y) / (float)(childBox->bounds.h - childThumb->bounds.h);
	}

	if (xLimit != nullptr) {
		childThumb->SetGlobalPos({ *xLimit, childThumb->GetGlobalPos().y });
	}
	if (yLimit != nullptr) {
		childThumb->SetGlobalPos({ childThumb->GetGlobalPos().x, *yLimit });
	}
	return false;
}

bool GUISlider::CleanUp()
{
	RELEASE(xLimit);
	RELEASE(yLimit);
	return true;
}
