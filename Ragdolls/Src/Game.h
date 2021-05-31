#pragma once

#include <Box2D/Box2D.h>

#include "Avatar.h"

#include "Ragdoll.h"

// La cantidad de ragdolls a instanciar en la escena
#define PERSONAJES_COUNT 4

#define ANCHO_ESCENA 20.0f

#define GRAVEDAD -9.8f

//Un callback para hacer peeking en la escena con el mouse
class MousePeekCallback: public b2QueryCallback
{
	public:
	MousePeekCallback(float mouseX, float mouseY): m_pfixture(NULL), m_mousePosition(mouseX,mouseY)
	{ }
	virtual ~MousePeekCallback()
	{ }

	/// Se llama por cada fixture encontrado en el AABB
	/// Return:: Si se devuelve false se termina el callback
	virtual bool ReportFixture(b2Fixture* fixture);

	// Un puntero al fixture en el cual se hizo click
	b2Fixture* m_pfixture;

	// La posicion del mouse en el momento de hacer la query
	b2Vec2 m_mousePosition;
};

// Clase principal del juego, contiene toda la estructura de juego
// Este objeto sera el responsable de:
//	-Crear la escena
//	-Dibujar la pantalla
//	-Actualizar el juego
class Game
{
private:

	// Mundo de box2d
	b2World          m_World;

	// Ventana principal
	sf::RenderWindow m_Window;

	sf::Event        m_Evento;

	// Elementos de la escena:
	// -----------------------
	Ragdoll*         m_pPersonajes[PERSONAJES_COUNT];
	Avatar*          m_pLimites[4];

	b2MouseJoint*    m_pMouseJoint;

	b2Body*          m_pNoBodyBody;// Solo se lo usa para el mouseJoint

public:

	Game(b2Vec2 gravity);

	~Game();

	// Inicializa la escena, se llama luego de creada la instacia de Game
	void Iniciar();

	// Crea la escena
	void CrearEscena();

	// Actualiza la escena
	void Actualizar();

	// Dibuja la escena
	void Dibujar();
};