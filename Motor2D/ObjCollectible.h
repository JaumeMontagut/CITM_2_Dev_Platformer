#ifndef _OBJ_COLLECTIBLE_H_
#define _OBJ_COLLECTIBLE_H_

#include "j1Object.h"
#include "p2Point.h"
//#include "p2Animation.h"
#include "PugiXml/src/pugixml.hpp"
#include "ObjPlayer.h"

struct Collider;

class ObjCollectible : public GameObject {
public:
	ObjCollectible(fPoint &position, int index, pugi::xml_node& object_node, const uint* pickfx,int objectID = -1);
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool OnDestroy() override;
	//void MarkObject(bool mark) override;

	void OnCollision(Collider * c1, Collider * c2) override;
	bool OnTriggerEnter();
	bool OnTriggerExit();

	//bool Load(pugi::xml_node& loadNode);
	//bool Save(pugi::xml_node& saveNode) const;

public:
	/*Animation inactiveAnim;
	Animation activeAnim;
	Animation * currAnim = nullptr;*/

protected:

	SDL_Rect spriteRect;
	const uint* pickSFX = nullptr;

protected:
	//

};

// ------------------------------------------------------------------------------------
// for now the two collectibles casually shares its movement pattern, but maybe we need other collectible movements
// for that is why not fits on collectible parent class for now and "repeats movement vars" for two child classes pizza and nut
class ObjPizza : public ObjCollectible
{
public:
	ObjPizza(fPoint& position, int index, pugi::xml_node& object_node, const uint* picksfx, int objectID);

	bool Update(float dt);
	void OnCollision(Collider * c1, Collider * c2) override;

	bool Save(pugi::xml_node& saveNode) const;

private:
	// movement
	float waveUp = 0.0f;
	float waveDown = 0.0f;
	float waveAmplitude = 0.0f;
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;

	// lives to increase when picked up
	int increaseLivesValue = 0;
};

class ObjNutCoins : public ObjCollectible
{
public:
	ObjNutCoins(fPoint& position, int index, pugi::xml_node& object_node, const uint* picksfx, int objectID);

	bool Update(float dt);
	void OnCollision(Collider* c1, Collider* c2) override;

	bool Save(pugi::xml_node& saveNode) const;
	
private:
	// movement
	float waveUp = 0.0f;
	float waveDown = 0.0f;
	float waveAmplitude = 0.0f;
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;

	// score to increase when picked up
	uint score = 0u;
};

#endif