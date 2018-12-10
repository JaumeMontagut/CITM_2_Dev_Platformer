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
	// button textures || if we put all of them on an atlas, is needed a fast code adaptation, we must think it good
	buttonup_filename = gui_node.child("button_up").attribute("file").as_string("");
	buttondown_filename = gui_node.child("button_down").attribute("file").as_string("");
	buttonhighlight_filename = gui_node.child("button_hover").attribute("file").as_string("");
	// checkbox textures
	checkbox_up_filename = gui_node.child("checkbox_up").attribute("file").as_string("");
	checkbox_down_filename = gui_node.child("checkbox_down").attribute("file").as_string("");
	checkbox_highlight_filename = gui_node.child("checkbox_highlight").attribute("file").as_string("");
	checkbox_check_filename = gui_node.child("checkbox_check").attribute("file").as_string("");
	//checkbox_check_locked_filename = conf.child("checkbox_check_locked").attribute("file").as_string("");


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
	for (int i = 0; i < elements.Count(); ++i)
	{
		GUIElement* elem = *elements.At(i);
		//Check and updates mouse state -----------------------
		int mouse_x, mouse_y = 0;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x *= (int)App->win->GetScale();
		mouse_y *= (int)App->win->GetScale();
		SetState(elem, mouse_x, mouse_y);
		elem->PreUpdate();
		elem->OnMouseHover();
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
	for (int i = 0; i < elements.Count(); ++i)
	{
		GUIElement* e = *elements.At(i);
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

	elements.Clear(); // dynarray clears itselfs when destructor

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


//GUIElement methods --------------------------------------------------------------------------------------------

bool GUIElement::CheckBounds(int x, int y)
{
	return (x > bounds.x && x < (bounds.x + bounds.w) && y > bounds.y && y < (bounds.y + bounds.h));
}

void GUIElement::OnMouseHover()
{
	if (state == MOUSE_STATE::M_ENTER) {
		App->audio->PlayFx(hoverSFX);
		//Change texture

	}
	else if (state == MOUSE_STATE::M_EXIT) {
		//Change texture
	}
}

// UIelements constructions
GUIImage* j1Gui::AddGUIImage(const SDL_Rect & section, const iPoint& position)
{
	GUIImage* ret = nullptr;
	ret = new GUIImage(section, position);
	elements.PushBack(ret);
	ret->index = elements.Count();

	return ret;
}

GUIText* j1Gui::AddGUIText(const iPoint& position, const char* text, SDL_Color color = WHITE)
{
	GUIText* ret = nullptr;
	ret = new GUIText(position, text, color);
	elements.PushBack(ret);
	ret->index = elements.Count();

	return ret;
}

//GUIButton* j1Gui::AddGUIButton(SDL_Texture* clickTexture, SDL_Texture* unclickTexture, const SDL_Rect& rect, const iPoint& position, const char* text, TextPos targetTextPos, SDL_Texture* onMouseTex)
//{
//	GUIButton* ret = nullptr;
//	ret = new GUIButton(clickTexture, unclickTexture, rect, position, text, targetTextPos, onMouseTex);
//	elements.PushBack(ret);
//	ret->index = elements.Count();
//
//	return ret;
//}
//
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

// GUIButton relative ============================================
//GUIButton::GUIButton(SDL_Texture* click_texture, SDL_Texture* unclick_texture, const SDL_Rect& rect, const iPoint& position, const char* text, GUI_ADJUST targetPos, SDL_Texture* hoverTex)
//	: clicked_texture(click_texture), unclicked_texture(unclick_texture), hover_texture(hoverTex), GUIImage(unclick_texture, rect, position, text, targetPos, hoverTex) {}
//
//bool GUIButton::PreUpdate()
//{
//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && guiState == MOUSE_STATE::HOVER)
//	{
//		image_texture = clicked_texture;
//		LOG("button clicked");
//		guiState = MOUSE_STATE::CLICK;
//	}
//	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && guiState == MOUSE_STATE::CLICK) // always unclick
//	{
//		image_texture = unclicked_texture;
//		LOG("button unclicked");
//		guiState = MOUSE_STATE::DONTCARE;
//	}
//
//	return true;
//}
// ===============================================================

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