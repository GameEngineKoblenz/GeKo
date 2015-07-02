#include "GeKo_Gameplay/Object/StaticObject.h"

StaticObject::StaticObject(){
	m_type = ObjectType::DEFAULTOBJECT;

	m_id = 0;
	m_name = "defaultstaticobject";

	m_position = glm::vec4(0.0, 0.0, 0.0, 1.0);

	m_inventory = new Inventory();

	m_class = ClassType::STATIC;
}

StaticObject::~StaticObject(){}

void StaticObject::setTree(){
	m_type = ObjectType::TREE;
	m_name = "CookieTree";

	Item cookie(1);
	cookie.setName("Cookie");
	cookie.setTypeId(ItemType::COOKIE);
	Item branch(2);
	branch.setName("Branch");
	branch.setTypeId(ItemType::BRANCH);
	m_inventory->addItem(&cookie, 25);
	m_inventory->addItem(&branch, 50);
}

void StaticObject::setTree(int i){
	m_type = ObjectType::TREE;
	m_name = "CookieTree";

	Item cookie(1);
	cookie.setName("Cookie");
	cookie.setTypeId(ItemType::COOKIE);
	Item branch(2);
	branch.setName("Branch");
	branch.setTypeId(ItemType::BRANCH);
	m_inventory->addItem(&cookie, 25);
	m_inventory->addItem(&branch, i);
}

void StaticObject::setExampleAntHome(){
	m_type = ObjectType::HOUSE;
	m_name = "AntHome";
}

void StaticObject::resetDeadAnt(AntType type){}