#ifndef _OBJ_COLLECTIBLE_H_
#define _OBJ_COLLECTIBLE_H_

#include "j1Object.h"
#include "p2Point.h"
//#include "p2Animation.h"
#include "PugiXml/src/pugixml.hpp"

struct Collider;

class ObjCollectible : public GameObject {
public:
	ObjCollectible(fPoint &position, int index, pugi::xml_node& object_node, uint pickfx,int objectID = -1);
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool OnDestroy() override;
	//void MarkObject(bool mark) override;

	void OnCollision(Collider * c1, Collider * c2) override;
	bool OnTriggerEnter();
	bool OnTriggerExit();

	bool Load(pugi::xml_node& loadNode);
	bool Save(pugi::xml_node& saveNode) const;

public:
	/*Animation inactiveAnim;
	Animation activeAnim;
	Animation * currAnim = nullptr;*/

protected:

	SDL_Rect spriteRect;
	uint pickSFX = 0u;
	
};

// ------------------------------------------------------------------------------------

class ObjPizza : public ObjCollectible
{
public:
	ObjPizza(fPoint& position, int index, pugi::xml_node& object_node, uint picksfx, int objectID);

	bool Update(float dt);
	void OnCollision(Collider * c1, Collider * c2) override;

private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	iPoint original_pos;

};

class ObjNutCoins : public ObjCollectible
{
public:
	ObjNutCoins(fPoint& position, SDL_Rect& spriteRect ,SDL_Rect& colRect, int index, pugi::xml_node& object_node, uint picksfx, int objectID);

	void OnCollision(Collider* c1, Collider* c2) override;

};

#endif