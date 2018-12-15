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
#include "Brofiler/Brofiler.h"
#include "ButtonFunctions.h"
#include <map>
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

	FillFunctionsList();

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
			templateType.moveTextDown = node.child("down").attribute("offset").as_int(0);
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
		else {
			templateType.bounds = { 0,0,0,0 };
		}
		if (node.child("move_text_down")) {
			templateType.moveTextDown = node.child("move_text_down").attribute("amount").as_int(0);
		}
		else {
			templateType.moveTextDown = 0;
		}

		
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

void j1Gui::FillFunctionsList()
{
	functionMap["SayHelloButton"] = &SayHelloButton;
	//functionMap["PlayGame"] = &PlayGame;
	//functionMap["ContinueGame"] = &ContinueGame;
	//functionMap["ExitGame"] = &ExitGame;
	//functionMap["ResumeGame"] = &ResumeGame;
	//functionMap["OpenMainMenu"] = &OpenMainMenu;
	//functionMap["OpenSettings"] = &OpenSettings;
	//functionMap["OpenCredits"] = &OpenCredits;
}

void (*j1Gui::GetButtonFunction(p2SString functionName))() {
	return functionMap[functionName.GetString()];
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
	BROFILER_CATEGORY("GUI", Profiler::Color::DarkOliveGreen);
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debugGUI = !debugGUI;
	}

	//Check and updates mouse state -----------------------
	int mouse_x, mouse_y = 0;
	App->input->GetMousePosition(mouse_x, mouse_y);
	mouse_x *= (int)App->win->GetScale();
	mouse_y *= (int)App->win->GetScale();

	//Iteration
	p2List<GUIElement*> elems;
	//1. Get the first element
	elems.add(guiScreen);
	//2. Draw every element in the list and add their childs
	//   They will end up in a generational order (first gen first, second gen second, etc.)
	for (p2List_item<GUIElement*>* iterator = elems.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->IsActive()) {
			iterator->data->SetState(mouse_x, mouse_y);
			iterator->data->PreUpdate();
			//If the object is not visible we simply don't get their children to draw them
			for (p2List_item<GUIElement*>* childIterator = iterator->data->GetChilds()->start; childIterator != nullptr; childIterator = childIterator->next) {
				elems.add(childIterator->data);
			}
		}
	}

	//for (int i = 0; i < fonts.Count(); ++i) {
	//	LOG("fonts count: %i, name:%s", i + 1, fonts.At(i)->data.fontName.GetString());
	//}

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
	BROFILER_CATEGORY("GUI", Profiler::Color::DarkOliveGreen);
	//Iteration
	p2List<GUIElement*> elemsToDraw;
	//1. Get the first element
	elemsToDraw.add(guiScreen);
	//2. Draw every element in the list and add their childs
	//   They will end up in a generational order (first gen first, second gen second, etc.)
	for (p2List_item<GUIElement*>* iterator = elemsToDraw.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->IsActive()) {
			iterator->data->PostUpdate();
			iterator->data->DrawOutline();
			//If the object is not visible we simply don't get their children to draw them
			for (p2List_item<GUIElement*>* childIterator = iterator->data->GetChilds()->start; childIterator != nullptr; childIterator = childIterator->next) {
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
		//Iteration
		iPoint globalPos(0, 0);
		for (GUIElement * iterator = this; iterator != nullptr; iterator = iterator->parent) {
			globalPos += iterator->localPos;
		}
		App->render->DrawQuad(globalPos + bounds, 255, 255, 255, 255, false, false);
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


	////Recursive
	//if (parent != nullptr) {
	//	return localPos + parent->GetGlobalPos();
	//}
	//return localPos;
}

void GUIElement::SetActive(bool active)
{
	if (active == false) {
		state == MOUSE_STATE::M_OUT;//When it is deactivated, it loses focus
	}
	this->active = active;
}

bool GUIElement::IsActive()
{
	return active;
}

p2List<GUIElement*>* GUIElement::GetChilds()
{
	return &childs;
}

GUIElement * GUIElement::GetParent()
{
	return parent;
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
	guiElem->SetParent(parent);
	return guiElem;
}

GUIText* j1Gui::CreateText(const iPoint& position, const char* text, SDL_Color color, _TTF_Font* font, GUIElement * parent)
{
	GUIText* guiElem = nullptr;
	guiElem = new GUIText(position, text, color, font);
	guiElems.PushBack(guiElem);
	guiElem->SetParent(parent);
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

GUIButton* j1Gui::CreateButton(const iPoint & position, const SDL_Rect & bounds, p2SString functionName, const char * text, const SDL_Rect * out_section, const SDL_Rect * in_section, const SDL_Rect * click_section, uint clickSfx, GUIElement * parent)
{
	GUIButton* guiElem = nullptr;
	guiElem = new GUIButton(position, bounds, functionName, text, out_section, in_section, click_section);
	guiElems.PushBack(guiElem);
	guiElem->SetParent(parent);
	return guiElem;
}

GUIButton* j1Gui::CreateButton(ButtonTemplates& templateType, const iPoint& position, p2SString functionName, const char* text, GUIElement* parent)
{
	GUIButton* guiElem = nullptr;

	guiElem = new GUIButton(position, templateType, functionName, text);
	guiElems.PushBack(guiElem);
	guiElem->SetParent(parent);

	return guiElem;

}

GUICheckbox* j1Gui::CreateCheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr, const char * text, const SDL_Rect * outUncheckSection, const SDL_Rect * inUncheckSection, const SDL_Rect * clickUncheckSection, const SDL_Rect * outCheckSection, const SDL_Rect * inCheckSection, const SDL_Rect * clickCheckSection, uint clickSfx, GUIElement * parent)
{
	GUICheckbox* guiElem = nullptr;
	guiElem = new GUICheckbox(position, bounds, boolPtr, text, outUncheckSection, inUncheckSection, clickUncheckSection, outCheckSection, inCheckSection, clickCheckSection, clickSfx);
	guiElems.PushBack(guiElem);
	guiElem->SetParent(parent);
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

void GUIElement::SetParent(GUIElement* parent) {
	if (App->gui->guiScreen == nullptr) {
		LOG("Error, screen was not created, please create all GUIElements after the GUIScreen has been created");
	}
	assert(App->gui->guiScreen != nullptr);
	//If no parent was detected, set it to be directly a child of the screen
	if (parent == nullptr) {
		this->parent = App->gui->guiScreen;
		this->localPos -= this->parent->localPos;
	}
	else {
		this->parent = parent;
		this->localPos -= parent->localPos;
	}
	this->parent->childs.add(this);
}

// =================================================================================

bool j1Gui::LoadGUI(p2SString gui_xml_path)
{
	bool ret = true;

	pugi::xml_document gui_xml_doc;
	pugi::xml_parse_result result = gui_xml_doc.load_file(gui_xml_path.GetString());

	// fast prevention
	if (result == NULL)
	{
		LOG("Could not load gui map xml file config.xml. pugi error: %s", result.description());
		//return false;
		ret = false;
	}

	pugi::xml_node gui_tmx_node = gui_xml_doc.child("map");

	// travel all possible object groups (for if we decide at end add more layer objects for different gui templates)

	for (pugi::xml_node objectGroup = gui_tmx_node.child("objectgroup"); objectGroup && ret; objectGroup = objectGroup.next_sibling("objectgroup"))
	{
		//p2SString tmp = objectGroup.attribute("name").as_string();

		// iterate all objects
		for (pugi::xml_node object = objectGroup.child("object"); object; object = object.next_sibling("object"))
		{
			// detects gui element type desired to load
			p2SString gui_element_type = object.attribute("name").as_string();

			// different loaders for buttons, images, checkboxes, etc
			if (gui_element_type == "button")
			{
				// loads a button with desired parameters
				if (!LoadGUIButton(object))//.child("properties")))
				{
					LOG("failed to load gui element button");
				}

			}
			else if (gui_element_type == "image")
			{
				if (!LoadGUIImage(object))
				{
					LOG("failed to load gui element image");
				}

			}
		}
	}


	return ret;
}

bool j1Gui::LoadGUIImage(pugi::xml_node& node)
{
	bool ret = true;

	GUIImage* newImage = nullptr;

	iPoint position;
	position.x = node.attribute("x").as_int();
	position.y = node.attribute("y").as_int();

	// stores object id
	int object_tiled_id = node.attribute("id").as_int(-1); // id to search and set family

	// acces to extra properties
	pugi::xml_node propertiesNode = node.child("properties");
	if (propertiesNode == NULL)
	{
		LOG("properties not found");
		ret = false;
	}
	else
	{
		SDL_Rect section;
		section.x = propertiesNode.find_child_by_attribute("name", "section_rect_x").attribute("value").as_int();
		section.y = propertiesNode.find_child_by_attribute("name", "section_rect_y").attribute("value").as_int();
		section.w = propertiesNode.find_child_by_attribute("name", "section_rect_w").attribute("value").as_int();
		section.h = propertiesNode.find_child_by_attribute("name", "section_rect_h").attribute("value").as_int();

		newImage = new GUIImage(position, section);

		// assign the rest of extra properties
		newImage->draggable = propertiesNode.find_child_by_attribute("name", "draggable").attribute("value").as_bool(true);
		newImage->interactable = propertiesNode.find_child_by_attribute("name", "interactable").attribute("value").as_bool(true);
		newImage->active = propertiesNode.find_child_by_attribute("name", "visible").attribute("value").as_bool(true);
		newImage->ObjectID = object_tiled_id;
		newImage->ParentID = propertiesNode.find_child_by_attribute("name", "parentID").attribute("value").as_int(-1);

		//newImage->SetParent(nullptr);
		guiElems.PushBack(newImage);

	}

	return ret;

}

bool j1Gui::LoadGUIButton(pugi::xml_node& node)
{
	bool ret = true;

	p2SString stringComparer = node.attribute("type").as_string();

	// check type of templatetype (if's for now) TODO: if we have time improve this with some enum and improved template creation
	// maybe adds a list of templates with enum and some assert for prevention
	// maybe adds complete template import directly from tmx, but we use for now the templates method currently in use

	ButtonTemplates* templatePtr = nullptr;

	if (stringComparer == "templateType1")
	{
		LOG("meec");
		templatePtr = &buttonType1;
	}
	else if (stringComparer == "templateType2")
	{
		templatePtr = &buttonType2;
	}
	else
	{
		LOG("templatetype not found, failed");
		return false;
	}
	// and so on
	// ...

	// -----------------

	// assigns all extra flags and creates gui button element

	GUIButton* newButton = nullptr;

	// gets position
	iPoint position;
	position.x = node.attribute("x").as_int(0);
	position.y = node.attribute("y").as_int(0);
	/* maybe on next upgrade we get the boundaries directly here, but for now included on templatetype on gui_config.xml */
	
	// stores object id
	int object_tiled_id = node.attribute("id").as_int(-1); // id to search and set family

	// acces to extra properties
	// stores string text
	p2SString text;
	pugi::xml_node propertiesNode = node.child("properties");
	if (propertiesNode == NULL)
	{
		LOG("properties not found");
		ret = false;
	}
	else
	{
		text = propertiesNode.find_child_by_attribute("name", "text").attribute("value").as_string("");
	}
	p2SString functionName(propertiesNode.find_child_by_attribute("name", "function").attribute("value").as_string("\0"));
	newButton = new GUIButton(position, *templatePtr, functionName, text.GetString());
	// adds object id
	newButton->ObjectID = object_tiled_id;

	// assign the rest of extra properties
	newButton->draggable = propertiesNode.find_child_by_attribute("name", "draggable").attribute("value").as_bool(true);
	newButton->interactable = propertiesNode.find_child_by_attribute("name", "interactable").attribute("value").as_bool(true);
	newButton->active = propertiesNode.find_child_by_attribute("name", "visible").attribute("value").as_bool(true);
	newButton->ParentID = propertiesNode.find_child_by_attribute("name", "parentID").attribute("value").as_int(-1);
	// ... if we are more needed properties for something
	// ...
	
	// adds default parent (screen)
	// default parent are added when all gui objects are loaded
	// scene calls AssociateParentsID and links them
	//newButton->SetParent(nullptr);
	
	// adds button element to list
	guiElems.PushBack(newButton);

	return true;
}

bool j1Gui::AssociateParentsID()
{
	bool ret = true;

	// search and set family to each gui objet who wants it
	// REMEMBER: each gui element created by parent call the object id is -1, skip this, the parent is already set
	
	LOG("count: %i", guiElems.Count());

	
	p2List<GUIElement*> e;

	// filter
	for (int i = 0; i < guiElems.Count(); ++i)
	{
		if (guiElems[i]->ObjectID == -1)
			continue;

		e.add(guiElems[i]);
	}

	LOG("real count %i", e.Count());

	p2List_item<GUIElement*>* itemC1 = e.start;
	//p2List_item<GUIElement*>* itemC2 = e.start;

	for(;itemC1; itemC1=itemC1->next)
	{
		LOG("%i", itemC1->data->ObjectID);
		for (p2List_item<GUIElement*>* itemC2 = e.start; itemC2; itemC2 = itemC2->next)
		{
			if (itemC1->data->ParentID == itemC2->data->ObjectID)
			{
				itemC1->data->SetParent(itemC2->data);
			}

			//// 
			//if (itemC2 == e.end)
			//{
			//	itemC1->data->SetParent(nullptr);
			//}
		}
	}

	for (itemC1 = e.start; itemC1; itemC1 = itemC1->next)
	{
		if (itemC1->data->parent == nullptr)
			itemC1->data->SetParent(nullptr);
	}


	/*for (int i = 0; i < guiElems.Count(); ++i)
	{
		if (guiElems[i]->ObjectID == -1)
			continue;

		LOG("id %i", guiElems[i]->ObjectID);
		for (int j = 0; j < guiElems.Count() && j != i; ++j)
		{
			if (guiElems[j]->ObjectID == -1)
				continue;

			if (guiElems[i]->ParentID == guiElems[j]->ObjectID)
			{
				guiElems[i]->SetParent(guiElems[j]);
				LOG("parent found and linked");
				LOG("parent id: %i", guiElems[i]->ParentID);
			}
		}
	}*/

	return ret;
}