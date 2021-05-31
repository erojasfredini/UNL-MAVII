#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Game.h"

//      120 px ---- 1.8m
// 66.7 = x px ---- 1.0m
//#define METRO_A_PIXEL 66.7f

using namespace std;

int main(int argc, char* argv[])
{
	// Definimos el vector de gravedad
	//b2Vec2 gravity(0.0f, 9.8f*METRO_A_PIXEL);
	b2Vec2 gravity(0.0f, GRAVEDAD);
	//b2Vec2 gravity(0.0f, 0.0f);

	// Vamos a dejar que los cuerpos duerman?
	bool doSleep = true;
	Game MyGame(gravity);	// El objeto de juego principal
							//recibe atributos para instanciar el juego

	srand((unsigned int)time(NULL));// Ponemos una semilla pseudo-aleatoria

	MyGame.Iniciar();

	return 0;
}