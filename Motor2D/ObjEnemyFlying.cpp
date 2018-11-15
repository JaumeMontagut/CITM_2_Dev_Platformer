#include "ObjEnemyFlying.h"
#include "j1Object.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1App.h"
#include "p2Animation.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "ObjPlayer.h"
#include "j1Map.h"
#include <random>

ObjEnemyFlying::ObjEnemyFlying(fPoint position, int index, pugi::xml_node &enemy_node) : GameObject(position, index) 
{

	SDL_Rect colRect = {(int)position.x, (int)position.y, 14, 22};
	collider = App->collision->AddCollider(colRect, COLLIDER_TYPE::COLLIDER_BOX, this);

	lastValidPos = position; // understands that spawn position is a valid one
	previousPos = position; // for facing direction

	// idle tile points
	minTiles = enemy_node.child("idle_path_tilePoints").attribute("minTiles").as_int();
	maxTiles = enemy_node.child("idle_path_tilePoints").attribute("maxTiles").as_int();

	// idle path pushbacks
	// generate random idle paths for every enemy
	GenerateNewIdlePath(minTiles, maxTiles); // in tiles, its counts from lastValidPos

	// get min and max frequency for "asynchronous" path calc between enemies and threads creation
	min_ms = enemy_node.child("path_frequency_ms").attribute("min_ms").as_uint();
	max_ms = enemy_node.child("path_frequency_ms").attribute("max_ms").as_uint();

	frequency_time = GetRandomValue(min_ms, max_ms); // define a initial random value too

	//Animation
	animTileWidth = enemy_node.child("animation").attribute("tile_width").as_uint();
	animTileHeight = enemy_node.child("animation").attribute("tile_height").as_uint();

	LoadAnimation(enemy_node.child("animation").child("idle_animation_searching"), idleAnimSearching);
	idleAnimSearching.speed = enemy_node.child("animation").child("idle_animation_searching").attribute("speed").as_float();
	LoadAnimation(enemy_node.child("animation").child("idle_animation_detected"), idleAnimDetected);
	idleAnimDetected.speed = enemy_node.child("animation").child("idle_animation_detected").attribute("speed").as_float();
	LoadAnimation(enemy_node.child("animation").child("idle_animation_marked"), idleAnimMarked);
	idleAnimMarked.speed = enemy_node.child("animation").child("idle_animation_marked").attribute("speed").as_float();
	LoadAnimation(enemy_node.child("animation").child("jetpack_fire_animation"), jetPackFire);
	jetPackFire.speed = enemy_node.child("animation").child("jetpack_fire_animation").attribute("speed").as_float();

	currAnim = &idleAnimSearching;

	// speed
	chasingSpeed = enemy_node.child("speed").attribute("chasing_speed").as_float();
	idleSpeed = enemy_node.child("speed").attribute("idle_speed").as_float();


}

int ObjEnemyFlying::GetRandomValue(const int min, const int max) const
{
	int value = 0;

	// recalcule new frequency time c++11 random engine
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> newRN(min, max); // distribution in range [min, max]

	value = newRN(rng);

	//LOG("random value: %i", value);

	return value;
}

bool ObjEnemyFlying::OnDestroy() {
	App->collision->DeleteCollider(collider);
	return true;
}

void ObjEnemyFlying::MarkObject(bool mark)
{
	if (mark) {
		currAnim = &idleAnimMarked;
	}
	else {
		currAnim = &idleAnimSearching;
	}
	
	marked = mark; // updates internal state
}

bool ObjEnemyFlying::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		if (pathDebugDraw) pathDebugDraw = false;
		else
			pathDebugDraw = true;
	}

	return true;
}

bool ObjEnemyFlying::Update(float dt) {


	switch (enemy_state)
	{
	case SEARCHING:
		if (isPlayerInTileRange(MIN_DISTANCE) && !marked) // minimum distance to follow player
		{
			enemy_state = enemyState::FOLLOWING;
			// updates current animation too
			currAnim = &idleAnimDetected;
		}
		// and if we have a previous still non traveled path, finish them
		if (pathData.last_path.Count() > 0 && !marked) // always that the enemy is not marked
		{
			followPath(dt);
			// updates last valid pos
			lastValidPos = position;
		}
		else
		{
			// idle default movement
			// moves on x desired tiles from lastvalid position
			// but if we mark the enemy, makes it static
			if(!marked)
				idleMovement(dt);
		}
		break;

	case FOLLOWING:
		// pathfinding
		if (SDL_GetTicks() > start_time + frequency_time && !pathData.waitingForPath)
			StartNewPathThread();
		if (pathData.last_path.Count() > 0 && isPlayerInTileRange(MAX_DISTANCE) && !marked) // minimum distance to stop follow
			followPath(dt);
		else
		{
			enemy_state = enemyState::SEARCHING;
			// updates last valid pos
			lastValidPos = position;
			//check current animation
			if (!marked)
				currAnim = &idleAnimSearching;
			else
				currAnim = &idleAnimMarked;
		}
		break;
	}

	// and check facing direction
	if(!marked)
		CheckFacingDirection();

	if (pathData.waitingForPath)
	{
		if (pathData.ready)
		{
			LOG("thread ended");
			start_time = SDL_GetTicks();
			pathData.waitingForPath = false;
			pathData.ready = false;
		}
	}

	iPoint colPos = GetRectPos(pivot::bottom_middle, position.x, position.y, animTileWidth, animTileHeight);
	collider->SetPos(colPos.x, colPos.y);

	return true;
}

