#include <math.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "ObjPlayer.h"
#include "j1Gui.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "Brofiler/Brofiler.h"

bool j1FadeToBlack::Awake(pugi::xml_node&)
{
	name.create("FadeToBlack");

	uint width, height = 0u;
	App->win->GetWindowSize(width, height);

	screen = { 0, 0, (int)width * (int)App->win->GetScale(), (int)height * (int)App->win->GetScale() };
	return true;
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::PostUpdate()//float dt)
{
	BROFILER_CATEGORY("FadeToBlack PostUpdate", Profiler::Color::Bisque);
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	
	if (current_step == fade_step::fade_to_black) {
		fadePerCent = MIN(1.0F, ((float)now * 2.0F) / (float)total_time);
		if (now >= total_time * 0.5F)
		{ 
			// '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
			// NOTE: BECAUSE we always pass throught fade to black (when dies, swap scene, etc)
			// before we reset data, saves temporaly current score and player lives
			uint score = 0;
			int lives = 0;
			if (App->object->player != nullptr)
			{
				score = App->object->player->score;
				lives = App->object->player->lives;
			}
			// .................................................................................
			//Change scenes
			App->scene->Disable();
			App->gui->Disable();
			if (App->map->Reset())
			{
				//If the fade to black is called for loading
				if (App->want_to_load) {
					App->readyToLoad = true;
				}
				if (App->map->Load(lvl_to_load.GetString())) {
					App->scene->Enable();
					App->gui->Enable();

					// restores lives and score for player data
					if (App->object->player != nullptr)
					{
						if (lives == 0) lives = App->object->player->lives; // TODO: lives/nuts error is for this zone (inside fade to black step)
						if (score == 0) score = App->object->player->score; // 
						App->object->player->lives = lives;
						App->object->player->score = score;
						App->object->player->UpdateLives();
						App->object->player->UpdateScore();
					}
				}
			}
			App->transition = true;
			current_step = fade_step::fade_from_black;
		}
	}
	else if (current_step == fade_step::fade_from_black) {
		fadePerCent = MAX(0.0F, 2.0F - ((float)now * 2.0F) / (float)total_time);
		if (now >= total_time) {
			current_step = fade_step::none;
			fadePerCent = 0.0F;
		}
	}

	//LOG("Fade percent: %f", fadePerCent);
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(fadePerCent * 255.0F));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(const char* lvlName, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		total_time = (Uint32)(time * 1000.0F);
		start_time = SDL_GetTicks();
		lvl_to_load.create(lvlName);
		ret = true;
	}
	//else if (current_step == fade_step::fade_to_black) {
	//	total_time = (Uint32)(time * 1000.0F);
	//	start_time = SDL_GetTicks() - fadePerCent * (total_time * 0.5F);
	//	lvl_to_load = lvlName;
	//	ret = true;
	//}
	//else if (current_step == fade_step::fade_from_black) {
	//	current_step = fade_step::fade_to_black;
	//	total_time = (Uint32)(time * 1000.0F);
	//	start_time = SDL_GetTicks() - fadePerCent * (total_time * 0.5F);
	//	lvl_to_load = lvlName;
	//	ret = true;
	//}

	return ret;
}

bool j1FadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}