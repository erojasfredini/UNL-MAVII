#pragma once

#include "Avatar.h"

class Ragdoll
{
	public:

		enum Miembro{Cabeza, Torso, BrazoI, BrazoD, PiernaI, PiernaD, Todo};

		enum TipoJoint{ Rolvolucion, Distancia };

		enum OrientacionInicial{ CabezaAPositivo, CabezaANegativo};

	private:

		b2World* m_pWorld;

		Avatar*  m_pHead;

		Avatar*  m_pCore;

		Avatar*  m_pLArm;

		Avatar*  m_pRArm;

		Avatar*  m_pLLeg;

		Avatar*  m_pRLeg;

		b2RevoluteJoint* m_pCoreLArmJoint;
		b2RevoluteJoint* m_pCoreRArmJoint;
		b2RevoluteJoint* m_pCoreILegJoint;
		b2RevoluteJoint* m_pCoreRLegJoint;
		b2RevoluteJoint* m_pCoreHeadJoint;

	public:

		// Parametros:
		//	-La posicion inicial de la cabeza del ragdoll en coordenadas del mundo
		//	-El objeto de mundo donde se creara el ragdoll
		Ragdoll(const b2Vec2& Posicion, float RagdollAltura, float RagdollPeso, b2World* pWorld, TipoJoint tipoJoint = TipoJoint::Rolvolucion, OrientacionInicial RagdollOrientacion = OrientacionInicial::CabezaAPositivo, float RagdollRestitution = 0.5f, float linearDamping = 0.1f, float angularDamping = 0.1f);

		~Ragdoll();

		// Actualiza la posicion del ragdoll
		void Actualizar();

		// Dibuja el ragdoll
		// Parametros:
		//	-Una referencia al RenderWindow donde dibujar el ragdoll
		void Dibujar(sf::RenderWindow &RW);

		// Aplica una fuerza sobre el ragdoll
		// Parametros:
		//	-El vector de fuerza a aplicar
		//	-El origen de aplicacion de la fuerza en coordenadas locales al miembro
		//	-Un enum estableciendo el miembro donde se aplica la fuerza
		void AplicarFuerza(b2Vec2& fuerza, b2Vec2& origen = b2Vec2(0.0f,0.0f), Miembro parte = Cabeza);

		// Aplica un impulso sobre el ragdoll
		// Parametros:
		//	-El vector de impulso a aplicar
		//	-El origen de aplicacion el impulso en coordenadas locales al miembro
		//	-Un enum estableciendo el miembro donde se aplica la fuerza
		void AplicarImpulso(b2Vec2& impulso, b2Vec2& origen = b2Vec2(0.0f, 0.0f), Miembro parte = Cabeza);
};