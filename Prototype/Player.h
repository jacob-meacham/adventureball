#pragma once

#include <vector>
#include "..\Framework\Sprite.h"


#define NUM_ITEMS 5 //Max number of items currently supported by the game. Set low for testing.

/*This enum should contain all the possible items the player can have.*/
enum Items {
	RED_KEY, //We'll use keys in the inventory as placeholder to show how it can be used.
	BLUE_KEY,
	YELLOW_KEY
};

/*The Player class can also be thought of as the player class. It maintains progress specific to the player.
The class will hold inventory. DESIGN: Maybe this class should also keep track of rooms the player has
visited so a map can be created showing only the areas visited. May also want to keep info for saving the game
in this class.*/
class Player {
	private:
		std::vector<Items> m_inventory; //We'll store items in a vector.
		Items m_activeItem; //The player will have one item he has active and can "use".

	public:
		Player(); //Constructor. Clears everything.
		void AddItem(Items item); //Adds an item to the inventory

		//Set:
		//Sets the input value as the active item as chosen as the ith item in the inventory. NOTE: If a location not 
		//in the inventory is supplied, the active item will not change.
		void SetActiveItem(u32 location);
		void SetNextActiveItem(); //The next item in the inventory is set as the active item

		//Get:
		Items GetActiveItemLocation() { return m_activeItem; }
		Items GetActiveItem() { return m_inventory.at(m_activeItem); }

};