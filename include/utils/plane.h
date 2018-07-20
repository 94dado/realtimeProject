#ifndef __PLANE_H__
#define __PLANE_H__

#include <glm/glm.hpp>

class FixedYPlane {
public:
	float minX, minZ, maxX, maxZ, y;
	float width, length;	//width on x-axe, length on z-axe
	
	FixedYPlane(glm::vec2 min, glm::vec2 max, float y){
		minX = min.x;
		minZ = min.y;
		maxX = max.x;
		maxZ = max.y;
		this->y = y;
		
		width = maxX-minX;
		length = maxZ-minZ;
	}
	
	bool IsInsidePlane(glm::vec3 point){
		if(point.y != y) return false;
		return IsInsidePlane(glm::vec2(point.x, point.z));
	}
	
	bool IsInsidePlane(glm::vec2 point){
		if (point.x < minX || point.x > maxX) return false;
		if (point.y < minZ || point.y > maxZ) return false;
		return true;
	}
	
	static float RandomFloat(){	//return number between 0 and 1
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
	
	glm::vec3 RandomPoint(){
		glm::vec2 point = Random2DPoint();
		return glm::vec3(point.x, y, point.y);
	}
	
private:
	glm::vec2 Random2DPoint(){
		float x = RandomFloat() * width + minX;
		float z = RandomFloat() * length + minZ;
		return glm::vec2(x,z);
	}
};

#endif // __PLANE_H__