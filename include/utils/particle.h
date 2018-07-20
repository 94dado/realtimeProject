#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <glm/glm.hpp>

class Particle {
public:
	glm::vec3 pos, speed;
	float life;	//if <0: particle unused
	float cameraDistance;
	bool toDraw;
	float rotationDegree;
	
	Particle(){
		life = 0.0;
		cameraDistance = 0.0f;
		toDraw = false;
		rotationDegree = 0.0f;
	}
	
	bool operator <(const Particle& that){
		return cameraDistance > that.cameraDistance;
	}
	
	bool operator >(const Particle& that){
		return cameraDistance < that.cameraDistance;
	}
};

#endif	// __PARTICLE_H__