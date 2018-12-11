#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Audio.h"
#include "j1Window.h"
//Gui Elements
#include "GUIImage.h"
#include "GUIText.h"
#include "GUIButton.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	//LOG("Loading GUI atlas");
	bool ret = true;

	gui_config_filename.create(conf.child("gui_config_filename").attribute("path").as_string(""));
	gui_node = LoadConfig(gui_config_doc);

	atlas_file_name = gui_node.child("atlas").attribute("file").as_string("");

	if (!LoadElementTemplate(buttonType1, gui_node.child("gui_element_templates").child("buttons").child("button_type_1")))
		ret = false;
	if (!LoadElementTemplate(checkboxType1, gui_node.child("gui_element_templates").child("checkboxes").child("checkbox_type_1")))
		ret = false;

	//// button textures || if we put all of them on an atlas, is needed a fast code adaptation, we must think it good
	//buttonup_filename = gui_node.child("button_up").attribute("file").as_string("");
	//buttondown_filename = gui_node.child("button_down").attribute("file").as_string("");
	//buttonhighlight_filename = gui_node.child("button_hover").attribute("file").as_string("");
	//// checkbox textures
	//checkbox_up_filename = gui_node.child("checkbox_up").attribute("file").as_string("");
	//checkbox_down_filename = gui_node.child("checkbox_down").attribute("file").as_string("");
	//checkbox_highlight_filename = gui_node.child("checkbox_highlight").attribute("file").as_string("");
	//checkbox_check_filename = gui_node.child("checkbox_check").attribute("file").as_string("");
	////checkbox_check_locked_filename = conf.child("checkbox_check_locked").attribute("file").as_string("");


	return ret;
}

bool j1Gui::LoadElementTemplate(ButtonTemplates& templateType, pugi::xml_node& node)
{
	bool ret = false;
	
	if (!node.empty())
	{
		if (node.child("up"))
		{
			templateType.sectionUp.x = node.child("up").attribute("x").as_int(0);
			templateType.sectionUp.y = node.child("up").attribute("y").as_int(0);
			templateType.sectionUp.w = node.child("up").attribute("w").as_int(0);
			templateType.sectionUp.h = node.child("up").attribute("h").as_int(0);
		}
		if (node.child("hover"))
		{
			templateType.sectionHover.x = node.child("hover").attribute("x").as_int(0);
			templateType.sectionHover.y = node.child("hover").attribute("y").as_int(0);
			templateType.sectionHover.w = node.child("hover").attribute("w").as_int(0);
			templateType.sectionHover.h = node.child("hover").attribute("h").as_int(0);
		}
		if (node.child("down"))
		{
			templateType.sectionDown.x = node.child("down").attribute("x").as_int(0);
			templateType.sectionDown.y = node.child("down").attribute("y").as_int(0);
			templateType.sectionDown.w = node.child("down").attribute("w").as_int(0);
			templateType.sectionDown.h = node.child("down").attribute("h").as_int(0);
		}
		if (node.child("disabled"))
		{
			templateType.sectionDisabled.x = node.child("disabled").attribute("x").as_int(0);
			templateType.sectionDisabled.y = node.child("disabled").attribute("y").as_int(0);
			templateType.sectionDisabled.w = node.child("disabled").attribute("w").as_int(0);
			templateType.sectionDisabled.h = node.child("disabled").attribute("h").as_int(0);
		}
		if (node.child("check"))
		{
			CheckboxTemplates* link = &(CheckboxTemplates&)templateType;
			link->sectionCheck.x = node.child("check").attribute("x").as_int(0);
			link->sectionCheck.y = node.child("check").attribute("y").as_int(0);
			link->sectionCheck.w = node.child("check").attribute("w").as_int(0);
			link->sectionCheck.h = node.child("check").attribute("h").as_int(0);
		}
	
		ret = true;
	}
	

	return ret;
}

pugi::xml_node j1Gui::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file(gui_config_filename.GetString());

	if (result == NULL) {
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	}
	else {
		ret = gui_config_doc.child("gui_config");
	}

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->LoadTexture(atlas_file_name.GetString());
	// buttons
	buttonup_texture = App->tex->LoadTexture(buttonup_filename.GetString());
	buttondown_texture = App->tex->LoadTexture(buttondown_filename.GetString());
	buttonhighlight_texture = App->tex->LoadTexture(buttonhighlight_filename.GetString());
	// sets blend mode additive to highlight texture
	SDL_SetTextureBlendMode(buttonhighlight_texture, SDL_BLENDMODE_ADD);
	// checkbox
	checkbox_up_texture = App->tex->LoadTexture(checkbox_up_filename.GetString());
	checkbox_down_texture = App->tex->LoadTexture(checkbox_down_filename.GetString());
	checkbox_highlight_texture = App->tex->LoadTexture(checkbox_highlight_filename.GetString());
	checkbox_check_texture = App->tex->LoadTexture(checkbox_check_filename.GetString());
	SDL_SetTextureBlendMode(checkbox_highlight_texture, SDL_BLENDMODE_ADD);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	for (int i = 0; i < guiElems.Count(); ++i)
	{
		GUIElement* elem = *guiElems.At(i);
		//Check and updates mouse state -----------------------
		int mouse_x, mouse_y = 0;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x *= (int)App->win->GetScale();
		mouse_y *= (int)App->win->GetScale();
		SetState(elem, mouse_x, mouse_y);
		elem->PreUpdate();
	}

	return true;
}

