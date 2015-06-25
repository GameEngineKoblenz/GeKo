#pragma once
#include <glm/ext.hpp>
#include <GeKo_Gameplay/Observer/Observer.h>
#include <GeKo_Graphics/Scenegraph/Node.h>
#include <GeKo_Gameplay/Object/ObjectType.h>
#include <GeKo_Gameplay/Questsystem/Goal_Collect.h>
#include <GeKo_Gameplay/Questsystem/Counter.h>

/**This Observer handles all the collisions between two objects. Espacially the fight between AI and Player will be started here
and collisions with static objects like trees will be handled as well.*/
class CollisionObserver : public Observer<Node, Collision_Event>
{
public:
	CollisionObserver(Level* level){ m_level = level; m_counter = new Counter(0); }

	~CollisionObserver(){}

	void onNotify(Node& node, Collision_Event event)
	 {
		 std::vector<ParticleSystem*>* ps = m_level->getActiveScene()->getScenegraph()->getParticleSet();
		 switch (event)
		 {
		 case Collision_Event::COLLISION_DETECTED:
			 node.getBoundingSphere()->setCollisionDetected(true);
			 break;

		 case Collision_Event::COLLISION_KI_PLAYER:
			 node.getBoundingSphere()->setCollisionDetected(true);
			 node.getAI()->viewArea(true);
			 break;

		 case Collision_Event::NO_COLLISION_KI_PLAYER:
			 node.getBoundingSphere()->setCollisionDetected(false);
			 node.getAI()->viewArea(false);
			 for (auto particle : *ps)
			 {
				 if (particle->m_type == ParticleType::FIGHT)
				 {
					 particle->stop();
				 }
			 }
			 break;
		 }
	 }

