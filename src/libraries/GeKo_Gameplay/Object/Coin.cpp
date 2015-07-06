#include "GeKo_Gameplay/Object/Coin.h"

Coin::Coin(){

}

Coin::Coin(Terrain *terrain, Geometry *coinGeometry, Texture *coinTexture, Node *rootNode){
	m_terrain = terrain;
	m_coinGeometry = coinGeometry;
	m_coinTexture = coinTexture;
	m_rootNode = rootNode;
	srand((unsigned int)time(NULL));

	m_coinInstance = new Node("Coin");
	m_coinInstance->addGeometry(m_coinGeometry);
	m_coinInstance->addTexture(coinTexture);
	m_coinInstance->addScale(3.0, 3.0, 3.0);
	m_coinInstance->addRotation(90.0, glm::vec3(0.0, 0.0, 1.0));
	m_coinInstance->getBoundingSphere()->radius = 0.5;
	m_staticCoin = new StaticObject();
	m_staticCoin->setObjectType(ObjectType::COIN);
	m_coinInstance->setObject(m_staticCoin);
	m_rootNode->addChildrenNode(m_coinInstance);
	//generateNewPosition();

	m_coinInstance->addTranslation(glm::vec3(100.0f, terrain->getHeight(glm::vec2(100.0f, 80.0f)) + 2.0f, 80.0f));
	m_coinInstance->getStaticObject()->setPosition(glm::vec3(100.0f, terrain->getHeight(glm::vec2(100.0f, 80.0f)) + 100.0f, 80.0f));
	//std::cout << "Coin Pos :" << m_coinInstance->getStaticObject()->getPosition().x << " " << m_coinInstance->getStaticObject()->getPosition().y << " " << m_coinInstance->getStaticObject()->getPosition().z << std::endl;
}

void Coin::generateCoin(){

	generateNewPosition();
	
}

void Coin::generateNewPosition(){
	glm::vec3 temp;
	temp.x = 5.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 190.0));
	temp.z = 5.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 190.0));
	temp.y = m_terrain->getHeight(glm::vec2(temp.x, temp.z)) + 2.0;
	m_coinInstance->addTranslation(temp);
	m_coinInstance->getStaticObject()->setPosition(temp);
	//std::cout << "Coin Pos :" << temp.x << " " << temp.y << " " << temp.z << std::endl;
}

void Coin::update(){
	m_coinInstance->addRotation(3.0, glm::vec3(1.0, 0.0, 0.0));
}