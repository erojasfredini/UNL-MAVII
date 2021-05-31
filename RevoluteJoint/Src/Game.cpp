#include "Game.h"
#include "Box2DHelper.h"

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd= new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while(wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{ }

void Game::DoEvents()
{
	Event evt;
	while(wnd->pollEvent(evt))
	{
		switch(evt.type)
		{
			case Event::Closed:
				wnd->close();
				break;
			case Event::MouseButtonPressed:
				b2Body* body = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
				// Transformamos las coordenadas segun la vista activa
				Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
				body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
				break;
		}
	}
}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	// Definimos el area visible
	View v(Vector2f(50.0f, 50.0f), Vector2f(100.0f, 100.0f));
	wnd->setView(v);
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld= new b2World(b2Vec2(0.0f, 9.8f));

	//MyContactListener* l= new MyContactListener();
	//phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender= new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT32_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	//------------------------------------
	// Revolute and prismatic joints
	//------------------------------------
	b2Body* EsferaRev= Box2DHelper::CreateCircularStaticBody(phyWorld, 5.0f);
	EsferaRev->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
	b2Body* CajaRev=Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5, 5, 1.0f, 1.0f, 1.0f);
	CajaRev->SetTransform(b2Vec2(50.0f, 70.0f), 0.0f);
	b2RevoluteJoint* revJoint = Box2DHelper::CreateRevoluteJoint(phyWorld, EsferaRev, EsferaRev->GetWorldCenter(),
		CajaRev, -b2_pi / 2.0f, b2_pi /2.0f, 2.0f, 1000.0f, true, true);
}

Game::~Game(void)
{ }