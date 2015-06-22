#include "GeKo_Gameplay/Item/Inventory.h"


Inventory::Inventory(){
	std::pair<int, ItemType> p (NULL, ItemType::DEFAULTITEM);
	m_content.push_back(p);

	m_id = 0;
}

Inventory::~Inventory(){}

void Inventory::addItem(Item* item){
	addItem(item->getTypeId());
}

void Inventory::addItem(ItemType typeId){
	bool exist = false;

	if (m_content.at(0).second == ItemType::DEFAULTITEM || m_content.empty()){
		m_content.pop_back();
		std::pair<int, ItemType> p(1, typeId);
		m_content.push_back(p);
	}
	else {
		for (int i = 0; i < m_content.size(); i++){

			if (m_content.at(i).second == typeId){
				m_content.at(i).first++;
				exist = (m_content.at(i).second == typeId);
			}
		}
		if (!exist){
			std::pair<int, ItemType> p(1, typeId);
			m_content.push_back(p);
		}
	}
}

void Inventory::addItem(Item* item, int count){
	for (int i = 0; i < count; i++){
		addItem(item->getTypeId());
	}
}

void Inventory::print(){
	std::cout << "Inventory: " << std::endl;
	for (int i = 0; i < m_content.size(); i++){
		std::cout << "Item Type: ";
		switch (m_content.at(i).second){
		case ItemType::DEFAULTITEM: std::cout << "Default"; break;
		case ItemType::BRANCH: std::cout << "Branch"; break;
		case ItemType::COOKIE: std::cout << "Cookie"; break;
		case ItemType::HONEY: std::cout << "Honey"; break;
		}
		std::cout << "; Count: " << m_content.at(i).first << std::endl;
	}
}

ItemType Inventory::getItemTypeIdAt(int position){
	if (position < m_content.size()){
		if (m_content.at(position).first != NULL && !m_content.empty()){
			ItemType typeID = m_content.at(position).second;
			reduceItem(position, 1);
			return typeID;
		}
	}
	return ItemType::DEFAULTITEM;
}

void Inventory::deleteItem(int position){
	//TODO: Fix
	//This method deletes the content of the position, but don´t delete the position... Or so...
	//So that there comes an error, if you delete the item and after that add the same item again!
	m_content.erase(m_content.begin() + position);
}

/// Attention: Deletes all the items with the typeId!!!
void Inventory::deleteItem(Item* item){
	ItemType typeID = item->getTypeId();
	for (int i = 0; i < m_content.size(); i++){
		if (m_content.at(i).second == typeID){
			deleteItem(i);
		}
	}
}

void Inventory::reduceItem(int position, int count){
	for (int i = 0; i < count; i++){
		m_content.at(position).first--;
	}
	if (m_content.at(position).first <= 0){
		m_content.at(position).first = 0;
		//deleteItem(position);
	}
}

void Inventory::reduceItem(ItemType typeId, int count){
	for (int i = 0; i < m_content.size(); i++){
		if (m_content.at(i).second == typeId){
			reduceItem(i, count);
		}
	}
}

void Inventory::clearInventory(){
	m_content.clear();
}

int Inventory::countItem(ItemType typeId){
	for (int i = 0; i < m_content.size(); i++){
		if (m_content.at(i).second == typeId){
			return m_content.at(i).first;
		}
	}
	return 0;
}