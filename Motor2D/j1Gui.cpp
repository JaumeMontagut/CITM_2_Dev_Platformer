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
#include "GUIInputText.h"

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

	// load general sfx from gui_config.xml
	LoadGUISfx(gui_node);

	if (!LoadElementTemplate(buttonType1, gui_node.child("gui_element_templates").child("buttons").child("button_type_1")))
		ret = false;
	if (!LoadElementTemplate(buttonType2, gui_node.child("gui_element_templates").child("buttons").child("button_type_2")))
		ret = false;
	if (!LoadElementTemplate(checkboxType1, gui_node.child("gui_element_templates").child("checkboxes").child("checkbox_type_1")))
		ret = false;


	return ret;
}

void j1Gui::LoadGUISfx(pugi::xml_node& node)
{
	for (pugi::xml_node sfx_node = node.child("general_sfx").child("sfx"); sfx_node; sfx_node = sfx_node.next_sibling("sfx"))
	{
		GUISfx fx;
		fx.fx = App->audio->LoadFx(sfx_node.attribute("path").as_string());
		fx.fx_name.create(sfx_node.attribute("name").as_string());
		sfx.add(fx);
	}
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
			// sets offset too
			templateType.downOffset = node.child("down").attribute("offset").as_int(0);
		}
		if (node.child("disabled"))
		{
			templateType.sectionDisabled.x = node.child("disabled").attribute("x").as_int(0);
			templateType.sectionDisabled.y = node.child("disabled").attribute("y").as_int(0);
			templateType.sectionDisabled.w = node.child("disabled").attribute("w").as_int(0);
			templateType.sectionDisabled.h = node.child("disabled").attribute("h").as_int(0);
		}
		if (node.child("bounds"))
		{
			templateType.bounds = { node.child("bounds").attribute("x").as_int(0) ,node.child("bounds").attribute("y").as_int(0),
									node.child("bounds").attribute("w").as_int(0),node.child("bounds").attribute("h").as_int(0) };
		}
		else
			templateType.bounds = { 0,0,0,0 };

		
		// if we pass a checkbox element
		if (node.child("upCheck"))
		{
			CheckboxTemplates* link = &(CheckboxTemplates&)templateType;
			link->sectionUpCheck.x = node.child("upCheck").attribute("x").as_int(0);
			link->sectionUpCheck.y = node.child("upCheck").attribute("y").as_int(0);
			link->sectionUpCheck.w = node.child("upCheck").attribute("w").as_int(0);
			link->sectionUpCheck.h = node.child("upCheck").attribute("h").as_int(0);
		}
		if (node.child("hoverCheck"))
		{
			CheckboxTemplates* link = &(CheckboxTemplates&)templateType;
			link->sectionHoverCheck.x = node.child("hoverCheck").attribute("x").as_int(0);
			link->sectionHoverCheck.y = node.child("hoverCheck").attribute("y").as_int(0);
			link->sectionHoverCheck.w = node.child("hoverCheck").attribute("w").as_int(0);
			link->sectionHoverCheck.h = node.child("hoverCheck").attribute("h").as_int(0);
		}
		if (node.child("downCheck"))
		{
			CheckboxTemplates* link = &(CheckboxTemplates&)templateType;
			link->sectionDownCheck.x = node.child("downCheck").attribute("x").as_int(0);
			link->sectionDownCheck.y = node.child("downCheck").attribute("y").as_int(0);
			link->sectionDownCheck.w = node.child("downCheck").attribute("w").as_int(0);
			link->sectionDownCheck.h = node.child("downCheck").attribute("h").as_int(0);
		}

		// extra data ==========================

		// font relative  ---

		if (node.child("font"))
		{
			p2SString fontName = node.child("font").attribute("name").as_string();
			int fontSize = node.child("font").attribute("size").as_int(-1);

			templateType.font = CheckGUIFont(fontName, fontSize);

			// if we doesnt have the same font with same size loaded
			if (templateType.font == nullptr)
			{
				templateType.font = LoadGUIFont(fontName.GetString(), node.child("font").attribute("path").as_string(), fontSize);
			}

		}

		// font color
		
		if (node.child("font_color"))
		{
			pugi::xml_node colorNode = node.child("font_color");
			SDL_Color fontColor = { colorNode.attribute("r").as_int(), colorNode.attribute("g").as_int(),
									colorNode.attribute("b").as_int(), colorNode.attribute("a").as_int() };
			templateType.fontColor = fontColor;
		}
		// ======================================

		// SFX
		// checks if any desired sfx are loaded on guisfx list and associates to a template
		if (node.child("sfx"))
		{
			p2SString clickSfxName = node.child("sfx").attribute("click").as_string();
			p2SString hoverSfxName = node.child("sfx").attribute("hover").as_string();

			p2List_item<GUISfx>* item = sfx.start;
			while (item != NULL)
			{
				if (clickSfxName == item->data.fx_name)
				{
					templateType.clickSfx = item->data.fx;
				}
				if (hoverSfxName == item->data.fx_name)
				{
					templateType.hoverSfx = item->data.fx;
				}

				item = item->next;
			}
		}

		ret = true;
	}
	

	return ret;
}