void ObjEnemyFlying::idleMovement(float dt)
{

	iPoint lastValid = App->map->WorldToMap(lastValidPos.x, lastValidPos.y);
	iPoint targetTile = lastValid + *idlePath.At(posIndex);

	iPoint movement_vec = App->map->MapToWorld(targetTile.x, targetTile.y) - iPoint((int)position.x, (int)position.y);

	//LOG("movement vec: %i,%i", movement_vec.x, movement_vec.y);

	if (movement_vec == iPoint(0, 0))
	{
		//LOG("fatal error, randomness going to fail");
		GenerateNewIdlePath(minTiles, maxTiles);
	}
	else
	{
		fPoint move = { 0,0 };
		move.x = movement_vec.x;
		move.y = movement_vec.y;
		move.Normalize();

		position.x += move.x * dt * idleSpeed;
		position.y += move.y * dt * idleSpeed;

		// check if we arrived at target
		if (App->map->WorldToMap(position.x, position.y) == targetTile)
		{
			//LOG("targetreached %i,%i", targetTile.x, targetTile.y);
			//nextTravelPos *= -1;
			posIndex++;

			if (posIndex > idlePath.Count() - 1)
			{
				posIndex = 0;
				GenerateNewIdlePath(minTiles, maxTiles);
			}
		}
	}
	//LOG("EnemyPos: %f,%f", position.x, position.y);

}

bool ObjEnemyFlying::PostUpdate() {

	// draw
	iPoint blitPos = GetRectPos(pivot::bottom_middle, position.x, position.y, animTileWidth, animTileHeight);
	// jetpack fire
	if(!flip)
		App->render->Blit(App->object->robotTilesetTex, blitPos.x, blitPos.y + 16, &jetPackFire.GetCurrentFrame(), 1.0F, flip);
	else
		App->render->Blit(App->object->robotTilesetTex, blitPos.x + 10, blitPos.y + 16, &jetPackFire.GetCurrentFrame(), 1.0F, flip);
	// enemy
	App->render->Blit(App->object->robotTilesetTex, blitPos.x, blitPos.y, &currAnim->GetCurrentFrame(), 1.0F, flip);
	
	// pathfinding debug draw ---------------------------------------------------
	if (pathDebugDraw)
	{
		for (uint i = 0; i < pathData.last_path.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(pathData.last_path.At(i)->x, pathData.last_path.At(i)->y);
			App->render->Blit(App->object->debugEnemyPathTex, pos.x, pos.y);
		}
	}
	// --------------------------------------------------------------------------

	return true;
}

bool ObjEnemyFlying::Load(pugi::xml_node& node)
{
	p2SString flipDir = node.child("FlyingEnemy").attribute("flip_direction").as_string();
	// check facing direction
	if (flipDir == "true")
	{
		flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}
	else {
		flip = SDL_RendererFlip::SDL_FLIP_NONE;
	}

	return true;
}

bool ObjEnemyFlying::Save(pugi::xml_node& node) const
{
	LOG("Saving obj Enemy Flying");

	pugi::xml_node flyingEnemy = node.append_child("FlyingEnemy");

	fPoint temporalPos = position; // stores the actual position to return enemy at

	if (pathData.last_path.Count() > 0)
	{
		while (!App->pathfinding->IsWalkable(GetMapPosition())) // force to have a real walkable path
		{
			MoveToWorldNode(GetNextWorldNode(), 1.0f); // i dont care dt for save, as more delta, more fast is the travel simulation
		}
	}
	
	iPoint pos = App->map->WorldToMap((int)position.x,(int)position.y); // stick to map coords.
	pos = App->map->MapToWorld(pos.x, pos.y); // and save in world coords to easy load

	flyingEnemy.append_attribute("x") = pos.x;
	flyingEnemy.append_attribute("y") = pos.y;
	flyingEnemy.append_attribute("velocity_x") = velocity.x;
	flyingEnemy.append_attribute("velocity_y") = velocity.y;

	if (marked)
		flyingEnemy.append_attribute("isMarked") = true;

	if (flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL)
		flyingEnemy.append_attribute("flip_direction") = "true";
	else
		flyingEnemy.append_attribute("flip_direction") = "false";

	// restores the previous pos to enemy

	position = temporalPos;

	return true;
}

