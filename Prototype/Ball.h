#pragma once

#include <vector>
#include "Framework\Sprite.h"
#include "CollisionManager.h"

/*This enum should contain all the possible items the player can have.*/
enum Items {
	RED_KEY, //We'll use keys in the inventory as placeholder to show how it can be used.
	BLUE_KEY,
	YELLOW_KEY
};

/*This class will hold information for the sprite ball the player will direct through the game.*/
class Ball : public Sprite {
	private:
		std::vector<Items> m_inventory; //We'll store items in a vector.
		Items m_activeItem; //The player will have one item he has active and can "use".

	public:
		Ball();

		virtual void Collide(PhysicalEntity *other);
		virtual bool checkType(EntityType e) const;
		void Bounce(float x, float y);

		void AddItem(Items item); //Adds an item to the inventory

		//Set:
		//Sets the input value as the active item as chosen as the ith item in the inventory. NOTE: If a location not 
		//in the inventory is supplied, the active item will not change.
		void SetActiveItem(u32 location);
		void SetNextActiveItem(); //The next item in the inventory is set as the active item

		//Get:
		Items GetActiveItemLocation() const { return m_activeItem; }
		Items GetActiveItem() const { return m_inventory.at(m_activeItem); }
};