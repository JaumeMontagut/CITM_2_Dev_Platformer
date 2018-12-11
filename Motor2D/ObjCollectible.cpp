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
	// sprite section of texture rect -----
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

//bool ObjCollectible::Load(pugi::xml_node& node)
//{
//
//	return true;
//}

void ObjCollectible::OnCollision(Collider* c1, Collider* c2)
{}

//bool ObjCollectible::Save(pugi::xml_node& node) const
//{
//	LOG("Saving obj collectible");
//
//	pugi::xml_node collectibleNode = node.append_child("Collectible");
//
//	collectibleNode.append_attribute("x") = position.x;
//	collectibleNode.append_attribute("y") = position.y;
//	collectibleNode.append_attribute("id") = objectID;
//
//	return true;
//}

// childs constructors ===========================================================

ObjPizza::ObjPizza(fPoint& position,  int index, pugi::xml_node& node, uint picksfx, int objectID) 
	: ObjCollectible(position, index, node, picksfx, objectID) 
{
	original_y = position.y;
	// movement waves values
	waveUp = node.child("wave").attribute("up").as_float(1.0f);
	waveDown = node.child("wave").attribute("down").as_float(-1.0f);
	waveAmplitude = node.child("wave").attribute("amplitude").as_float(20.0f);
	// increase lives value
	increaseLivesValue = node.attribute("lives").as_int(0);
}

ObjNutCoins::ObjNutCoins(fPoint& position, int index, pugi::xml_node& node, uint picksfx, int objectID)
	: ObjCollectible(position, index, node, picksfx, objectID) 
{
	original_y = position.y;
	// movement waves values
	waveUp = node.child("wave").attribute("up").as_float(1.0f);
	waveDown = node.child("wave").attribute("down").as_float(-1.0f);
	waveAmplitude = node.child("wave").attribute("amplitude").as_float(20.0f);
	// increase score value
	score = node.attribute("score").as_int(0);
}

// ================================================================================
// PIZZA COLLECTABLE

bool ObjPizza::Update(float dt)
{
	if (going_up)
	{
		if (wave > 1.0f)
			going_up = false;
		else
			wave += waveUp * dt;
	}
	else
	{
		if (wave < -1.0f)
			going_up = true;
		else
			wave -= waveDown * dt;
	}

	position.y = original_y + int(waveAmplitude * sinf(wave));

	col->SetPos((int)position.x, (int)position.y);

	return true;
}

void ObjPizza::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		App->object->player->lives += increaseLivesValue;
		App->object->DeleteObject(this);
	}
}

bool ObjPizza::Save(pugi::xml_node& node) const
{
	LOG("Saving obj pizza");

	pugi::xml_node collectibleNode = node.append_child("pizza");

	collectibleNode.append_attribute("x") = (int)position.x;
	collectibleNode.append_attribute("y") = original_y;
	collectibleNode.append_attribute("id") = objectID;

	return true;
}
// ===================================================================================
// NUT COIN COLLECTABLE

bool ObjNutCoins::Update(float dt)
{
	if (going_up)
	{
		if (wave > 1.0f)
			going_up = false;
		else
			wave += waveUp * dt;
	}
	else
	{
		if (wave < -1.0f)
			going_up = true;
		else
			wave -= waveDown * dt;
	}

	position.y = original_y + int(waveAmplitude * sinf(wave));

	col->SetPos((int)position.x, (int)position.y);
	return true;
}

void ObjNutCoins::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		App->object->player->score += score;
		App->object->DeleteObject(this);
	}
}

bool ObjNutCoins::Save(pugi::xml_node& node) const
{
	LOG("Saving obj nutcoin");

	pugi::xml_node collectibleNode = node.append_child("nutCoin");

	collectibleNode.append_attribute("x") = (int)position.x;
	collectibleNode.append_attribute("y") = original_y;
	collectibleNode.append_attribute("id") = objectID;

	return true;
}
