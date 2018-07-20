/*
Physics class - v1:
- initialization of the physics simulation using the Bullet librarty

The class sets up the collision manager and the resolver of the constraints, using basic general-purposes methods provided by the library. Advances and multithread methods are available, please consult Bullet documentation and examples

createRigidBody method sets up a  Box or Sphere Collision Shape. For other Shapes, you must extend the method.

author: Davide Gadia

Real-time Graphics Programming - a.a. 2017/2018
Master degree in Computer Science
Universita' degli Studi di Milano
*/

#pragma once

#include  <bullet\src\btBulletDynamicsCommon.h>

#include <bullet\examples\Importers\ImportObjDemo\LoadMeshFromObj.h>
#include <bullet\examples\Utils\b3ResourcePath.h>
#include <bullet\examples\ThirdPartyLibs\Wavefront\tiny_obj_loader.h>
#include <bullet\src\Bullet3Common\b3FileUtils.h>
#include <bullet\examples\OpenGLWindow\GLInstanceGraphicsShape.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

///////////////////  Physics class ///////////////////////
class Physics
{
public:

    btDiscreteDynamicsWorld* dynamicsWorld; // the main physical simulation class
    btAlignedObjectArray<btCollisionShape*> collisionShapes; // a vector for all the Collision Shapes of the scene
    btDefaultCollisionConfiguration* collisionConfiguration; // setup for the collision manager
    btCollisionDispatcher* dispatcher; // collision manager
    btBroadphaseInterface* overlappingPairCache; // method for the broadphase collision detection
    btSequentialImpulseConstraintSolver* solver; // constraints solver


    //////////////////////////////////////////
    // constructor
    // we set all the classes needed for the physical simulation
    Physics()
    {
        // Collision configuration, to be used by the collision detection class
        //collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        this->collisionConfiguration = new btDefaultCollisionConfiguration();

        //default collision dispatcher (=collision detection method). For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

        //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        this->overlappingPairCache = new btDbvtBroadphase();

        // we set a ODE solver, which considers forces, constraints, collisions etc., to calculate positions and rotations of the rigid bodies.
        //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        this->solver = new btSequentialImpulseConstraintSolver;

        //  DynamicsWorld is the main class for the physical simulation
        this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

        // we set the gravity force
        this->dynamicsWorld->setGravity(btVector3(0,-9.82,0));
    }

    //////////////////////////////////////////
    // Method for the creation of a rigid body, based on a Box or Sphere Collision Shape
   // The Collision Shape is a reference solid that approximates the shape of the actual object of the scene. The Physical simulation is applied to these solids, and the rotations and positions of these solids are used on the real models.
   btRigidBody* createRigidBody(int type, char* filename, glm::vec3 pos, glm::vec3 size, glm::vec3 rot, float m, float friction , float restitution)
    {

        btCollisionShape* cShape = NULL;

        // we convert the glm vector to a Bullet vector
        btVector3 position = btVector3(pos.x,pos.y,pos.z);

        // we set a quaternion from the Euler angles passed as parameters
        btQuaternion rotation;
        rotation.setEuler(rot.x,rot.y,rot.z);

        // Box Collision shape
        if (type == 0)
        {
            // we convert the glm vector to a Bullet vector
            btVector3 dim = btVector3(size.x,size.y,size.z);
            // BoxShape
            cShape = new btBoxShape(dim);
        }
        // Sphere Collision Shape (in this case we consider only the first component)
        else if (type == 1)
            cShape = new btSphereShape(size.x);

        // create the mesh
        else if (type == 2 && filename != "") {
            char relativeFilename [1024];
            if (b3ResourcePath::findResourcePath(filename, relativeFilename, 1024)) {
                char pathPrefix[1024];
                b3FileUtils::extractPath(relativeFilename, pathPrefix, 1024);
            }
            // load mesh from .obj
            GLInstanceGraphicsShape* glmesh = LoadMeshFromObj(relativeFilename, "");
            // generate convex hull by vertices
            const GLInstanceVertex& v = glmesh->m_vertices->at(0);
            btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(v.xyzw[0])), glmesh->m_numvertices, sizeof(GLInstanceVertex));
            shape->optimizeConvexHull();
            cShape = shape;
        }
        // we add this Collision Shape to the vector
        this->collisionShapes.push_back(cShape);

        // We set the initial transformations
        btTransform objTransform;
        objTransform.setIdentity();
        objTransform.setRotation(rotation);
        // we set the initial position (it must be equal to the position of the corresponding model of the scene)
        objTransform.setOrigin(position);

        // if objects has mass = 0 -> then it is static (it does not move and it is not subject to forces)
        btScalar mass = m;
        bool isDynamic = (mass != 0.0f);

        // if it is dynamic (mass > 0) then we calculates local inertia
        btVector3 localInertia(0.0,0.0,0.0);
        if (isDynamic)
            cShape->calculateLocalInertia(mass,localInertia);

        // we initialize the Motion State of the object on the basis of the transformations
        // Using the Motion State, the physical simulation will calculate the positions and rotations of the rigid body
        btDefaultMotionState* motionState = new btDefaultMotionState(objTransform);

        // we set the data structure for the rigid body
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,cShape,localInertia);
        // we set friction and restitution
        rbInfo.m_friction = friction;
        rbInfo.m_restitution = restitution;

        // if the Collision Shape is a sphere
        if (type == 1){
            // the sphere touches the plane on the plane on a single point, and thus the friction between sphere and the plane does not works -> the sphere does not stop
            // To avoid the problem, we apply the rolling friction together with an angular damping (which applies a resistence during the rolling movement), in order to make the sphere to stop after a while
            rbInfo.m_angularDamping =0.3;
            rbInfo.m_rollingFriction = 0.3;
        }

        // we create the rigid body
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        this->dynamicsWorld->addRigidBody(body);

        // the function returns a pointer to the created rigid body
        // in a standard simulation (e.g., only objects falling), it is not needed to have a reference to a single rigid body, but in some cases (e.g., the application of an impulse), it is needed.
        return body;
    }

    //////////////////////////////////////////
    // We delete the data of the physical simulation when the program ends
    void Clear()
    {
        //we remove the rigid bodies from the dynamics world and delete them
        for (int i=this->dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
            // we remove all the Motion States
            btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            this->dynamicsWorld->removeCollisionObject( obj );
            delete obj;
        }

        // we remove all the Collision Shapes
        for (int j=0;j<this->collisionShapes.size();j++)
        {
            btCollisionShape* shape = this->collisionShapes[j];
            this->collisionShapes[j] = 0;
            delete shape;
        }

        //delete dynamics world
        delete this->dynamicsWorld;

        //delete solver
        delete this->solver;

        //delete broadphase
        delete this->overlappingPairCache;

        //delete dispatcher
        delete this->dispatcher;

        delete this->collisionConfiguration;

        this->collisionShapes.clear();
    }
};