void ObjEnemyFlying::followPath(float dt) const
{
	iPoint nextNode = GetNextWorldNode();
	//iPoint thisPos = GetMapPosition();

	//LOG("next world node: %i,%i", nextNode.x, nextNode.y);
	//LOG("this position: %i,%i", (int)position.x, (int)position.y);

	MoveToWorldNode(nextNode, dt);

}

iPoint ObjEnemyFlying::GetMapPosition() const
{
	return App->map->WorldToMap(position.x, position.y);
}


void ObjEnemyFlying::MoveToWorldNode(const iPoint& node, float dt) const
{
	fPoint velocity_vector;
	velocity_vector.x = (int)node.x;
	velocity_vector.y = (int)node.y;

	velocity_vector -= position;
	velocity_vector.Normalize();

	//LOG("velocity %f,%f", velocity_vector.x, velocity_vector.y);

	position.x += velocity_vector.x * dt * chasingSpeed;
	position.y += velocity_vector.y * dt * chasingSpeed;

}

iPoint ObjEnemyFlying::GetNextWorldNode() const
{
	// get the enemy pos (this position) on map coords. (tile coords.)
	iPoint thisPos;
	thisPos = App->map->WorldToMap((int)position.x, (int)position.y);

	// get the nextNodePos, the last on dyn array (the first pop out) || copylastgenerated path flip the order
	iPoint nextNodePos = *pathData.last_path.At(pathData.last_path.Count() - 1);

	// compare enemy and nextNode on tile coords, if is the same, pop and get the new nextNode
	iPoint areaPoint = { 1,1 }; // tile values
	if (!(thisPos.x >= (nextNodePos.x + areaPoint.x) || (thisPos.x + 2) <= nextNodePos.x || // enemy tile width 
		thisPos.y >= (nextNodePos.y + areaPoint.y) || (thisPos.y + 3) <= nextNodePos.y)) // enemy tile height
	{
		pathData.last_path.Pop(nextNodePos); //.last_path.Pop(nextNodePos);
		//LOG("enemy are on target tile pos: tile: %i,%i enemy: %i,%i", nextNodePos.x, nextNodePos.y, thisPos.x, thisPos.y);
	}

	if (pathData.last_path.Count() > 0)
		return App->map->MapToWorld(nextNodePos.x, nextNodePos.y);
	else
		return thisPos;
}

bool ObjEnemyFlying::isPlayerInTileRange(const uint range) const
{
	// translate to map coords
	iPoint thisPos = App->map->WorldToMap((int)position.x, (int)position.y);
	iPoint playerPos = App->map->WorldToMap((int)App->object->player->position.x, (int)App->object->player->position.y);
	
	return (thisPos.DistanceManhattan(playerPos) < range);
}

void ObjEnemyFlying::GenerateNewIdlePath(const int minTiles, const int maxTiles)
{
	// and generate new random path
	idlePath.Clear();
	for (uint i = 0; i < MAX_IDLE_RPATH; ++i)
		idlePath.PushBack({ GetRandomValue(minTiles, maxTiles), GetRandomValue(minTiles, maxTiles) });
}

void ObjEnemyFlying::CheckFacingDirection()
{
	
	//LOG("previous pos x:%f current pos x: %f", previousPos.x,position.x);

	if (previousPos.x > position.x) // only on x axis
	{
		flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}
	else
	{
		flip = SDL_RendererFlip::SDL_FLIP_NONE;
	}

	previousPos = position;

}

void threadData::CopyLastGeneratedPath()
{
	const p2DynArray<iPoint>* pathToCopy = App->pathfinding->GetLastPath();

	last_path.Clear();
	for (uint i = 0; i < pathToCopy->Count(); ++i)
	{
		last_path.PushBack(*pathToCopy->At(i));
	}
	last_path.Flip();
}

void ObjEnemyFlying::StartNewPathThread()
{
	iPoint thisPos = App->map->WorldToMap((int)position.x, (int)position.y);
	iPoint playerPos = App->map->WorldToMap((int)App->object->player->position.x, (int)App->object->player->position.y);

	if (thisPos.DistanceManhattan(playerPos) > 1) // if the enemy is at more than 1 distance manhattan
	{
		pathData.origin = thisPos;
		pathData.destination = playerPos;
		pathData.index = index;
		pathData.waitingForPath = true;

		j1PathFinding* newPathfinding = new j1PathFinding();
		threadID = SDL_CreateThread(newPathfinding->multiThreadCreatePath, "test", (void*)&pathData);

		//SDL_WaitThread(threadID, 0);

		frequency_time = GetRandomValue(min_ms, max_ms);

		delete newPathfinding;
	}
}