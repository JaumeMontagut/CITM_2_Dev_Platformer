#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "j1Object.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "ObjPlayer.h"
#include "j1Pathfinding.h"
#include "Brofiler/Brofiler.h"

#include "j1Particles.h"
#include "j1Gui.h"
#include "GUISlider.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	bool ret = true;

	LOG("Loading Scene");

	cameraHMultiplier = node.child("camera").child("horizontal_multiplier").text().as_float();
	cameraJumpMultiplier = node.child("camera").child("jump_multiplier").text().as_float();
	cameraFallMultiplier = node.child("camera").child("fall_multiplier").text().as_float();

	uint width, height;
	App->win->GetWindowSize(width, height);
	horizontalScreenDivision = (float)width / node.child("camera").child("horizontal_screen_divisions").text().as_float();
	verticalScreenDivision = (float)height / node.child("camera").child("vertical_screen_division").text().as_float();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	// Load the first level of the list on first game start -------------------------
	if (firstStart)
	{
		p2List_item<Levels*>* levelData = App->map->data.levels.start;
		App->map->Load(levelData->data->name.GetString());
		firstStart = false;
	}
	// ----------------------------------------------------------
	if (!App->collision->IsEnabled()) { App->collision->Enable(); }
	// TODO, search a workaround to reload player info
	if (!App->object->IsEnabled()) { App->object->Enable(); }

	// check if we are on mainMenu screen and deactivates player
	if (App->map->data.loadedLevel == "main_menu.tmx")
	{
		LOG("currently on mainScreen");
		//App->object->DeleteObject(App->object->player);
		onMainScreen = true;
	}
	else // adds player
	{
		onMainScreen = false;
		//Add objects
		fPoint playerStartPos;
		playerStartPos.x = App->map->playerData.x;
		playerStartPos.y = App->map->playerData.y;
		App->object->player = App->object->AddObjPlayer(playerStartPos);
	}
		


	// create walkability map
	if (App->map->map_loaded)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->LoadTexture("maps/calculatedPathTex.png");

	// TODO, search a less ugly tornaround, maybe in module player?
	// to loads its position on every new map load

	SearchValidCameraPos();

	// loads music
	App->audio->PlayMusic(App->map->data.properties.music_name.GetString(), 0.0f);

	//Screen, needs to be created first because it's the parent as default
	App->gui->guiScreen = App->gui->CreateScreen();

	// Load all gui elements associated with this map
	if (App->map->data.properties.gui_xml_path != nullptr)
	{
		// Load all elements
		if (!App->gui->LoadGUI(App->map->data.properties.gui_xml_path))
		{
			LOG("GUI elements cannot be loaded, path or gui map xml not found or error");
		}
		// set parents
		if (!App->gui->AssociateParentsID())
		{
			LOG("failed to establish a family");
		}
	}
	if (App->object->player != nullptr) {
		App->object->player->UpdateLives();
		App->object->player->UpdateScore();
	}
	//TODO: Make it get from the name, instead of the ID (currently name is being used to get the type of GUIElement)

	// GUI elements creation example ---------------------------------------------------

	//Image example---------------------------------------------------------------------------------------------------------
	//App->gui->CreateImage(iPoint(20, 60), SDL_Rect(485, 829, 328, 103));
	/*App->gui->CreateImage(iPoint(0, 0), SDL_Rect(344, 0, 512, 385));*/
	//GUIImage* blablaImage = App->gui->CreateImage(iPoint(320, 60), SDL_Rect(133, 0, 210, 256));

	//Text example----------------------------------------------------------------------------------------------------------
	//SDL_Color textColor = { 255,13,255,255 };
	//App->gui->CreateText({ 640,360 }, "<- Banner without text, this is a ONLY TEXT element", textColor);

	//Button example--------------------------------------------------------------------------------------------------------
	/*SDL_Rect outRect ( 0, 0, 120, 40 );
	SDL_Rect inRect ( 0, 40, 120, 40 );
	SDL_Rect clickRect ( 0, 80, 120, 40 );
	const char * string = "hello button";
	App->gui->CreateButton({ 410, 100 }, SDL_Rect(0, 0, 100, 100), &SayHelloButton, string, &outRect, &inRect, &clickRect); */

	// templatized button example, wip
	//GUIElement* e = nullptr;
	//e = (GUIElement*)App->gui->CreateButton(App->gui->buttonType1, { 410, 120 }, nullptr, "TEMPLATIZED button");// ,(GUIElement*)blablaImage);
	//e->SetParent((GUIElement*)blablaImage);
	
	/*App->gui->CreateButton(App->gui->buttonType1, { 410, 220 }, nullptr, "Templatized button 2", blablaImage);
	App->gui->CreateButton(App->gui->buttonType1, { 410, 320 }, nullptr, "button", blablaImage);
	App->gui->CreateButton(App->gui->buttonType1, { 410, 420 }, nullptr, "PLAY", blablaImage);


	App->gui->CreateButton(App->gui->buttonType2, { 700, 80 }, &SayHelloButton, "blablabla");*/

	////Checkbox--------------------------------------------------------------------------------------------------------------
	/*SDL_Rect outUncheckRect (16,125,16,16);
	SDL_Rect inUncheckRect (0, 125, 16, 16);
	SDL_Rect clickUncheckRect (32, 125, 16, 16);
	SDL_Rect outCheckRect (16, 141, 16, 16);
	SDL_Rect inCheckRect (0, 141, 16, 16);
	SDL_Rect clickCheckRect (32, 141, 16, 16);
	const char * string = "hello checkbox";
	App->gui->CreateCheckbox({ 0, 0 }, { 0, 0, 100, 100 }, &App->collision->debug, string, &outUncheckRect, &inUncheckRect, &clickUncheckRect, &outCheckRect, &inCheckRect, &clickCheckRect);*/

	//App->gui->CreateCheckbox({ 0,0 }, App->gui->checkboxType1, "blabla check", &App->collision->debug);

	SDL_Rect boxRect();
	SDL_Rect thumbRect();


	return true;
}

