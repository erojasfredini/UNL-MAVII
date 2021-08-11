#include "MyContactListener.h"
#include <iostream>

MyContactListener::MyContactListener()
{ }

MyContactListener::~MyContactListener()
{ }

void MyContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* aux1 = contact->GetFixtureA();
	int* bodyAData = (int*)aux1->GetBody()->GetUserData().pointer;
	int idBodyA = bodyAData != nullptr ? *bodyAData : 0;
	b2Fixture* aux2 = contact->GetFixtureB();
	int* bodyBData = (int*)aux2->GetBody()->GetUserData().pointer;
	int idBodyB = bodyBData != nullptr ? *bodyBData : 0;

	// Pelota con pared
	if ((idBodyA == 1 && idBodyB == 2) || (idBodyA == 2 && idBodyB == 1))
	{
		std::cout << "Colision entre la pelota y una pared!" << std::endl;
	}

	// Triangulo con pared
	if ((idBodyA == 1 && idBodyB == 3) || (idBodyA == 3 && idBodyB == 1))
	{
		// El caso que quiero manejar
		std::cout << "Colision entre un triangulo y una pared!" << std::endl;
	}

	// Pelota y triangulo
	if ((idBodyA == 2 && idBodyB == 3) || (idBodyA == 3 && idBodyB == 2))
	{
		// El caso que quiero manejar
		std::cout << "Colision entre la pelota y un triangulo!" << std::endl;
	}
}

void MyContactListener::EndContact(b2Contact* contact)
{ }

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{ }

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{ }