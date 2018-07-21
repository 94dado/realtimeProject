#pragma once

#include  <bullet\src\btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct bulletObject {
	int type;
	bool hit;
	glm::vec3 pos;
	glm::vec3 rot;
	btRigidBody* body;
	bulletObject(btRigidBody* b, int t, glm::vec3 p, glm::vec3 r) : body(b), type(t), pos(p), rot(r), hit(false) {};
};