bool j1Scene::PreUpdate() {
	//// debug pathfing ------------------
	//static iPoint origin;
	//static bool origin_selected = false;

	//int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	//p = App->map->WorldToMap(p.x, p.y);

	//if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	if (origin_selected == true)
	//	{
	//		//App->pathfinding->CreatePath(origin, p);
	//		//App->pathfinding->CreateJumpingLandPath(origin, p, 1, 1, 6);
	//		//App->pathfinding->CreatePathLand(origin, p);
	//		CopyLastGeneratedPath();
	//		origin_selected = false;
	//	}
	//	else
	//	{
	//		origin = p;
	//		origin_selected = true;
	//	}
	//}
	// // ---------------------------------

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		App->map->showNavLayer = !App->map->showNavLayer;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("SCENE UPDATE", Profiler::Color::DeepSkyBlue);
	if (!App->render->cameraDebug && !onMainScreen) {
		CameraLogic(dt);
	}

	// checks for debug input
	DebugInput();
	// ----------------------
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		//&& we're on a level or we have in game menu
		if (App->pause) {
			//Hide in game menu
			App->pause = false;
		}
		else {
			//Show in game menu
			App->pause = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !App->fade_to_black->IsFading()) {
		App->LoadGame("savegame.xml");
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		App->SaveGame("savegame.xml");
	}

	App->map->Draw();

	//// Debug pathfinding ------------------------------
	//int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	//p = App->map->WorldToMap(p.x, p.y);
	//p = App->map->MapToWorld(p.x, p.y);

	//App->render->Blit(debug_tex, p.x, p.y);

	//for (uint i = 0; i < last_path.Count(); ++i)
	//{
	//	iPoint pos = App->map->MapToWorld(last_path.At(i)->x, last_path.At(i)->y);
	//	App->render->Blit(debug_tex, pos.x, pos.y);
	//}
	//// ------------------------------------------------

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	//App->collision->Disable();
	if(App->object->IsEnabled())
		App->object->Disable();


	return true;
}

