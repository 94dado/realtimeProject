#pragma once

#include  <bullet\src\btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <utils\particle.h>

enum ContactType { 
	PARTICLE, 
	MAP
};

class bulletObject {
public:
	ContactType type;
	btRigidBody* body;
	Particle* particle;

	bulletObject(btRigidBody* b, ContactType t, Particle *p) {
		type = t;
		body = b;
		particle = p;
	}
};