#include "ObjCollectible.h"
#include "j1Object.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1App.h"
#include "p2Animation.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2Log.h"
#include "j1Audio.h"

ObjCollectible::ObjCollectible(fPoint &position, int index, pugi::xml_node& node, uint pickfx, int objectID) : 
	pickSFX(pickfx), GameObject(position, index, objectID) 
{
	/*velocity = fPoint(0.0f, 0.0f); 
	acceleration = fPoint(0.0f, 0.0f);*/

	//col = App->collision->AddCollider(colRect, COLLIDER_TYPE::COLLIDER_BOX, this);

	spriteRect.x = node.child("sprite").attribute("x").as_int(0);
	spriteRect.y = node.child("sprite").attribute("y").as_int(0);
	spriteRect.w = node.child("sprite").attribute("w").as_int(0);
	spriteRect.h = node.child("sprite").attribute("h").as_int(0);

	col = App->collision->AddCollider(spriteRect, COLLIDER_TYPE::COLLIDER_TRIGGER, this);
	col->SetPos((int)position.x, (int)position.y);

}
bool ObjCollectible::PreUpdate()
{
	return true;
}

bool ObjCollectible::Update(float dt)
{
	return true;
}

bool ObjCollectible::PostUpdate()
{

	App->render->Blit(App->object->robotTilesetTex, (int)position.x, (int)position.y, &spriteRect);

	return true;
}

bool ObjCollectible::OnTriggerEnter()
{
	return true;
}

bool ObjCollectible::OnTriggerExit()
{
	return true;
}

bool ObjCollectible::OnDestroy()
{
	App->audio->PlayFx(pickSFX);
	App->collision->DeleteCollider(col);

	return true;
}

bool ObjCollectible::Load(pugi::xml_node& node)
{

	return true;
}

void ObjCollectible::OnCollision(Collider* c1, Collider* c2)
{}

bool ObjCollectible::Save(pugi::xml_node& node) const
{
	LOG("Saving obj collectible");

	pugi::xml_node collectibleNode = node.append_child("Collectible");

	collectibleNode.append_attribute("x") = position.x;
	collectibleNode.append_attribute("y") = position.y;
	collectibleNode.append_attribute("velocity_x") = velocity.x;
	collectibleNode.append_attribute("velocity_y") = velocity.y;
	collectibleNode.append_attribute("id") = objectID;

	return true;
}

// childs constructors ----------------------------------------------------------------------------

ObjPizza::ObjPizza(fPoint& position,  int index, pugi::xml_node& object_node, uint picksfx, int objectID) 
	: ObjCollectible(position, index, object_node, picksfx, objectID) 
{
	original_y = position.y;
}

ObjNutCoins::ObjNutCoins(fPoint& position, SDL_Rect& spriteRect, SDL_Rect& colRect, int index, pugi::xml_node& object_node, uint picksfx, int objectID)
	: ObjCollectible(position, index, object_node, picksfx, objectID) {}

// ------------------------------------------------------------------------------------------------

bool ObjPizza::Update(float dt)
{

	if (going_up)
	{
		if (wave > 1.0f)
			going_up = false;
		else
			wave += 6.f * dt;
	}
	else
	{
		if (wave < -1.0f)
			going_up = true;
		else
			wave -= 9.f * dt;
	}

	position.y = original_y + int(6.0f * sinf(wave));

	return true;
}

void ObjPizza::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		// TODO: call to increment one life to player entity
		// .. code here ..
		App->object->DeleteObject(this);
	}
}

void ObjNutCoins::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		// TODO: call to increment player score
		// ...
		App->object->DeleteObject(this);
	}
}
