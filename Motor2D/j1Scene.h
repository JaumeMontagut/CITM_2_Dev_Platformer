#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"

struct SDL_Texture;
class GUIText;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void CameraLogic(float dt);

	bool SearchValidCameraPos();

	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;

private:
	void DebugInput();
	// debug path generation with mouse
	p2DynArray<iPoint> last_path = NULL;
	void CopyLastGeneratedPath();

public:
	GUIText * coinsText = nullptr;
	GUIText * livesText = nullptr;
	GUIText * timeText = nullptr;
	float volumeMultiplier = 0.5f;
	GUIText * creditsText = nullptr;
	int creditsStartPosition = -1;

private:
	int creditsScrollSpeed = 40;
	float creditsYPos;//Can't have smooth transition only with ints

	fPoint cameraPos = { 0,0 };
	bool firstStart = true;
	bool teleport = false; // testing var, must to be player relative
	SDL_Texture* debug_tex = nullptr;
	//Factor for which the distance to the target position is going to be multiplied
	float	cameraHMultiplier = 0.0f;//When moving horizontally
	float	cameraJumpMultiplier = 0.0f;//When jumping
	float	cameraFallMultiplier = 0.0f;//When falling
	float	horizontalScreenDivision = 0.0f;
	float	verticalScreenDivision = 0.0f;

	//
	bool onMainScreen = false;
};

#endif // __j1SCENE_H__