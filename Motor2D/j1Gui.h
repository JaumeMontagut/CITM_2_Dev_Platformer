#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL/include/SDL_rect.h"

#define CURSOR_WIDTH 2

#define WHITE {255,255,255}
#define BLUE {0,0,255}
#define DEFAULT_TEXT_SIZE 12

struct SDL_Texture;
class GUIImage;
class GUIText;
class GUIButton;
class GUICheckbox;
class GUIInputText;
struct _TTF_Font;

enum class MOUSE_STATE
{
	M_OUT = 0,
	M_ENTER,
	M_IN,//Same as hover
	M_EXIT,
	MAX
};

struct GUIFonts
{
	_TTF_Font* font = nullptr;
	int size = -1;
	p2SString fontName;
};

struct ButtonTemplates
{
	// sections of atlas texture
	SDL_Rect sectionUp;
	SDL_Rect sectionHover;
	SDL_Rect sectionDown;
	SDL_Rect sectionDisabled;
	// extra data
	SDL_Rect bounds;
	p2SString fontPath;
	_TTF_Font* font = nullptr;
	int fontSize = 0;
	SDL_Color fontColor;
	uint* clickSfx = nullptr;
	uint* hoverSfx = nullptr;

};

struct CheckboxTemplates : public ButtonTemplates
{
	SDL_Rect sectionUpCheck;
	SDL_Rect sectionHoverCheck;
	SDL_Rect sectionDownCheck;
};

// maybe we need a structures of predefined elements somewhere on xml and creates the guielements using it with simple gui methods
class GUIElement
{
public:
	bool visible = true;
	bool draggable = false;
	//Position
	iPoint localPos = { 0, 0 };
	//iPoint globalPos = { 0, 0 };//We also store the global position to avoid recalculating it, adding the parent's one
	//Tree structure
	GUIElement* parent = nullptr;
	p2List<GUIElement*> childs;
	//Hovering control
	SDL_Rect bounds = { 0, 0, 0, 0 }; // stores "general" boundaries for mouse checking
	MOUSE_STATE state = MOUSE_STATE::M_OUT;

public:
	GUIElement(const iPoint& position);
	virtual bool PreUpdate();
	virtual bool PostUpdate();
	virtual bool CleanUp();

	void SetState(int x, int y);
	void SetFamily(GUIElement * parent);
	bool CheckBounds(int x, int y);
	void DrawOutline();
	iPoint GetGlobalPos();
};

enum class GUI_ADJUST
{
	//INVALID = -1,
	//IN_LEFT_UP,
	//IN_MIDDLE_UP,
	//IN_RIGHT_UP,
	//IN_LEFT_CENTER,
	//IN_MIDDLE_CENTER,
	//IN_RIGHT_CENTER,
	//IN_LEFT_DOWN,
	//IN_MIDDLE_DOWN,
	//IN_RIGHT_DOWN,
	//OUT_UP_LEFT,
	//OUT_UP_MIDDLE,
	//OUT_UP_RIGHT,
	//OUT_LEFT_UP,
	//OUT_LEFT_MIDDLE,
	//OUT_LEFT_DOWN,
	//OUT_RIGHT_UP,
	//OUT_RIGHT_MIDDLE,
	//OUT_RIGHT_DOWN,
	//OUT_DOWN_LEFT,
	//OUT_DOWN_MIDDLE,
	//OUT_DOWN_RIGHT
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	pugi::xml_node LoadConfig(pugi::xml_document& gui_config_doc) const;

	//Create GUI Objects
	GUIImage* CreateImage(const iPoint& position, const SDL_Rect & section, GUIElement * parent = nullptr);
	GUIText* CreateText(const iPoint& position, const char* text, SDL_Color color = WHITE, GUIElement * parent = nullptr);
	GUIButton* CreateButton(const iPoint & position, const SDL_Rect & bounds, void(*clickFunction)() = nullptr, const char * text = nullptr, const SDL_Rect * out_section = nullptr, const SDL_Rect * in_section = nullptr, const SDL_Rect * click_section = nullptr, uint clickSfx = 0u, GUIElement * parent = nullptr);
	GUICheckbox* CreateCheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr = nullptr, const char * text = nullptr, const SDL_Rect * out_section = nullptr, const SDL_Rect * in_section = nullptr, const SDL_Rect * click_section = nullptr, const SDL_Rect * check_section = nullptr, uint clickSFX = 0u, GUIElement * parent = nullptr);
	GUIInputText* CreateInputText(const iPoint & position, const SDL_Rect & bounds, const char* text = nullptr, SDL_Color color = WHITE, uint size = DEFAULT_TEXT_SIZE, GUIElement * parent = nullptr);

	SDL_Texture* GetAtlas() const;

private:
	bool LoadElementTemplate(ButtonTemplates& templateType, pugi::xml_node& node);

public:
	// TODO: maybe adds a structure to pack all needed textures for specific button type
	// general buttons textures
	//SDL_Texture* buttonup_texture = nullptr;
	//SDL_Texture* buttondown_texture = nullptr;
	//SDL_Texture* buttonhighlight_texture = nullptr;
	//// general checkboxes textures
	//SDL_Texture* checkbox_up_texture = nullptr;
	//SDL_Texture* checkbox_down_texture = nullptr;
	//SDL_Texture* checkbox_highlight_texture = nullptr;
	//SDL_Texture* checkbox_check_texture = nullptr;

	// TEMPLATIZED element types
	ButtonTemplates buttonType1;
	ButtonTemplates buttonType2;
	CheckboxTemplates checkboxType1;
	p2List<GUIFonts> fonts;

	GUIElement * guiScreen = nullptr;
	bool debugGUI = false;



private:
	pugi::xml_document gui_config_doc;
	pugi::xml_node gui_node;
	p2SString gui_config_filename;

	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
	// pass a node or save strings, if we delete a document node after awake phase
	// buttons filenames
	p2SString buttonup_filename;
	p2SString buttondown_filename;
	p2SString buttonhighlight_filename;
	// checkbox filenames
	p2SString checkbox_up_filename;
	p2SString checkbox_down_filename;
	p2SString checkbox_highlight_filename;
	p2SString checkbox_check_filename;
	//p2SString checkbox_check_locked_filename;
	p2DynArray<GUIElement*> guiElems = NULL;
};

#endif // __j1GUI_H__