#include "Player.h"

//Constructor. Clears everything.
Player::Player() {
	//Initialize: Set up and clear everything in the Ball class
	m_inventory.clear();
	m_inventory.reserve(NUM_ITEMS);
	m_activeItem = (Items)0;
}

//Adds an item to the inventory
void Player::AddItem(Items item) {
	m_inventory.push_back(item);
}

//Sets the location'th item to be the active item NOTE: If a location not in the
//inventory is supplied, the active item will not change.
void Player::SetActiveItem(u32 location) {
	if(location < m_inventory.size())
		m_activeItem = (Items)location;
}

//The next item in the inventory is set as the active item
void Player::SetNextActiveItem() {
	u32 next_item = (u32)m_activeItem+1;
	if(next_item >= m_inventory.size())
		next_item = 0;

	m_activeItem = (Items)next_item;
}