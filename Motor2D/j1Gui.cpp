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
#include "j1Input.h"
//Gui Elements
#include "GUIImage.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "GUICheckbox.h"

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
	//// buttons
	//buttonup_texture = App->tex->LoadTexture(buttonup_filename.GetString());
	//buttondown_texture = App->tex->LoadTexture(buttondown_filename.GetString());
	//buttonhighlight_texture = App->tex->LoadTexture(buttonhighlight_filename.GetString());
	//// sets blend mode additive to highlight texture
	//SDL_SetTextureBlendMode(buttonhighlight_texture, SDL_BLENDMODE_ADD);
	//// checkbox
	//checkbox_up_texture = App->tex->LoadTexture(checkbox_up_filename.GetString());
	//checkbox_down_texture = App->tex->LoadTexture(checkbox_down_filename.GetString());
	//checkbox_highlight_texture = App->tex->LoadTexture(checkbox_highlight_filename.GetString());
	//checkbox_check_texture = App->tex->LoadTexture(checkbox_check_filename.GetString());
	//SDL_SetTextureBlendMode(checkbox_highlight_texture, SDL_BLENDMODE_ADD);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debugGUI = !debugGUI;
	}

	for (int i = 0; i < guiElems.Count(); ++i)
	{
		GUIElement* elem = *guiElems.At(i);
		//Check and updates mouse state -----------------------
		int mouse_x, mouse_y = 0;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x *= (int)App->win->GetScale();
		mouse_y *= (int)App->win->GetScale();
		elem->SetState(mouse_x, mouse_y);
		elem->PreUpdate();
	}

	return true;
}

void GUIElement::SetState(int x, int y) {
	if (CheckBounds(x, y)) {
		if (state == MOUSE_STATE::M_OUT) {
			state = MOUSE_STATE::M_ENTER;
		}
		else {
			state = MOUSE_STATE::M_IN;
		}
	}
	else {
		if (state == MOUSE_STATE::M_IN || state == MOUSE_STATE::M_ENTER) {
			state = MOUSE_STATE::M_EXIT;
		}
		else {
			state = MOUSE_STATE::M_OUT;
		}
	}
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < guiElems.Count(); ++i)
	{
		GUIElement* e = *guiElems.At(i);
		e->PostUpdate();
	}

	if (debugGUI) {
		for (int i = 0; i < guiElems.Count(); ++i) {
			GUIElement* e = *guiElems.At(i);
			e->DrawOutline();
		}
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


//GUIElement methods --------------------------------------------------------------------------------------------

bool GUIElement::CheckBounds(int x, int y)
{
	iPoint globalPos = GetGlobalPos();
	return (x > globalPos.x + bounds.x && x < (globalPos.x + bounds.x + bounds.w) && y > globalPos.y + bounds.y && y < (globalPos.y + bounds.y + bounds.h));
}

void GUIElement::DrawOutline()
{
	App->render->DrawQuad(bounds, 255, 255, 255, 255, false, false);
}

iPoint GUIElement::GetGlobalPos()
{
	if (parent != nullptr) {
		return localPos + parent->GetGlobalPos();
	}
	return localPos;
}

// UIelements constructions
GUIImage* j1Gui::CreateImage(const iPoint& position, const SDL_Rect & section, GUIElement * parent)
{
	GUIImage* guiElem = nullptr;
	guiElem = new GUIImage(position, section);
	guiElems.PushBack(guiElem);
	guiElem->SetFamily(parent);
	return guiElem;
}

GUIText* j1Gui::CreateText(const iPoint& position, const char* text, SDL_Color color, GUIElement * parent)
{
	GUIText* guiElem = nullptr;
	guiElem = new GUIText(position, text, color);
	guiElems.PushBack(guiElem);
	guiElem->SetFamily(parent);
	return guiElem;
}

GUIButton* j1Gui::CreateButton(const iPoint & position, const SDL_Rect & bounds, void(*clickFunction)(), const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section, uint clickSfx, GUIElement * parent)
{
	GUIButton* guiElem = nullptr;
	guiElem = new GUIButton(position, bounds, clickFunction, text, out_section, in_section, click_section);
	guiElems.PushBack(guiElem);
	guiElem->SetFamily(parent);
	return guiElem;
}

GUICheckbox* j1Gui::CreateCheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr, const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section, const SDL_Rect * check_section, uint clickSfx, GUIElement * parent)
{
	GUICheckbox* guiElem = nullptr;
	guiElem = new GUICheckbox(position, bounds, boolPtr, text, out_section, in_section, click_section, check_section, clickSfx);
	guiElems.PushBack(guiElem);
	guiElem->SetFamily(parent);
	return guiElem;
}

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

void GUIElement::SetFamily(GUIElement* parent) {
	//if (parent == nullptr) {
	//	parent = App->gui->guiScreen;
	//}
	//this->parent = parent;
	//parent->childs.add(this);
}