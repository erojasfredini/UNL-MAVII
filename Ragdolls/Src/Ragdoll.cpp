#include "Ragdoll.h"

#include <iostream>

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Ragdoll::Ragdoll(const b2Vec2& Posicion, float RagdollAltura, float RagdollPeso, b2World* pWorld, TipoJoint tipoJoint, OrientacionInicial RagdollOrientacion, float RagdollRestitution, float linearDamping, float angularDamping) : m_pWorld(pWorld)
{
	float RagdollScale = RagdollAltura / (20.0f + 30.0f + 40.0f);

	float RagdollVolume = (20.0f*RagdollScale*20.0f*RagdollScale) + (30.0f*RagdollScale*60.0f*RagdollScale) + (20.0f*RagdollScale*40.0f*RagdollScale)*2.0f + (20.0f*RagdollScale*40.0f*RagdollScale)*2.0f;
	float RagdollDensity = RagdollPeso / RagdollVolume;

	b2Fixture*   pAdorno;

	b2BodyDef    CuerpoDefinicion;
	b2FixtureDef AdornoDefinicion;

	b2Vec2 up;
	b2Vec2 right;
	if (RagdollOrientacion == OrientacionInicial::CabezaAPositivo)
	{
		up = b2Vec2(0.0f, 1.0f);
		right = b2Vec2(-1.0f, 0.0f);
	}else
	{
		up = b2Vec2(0.0f, -1.0f);
		right = b2Vec2(1.0f, 0.0f);
	}
	b2Vec2 auxVec;

	sf::Shape* formaSFML;

	// Creacion de la cabeza
	CuerpoDefinicion.type     = b2BodyType::b2_dynamicBody;
	CuerpoDefinicion.position = b2Vec2(0,0)+Posicion;
	CuerpoDefinicion.linearDamping = linearDamping;
	CuerpoDefinicion.angularDamping = angularDamping;

	AdornoDefinicion.density = RagdollDensity;
	AdornoDefinicion.restitution = RagdollRestitution;

	b2PolygonShape cabeza;
	cabeza.SetAsBox(10 * RagdollScale, 10 * RagdollScale);
	AdornoDefinicion.shape = &cabeza;

	b2Body* pCabezaCuerpo = pWorld->       CreateBody   (&CuerpoDefinicion);
	pAdorno               = pCabezaCuerpo->CreateFixture(&AdornoDefinicion);

	formaSFML = new sf::RectangleShape(sf::Vector2f(20 * RagdollScale, 20 * RagdollScale));
	formaSFML->setOrigin(10 * RagdollScale, 10 * RagdollScale);
	formaSFML->setFillColor(sf::Color::Green);

	m_pHead = new Avatar(pCabezaCuerpo, formaSFML);

	// Creacion del toso
	CuerpoDefinicion.type     = b2BodyType::b2_dynamicBody;
	CuerpoDefinicion.position = b2Vec2(up.x* -40.0f* RagdollScale, up.y* -40.0f* RagdollScale) + Posicion;

	AdornoDefinicion.density = RagdollDensity;
	b2PolygonShape torso;
	torso.SetAsBox(15 * RagdollScale, 30 * RagdollScale);
	AdornoDefinicion.shape = &torso;

	b2Body* pTorsoCuerpo = pWorld->      CreateBody   (&CuerpoDefinicion);
	pAdorno              = pTorsoCuerpo->CreateFixture(&AdornoDefinicion);

	formaSFML = new sf::RectangleShape(sf::Vector2f(30 * RagdollScale, 60 * RagdollScale));
	formaSFML->setOrigin(15 * RagdollScale, 30 * RagdollScale);
	formaSFML->setFillColor(sf::Color::Yellow);

	m_pCore = new Avatar(pTorsoCuerpo, formaSFML);

	// Creacion del brazo izquierdo
	CuerpoDefinicion.type     = b2BodyType::b2_dynamicBody;
	auxVec = b2Vec2(up.x* -30.0f* RagdollScale, up.y* -30.0f* RagdollScale) + b2Vec2(right.x* -25.0f * RagdollScale, right.y * -25.0f * RagdollScale);
	CuerpoDefinicion.position = auxVec + Posicion;

	AdornoDefinicion.density = RagdollDensity;
	b2PolygonShape brazoI;
	brazoI.SetAsBox(10 * RagdollScale, 20 * RagdollScale);
	AdornoDefinicion.shape = &brazoI;

	b2Body* pBrazoICuerpo = pWorld->       CreateBody   (&CuerpoDefinicion);
	pAdorno               = pBrazoICuerpo->CreateFixture(&AdornoDefinicion);

	formaSFML = new sf::RectangleShape(sf::Vector2f(20 * RagdollScale, 40 * RagdollScale));
	formaSFML->setOrigin(10 * RagdollScale, 20 * RagdollScale);
	formaSFML->setFillColor(sf::Color::Cyan);

	m_pLArm = new Avatar(pBrazoICuerpo, formaSFML);

	// Creacion del brazo derecho
	CuerpoDefinicion.type     = b2BodyType::b2_dynamicBody;
	auxVec = b2Vec2(up.x* -30.0f* RagdollScale, up.y* -30.0f* RagdollScale) + b2Vec2(right.x* 25.0f * RagdollScale, right.y * 25.0f * RagdollScale);
	CuerpoDefinicion.position = auxVec + Posicion;

	AdornoDefinicion.density = RagdollDensity;
	b2PolygonShape brazoD;
	brazoD.SetAsBox(10 * RagdollScale, 20 * RagdollScale);
	AdornoDefinicion.shape = &brazoD;

	b2Body* pBrazoDCuerpo = pWorld->       CreateBody   (&CuerpoDefinicion);
	pAdorno               = pBrazoDCuerpo->CreateFixture(&AdornoDefinicion);

	formaSFML = new sf::RectangleShape(sf::Vector2f(20 * RagdollScale, 40 * RagdollScale));
	formaSFML->setOrigin(10 * RagdollScale, 20 * RagdollScale);
	formaSFML->setFillColor(sf::Color::Cyan);

	m_pRArm = new Avatar(pBrazoDCuerpo, formaSFML);

	// Creacion de la pierna izquierda
	CuerpoDefinicion.type     = b2BodyType::b2_dynamicBody;
	auxVec = b2Vec2(up.x* -90.0f* RagdollScale, up.y* -90.0f* RagdollScale) + b2Vec2(right.x* -25.0f * RagdollScale, right.y * -25.0f * RagdollScale);
	CuerpoDefinicion.position = auxVec + Posicion;

	AdornoDefinicion.density = RagdollDensity;
	b2PolygonShape piernaI;
	piernaI.SetAsBox(10 * RagdollScale, 20 * RagdollScale);
	AdornoDefinicion.shape = &piernaI;

	b2Body* pPiernaICuerpo = pWorld->        CreateBody   (&CuerpoDefinicion);
	pAdorno                = pPiernaICuerpo->CreateFixture(&AdornoDefinicion);

	formaSFML = new sf::RectangleShape(sf::Vector2f(20 * RagdollScale, 40 * RagdollScale));
	formaSFML->setOrigin(10 * RagdollScale, 20 * RagdollScale);
	formaSFML->setFillColor(sf::Color::Magenta);

	m_pLLeg = new Avatar(pPiernaICuerpo, formaSFML);

	// Creacion de la pierna derecha
	CuerpoDefinicion.type     = b2BodyType::b2_dynamicBody;
	auxVec = b2Vec2(up.x* -90.0f* RagdollScale, up.y* -90.0f* RagdollScale) + b2Vec2(right.x* 25.0f * RagdollScale, right.y * 25.0f * RagdollScale);
	CuerpoDefinicion.position = auxVec + Posicion;

	AdornoDefinicion.density = RagdollDensity;
	b2PolygonShape piernaD;
	piernaD.SetAsBox(10 * RagdollScale, 20 * RagdollScale);
	AdornoDefinicion.shape = &piernaD;

	b2Body* pPiernaRCuerpo = pWorld->        CreateBody   (&CuerpoDefinicion);
	pAdorno                = pPiernaRCuerpo->CreateFixture(&AdornoDefinicion);

	formaSFML = new sf::RectangleShape(sf::Vector2f(20 * RagdollScale, 40 * RagdollScale));
	formaSFML->setOrigin(10 * RagdollScale, 20 * RagdollScale);
	formaSFML->setFillColor(sf::Color::Magenta);

	m_pRLeg = new Avatar(pPiernaRCuerpo, formaSFML);

	switch (tipoJoint)
	{
		case TipoJoint::Rolvolucion:
		{
			b2RevoluteJointDef JointDefinicion;
			// Torso
			JointDefinicion.collideConnected = true;//lo mantiene mas armado

			JointDefinicion.enableLimit = true;
			JointDefinicion.upperAngle = 40 / (180 / 3.14f);
			JointDefinicion.lowerAngle = -40 / (180 / 3.14f);

			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pCabezaCuerpo;
			auxVec = b2Vec2(up.x* 30.0f* RagdollScale, up.y* 30.0f* RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* -10.0f* RagdollScale, up.y* -10.0f* RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			m_pCoreHeadJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Brazo izquierdo
			JointDefinicion.lowerAngle = -130 / (180 / 3.14f);
			JointDefinicion.upperAngle = 85 / (180 / 3.14f);

			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pBrazoICuerpo;
			auxVec = b2Vec2(up.x* 30.0f* RagdollScale, up.y* 30.0f* RagdollScale) + b2Vec2(right.x* -15.0f * RagdollScale, right.y * -15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* 10.0f * RagdollScale, right.y * 10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			m_pCoreLArmJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Brazo derecho
			JointDefinicion.lowerAngle = -85 / (180 / 3.14f);
			JointDefinicion.upperAngle = 130 / (180 / 3.14f);

			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pBrazoDCuerpo;
			auxVec = b2Vec2(up.x* 30.0f* RagdollScale, up.y* 30.0f* RagdollScale) + b2Vec2(right.x* 15.0f * RagdollScale, right.y * 15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* -10.0f * RagdollScale, right.y * -10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			m_pCoreRArmJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Pierna izquierda
			JointDefinicion.lowerAngle = -45 / (180 / 3.14f);
			JointDefinicion.upperAngle = 25 / (180 / 3.14f);

			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pPiernaICuerpo;
			auxVec = b2Vec2(up.x* -30.0f* RagdollScale, up.y* -30.0f* RagdollScale) + b2Vec2(right.x* -15.0f * RagdollScale, right.y * -15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* 10.0f * RagdollScale, right.y * 10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			m_pCoreILegJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Pierna derecha
			JointDefinicion.lowerAngle = -25 / (180 / 3.14f);
			JointDefinicion.upperAngle = 45 / (180 / 3.14f);

			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pPiernaRCuerpo;
			auxVec = b2Vec2(up.x* -30.0f* RagdollScale, up.y* -30.0f* RagdollScale) + b2Vec2(right.x* 15.0f * RagdollScale, right.y * 15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* -10.0f * RagdollScale, right.y * -10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			m_pCoreRLegJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);
		}
		break;

		case TipoJoint::Distancia:
		{
			b2DistanceJointDef JointDefinicion;
			JointDefinicion.length = 0.05f;

			const float frec = 100.0f;
			const float dampRatio = 0.1f;

			// Torso
			JointDefinicion.collideConnected = true;//lo mantiene mas armado

			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pCabezaCuerpo;
			auxVec = b2Vec2(up.x* 30.0f* RagdollScale, up.y* 30.0f* RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* -10.0f* RagdollScale, up.y* -10.0f* RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			b2LinearStiffness(JointDefinicion.stiffness, JointDefinicion.damping, frec, dampRatio, JointDefinicion.bodyA, JointDefinicion.bodyB);
			m_pCoreHeadJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Brazo izquierdo
			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pBrazoICuerpo;
			auxVec = b2Vec2(up.x* 30.0f* RagdollScale, up.y* 30.0f* RagdollScale) + b2Vec2(right.x* -15.0f * RagdollScale, right.y * -15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* 10.0f * RagdollScale, right.y * 10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			b2LinearStiffness(JointDefinicion.stiffness, JointDefinicion.damping, frec, dampRatio, JointDefinicion.bodyA, JointDefinicion.bodyB);
			m_pCoreLArmJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Brazo derecho
			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pBrazoDCuerpo;
			auxVec = b2Vec2(up.x* 30.0f* RagdollScale, up.y* 30.0f* RagdollScale) + b2Vec2(right.x* 15.0f * RagdollScale, right.y * 15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* -10.0f * RagdollScale, right.y * -10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			b2LinearStiffness(JointDefinicion.stiffness, JointDefinicion.damping, frec, dampRatio, JointDefinicion.bodyA, JointDefinicion.bodyB);
			m_pCoreRArmJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Pierna izquierda
			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pPiernaICuerpo;
			auxVec = b2Vec2(up.x* -30.0f* RagdollScale, up.y* -30.0f* RagdollScale) + b2Vec2(right.x* -15.0f * RagdollScale, right.y * -15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* 10.0f * RagdollScale, right.y * 10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			b2LinearStiffness(JointDefinicion.stiffness, JointDefinicion.damping, frec, dampRatio, JointDefinicion.bodyA, JointDefinicion.bodyB);
			m_pCoreILegJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);

			// Pierna derecha
			JointDefinicion.bodyA = pTorsoCuerpo;
			JointDefinicion.bodyB = pPiernaRCuerpo;
			auxVec = b2Vec2(up.x* -30.0f* RagdollScale, up.y* -30.0f* RagdollScale) + b2Vec2(right.x* 15.0f * RagdollScale, right.y * 15.0f * RagdollScale);
			JointDefinicion.localAnchorA = auxVec;
			auxVec = b2Vec2(up.x* 20.0f* RagdollScale, up.y* 20.0f* RagdollScale) + b2Vec2(right.x* -10.0f * RagdollScale, right.y * -10.0f * RagdollScale);
			JointDefinicion.localAnchorB = auxVec;
			b2LinearStiffness(JointDefinicion.stiffness, JointDefinicion.damping, frec, dampRatio, JointDefinicion.bodyA, JointDefinicion.bodyB);
			m_pCoreRLegJoint = (b2RevoluteJoint*)pWorld->CreateJoint(&JointDefinicion);
		}
		break;
	}

	/*
	float mass = 0.0f;
	mass += ((b2Body*)*m_pCore)->GetMass();
	mass += ((b2Body*)*m_pHead)->GetMass();
	mass += ((b2Body*)*m_pLArm)->GetMass();
	mass += ((b2Body*)*m_pLLeg)->GetMass();
	mass += ((b2Body*)*m_pRArm)->GetMass();
	mass += ((b2Body*)*m_pRLeg)->GetMass();
	std::cerr << "Ragdoll mass: " << mass << std::endl;
	*/
}

Ragdoll::~Ragdoll()
{
	m_pWorld->DestroyJoint(m_pCoreLArmJoint);
	m_pCoreLArmJoint = NULL;

	m_pWorld->DestroyJoint(m_pCoreRArmJoint);
	m_pCoreRArmJoint = NULL;

	m_pWorld->DestroyJoint(m_pCoreILegJoint);
	m_pCoreILegJoint = NULL;

	m_pWorld->DestroyJoint(m_pCoreRLegJoint);
	m_pCoreRLegJoint = NULL;

	m_pWorld->DestroyJoint(m_pCoreHeadJoint);
	m_pCoreHeadJoint = NULL;

	delete m_pHead;

	delete m_pCore;

	delete m_pLArm;

	delete m_pRArm;

	delete m_pLLeg;

	delete m_pRLeg;
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void Ragdoll::Actualizar()
{
	m_pHead->Actualizar();

	m_pCore->Actualizar();

	m_pLArm->Actualizar();

	m_pRArm->Actualizar();

	m_pLLeg->Actualizar();

	m_pRLeg->Actualizar();
}

void Ragdoll::Dibujar(sf::RenderWindow &RW)
{
	m_pHead->Dibujar(RW);

	m_pCore->Dibujar(RW);

	m_pLArm->Dibujar(RW);

	m_pRArm->Dibujar(RW);

	m_pLLeg->Dibujar(RW);

	m_pRLeg->Dibujar(RW);
}

void Ragdoll::AplicarFuerza(b2Vec2& fuerza, b2Vec2& origen, Miembro parte)
{
	switch( parte )
	{
		case Cabeza:
			m_pHead->AplicarFuerza(fuerza, m_pHead->GetPosition()+origen);
			break;
		case Torso:
			m_pCore->AplicarFuerza(fuerza, m_pCore->GetPosition()+origen);
			break;
		case BrazoI:
			m_pLArm->AplicarFuerza(fuerza, m_pLArm->GetPosition()+origen);
			break;
		case BrazoD:
			m_pRArm->AplicarFuerza(fuerza, m_pRArm->GetPosition()+origen);
			break;
		case PiernaI:
			m_pLLeg->AplicarFuerza(fuerza, m_pLLeg->GetPosition()+origen);
			break;
		case PiernaD:
			m_pRLeg->AplicarFuerza(fuerza, m_pRLeg->GetPosition()+origen);
			break;
		case Todo:
			m_pHead->AplicarFuerza(fuerza, m_pHead->GetPosition() + origen);
			m_pCore->AplicarFuerza(fuerza, m_pCore->GetPosition() + origen);
			m_pLArm->AplicarFuerza(fuerza, m_pLArm->GetPosition() + origen);
			m_pRArm->AplicarFuerza(fuerza, m_pRArm->GetPosition() + origen);
			m_pLLeg->AplicarFuerza(fuerza, m_pLLeg->GetPosition() + origen);
			m_pRLeg->AplicarFuerza(fuerza, m_pRLeg->GetPosition() + origen);
			break;
	}
}

void Ragdoll::AplicarImpulso(b2Vec2& impulso, b2Vec2& origen, Miembro parte)
{
	switch (parte)
	{
		case Cabeza:
			m_pHead->AplicarImpulso(impulso, m_pHead->GetPosition() + origen);
			break;
		case Torso:
			m_pCore->AplicarImpulso(impulso, m_pCore->GetPosition() + origen);
			break;
		case BrazoI:
			m_pLArm->AplicarImpulso(impulso, m_pLArm->GetPosition() + origen);
			break;
		case BrazoD:
			m_pRArm->AplicarImpulso(impulso, m_pRArm->GetPosition() + origen);
			break;
		case PiernaI:
			m_pLLeg->AplicarImpulso(impulso, m_pLLeg->GetPosition() + origen);
			break;
		case PiernaD:
			m_pRLeg->AplicarImpulso(impulso, m_pRLeg->GetPosition() + origen);
			break;
		case Todo:
			m_pHead->AplicarImpulso(impulso, m_pHead->GetPosition() + origen);
			m_pCore->AplicarImpulso(impulso, m_pCore->GetPosition() + origen);
			m_pLArm->AplicarImpulso(impulso, m_pLArm->GetPosition() + origen);
			m_pRArm->AplicarImpulso(impulso, m_pRArm->GetPosition() + origen);
			m_pLLeg->AplicarImpulso(impulso, m_pLLeg->GetPosition() + origen);
			m_pRLeg->AplicarImpulso(impulso, m_pRLeg->GetPosition() + origen);
			break;
	}
}