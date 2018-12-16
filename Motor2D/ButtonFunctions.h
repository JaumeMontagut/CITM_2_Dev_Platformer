#ifndef _BUTTON_FUNCTIONS_H_
#define _BUTTON_FUNCTIONS_H_
//Important: Don't include this file (only has to be included on Gui)

#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"

//Steps to create a function that's going to be called on button press
//In the tmx (Tiled), in the object 
//1- Go to the ui tmx (Tiled), select the button, make its Function property be the name of the function you want to call
//2- Add the function in this file
//3- Add a new association to the functionMap. Example: ["name_of_the_function"] = name_of_the_function

//Warnings:
//All functions must return void and get no parameters

//Source for this solution
//https://stackoverflow.com/questions/19473313/how-to-call-a-function-by-its-name-stdstring-in-c

void SayHelloButton()
{
	LOG("I'm a button and I say hello");
}

void PlayGame() {
	// Load level 001 (It's the second one since the first one is the main menu screen)
	p2List_item<Levels*>* levelData = App->map->data.levels.start->next;
	App->fade_to_black->FadeToBlack(levelData->data->name.GetString(), 1.0f);
}

void ContinueGame() {
	if (!App->fade_to_black->IsFading()) {
		App->LoadGame("savegame.xml");
	}
}

void ExitGame() {
	App->requestExit = true;
}

void ResumeGame() {
	App->gui->ToggleElementVisibility("returnGame");
	App->pause = false;
	App->transition = true;
}

void OpenInGameMenu() {
	App->pause = true;
	//TODO: Open InGame menu
}

void OpenMainMenu() {
	// Load level 001 (It's the second one since the first one is the main menu screen)
	p2List_item<Levels*>* levelData = App->map->data.levels.start;
	App->fade_to_black->FadeToBlack(levelData->data->name.GetString(), 1.0f);
}

void OpenSettings() {
	//p2List<GUIElement*>* elementsData = App->gui->SetParentOff(();
	App->gui->ToggleElementVisibility("OpenSettings"); // search for element with name OpenSettings and toggle the visibility of custom properties
}

void CloseSettings() {
	App->gui->ToggleElementVisibility("CloseSettings");
}

void OpenCredits() {

}

#endif
