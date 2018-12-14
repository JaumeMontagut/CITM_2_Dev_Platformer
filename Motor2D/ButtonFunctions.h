#ifndef _BUTTON_FUNCTIONS_H_
#define _BUTTON_FUNCTIONS_H_

#include "p2Log.h"

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

}

void ContinueGame() {

}

void ExitGame() {

}

void ResumeGame() {

}

void OpenMainMenu() {

}

void OpenSettings() {

}

void OpenCredits() {

}

#endif
