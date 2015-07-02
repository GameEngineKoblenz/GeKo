#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Object.h"

#include "AntType.h"

///This class represents objects like trees, which are not controlled by the player or by a AI
class StaticObject : public Object
{
public: 
	StaticObject();
	~StaticObject();

	void setTree();
	void setTree(int i);
	void setExampleAntHome();

	virtual void resetDeadAnt(AntType type);

};