void j1Gui::SetState(GUIElement * elem, int mouse_x, int mouse_y)
{
	if (elem->CheckBounds(mouse_x, mouse_y)) {
		if (elem->state == MOUSE_STATE::M_OUT) {
			elem->state == MOUSE_STATE::M_ENTER;
		}
		else {
			elem->state == MOUSE_STATE::M_IN;
		}
	}
	else {
		if (elem->state == MOUSE_STATE::M_IN || elem->state == MOUSE_STATE::M_ENTER) {
			elem->state == MOUSE_STATE::M_EXIT;
		}
		else {
			elem->state == MOUSE_STATE::M_OUT;
		}
	}
}

//void GUIElement::SetMouseState(MOUSE_EVENT event)
//{
//	switch (event)
//	{
//	case MOUSE_EVENT::ENTER:
//		if (guiState == MOUSE_STATE::ENTER)
//		{
//			LOG("Mouse is hovering");
//			guiState = MOUSE_STATE::HOVER;
//		}
//		else if (guiState == MOUSE_STATE::DONTCARE || guiState == MOUSE_STATE::EXIT)
//		{
//			LOG("Mouse entered on boundaries");
//			guiState = MOUSE_STATE::ENTER;
//		}
//		break;
//	case MOUSE_EVENT::EXIT:
//		LOG("Mouse Exit");
//		guiState = MOUSE_STATE::EXIT;
//		break;
//	case MOUSE_EVENT::FAILED:
//		break;
//	default:
//		LOG("unknown mouse event");
//		break;
//	}
//}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < guiElems.Count(); ++i)
	{
		GUIElement* e = *guiElems.At(i);
		e->PostUpdate();
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	App->tex->UnloadTexture(atlas);
	App->tex->UnloadTexture(buttonup_texture);
	App->tex->UnloadTexture(buttondown_texture);
	App->tex->UnloadTexture(buttonhighlight_texture);

	guiElems.Clear(); // dynarray clears itselfs when destructor

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void SayHelloButton()
{
	LOG("I'm a button and I say hello");
}


//GUIElement methods --------------------------------------------------------------------------------------------

bool GUIElement::CheckBounds(int x, int y)
{
	return (x > bounds.x && x < (bounds.x + bounds.w) && y > bounds.y && y < (bounds.y + bounds.h));
}

// UIelements constructions
GUIImage* j1Gui::CreateImage(const iPoint& position, const SDL_Rect & section)
{
	GUIImage* ret = nullptr;
	ret = new GUIImage(position, section);
	guiElems.PushBack(ret);
	return ret;
}

GUIText* j1Gui::CreateText(const iPoint& position, const char* text, SDL_Color color)
{
	GUIText* ret = nullptr;
	ret = new GUIText(position, text, color);
	guiElems.PushBack(ret);
	return ret;
}

GUIButton* j1Gui::CreateButton(const iPoint & position, void(*clickFunction)(), const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section)
{
	GUIButton* ret = nullptr;
	ret = new GUIButton(position, clickFunction, text, out_section, in_section, click_section);
	guiElems.PushBack(ret);
	return ret;
}

//GUICheckBox* j1Gui::AddGUICheckBox(SDL_Texture* clickTexture, SDL_Texture* unclickTexture, const SDL_Rect& rect, const iPoint& position, const char* text, TextPos targetTextPos, SDL_Texture* onMouseTex, SDL_Texture* checkTex)
//{
//	GUICheckBox* ret = nullptr;
//	ret = new GUICheckBox(clickTexture, unclickTexture, rect, position, text, targetTextPos, onMouseTex, checkTex);
//	elements.PushBack(ret);
//	ret->index = elements.Count();
//
//	return ret;
//}


// class Gui ---------------------------------------------------

GUIElement::GUIElement(const iPoint& position) : localPos(position) {}

bool GUIElement::PreUpdate()
{
	return true;
}
bool GUIElement::PostUpdate()
{
	return true;
}
bool GUIElement::CleanUp()
{
	return true;
}

// CHECKBOX relative =============================================

//GUICheckBox::GUICheckBox(SDL_Texture* click_texture, SDL_Texture* unclick_texture, const SDL_Rect& rect, const iPoint& position, const char* text, GUI_ADJUST targetPos, SDL_Texture* hoverTex, SDL_Texture* checkTex)
//	: checkTexture(checkTex), GUIButton(click_texture, unclick_texture, rect, position, text, targetPos, hoverTex)
//{
//
//}

//bool GUICheckBox::PreUpdate()
//{
//	GUIButton::PreUpdate(); // calls overrided preUpdate from parent too
//
//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && guiState == MOUSE_STATE::DONTCARE)
//	{
//		active = !active;
//	}
//
//	return true;
//}
//
//bool GUICheckBox::PostUpdate()
//{
//	//GUIBanner::PostUpdate(); // GUIButton doesnt had postupdate yet, if we need it, call parent of button(guibanner) on button postupdate, and here the button
//	//Instead of calling the update of the parent, make an image and use its own update
//
//	if (active && checkTexture != nullptr)
//		App->render->BlitGUIUnscaled(checkTexture, localPos.x, localPos.y, NULL);
//
//	return true;
//}