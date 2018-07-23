#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <glm/glm.hpp>

class Particle {
public:
	glm::vec3 pos, speed;
	bool alive;
	float cameraDistance;
	bool toDraw;
	float rotationDegree;
	btRigidBody *rb;
	
	Particle(){
		alive = false;
		cameraDistance = 0.0f;
		toDraw = false;
		rotationDegree = 0.0f;
		rb = NULL;
	}
	
	bool operator <(const Particle& that){
		return cameraDistance > that.cameraDistance;
	}
	
	bool operator >(const Particle& that){
		return cameraDistance < that.cameraDistance;
	}
};

#endif	// __PARTICLE_H__