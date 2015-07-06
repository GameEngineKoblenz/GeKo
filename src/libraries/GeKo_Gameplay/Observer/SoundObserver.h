#pragma once
#include <glm/ext.hpp>
#include <GeKo_Gameplay/Observer/Observer.h>
#include <GeKo_Graphics/Scenegraph/Level.h>
#include <GeKo_Gameplay/Questsystem/Counter.h>
#include <sstream>

/**This Observer handles all the sound actions like the move-sound or the fight-sounds.*/
class SoundObserver : public Observer<AI, Object_Event>, public Observer<Player, Object_Event>, public Observer<Node, Collision_Event>, public Observer<Quest, Quest_Event>
{
public:
	SoundObserver(Level* level){ m_level = level; m_fires = 0; m_counter = new Counter(90); m_fightMusicIsPlaying = false; }

	~SoundObserver(){}

	void  onNotify(AI& node, Object_Event event)
	{
		std::string name = node.getNodeName();
		std::string soundName;
		std::stringstream streamName;

		Node* tmp = m_level->getActiveScene()->getScenegraph()->searchNode(name);
		switch (event)
		{

		case Object_Event::OBJECT_MOVED:
			soundName = node.getSourceName(MOVESOUND_AI);
		//	node.getSoundHandler()->updateListenerPosition(node.getPosition());
			node.updateSourcesInMap();
			if (soundName != "oor")
			{
				if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->playSource(soundName);
				}
			}
			break;

		

		case Object_Event::OBJECT_STOPPED:
			soundName = node.getSourceName(MOVESOUND_AI);
			if (soundName != "oor")
			{
				if ((node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->pauseSource(soundName);
				}
			}
			break;

		case Object_Event::OBJECT_DIED:

			soundName = node.getSourceName(DEATHSOUND_AI);
			if (soundName != "oor")
			{
				if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->playSource(soundName);
				}
			}

			soundName = node.getSourceName(DEATHSOUND_FLIES_AI);
			if (soundName != "oor")
			{
				if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				{

					node.getSoundHandler()->playSource(soundName);
				}
			}

				
			break;

		}
	}

	void onNotify(Player& node, Object_Event event)
	 {
		 std::string name = node.getNodeName();
		 std::string soundName;
		 std::stringstream streamName;
		 switch (event)
		 {

		 case Object_Event::OBJECT_MOVED:
			 soundName =  node.getSourceName(MOVESOUND);
			 node.getSoundHandler()->updateListenerPosition(glm::vec3(node.getPosition()));
			 node.updateSourcesInMap();
			 if (soundName != "oor")
			 {
				 if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				 {
					 node.getSoundHandler()->playSource(soundName);
				 }
			 }
	
			 break;

		 case Object_Event::OBJECT_ROTATED:
			 node.getSoundHandler()->updateListenerOrientation(glm::vec3(node.getViewDirection()), glm::vec3(0.0, 1.0, 0.0));
			 break;

		 case Object_Event::PLAYER_LVLUP:
			 soundName = node.getSourceName(LEVELUP);
				 if (soundName != "oor")
				 {
					if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
					{
						 node.getSoundHandler()->playSource(soundName);
					 }
				 }
			 break;

		 case Object_Event::OBJECT_STOPPED:
			 soundName = node.getSourceName(MOVESOUND);
			 if (soundName != "oor")
			 {
				 if ((node.getSoundHandler()->sourceIsPlaying(soundName)))
				 {
					 node.getSoundHandler()->pauseSource(soundName);
				 }
			 }
			 break;

		 case Object_Event::PLAYER_COLLECT:
			soundName = node.getSourceName(ITEMSOUND);
			 if (soundName != "oor")
			 {
					 node.getSoundHandler()->playSource(soundName);
			 }
			 break;

		 case Object_Event::PLAYER_SET_ON_FIRE:
			 streamName << "Fire" << ++m_fires;
			 name = streamName.str();
			 node.getSoundHandler()->generateSource(name, glm::vec3(node.getPosition()), RESOURCES_PATH "/Sound/Feuer_kurz.wav");
			 node.getSoundHandler()->playSource(name);
			 streamName.str("");
			 break;
		 }
	 }

	void onNotify(Node& nodeA, Node& nodeB, Collision_Event event)
	{
		std::string fightsound;
		std::string battlesound;
		std::string backgroundsound;
		std::string soundName;
		switch (event)
		{
		case Collision_Event::COLLISION_AI_FIGHT_PLAYER:
			fightsound = nodeB.getPlayer()->getSourceName(FIGHTSOUND);
			battlesound = nodeB.getPlayer()->getSourceName(BATTLEMUSIC);
			backgroundsound = nodeB.getPlayer()->getSourceName(BACKGROUNDMUSIC);


			if (glm::length(nodeA.getBoundingSphere()->center - nodeB.getBoundingSphere()->center) <= 4.5)
			{

				if (nodeA.getAI()->getHealth() > 0)
				{
					m_counter->setTime(100);
					m_counter->start();
					m_fightMusicIsPlaying = true;

				
					if (backgroundsound != "oor")
					{
						nodeB.getPlayer()->getSoundHandler()->pauseSource(backgroundsound);
					}
					if (battlesound != "oor")
					{
						if (!nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(battlesound))
							nodeB.getPlayer()->getSoundHandler()->playSource(battlesound);
					}
					if (fightsound != "oor")
					{
						if (!nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(fightsound))
						{
							nodeB.getPlayer()->getSoundHandler()->playSource(fightsound);
						}

					}

				}
				else{
					nodeB.getPlayer()->getSoundHandler()->stopSource(fightsound);
				}
			}

			else{
				if (nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(fightsound))
				{
					nodeB.getPlayer()->getSoundHandler()->stopSource(fightsound);
				}
			}
			break;

		case Collision_Event::NO_COLLISION_KI_PLAYER:
			m_counter->update();
			if (m_counter->getTime() <= 0 && m_fightMusicIsPlaying == true)
			{
				battlesound = nodeB.getPlayer()->getSourceName(BATTLEMUSIC);
				backgroundsound = nodeB.getPlayer()->getSourceName(BACKGROUNDMUSIC);
				if (backgroundsound != "oor")
				{
					nodeB.getPlayer()->getSoundHandler()->playSource(backgroundsound);
				}
				if (battlesound != "oor")
				{
					//if (nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(battlesound))
					nodeB.getPlayer()->getSoundHandler()->pauseSource(battlesound);
				}
				m_fightMusicIsPlaying = false;
			}
			break;

		case Collision_Event::COLLISION_KI_PLAYER:

			if (!nodeA.getAI()->getStates(States::HEALTH))
			{
				soundName = nodeB.getPlayer()->getSourceName(EATSOUND);
				if (soundName != "oor")
				{
					if (!(nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(soundName)))
					{
						nodeB.getPlayer()->getSoundHandler()->playSource(soundName);
					}
				}
				soundName = nodeA.getAI()->getSourceName(DEATHSOUND_FLIES_AI);
				nodeA.getAI()->getSoundHandler()->stopSource(soundName);
			}
		}
	}

	void  onNotify(Quest& quest, Quest_Event event)
	{
		switch (event){
		case Quest_Event::QUEST_FINISHED:
			std::vector<Node*>* tmp = m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet();
			Node* tmp2;
			for (int i = 0; i < tmp->size(); i++)
			{
				if (tmp->at(i)->getType() == ClassType::PLAYER)
				{
					tmp2 = tmp->at(i);
				}
			}

			std::string soundName = tmp2->getPlayer()->getSourceName(QUESTSOUND);

			if (soundName != "oor")
			{
				tmp2->getPlayer()->getSoundHandler()->playSource(soundName);
			}
		}

	}

	void onNotify(Goal& goal, Quest_Event event)
	{
		switch (event)
		{
		case Quest_Event::GOAL_FINISHED:
			std::vector<Node*>* tmp = m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet();
			Node* tmp2;
			for (int i = 0; i < tmp->size(); i++)
			{
				if (tmp->at(i)->getType() == ClassType::PLAYER)
				{
					tmp2 = tmp->at(i);
				}
			}

			std::string soundName = tmp2->getPlayer()->getSourceName(QUESTSOUND);

			if (soundName != "oor")
			{
				tmp2->getPlayer()->getSoundHandler()->playSource(soundName);
			}
		}
	}
protected: 
	Level* m_level;
	int m_fires;
	int m_flies;
	Counter* m_counter;
	bool m_fightMusicIsPlaying;
};