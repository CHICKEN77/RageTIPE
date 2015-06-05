

#include "defs.h"

/* Structures qui seront utilisées pour gérer le jeu */

/* Structure pour gérer l'input (clavier puis joystick) */

typedef struct Input
{
	int left, right, up, down, add, remove;
	int previous, next, load, save, copy, reinit;
	int mouseX, mouseY;

} Input;


/* Structure pour gérer le niveau (à compléter plus tard) */

typedef struct Gestion
{

    SDL_Surface *screen;

} Gestion;


/* Structure pour gérer la map à afficher (à compléter plus tard) */

typedef struct Map
{

	SDL_Surface *background, *tileSet;

	/* Coordonnées de début, lorsqu'on doit dessiner la map */
	int startX, startY;

	/* Coordonnées max de fin de la map */
	int maxX, maxY;

	/* Tableau à double dimension représentant la map de tiles */
	int tile[MAX_MAP_Y][MAX_MAP_X];

} Map;


typedef struct Cursor
{
	int x, y, tileID;
} Cursor;