	void onNotify(Node& nodeA, Node& nodeB, Collision_Event event)
	 {
		 if (((int)glfwGetTime() % 3) < 1 && particleFightIsStarted) particleFightIsStarted = false;

		int tp;
		std::vector<ParticleSystem*>* ps = m_level->getActiveScene()->getScenegraph()->getParticleSet();
		 switch (event)
		 {
		 case Collision_Event::COLLISION_DETECTED:
			 nodeA.getBoundingSphere()->setCollisionDetected(true);
			 break;

		 case Collision_Event::COLLISION_KI_PLAYER:
			 tp = nodeA.getAI()->getInventory()->countItem(ItemType::COOKIE);
			 nodeA.getBoundingSphere()->setCollisionDetected(true);
			 nodeA.getAI()->getGraph()->searchNode(GraphNodeType::OBJECT)->setPosition(glm::vec3(nodeB.getPlayer()->getPosition()));
			 nodeA.getAI()->viewArea(true);

			
			 if (!nodeA.getAI()->getStates(States::HEALTH))
			 {
				 nodeB.getPlayer()->eat();

				 if (tp > 0){
					nodeB.getPlayer()->collectItem(ItemType::COOKIE, nodeA.getAI()->getInventory()->countItem(ItemType::COOKIE));
					m_level->getPlayerGUI()->setTexture((char*)RESOURCES_PATH "/Texture/Cookie_02.png");
					m_level->getPlayerGUI()->getInventory()->insert(std::pair<std::string, Texture*>(std::string("Cookie"), m_level->getPlayerGUI()->getTextures()->back()));
					nodeA.getAI()->getInventory()->clearInventory();
				 }
				 

				 m_level->getActiveScene()->getScenegraph()->getRootNode()->deleteChildrenNode(nodeA.getNodeName());

				 //std::vector<ParticleSystem*>* ps = m_level->getActiveScene()->getScenegraph()->getParticleSet();
				 for (auto particle : *ps)
				 {
					 if (particle->m_type == ParticleType::SWARMOFFLIES)
					 {
						 particle->stop();
						 nodeA.getAI()->getSoundHandler()->stopSource("Flies");
					 }
				 }

				 std::vector<Goal*> tmp = m_level->getQuestHandler()->getQuests(GoalType::EATEN);

				 for (int i = 0; i < tmp.size(); i++)
				 {
					 tmp.at(i)->increase();
				 }
			 }
			 
			 break;

		 case Collision_Event::COLLISION_AI_FIGHT_PLAYER:
			 if (glm::length(nodeA.getBoundingSphere()->center - nodeB.getBoundingSphere()->center) <= 4.5)
			 {
				 if (nodeA.getAI()->getHealth() > 0)
				 {
					 //std::vector<ParticleSystem*>* ps = m_level->getActiveScene()->getScenegraph()->getParticleSet();
					 for (auto particle : *ps)
					 {
						 if (particle->m_type == ParticleType::FIGHT && !particleFightIsStarted)
						 {
							 particle->stop();
							 particle->start();
							 particleFightIsStarted = true;
							 //particle->update(*nodeB.getCamera());
							 //particle->render(*nodeB.getCamera());
						 }
						 if (particle->m_type == ParticleType::FIGHT)
						 {
							 particle->setPosition(glm::vec3(nodeB.getPlayer()->getPosition()));
							 particle->update(*nodeB.getCamera());
							 particle->render(*nodeB.getCamera());
						 }
					 }

					 if (m_counter->getTime() <= 0)
					 {
						 m_level->getFightSystem()->objectVSobject(nodeA.getAI(), nodeB.getPlayer());
						 if (nodeA.getAI()->getHealth() <= 0){

							 std::vector<Goal*> tmp = m_level->getQuestHandler()->getQuests(GoalType::KILL);
							 for (int i = 0; i < tmp.size(); i++)
							 {
								 tmp.at(i)->increase();
							 }
						 }
						 m_counter->setTime(0);
						 m_counter->start();
					 }
					 else{
						 m_counter->update();
					 }
				 }
			 }
			 break;

		 case Collision_Event::NO_COLLISION_KI_PLAYER:
			 nodeA.getBoundingSphere()->setCollisionDetected(false);
			 nodeA.getAI()->viewArea(false);

			 for (auto particle : *ps)
			 {
				 if (particle->m_type == ParticleType::FIGHT)
				 {
					 particle->stop();
				 }
			 }
			 break;

		 case Collision_Event::AI_STATIC_COLLISION:
			 if (nodeB.getStaticObject()->getObjectType() == ObjectType::TREE)
			 {
				 int count = nodeB.getStaticObject()->getInventory()->countItem(ItemType::COOKIE);
				 if (count >= 3)
				 {
					 nodeA.getAI()->collectItem(ItemType::COOKIE, 2);
					 nodeA.getAI()->eat();
				 }
				 else
				 {
					 if (count == 1)
					 {
						 nodeA.getAI()->eat();
						 nodeA.getAI()->setStates(States::HUNGER, false);
					 }
					 else if (count == 2)
					 {
						 nodeA.getAI()->eat();
						 nodeA.getAI()->setStates(States::HUNGER, false);
						 nodeA.getAI()->collectItem(ItemType::COOKIE, 1);
					 }
				 }
				 nodeB.getStaticObject()->getInventory()->reduceItem(ItemType::COOKIE, 3);
				 //TODO: Wahlweise aus ALLEN AI-FoodNodes
				 if (nodeB.getStaticObject()->getInventory()->countItem(ItemType::COOKIE) == 0){
					 nodeA.getAI()->deleteFoodNode(glm::vec3(nodeB.getStaticObject()->getPosition()));
				 }
			 }
			 if (nodeB.getStaticObject()->getObjectType() == ObjectType::HOUSE && nodeA.getAI()->getAntType() == AntType::GUARD)
			 {
				 int count = nodeB.getStaticObject()->getInventory()->countItem(ItemType::COOKIE);
				 if (count > 0)
				 {
					 nodeA.getAI()->eat();
				 }
			 }
			 break;

		 case Collision_Event::PLAYER_STATIC_COLLISION:
			 //TODO: GUI mit Bildern aktualisieren 
			 if (nodeB.getStaticObject()->getObjectType() == ObjectType::TREE)
			 {
				 int count = nodeB.getStaticObject()->getInventory()->countItem(ItemType::BRANCH);
				 nodeA.getPlayer()->collectItem(ItemType::BRANCH, count);

				 if (count != 0){
					 m_level->getPlayerGUI()->setTexture((char*)RESOURCES_PATH "/Texture/Branch_cookie.png");
					 m_level->getPlayerGUI()->getInventory()->insert(std::pair<std::string, Texture*>(std::string("Branch"), m_level->getPlayerGUI()->getTextures()->back()));
				 }

				 nodeB.getStaticObject()->getInventory()->clearInventory();
				 std::vector<Goal*> tmp = m_level->getQuestHandler()->getQuests(GoalType::COLLECT);
				 for (int i = 0; i < tmp.size(); i++)
				 {
					 if (tmp.at(i)->getItemType() == ItemType::BRANCH)
					 {
						 for (int j = 0; j < count; j++)
						 {
							 tmp.at(i)->increase();
						 }
					 }
				 }
			 }
		 }
	 }

	protected:
		Level* m_level;
		
		Counter* m_counter;

		std::vector<Texture*> m_textures;

		bool particleFightIsStarted = false;
};