/*
Copyright (C) 2011 by Ivan Safrin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#define PI 3.14159265

#include "PolyPhysicsScreenEntity.h"

using namespace Polycode;

PhysicsScreenEntity::PhysicsScreenEntity(ScreenEntity *entity, b2World *world, float worldScale, int entType, float friction, float density, float restitution, bool isSensor, bool fixedRotation) {
	
	this->worldScale = worldScale;
	
	screenEntity = entity;
	
	bodyDef = new b2BodyDef();
	bodyDef->position.Set(screenEntity->getPosition().x/worldScale, screenEntity->getPosition().y/worldScale);
	bodyDef->angle = screenEntity->getRotation()*(PI/180.0f);	
	bodyDef->bullet = isSensor;	
	bodyDef->fixedRotation = fixedRotation;
	
	if(entType != ENTITY_STATICRECT)
		bodyDef->type = b2_dynamicBody;	
	
	body = world->CreateBody(bodyDef);
		
	b2FixtureDef fDef;
	fDef.friction = friction;
	fDef.restitution = restitution;
	fDef.density = density;
	fDef.isSensor = isSensor;
		
	switch(entType) {
		case ENTITY_STATICRECT:
		{
			b2PolygonShape b2shape;			
			b2shape.SetAsBox(screenEntity->getWidth()/(worldScale*2.0f), screenEntity->getHeight()/(worldScale*2.0f));
			fDef.shape = &b2shape;			
		}
		break;
		case ENTITY_RECT: 
		{
			b2PolygonShape b2shape;			
			b2shape.SetAsBox(screenEntity->getWidth()/(worldScale*2.0f), screenEntity->getHeight()/(worldScale*2.0f));
			fDef.shape = &b2shape;						
		}
		break;			
		case ENTITY_CIRCLE:
		{			
			b2CircleShape b2shape;
			b2shape.m_radius = screenEntity->getWidth()/(worldScale*2.0f);
			fDef.shape = &b2shape;
		}
		break;
	}
	
	fixture = body->CreateFixture(&fDef);	
	
	lastPosition.x = screenEntity->getPosition2D().x;
	lastPosition.y = screenEntity->getPosition2D().y;

	collisionOnly = false;
}

void PhysicsScreenEntity::applyTorque(float torque) {
	body->ApplyTorque(torque);
}

void PhysicsScreenEntity::applyForce(Vector2 force){
	body->SetAwake(true);
	body->ApplyForce(b2Vec2(force.x,force.y), b2Vec2(body->GetPosition().x,body->GetPosition().y));
}

ScreenEntity *PhysicsScreenEntity::getScreenEntity() {
	return screenEntity;
}
			

void PhysicsScreenEntity::setTransform(Vector2 pos, float angle) {
	body->SetTransform(b2Vec2(pos.x/worldScale, pos.y/worldScale), angle*(PI/180.0f));
}

void PhysicsScreenEntity::Update() {
	b2Vec2 position = body->GetPosition();
	float32 angle = body->GetAngle();

	
//	if(lastRotation != screenEntity->getRotation() || collisionOnly) {
//		body->SetTransform(position, screenEntity->getRotation()*(PI/180.0f));		
//	} else {
		screenEntity->setRotation(angle*(180.0f/PI));	
//	}
/*	
	if(lastPosition != screenEntity->getPosition2D() || collisionOnly) {
		b2Vec2 newPos;
		newPos.x = screenEntity->getPosition2D().x/worldScale; 
		newPos.y = screenEntity->getPosition2D().y/worldScale;				
		body->SetTransform(newPos, screenEntity->getRotation()*(PI/180.0f));
		position.x = screenEntity->getPosition2D().x/worldScale; 
		position.y = screenEntity->getPosition2D().y/worldScale; 				
	} else {
 */
		screenEntity->setPosition(position.x*worldScale, position.y*worldScale);
//	}
	
	screenEntity->dirtyMatrix(true);
	screenEntity->rebuildTransformMatrix();
	
	lastPosition.x = position.x*worldScale;
	lastPosition.y = position.y*worldScale;	
	
	lastRotation = angle * (180.0f/PI);
}

PhysicsScreenEntity::~PhysicsScreenEntity() {

}