#include "header.h"

//#include "objects.cpp"

int main(int argc, char* argv[]){
	
	std::cout

	<< "Controles :\n"
	<< "\n"
	<< "   Z			\n"
	<< " Q S D : Bouger la caméra\n"
	<< "\n"
	<< "Fleches : Bouger le cube\n"
	<< "\n"
	<< "CTRL + Fleches : Tourner le cube\n"

	<< std::endl;

	SDL_Delay(1000);

	SceneOGL scene("Test");

	if(!scene.InitSDL()) return -1;

	if(!scene.InitGL()) return -1;

	scene.MainLoop();

    return 0;
}

int main2(int argc, char *argv[])
{
	Uint32 Start;
	Uint32 End;

	int a;

	Start = SDL_GetTicks();
	for (int i = 0; i < 1000000000; ++i) // OuiOui, c'est bien 1 milliard d'affectations
	{
		a = i;
	}

	End = SDL_GetTicks();

	std::cout << "Affectations : " << End - Start << "ms." << std::endl;

	Start = SDL_GetTicks();
	for (int i = 0; i < 1000000000; ++i)
	{
		if(a == i)
			;
	}

	End = SDL_GetTicks();

	std::cout << "Comparaisons : " << End - Start << "ms." << std::endl;

	return 0;
}