#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL/include/SDL_rect.h"
#include "p2SString.h"
#include <map>

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
class GUISlider;
struct _TTF_Font;

enum class FOCUS
{
	OUT_OF_FOCUS = 0,
	GET_FOCUS,
	ON_FOCUS,//Same as hover
	LOSE_FOCUS,
	MAX
};

// helper structure to manage fonts for guielements
struct GUIFonts
{
	_TTF_Font* font = nullptr;
	int size = -1;
	p2SString fontName;
};

// helper struct to manage gui needed sfx
struct GUISfx
{
	uint fx = 0u;
	p2SString fx_name;
	p2SString path;
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
	int moveTextDown = 0; // used for text/image displacement on y coord.
	//
	/*p2SString clickSfxPath;
	p2SString hoverSfxPath;*/
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
	//Position
	iPoint localPos = { 0, 0 };
//protected:
	//General invormation
	FOCUS state = FOCUS::OUT_OF_FOCUS;
	bool active = true;
	bool interactable = false;//Determines if the object can be interactuated with (clicked, dragged, etc). Focus only selects objects that are interactable.
	bool draggable = false;
	//Tree structure
	GUIElement * parent = nullptr;
	p2List<GUIElement*> childs;
	//Hovering control
	SDL_Rect bounds = { 0, 0, 0, 0 }; // stores "general" boundaries for mouse checking
	int ObjectID = -1; // associated with object id from basic property from tiled
	int ParentID = -1; // sets the parent of object tmx id
	p2SString object_name;
	int disableElementID = -1;
	int enableElementID = -1;

public:
	GUIElement(const iPoint& position);
	virtual bool PreUpdate();
	virtual bool PostUpdate();
	virtual bool CleanUp();

	void SetFocus(bool focus);
	void SetParent(GUIElement * parent);
	bool CheckBounds(int x, int y);
	void DrawOutline();
	void SetGlobalPos(iPoint pos);
	iPoint GetGlobalPos();
	void SetActive(bool active);
	bool IsActive();
	//Tree structure getters (we don't want other modules making changes to the tree structure, we just want them to be able to see them)
	//All changes to the tree structure should be made via the SetParent() method
	p2List<GUIElement*>* GetChilds();
	GUIElement* GetParent();
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

	void MouseFocus(p2List<GUIElement *> &generationalList, iPoint &mousePos);

	void GetGenerationalList(p2List<GUIElement *> &elems);

	void GetNextGUIElement(GUIElement * &focusedElement);
	void GetTopGUIElement(GUIElement * &focusedElement);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	pugi::xml_node LoadConfig(pugi::xml_document& gui_config_doc) const;
	_TTF_Font* CheckGUIFont(p2SString fontName, int fontSize) const; // utility to check if a specific font with specific size are already loaded on guifonts list
	_TTF_Font* LoadGUIFont(const char* fontName, p2SString fontPath, int fontSize);
	void LoadGUISfx(pugi::xml_node& node);
	//Returns a pointer to the function with the specified name
	//Used for the button functions
	void(*GetButtonFunction(p2SString functionName))();

	// load all gui elements required for current scene
	bool LoadGUI(p2SString gui_xml_path);
	// ---
	bool AssociateParentsID();
	bool LoadGUIButton(pugi::xml_node & node);
	bool LoadGUILabel(pugi::xml_node & node);
	bool LoadGUIImage(pugi::xml_node & node);
	bool LoadGUICheckbox(pugi::xml_node & node);
	bool LoadGUISlider(pugi::xml_node & node);
	void ReloadTemplatesSFX();
	//
	//bool SetGUIElementOff(p2SString name);
	//bool SetGUIElementOn(p2SString name);
	bool ToggleElementVisibility(const char* name = nullptr);//p2SString name);
	
	//Create GUI Objects
	GUIElement* CreateScreen();
	GUIImage* CreateImage(const iPoint& position, const SDL_Rect & section, GUIElement * parent = nullptr);
	GUIText* CreateText(const iPoint& position, const char* text, SDL_Color color = WHITE, int maxWidth = 256, _TTF_Font* font = nullptr, GUIElement * parent = nullptr);
	GUIButton* CreateButton(const iPoint & position, const SDL_Rect & bounds, p2SString functionName = "\0", const char * text = nullptr, const SDL_Rect * out_section = nullptr, const SDL_Rect * in_section = nullptr, const SDL_Rect * click_section = nullptr, uint clickSfx = 0u, GUIElement * parent = nullptr);
	GUIButton* CreateButton(ButtonTemplates& templateType, const iPoint& position, p2SString functionName = "\0", const char* text = nullptr, GUIElement* parent = nullptr);
	GUICheckbox* CreateCheckbox(const iPoint & position, const SDL_Rect & bounds, bool * boolPtr = nullptr, const char * text = nullptr, const SDL_Rect * outUncheckSection = nullptr, const SDL_Rect * inUncheckSection = nullptr, const SDL_Rect * clickUncheckSection = nullptr, const SDL_Rect * outCheckSection = nullptr, const SDL_Rect * inCheckSection = nullptr, const SDL_Rect * clickCheckSection = nullptr, uint clickSFX = 0u, GUIElement * parent = nullptr);
	GUICheckbox* CreateCheckbox(const iPoint& position, CheckboxTemplates& templateType, const char* text = nullptr, bool* boolPtr = nullptr, GUIElement* parent = nullptr);
	GUIInputText* CreateInputText(const iPoint & position, const SDL_Rect & bounds, const char* text = nullptr, SDL_Color color = WHITE, uint size = DEFAULT_TEXT_SIZE, GUIElement * parent = nullptr);
	GUISlider* CreateSlider(iPoint pos, SDL_Rect * boxSection, SDL_Rect * thumbSection, int sliderType = 0, float * multiplier1 = nullptr, float * multiplier2 = nullptr, GUIElement * parent = nullptr);

	SDL_Texture* GetAtlas() const;

private:
	bool LoadElementTemplate(ButtonTemplates& templateType, pugi::xml_node& node);
	void FillFunctionsMap();
	void AssociateLabel(p2SString & objectName, GUIText * label);
	float * AssociateSlider(p2SString name);

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

	// TEMPLATIZED element types ---
	ButtonTemplates buttonType1;
	ButtonTemplates buttonType2;
	CheckboxTemplates checkboxType1;
	p2List<GUIFonts> fonts;
	// SFX
	p2List<GUISfx> sfx;

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
	p2List<GUIElement*> guiElems;
	std::map<std::string, void(*)()> functionMap;
	std::map<std::string, GUIText**> labelMap;//During creation, if it finds a label with a custom property in this map, it will associate them
	iPoint lastMousePos = { 0, 0 };
	GUIElement * focusedElement = nullptr;
	bool dragging = false;
	iPoint dragOffset = { 0, 0 };
};

#endif // __j1GUI_H__