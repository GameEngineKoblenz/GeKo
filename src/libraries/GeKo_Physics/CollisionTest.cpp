#include "GeKo_Physics/CollisionTest.h"

CollisionTest::CollisionTest()
{

}
CollisionTest::CollisionTest(std::vector <Node*> TestObjects){
    objects.clear();
	for(int i = 0; i < TestObjects.size(); i++){
	    objects.push_back(TestObjects.at(i));
	}
}

CollisionTest::CollisionTest(Node* rootNode)
{
	collectNodes(rootNode);
}

CollisionTest::~CollisionTest(){
    objects.clear();
}

bool CollisionTest::collides(BoundingSphere* sphere1, BoundingSphere* sphere2)
{
	double testRadius = sphere1->radius + sphere2->radius;
	glm::vec3 space = glm::vec3(sphere1->center.x - sphere2->center.x, sphere1->center.y - sphere2->center.y, sphere1->center.z - sphere2->center.z);
	float distance = glm::length(space);

	if (testRadius >= distance){
		return true;
	}
	else {

		return false;
	}
}


void CollisionTest::update()
{
	if (objects.size() == 1)
	{
		objects.at(0)->getBoundingSphere()->setCollisionDetected(false);
	}
	else
	{
		//We test each object i from the scenegraph with every other object from there.
		for (int i = 0; i < objects.size(); i++)
		{
			bool collisionDetected = false;

			for (int j = 0; j < objects.size(); j++)
			{
				//If the object is the same we do not want to test for collision.
				if (i == j){ }
				//If the terrain should be testes, we skip this step.
				else if (objects.at(i)->getNodeName() == "Plane"){  }
				//If it is not the same object...
				else if(true){
					for (int k = 0; k < objects.at(i)->getBoundingList()->size(); k++)
					{
						bool collisionBefore = objects.at(i)->getBoundingList()->at(k)->getCollisionDetected();
						bool collisionAfter = collides(objects.at(i)->getBoundingList()->at(k), objects.at(j)->getBoundingSphere());

						//If the object did not collide untill now and is colliding now, we send a notify to the observers.
						if ((!collisionBefore || collisionBefore) && collisionAfter)
						{
							//We test with which objecttype our current object is collinding.
							if (objects.at(j)->getType() == ClassType::TERRAIN)
							{
								notify(*objects.at(i), Collision_Event::PLANE_COLLISION);
							}
							
							else if (objects.at(i)->getType() == ClassType::AI & objects.at(j)->getType() == ClassType::PLAYER)
							{
								
								notify(*objects.at(i), *objects.at(j), Collision_Event::COLLISION_KI_PLAYER);
								notify(*objects.at(i), *objects.at(j), Collision_Event::COLLISION_AI_FIGHT_PLAYER);
							}

							else if (objects.at(i)->getType() == ClassType::AI & objects.at(j)->getType() == ClassType::STATIC)
							{
								notify(*objects.at(i), *objects.at(j), Collision_Event::AI_STATIC_COLLISION);
							}
							else if (objects.at(i)->getType() == ClassType::PLAYER & objects.at(j)->getType() == ClassType::STATIC)
							{
								notify(*objects.at(i), *objects.at(j), Collision_Event::PLAYER_STATIC_COLLISION);
							}

							else if (objects.at(i)->getType() == ClassType::PLAYER & objects.at(j)->getType() == ClassType::AI)
							{
								notify(*objects.at(i), Collision_Event::COLLISION_DETECTED);
							}

							collisionDetected = true;
						}

						//If there is no collision nothing happens.
						else if (!collisionBefore && !collisionAfter)
						{
						}

						//Sometimes before there was a collision and untill now there was no new collision.
						else if (collisionBefore && !collisionAfter)
						{
							if (objects.at(i)->getType() == ClassType::AI && objects.at(j)->getType() == ClassType::PLAYER)
							{
								notify(*objects.at(i), Collision_Event::NO_COLLISION_KI_PLAYER);
							}
							else
							{
								notify(*objects.at(i), Collision_Event::NO_COLLISION_DETECTED);
							}
						}
					}
				}
			}
		}
	}
}

void CollisionTest::addNode(Node* nodeObject)
{
	objects.push_back(nodeObject);
}

void CollisionTest::collectNodes(Node* root)
{
	for (int i = 0; i < root->getChildrenSet()->size(); i++)
	{
		if (root->getChildrenSet()->at(i)->hasObject()){
			addNode(root->getChildrenSet()->at(i));
			collectNodes(root->getChildrenSet()->at(i));
		}
		//TODO: If you want a collision with sound, you have to upgrade it yourself 
	}
}