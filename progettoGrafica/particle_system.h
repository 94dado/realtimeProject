#ifndef PARTICLE_H
#define PARTICLE_H

#include <utils/shader_v1.h>
#include <utils/camera.h>
#include <utils/texture.h>
#include <utils/gl_error.h>
#include <glad/glad.h>
#include <utils/particle.h>
#include <utils/plane.h>

#include <glm/gtx/string_cast.hpp>

#define LIFETIME 5.0f

class ParticleSystem {
private:
	double lastTime, delta;
	Shader* shader;
	Model* model;
	std::vector<Particle> particlesContainer;
	Camera* camera;
	int maxParticles, lastUsedParticle;
	float modelRotation, minRandomRotation, widthRandomRotationDegree;
	glm::vec3 randomRotationAxes;
	bool isEnabledRandomRotation;
	glm::vec3 rotationAxes, scaleVec;
	GLint modelID, normalID;
	Physics *physic;
	
	void Render();
	int FindUnusedParticle();
	void SortParticles();
	void SetupParticles();
	void UpdateParticles();
	void DrawParticles();
	
public:
	glm::vec4 particleColor;
	FixedYPlane *spawnPlane;
	glm::vec3 direction;
	
	ParticleSystem();
	ParticleSystem(int maxP, Camera* camera, Shader *shader, Model *model, FixedYPlane *plane, Physics *physic);
	void SetRotationAndScale(float degree, glm::vec3 axes, glm::vec3 scale);
	void SetColor(glm::vec4 color);
	void SetDirection(glm::vec3 direction);
	void SetParticleRotation(float minDegree, float maxDegree, glm::vec3 axes);
	void EnableParticleRotation(bool enabled);
	void Update();
};

void ParticleSystem::Render(){
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	for(int i = 0; i < maxParticles; i++) {
		Particle &p = particlesContainer[i];
		if(p.toDraw){
			p.toDraw = false;
			modelMatrix = glm::translate(modelMatrix, p.pos);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotation), rotationAxes);
			if(isEnabledRandomRotation){
				modelMatrix = glm::rotate(modelMatrix, glm::radians(p.rotationDegree), randomRotationAxes);
			}
			modelMatrix = glm::scale(modelMatrix, scaleVec);
			normalMatrix = glm::inverseTranspose(glm::mat3(camera->GetViewMatrix()*modelMatrix));
			
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glCheckError();
			glUniformMatrix3fv(glGetUniformLocation(shader->Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
			glCheckError();
			
			model->Draw(*shader);
			modelMatrix = glm::mat4(1.0f);
		}
	}
}

// Finds a Particle in ParticlesContainer which isn't used yet
int ParticleSystem::FindUnusedParticle() {
	for(int i=lastUsedParticle; i<maxParticles; i++){
		if (!particlesContainer[i].alive){
			lastUsedParticle = i;
			Particle &p = particlesContainer[i];
			if (p.rb == NULL) {
				bulletObject *rigidBody = physic->createRigidBody(PARTICLE, "", p.pos, 0.2f, glm::vec3(0.0f, p.rotationDegree, 0.0f), 1.0f, 0.2f, 0.3f, glm::vec3(0));
				rigidBody->particle = &p;
				p.rb = rigidBody->body;
			}
			return i;
		}
	}
	for(int i=0; i<lastUsedParticle; i++){
		if (!particlesContainer[i].alive){
			lastUsedParticle = i;
			Particle &p = particlesContainer[i];
			if (p.rb == NULL) {
				bulletObject *rigidBody = physic->createRigidBody(PARTICLE, "", p.pos, 0.2f, glm::vec3(0.0f, p.rotationDegree, 0.0f), 1.0f, 0.2f, 0.3f, glm::vec3(0));
				rigidBody->particle = &p;
				p.rb = rigidBody->body;
			}
			return i;
		}
	}
	return 0; // All particles are taken, override the first one
}

void ParticleSystem::SortParticles(){
	std::sort(particlesContainer.begin(), particlesContainer.end());
}
	