void j1Scene::CameraLogic(float dt)
{
	iPoint offset = {0, 0};
	//-- The screen is horizontally divided into 8 parts (see config.xml)
	if (App->object->player->flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL) {
		//-- Place the player on the 5th part
		offset.x = horizontalScreenDivision * 5.0f;
	}
	else {
		//-- Place the player on the 3rd part
		offset.x = horizontalScreenDivision * 3.0f;
	}
	//-- The screen is vertically divided into 6 parts (see config.xml)
	//-- Place the player on the 5th part
	offset.y = verticalScreenDivision * 5.0f;	

	iPoint playerPivotPos;
	playerPivotPos.x = -(int)(App->object->player->position.x * (int)App->win->GetScale()); // center of current player pivot
	playerPivotPos.y = -(int)(App->object->player->position.y * (int)App->win->GetScale());

	fPoint target;
	target.x = (playerPivotPos.x + (int)offset.x);
	target.y = (playerPivotPos.y + (int)offset.y);

	cameraPos.x += (target.x - App->render->camera.x) * cameraHMultiplier * dt;
	if (App->render->camera.y >= target.y) {
		cameraPos.y += (target.y - App->render->camera.y) * cameraJumpMultiplier * dt;
	}
	else {
		cameraPos.y += (target.y - App->render->camera.y) * cameraFallMultiplier  *dt;
	}

	App->render->camera.x = cameraPos.x;
	App->render->camera.y = cameraPos.y;
	
}

 void j1Scene::DebugInput()
{
	 if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		 App->particles->AddParticle(App->particles->teleport02, 450, 250, COLLIDER_ENEMY);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		// Load level 001 (It's the second one since the first one is the main menu screen)
		p2List_item<Levels*>* levelData = App->map->data.levels.start->next;
		App->fade_to_black->FadeToBlack(levelData->data->name.GetString(), 1.0f);
	}

	// START from the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade_to_black->FadeToBlack(App->map->data.loadedLevel.GetString(), 1.0f);

	// switch between levels
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		p2List_item<Levels*>* levelData = App->map->data.levels.start;

		// check current level and loads the next, if next is null, load the first one
		while (levelData != NULL)
		{
			p2SString loadedLevel = App->map->data.loadedLevel.GetString();
			p2SString compareLevel = levelData->data->name.GetString();

			if (loadedLevel == compareLevel)
			{
				LOG("coincidence");
				levelData = levelData->next;

				if (levelData == NULL)
				{
					levelData = App->map->data.levels.start;
				}
				break;
			}
			levelData = levelData->next;
		}
		
		App->fade_to_black->FadeToBlack(levelData->data->name.GetString(), 1.0f);
	}
}

 bool j1Scene::Load(pugi::xml_node& loadNode)
 {
	 LOG("Loading scene data from saved game");

	 cameraPos.x = loadNode.child("camera").attribute("x").as_float();
	 cameraPos.y = loadNode.child("camera").attribute("y").as_float();

	 return true;
 }

 bool j1Scene::Save(pugi::xml_node& saveNode) const
 {
	pugi::xml_node camNode =  saveNode.append_child("camera");
	camNode.append_attribute("x") = cameraPos.x;
	camNode.append_attribute("y") = cameraPos.y;
	
	pugi::xml_node currentLevelNode = saveNode.append_child("current_level");
	currentLevelNode.append_attribute("name") = App->map->data.loadedLevel.GetString();
	
	 return true;
 }

 bool j1Scene::SearchValidCameraPos()
 {
	 // update offset
	 uint width, height = 0;
	 App->win->GetWindowSize(width, height);

	 float x = width * 0.25f * 1.5f; // situates player on the middle of second screen partition(of 4)
	 float y = height * 0.33f *2.5f; // 

	 iPoint offset = { (int)x , (int)y };
	 // -------------
	 fPoint startPosition;
	 startPosition.x = App->map->playerData.x;
	 startPosition.y = App->map->playerData.y;

	 App->render->camera.x = cameraPos.x = (-startPosition.x * App->win->GetScale()) + offset.x;
	 App->render->camera.y = cameraPos.y = (-startPosition.y * App->win->GetScale()) + offset.y;


	 return true;
 }

 /*iPoint j1Scene::UpdateCameraOffset()
 {

 }*/

 // debug path generation with mouse
 void j1Scene::CopyLastGeneratedPath()
 {
	 const p2DynArray<iPoint>* pathToCopy = App->pathfinding->GetLastPath();

	 last_path.Clear();
	 for (uint i = 0; i < pathToCopy->Count(); ++i)
	 {
		 last_path.PushBack(*pathToCopy->At(i));
	 }
 }