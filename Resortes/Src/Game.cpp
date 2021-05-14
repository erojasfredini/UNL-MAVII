﻿#include "Game.h"
#include "Box2DHelper.h"

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while (wnd->isOpen())
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
{
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
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
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// MyContactListener* l= new MyContactListener();
	// phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
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

	// Creamos un circulo que ataremos al primer resorte
	b2Body* circuloCentral = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloCentral->SetTransform(b2Vec2(50.0f, 10.0f), 0.0f);

	// Atamos la esfera mediante un resorte al techo
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloCentral, circuloCentral->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter(), 20.0f, 0.1f, 0.0f);


	// Creamos un circulo que ataremos al resorte derecho
	b2Body* circuloIzq = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloIzq->SetTransform(b2Vec2(80.0f, 10.0f), 0.0f);

	// Atamos la esfera mediante un resorte al techo. Este resorte se encuentra dampeado totalmente
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloIzq, circuloIzq->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter() + b2Vec2(30.0f, 0.0f), 20.0f, 0.1f, 1.0f);


	// Creamos un circulo que ataremos al resorte izquierdo
	b2Body* circuloDer = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloDer->SetTransform(b2Vec2(20.0f, 10.0f), 0.0f);

	// Atamos la esfera mediante un resorte al techo
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloDer, circuloDer->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter() + b2Vec2(-30.0f, 0.0f), 20.0f, 1.1f, 0.0f);
}

Game::~Game(void)
{
}