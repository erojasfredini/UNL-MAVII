// Box2dEmptyApp.cpp : Defines the entry point for the console application.
//
#include "Game.h"

using namespace sf;

int main(int argc, char* argv[])
{
	Game *Juego;
	Juego = new Game(800,600,"Physics kit");
	Juego->Loop();

	return 0;
}