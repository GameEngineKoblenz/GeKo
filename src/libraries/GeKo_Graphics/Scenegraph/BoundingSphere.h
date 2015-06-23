#pragma once
#include <vector>
#include <glm/ext.hpp>
#include <iostream>
#include <GeKo_Graphics/Geometry/Geometry.h>

/**A Bounding Sphere has a Radius and a Center-Point. With this box an easy Collision Test can be realized!*/
class BoundingSphere {
public:

	///Every Sphere needs exactly one object
	/**/
    BoundingSphere(Geometry* object, glm::mat4 modelMatrix);
	BoundingSphere(double rad, glm::vec3 cent);
    ~BoundingSphere();

	///The Center Point of the Sphere will be updated
	/**When the object moves, the center Point has to move with it, so a collision test can be possible!*/
	void update( glm::mat4 modelMatrix);

	///Will be used to set the boolean for a collision
	/**/
	void setCollisionDetected(bool collision);
	///returns true if a collision happened, false if not
	/**/
	bool getCollisionDetected();

	bool m_collisionDetected;
	//actuell center
	glm::vec3 center;
	//center from the beginning to use modelmatrix
	glm::vec3 originalCenter;
	double radius;

private:
	void createSphere(Geometry* object, glm::mat4 modelMatrix);
};