void ParticleSystem::SetupParticles(){
	double currentTime = glfwGetTime();
	delta = currentTime - lastTime;
	
	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);
	
	//spawn particles
	for(int i=0; i<newparticles; i++){
		int particleIndex = FindUnusedParticle();
		Particle &p = particlesContainer[particleIndex];
		
		p.alive = true;
		p.pos = spawnPlane->RandomPoint();
		if(isEnabledRandomRotation){
				p.rotationDegree = FixedYPlane::RandomFloat() * widthRandomRotationDegree + minRandomRotation;
		}
		else {
			p.rotationDegree = 0.0f;
		}
		//TODO setup rigidbody
		btTransform transform;
		transform.setOrigin(btVector3(p.pos.x, p.pos.y, p.pos.z));
		btQuaternion q;
		q.setEuler(0.0f, p.rotationDegree, 0.0f);
		transform.setRotation(q);
		p.rb->setWorldTransform(transform);
	}
}
	
void ParticleSystem::UpdateParticles(){
	double currentTime = delta + lastTime;
	// Simulate all particles
	int particlesCount = 0;
	for(int i=0; i<maxParticles; i++){
		Particle& p = particlesContainer[i];
		if(p.alive){
			// update position
			btVector3 rbPos = p.rb->getWorldTransform().getOrigin();
			p.pos = glm::vec3(rbPos[0], rbPos[1], rbPos[2]);
			p.cameraDistance = glm::length( p.pos - camera->Position );
			p.toDraw = true;
		}
	}
	lastTime = currentTime;
	SortParticles();
}

void ParticleSystem::DrawParticles(){
	//set particle color
	GLint colorID = glGetUniformLocation(shader->Program, "particleColor");
	glUniform4fv(colorID, 1, glm::value_ptr(this->particleColor));
	glCheckError();
	
	//enable color blending
	GLfloat old_blend_value;
	glGetFloatv(GL_BLEND_SRC_ALPHA, &old_blend_value);
	glCheckError();
	glEnable(GL_BLEND);
	glCheckError();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCheckError();
	
	//render all the particles
	Render();
	
	//disable color blending
	glBlendFunc(GL_SRC_ALPHA, old_blend_value);
	glCheckError();
	glDisable(GL_BLEND);
	glCheckError();
}

ParticleSystem::ParticleSystem() {}

ParticleSystem::ParticleSystem(int maxP, Camera* camera, Shader *shader, Model *model, FixedYPlane *plane, Physics *physic){
	this->maxParticles = maxP;
	this->camera = camera;
	this->shader = shader;
	this->model = model;
	this->spawnPlane = plane;
	this->physic = physic;
	
	direction = glm::vec3(0,0,0);		//no initial movement
	lastUsedParticle = 0;
	lastTime = glfwGetTime();
	modelID = glGetUniformLocation(shader->Program, "modelMatrix");
	normalID = glGetUniformLocation(shader->Program, "normalMatrix");
	isEnabledRandomRotation = false;
	
	for(int i=0; i < maxParticles; i++){
		Particle p;
		p.rb = NULL;
		particlesContainer.push_back(p);
	}
}

void ParticleSystem::SetRotationAndScale(float degree, glm::vec3 axes, glm::vec3 scale){
	this->modelRotation = degree;
	this->rotationAxes = axes;
	this->scaleVec = scale;
}

void ParticleSystem::SetColor(glm::vec4 color){
	this->particleColor = color;
}

void ParticleSystem::SetDirection(glm::vec3 direction){
	this->direction = direction;
}

void ParticleSystem::SetParticleRotation(float minDegree, float maxDegree, glm::vec3 axes){
	this->minRandomRotation = minDegree;
	this->widthRandomRotationDegree = maxDegree - minDegree;
	this->randomRotationAxes = axes;
	
}

void ParticleSystem::EnableParticleRotation(bool enabled){
	this->isEnabledRandomRotation = enabled;
}

void ParticleSystem::Update(){
	SetupParticles();
	UpdateParticles();
	DrawParticles();
}

#endif // PARTICLE_H