_TTF_Font* j1Gui::LoadGUIFont(const char* fontName, p2SString path, int fontSize)
{
	_TTF_Font* ret = nullptr;

	// prevention
	if (fontSize < 0)
	{
		LOG("invalid font size, font not loaded");
		return ret;
	}

	ret = App->font->Load(path.GetString(), fontSize);

	if (ret != NULL) // if succesful load
	{
		// create new guifont
		GUIFonts newFont;
		newFont.font = ret;
		newFont.fontName.create(fontName);
		newFont.size = fontSize;
		fonts.add(newFont);
	}
	else
	{
		LOG("failed to load new gui font");
	}

	return ret;
	
}

_TTF_Font* j1Gui::CheckGUIFont(p2SString fontName, int fontSize) const// check if we have a specific font+size loaded on guifonts list
{
	_TTF_Font* ret = nullptr;

	p2List_item<GUIFonts>* item = fonts.start;

	while (item != NULL)
	{
		if (item->data.fontName == fontName)
		{
			if (item->data.size == fontSize)
			{
				LOG("Font coincidence");
				ret = item->data.font;
				break;
			}
		}
		item = item->next;
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

	for(int i = 0; i < fonts.Count(); ++i)
		LOG("fonts count: %i, name:%s", i + 1, fonts.At(i)->data.fontName.GetString());

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
	//Iteration
	p2List<GUIElement*> elemsToDraw;
	//1. Get the first element
	elemsToDraw.add(guiScreen);
	//2. Draw every element in the list and add their childs
	//   They will end up in a generational order (first gen first, second gen second, etc.)
	for (p2List_item<GUIElement*>* iterator = elemsToDraw.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->visible) {
			iterator->data->PostUpdate();
			iterator->data->DrawOutline();
			//If the object is not visible we simply don't get their children to draw them
			for (p2List_item<GUIElement*>* childIterator = iterator->data->childs.start; childIterator != nullptr; childIterator = childIterator->next) {
				elemsToDraw.add(childIterator->data);
			}
		}

	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	App->tex->UnloadTexture(atlas);
	/*App->tex->UnloadTexture(buttonup_texture);
	App->tex->UnloadTexture(buttondown_texture);
	App->tex->UnloadTexture(buttonhighlight_texture);*/

	guiElems.Clear(); // dynarray clears itselfs when destructor
	fonts.clear(); // free all guifonts

	// unload only gui sfx (just in case)
	p2List_item<GUISfx>* item = sfx.start;
	while (item != NULL)
	{
		App->audio->UnloadDesiredSFX(item->data.fx);
		item = item->next;
	}
	sfx.clear();

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
	if (App->gui->debugGUI) {
		App->render->DrawQuad(localPos + bounds, 255, 255, 255, 255, false, false);
	}
}

iPoint GUIElement::GetGlobalPos()
{
	//Iteration
	iPoint globalPos (0,0);
	for (GUIElement * iterator = this; iterator != nullptr; iterator = iterator->parent) {
		globalPos += iterator->localPos;
	}
	return globalPos;


	//Recursive
	//if (parent != nullptr) {
	//	return localPos + parent->GetGlobalPos();
	//}
	//return localPos;
}

GUIElement * j1Gui::CreateScreen()
{
	GUIElement* guiElem = nullptr;
	guiElem = new GUIElement(iPoint(0,0));
	guiElems.PushBack(guiElem);
	//Doesn't have to set family because it's the first element
	return guiElem;
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

GUIText* j1Gui::CreateText(const iPoint& position, const char* text, SDL_Color color, _TTF_Font* font, GUIElement * parent)
{
	GUIText* guiElem = nullptr;
	guiElem = new GUIText(position, text, color, font);
	guiElems.PushBack(guiElem);
	guiElem->SetFamily(parent);
	return guiElem;
}

GUIInputText* j1Gui::CreateInputText(const iPoint& position, const SDL_Rect &bounds, const char* text, SDL_Color color, uint size, GUIElement * parent)
{
	GUIInputText* guiElem = nullptr;
	//guiElem = new GUIInputText(position, bounds, text, color, size);
	//guiElems.PushBack(guiElem);
	//guiElem->SetFamily(parent);
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

GUIButton* j1Gui::CreateButton(ButtonTemplates& templateType, const iPoint& position, void(*clickFunction)(), const char* text, GUIElement* parent)
{
	GUIButton* guiElem = nullptr;

	guiElem = new GUIButton(position, templateType, clickFunction, text);
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
	//If no parent was detected, set it to be directly a child of the screen
	if (parent == nullptr) {
		this->parent = App->gui->guiScreen;
	}
	else {
		this->parent = parent;
	}
	this->parent->childs.add(this);
}