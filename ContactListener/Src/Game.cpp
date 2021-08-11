#include "Game.h"
#include "Box2DHelper.h"

Game::Game(int ancho, int alto,std::string titulo)
{
	wnd= new RenderWindow(VideoMode(ancho,alto),titulo);
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
{
	controlBodyAvatar->Actualizar();
	controlBodyAvatar->Dibujar(*wnd);
}

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

				b2BodyUserData& dataTriangulo = body->GetUserData();
				dataTriangulo.pointer = (uintptr_t)idTriangulo;

				// Transformamos las coordenadas segun la vista activa
				Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
				body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
				break;
		}
	}

	// Movemos el cuerpo
	controlBody->SetAwake(true);
	if(Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
	if(Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
	if(Keyboard::isKeyPressed(Keyboard::Down))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, 50.0f));
	if (Keyboard::isKeyPressed(Keyboard::Up))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));

	//controlBody->SetAngularVelocity(5.0f);
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
	wnd->setView(camara); // Asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld= new b2World(b2Vec2(0.0f, 9.8f));

	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender= new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Contract listener
	listener = new MyContactListener();
	phyWorld->SetContactListener(listener);

	idPared = new int;
	*idPared = 1;
	idPelota = new int;
	*idPelota = 2;
	idTriangulo = new int;
	*idTriangulo = 3;

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
	b2BodyUserData& dataG1 = groundBody->GetUserData();
	dataG1.pointer = (uintptr_t)idPared;

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);
	b2BodyUserData& dataLW = leftWallBody->GetUserData();
	dataLW.pointer = (uintptr_t)idPared;

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);
	b2BodyUserData& dataRW = rightWallBody->GetUserData();
	dataRW.pointer = (uintptr_t)idPared;

	// Creamos un circulo que controlaremos con el teclado
	controlBody= Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

	texturaPelota.loadFromFile("..\\..\\Resources\\Pelota.png");

	controlBodyAvatar = new Avatar(controlBody, new sf::Sprite(texturaPelota));

	// Tener cuidado que Avatar pone en user data un puntero al objeto Avatar, pero por simplicidad aca usamos los ids.
	// Por eso es que hay que setearlos despues
	b2BodyUserData& dataPelota = controlBody->GetUserData();
	dataPelota.pointer = (uintptr_t)idPelota;
}

Game::~Game(void)
{ }