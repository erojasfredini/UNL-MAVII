#include <cstdlib>

#include "Game.h"
#include "SFMLRenderer.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Game::Game(b2Vec2 gravity): m_World(gravity), m_Window(sf::VideoMode(800,600,32), "Box2D: Ragdoll"), m_pMouseJoint(NULL)
{
	float windowHeight = ANCHO_ESCENA * m_Window.getSize().y / m_Window.getSize().x;
	sf::View v = m_Window.getView();
	v.setSize(sf::Vector2f(ANCHO_ESCENA, windowHeight));
	v.setCenter(sf::Vector2f(0.0f, windowHeight/2.0f));
	v.setRotation(180.0f);
	m_Window.setView(v);

	m_Window.setVisible(true);
	m_Window.setFramerateLimit(60);

	b2BodyDef bodyDef;
	m_pNoBodyBody = m_World.CreateBody(&bodyDef);

	CrearEscena();
}

Game::~Game()
{
	if( m_pMouseJoint )
	{
		m_World.DestroyJoint(m_pMouseJoint);
		m_pMouseJoint = NULL;
	}

	for(int k=0; k < PERSONAJES_COUNT ; ++k)
		delete m_pPersonajes[k];

	for (int k = 0; k < 4; ++k)
		delete m_pLimites[k];

	m_World.DestroyBody(m_pNoBodyBody);
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void Game::CrearEscena()
{
	float height = ANCHO_ESCENA * m_Window.getSize().y / m_Window.getSize().x;
	float width = ANCHO_ESCENA;

	sf::Shape* formaSFML;

	b2BodyDef    CuerpoDefinicion;
	b2FixtureDef AdornoDefinicion;

	b2Body*      pCuerpo;
	b2Fixture*   pAdorno;

	// Creacion del suelo:
	// -------------------
	// El suelo no sera afectado por fuerzas y no puede ser movido por objetos dentro de la simulacion por eso es estatico
	float floorHeight = height * 0.05f;

	CuerpoDefinicion.type     = b2BodyType::b2_staticBody;
	CuerpoDefinicion.position = b2Vec2(0.0f, 0.0f);

	AdornoDefinicion.restitution = 0.5f;
	AdornoDefinicion.isSensor    = false;
	AdornoDefinicion.friction    = 0.3f;
	AdornoDefinicion.density     = 0.0f;
	b2PolygonShape floor;
	floor.SetAsBox(width / 2.0f, floorHeight / 2.0f);
	AdornoDefinicion.shape = &floor;

	pCuerpo = m_World.CreateBody    (&CuerpoDefinicion);// Creamos el cuerpo rigido
	pAdorno = pCuerpo->CreateFixture(&AdornoDefinicion);// Creamos el fixture y se lo asignamos

	formaSFML = new sf::RectangleShape(sf::Vector2f(width, floorHeight));
	formaSFML->setFillColor(sf::Color::Blue);
	formaSFML->setOrigin(width / 2.0f, floorHeight / 2.0f);
	m_pLimites[0] = new Avatar(pCuerpo, formaSFML);

	// Pared izquierda
	CuerpoDefinicion.type = b2BodyType::b2_staticBody;
	CuerpoDefinicion.position = b2Vec2(-width/2.0f, height/2.0f);

	AdornoDefinicion.restitution = 0.5f;
	AdornoDefinicion.isSensor = false;
	AdornoDefinicion.friction = 0.3f;
	AdornoDefinicion.density = 0.0f;
	floor.SetAsBox(floorHeight / 2.0f, height / 2.0f );
	AdornoDefinicion.shape = &floor;

	pCuerpo = m_World.CreateBody(&CuerpoDefinicion);// Creamos el cuerpo rigido
	pAdorno = pCuerpo->CreateFixture(&AdornoDefinicion);// Creamos el fixture y se lo asignamos

	formaSFML = new sf::RectangleShape(sf::Vector2f(floorHeight, height));
	formaSFML->setFillColor(sf::Color::Blue);
	formaSFML->setOrigin(floorHeight / 2.0f, height / 2.0f);
	m_pLimites[1] = new Avatar(pCuerpo, formaSFML);

	// Pared derecha
	CuerpoDefinicion.type = b2BodyType::b2_staticBody;
	CuerpoDefinicion.position = b2Vec2(width / 2.0f, height / 2.0f);

	AdornoDefinicion.restitution = 0.5f;
	AdornoDefinicion.isSensor = false;
	AdornoDefinicion.friction = 0.3f;
	AdornoDefinicion.density = 0.0f;
	floor.SetAsBox(floorHeight / 2.0f, height / 2.0f);
	AdornoDefinicion.shape = &floor;

	pCuerpo = m_World.CreateBody(&CuerpoDefinicion);// Creamos el cuerpo rigido
	pAdorno = pCuerpo->CreateFixture(&AdornoDefinicion);// Creamos el fixture y se lo asignamos

	formaSFML = new sf::RectangleShape(sf::Vector2f(floorHeight, height));
	formaSFML->setFillColor(sf::Color::Blue);
	formaSFML->setOrigin(floorHeight / 2.0f, height / 2.0f);
	m_pLimites[2] = new Avatar(pCuerpo, formaSFML);

	// Pared superior
	CuerpoDefinicion.type = b2BodyType::b2_staticBody;
	CuerpoDefinicion.position = b2Vec2(0.0f, height);

	AdornoDefinicion.restitution = 0.5f;
	AdornoDefinicion.isSensor = false;
	AdornoDefinicion.friction = 0.3f;
	AdornoDefinicion.density = 0.0f;
	floor.SetAsBox(width / 2.0f, floorHeight / 2.0f);
	AdornoDefinicion.shape = &floor;

	pCuerpo = m_World.CreateBody(&CuerpoDefinicion);// Creamos el cuerpo rigido
	pAdorno = pCuerpo->CreateFixture(&AdornoDefinicion);// Creamos el fixture y se lo asignamos

	formaSFML = new sf::RectangleShape(sf::Vector2f(width, floorHeight));
	formaSFML->setFillColor(sf::Color::Blue);
	formaSFML->setOrigin(width / 2.0f, floorHeight / 2.0f);
	m_pLimites[3] = new Avatar(pCuerpo, formaSFML);

	//m_pSuelo->SetPosition( b2Vec2(width / 2.0f, height-25.0f));

	// Creacion de los ragdolls:
	// -------------------------
	for (int k = 0; k < PERSONAJES_COUNT; ++k)
	{
		b2Vec2 randomPosition = b2Vec2(((rand() / (float)RAND_MAX) * (int)(width / 6.0f*4.0f) + width / 6.0f) - width / 2.0f, height / 2.0f);
		m_pPersonajes[k] = new Ragdoll(randomPosition, 1.7f, 70.0f, &m_World, Ragdoll::TipoJoint::Rolvolucion, Ragdoll::OrientacionInicial::CabezaAPositivo, 0.0f, 0.05f, 0.05f);
	}

	SFMLRenderer* render = new SFMLRenderer(&m_Window);
	render->SetFlags(UINT_MAX);
	m_World.SetDebugDraw(render);
}

void Game::Iniciar()
{
	while( m_Window.isOpen() )
	{
		// Atrapamos los eventos para cerra la ventana y los clicks del mouse para
		// hacer peek sobre las pelotas
		while( m_Window.pollEvent(m_Evento) )
		{
			switch( m_Evento.type )
			{
			case sf::Event::Closed:
					m_Window.close();
				break;

			case sf::Event::KeyReleased:

				if (m_Evento.key.code == sf::Keyboard::Q)
				{
					for (int k = 0; k < PERSONAJES_COUNT; ++k)
						m_pPersonajes[k]->AplicarImpulso(b2Vec2(0.0f, 1000.0f), b2Vec2(0.0f, 0.0f), Ragdoll::Miembro::Torso);
				}

				break;

			case sf::Event::KeyPressed:
				if( m_Evento.key.code == sf::Keyboard::Escape)
					m_Window.close();

				break;

			case sf::Event::MouseButtonPressed:
				{
					if(!m_pMouseJoint)// Mientras tenga un mouse joint no agregaremos otro
					{
						sf::Vector2f mouseWorldPos = m_Window.mapPixelToCoords(sf::Vector2i(m_Evento.mouseButton.x, m_Evento.mouseButton.y));
						MousePeekCallback Peeker(mouseWorldPos.x, mouseWorldPos.y);
						b2AABB box;
						box.lowerBound = b2Vec2(mouseWorldPos.x - 0.1f, mouseWorldPos.y - 0.1f);
						box.upperBound = b2Vec2(mouseWorldPos.x + 0.1f, mouseWorldPos.y + 0.1f);
						m_World.QueryAABB(&Peeker, box);

						if (Peeker.m_pfixture)
						{
							b2Body* body = Peeker.m_pfixture->GetBody();
							b2MouseJointDef md;
							md.bodyA = m_pNoBodyBody;
							md.bodyB = body;
							md.target = b2Vec2(mouseWorldPos.x, mouseWorldPos.y);
							md.maxForce = 500.0f * body->GetMass();
							float frec = 100.0f;
							float dampRatio = 0.1f;
							b2LinearStiffness(md.stiffness, md.damping, frec, dampRatio, md.bodyA, md.bodyB);
							m_pMouseJoint = (b2MouseJoint*)m_World.CreateJoint(&md);
							body->SetAwake(true);
						}
					}
				}
				break;

			case sf::Event::MouseLeft:
			case sf::Event::MouseButtonReleased:
				{
					if( m_pMouseJoint )
					{
						m_World.DestroyJoint(m_pMouseJoint);
						m_pMouseJoint = NULL;
					}
				}
				break;

			case sf::Event::MouseMoved:
				if (m_pMouseJoint)
				{
					sf::Vector2f mouseWorldPos = m_Window.mapPixelToCoords(sf::Vector2i(m_Evento.mouseMove.x, m_Evento.mouseMove.y));
					m_pMouseJoint->SetTarget(b2Vec2(mouseWorldPos.x, mouseWorldPos.y));
				}
				break;
			}
		}

		// Aplicamos el control sobre el ragdoll numero 0
		if( sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
			m_pPersonajes[0]->AplicarFuerza(b2Vec2(0, 800));

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
			m_pPersonajes[0]->AplicarFuerza(b2Vec2(800, 0));

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
			m_pPersonajes[0]->AplicarFuerza(b2Vec2(-800, 0));

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
			m_pPersonajes[0]->AplicarFuerza(b2Vec2(0, -800));

		Actualizar();    // Actualizamos la escena

		Dibujar();       // Dibujamos todo el juego
	}
}

void Game::Actualizar()
{
	for(int k=0; k < PERSONAJES_COUNT ; ++k)
		m_pPersonajes[k]->Actualizar();

	for (int k = 0; k < 4; ++k)
		m_pLimites[k]->Actualizar();

	//m_World.Step(m_Window.GetFrameTime(), 10, 10);
	m_World.Step(1/60.0f, 10, 10);

	m_World.ClearForces();
}

void Game::Dibujar()
{
	m_Window.clear();

	m_World.DebugDraw();

	for(int k=0; k < PERSONAJES_COUNT ; ++k)
		m_pPersonajes[k]->Dibujar(m_Window);

	for (int k = 0; k < 4; ++k)
		m_pLimites[k]->Dibujar(m_Window);

	m_Window.display();
}

bool MousePeekCallback::ReportFixture(b2Fixture* fixture)
{
	b2Body* pBody = fixture->GetBody();
	switch( pBody->GetType() )
	{
		case b2BodyType::b2_dynamicBody:
			//MessageBox(NULL, L"Apretaste sobre un objeto dinamico", L"Peek", MB_OK);
			if( fixture->TestPoint(m_mousePosition) )
			{
				m_pfixture = fixture;
				return false;//que no siga buscando
			}
			break;
		case b2BodyType::b2_staticBody:
			//MessageBox(NULL, L"Apretaste sobre un objeto estatico", L"Peek", MB_OK);
			break;
	}
	return true;
}
