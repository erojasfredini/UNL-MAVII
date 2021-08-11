#include "Avatar.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Avatar::Avatar(b2Body* pCuerpo, sf::Shape* pShape) : m_pCuerpo(pCuerpo)
{
	m_posAux = m_pCuerpo->GetPosition();

	m_pDrawable = pShape;
	m_pTransform = pShape;

	m_pTransform->setPosition(m_posAux.x, m_posAux.y);

	m_pCuerpo->GetUserData().pointer = (uintptr_t)this;// Referencia al avatar. La forma correcta de hacerla es así, no usar SetUserData
}

Avatar::Avatar(b2Body* pCuerpo, sf::Sprite* pSprite) : m_pCuerpo(pCuerpo)
{
	m_posAux = m_pCuerpo->GetPosition();

	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	for (b2Fixture* f = m_pCuerpo->GetFixtureList(); f; f = f->GetNext())
		aabb.Combine(f->GetAABB(0));
	float sizeX = aabb.GetExtents().x*2.0f / pSprite->getTexture()->getSize().x;
	float sizeY = aabb.GetExtents().y*2.0f / pSprite->getTexture()->getSize().y;
	pSprite->setScale(sf::Vector2f(sizeX, sizeY));
	pSprite->setOrigin(sf::Vector2f(pSprite->getTexture()->getSize().x / 2.0f, pSprite->getTexture()->getSize().y / 2.0f));
	pSprite->setPosition(m_posAux.x, m_posAux.y);

	m_pDrawable = pSprite;
	m_pTransform = pSprite;

	m_pCuerpo->GetUserData().pointer = (uintptr_t)this;// Referencia al avatar. La forma correcta de hacerla es así, no usar SetUserData
}

Avatar::~Avatar()
{
	for (b2Fixture* f = m_pCuerpo->GetFixtureList(); f; f = f->GetNext())
		m_pCuerpo->DestroyFixture(f);

	if (m_pDrawable != nullptr)
		delete m_pDrawable;
	// m_pTransform debería apuntar a lo mismo

	m_pCuerpo->GetWorld()->DestroyBody(m_pCuerpo);

	m_pCuerpo = NULL;
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

Avatar::operator b2Body*()
{
	return m_pCuerpo;
}

void Avatar::Actualizar()
{
	m_posAux = m_pCuerpo->GetPosition();

	m_angAux = m_pCuerpo->GetAngle();// Devuelve el angulo en radianes

	m_pTransform->setPosition(m_posAux.x, m_posAux.y);

	m_pTransform->setRotation(m_angAux * 180 / 3.14f);	// Recibe el angulo en grados
														// Notamos que el sentido de rotacion de SFML y Box2D son opuestos
}

void Avatar::Dibujar(sf::RenderWindow &RW)
{
	RW.draw(*m_pDrawable);
}

b2Vec2 Avatar::GetPosition()
{
	return m_pCuerpo->GetPosition();
}

b2Body& Avatar::GetCuerpo()
{
	return *m_pCuerpo;
}

sf::Sprite* Avatar::GetSprite()
{
	return dynamic_cast<sf::Sprite*>(m_pDrawable);
}

void Avatar::SetPosition(b2Vec2 position)
{
	m_pCuerpo->SetTransform(position, m_pCuerpo->GetAngle());
}

void Avatar::AplicarFuerza(b2Vec2& fuerza, b2Vec2& origen)
{
	m_pCuerpo->ApplyForce(fuerza, origen, true);
}

void Avatar::AplicarImpulso(b2Vec2& impulso, b2Vec2& origen)
{
	m_pCuerpo->ApplyLinearImpulse(impulso, origen, true);
}