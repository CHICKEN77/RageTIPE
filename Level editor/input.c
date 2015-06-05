

#include "input.h"


void getInput(void)
{
	SDL_Event event;

	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

			case SDL_QUIT:
                exit(0);
			break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
					break;

					case SDLK_LEFT:
						input.left = 1;
					break;

					case SDLK_RIGHT:
						input.right = 1;
					break;

					case SDLK_UP:
						input.up = 1;
					break;

					case SDLK_DOWN:
						input.down = 1;
					break;

					/* La touche S sauvegardera */
					case SDLK_s:
						input.save = 1;
					break;

					/* La touche L chargera la map */
					case SDLK_l:
						input.load = 1;
					break;

					/* La touche DEL/Suppr réinitialisera la map */
					case SDLK_DELETE:
						input.reinit = 1;
					break;

					default:
					break;
				}
			break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
						input.left = 0;
					break;

					case SDLK_RIGHT:
						input.right = 0;
					break;

					case SDLK_UP:
						input.up = 0;
					break;

					case SDLK_DOWN:
						input.down = 0;
					break;

					default:
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
					/* Le clic gauche de la souris ajoutera la tile en cours */
					case SDL_BUTTON_LEFT:
						input.add = 1;
					break;

					/* Le clic central de la souris effacera la tile sélectionnée */
					case SDL_BUTTON_MIDDLE:
						input.remove = 1;
					break;

					/* Le clic droit de la souris copiera la tile sélectionnée */
					case SDL_BUTTON_RIGHT:
						input.copy = 1;
					break;

					/* La roue de la souris fera défiler les tiles */
					case SDL_BUTTON_WHEELUP:
                        input.next = 1;
                    break;

                    case SDL_BUTTON_WHEELDOWN:
                        input.previous = 1;
                    break;


					default:
					break;
				}
			break;

			case SDL_MOUSEBUTTONUP:
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:
						input.add = 0;
					break;

					case SDL_BUTTON_MIDDLE:
						input.remove = 0;
					break;

                    default:
					break;
				}
			break;
		}
	}

	/* Enregistre les coordonnées de la souris */

	SDL_GetMouseState(&input.mouseX, &input.mouseY);

	/* Cette série d'opérations permet d'obtenir les coordonnées exactes de chaque
	tile, tandis que la souris réagit au pixel près */

	input.mouseX /= TILE_SIZE;
	input.mouseY /= TILE_SIZE;

	input.mouseX *= TILE_SIZE;
	input.mouseY *= TILE_SIZE;
}



void update(void)
{

    /* Pour l'affichage du curseur (indiquant la tile à copier, mais on reviendra dessus
    dans la fonction draw() ), on récupère les coordonnées de la souris */

    cursor.x = input.mouseX;
	cursor.y = input.mouseY;

    /* Gestion de notre scrolling du chapitre précédent */
 	if (input.left == 1)
	{
		map.startX -= TILE_SIZE;

		if (map.startX < 0)
		{
			map.startX = 0;
		}
	}

	else if (input.right == 1)
	{
		map.startX += TILE_SIZE;

		if (map.startX + SCREEN_WIDTH >= map.maxX)
		{
			map.startX = map.maxX - SCREEN_WIDTH;
		}
	}

	if (input.up == 1)
	{
		map.startY -= TILE_SIZE;

		if (map.startY < 0)
		{
			map.startY = 0;
		}
	}

	else if (input.down == 1)
	{
		map.startY += TILE_SIZE;

		if (map.startY + SCREEN_HEIGHT >= map.maxY)
		{
			map.startY = map.maxY - SCREEN_HEIGHT;
		}
	}

    /* Gestion de la copie de tile :
    on retrouve les coordonnées de la tile pointée par la souris et on remplace
    sa valeur par celle de la tile sélectionnée dans le curseur */

    if (input.add == 1)
	{

		map.tile[(map.startY + cursor.y) / TILE_SIZE][(map.startX + cursor.x) / TILE_SIZE] = cursor.tileID;

	}

	/* Même fonctionnement, sauf qu'on réinitialise la tile pointée en lui donnant
	la valeur BLANK_TILE (0 par défaut) */

	else if (input.remove == 1)
	{

		map.tile[(map.startY + cursor.y) / TILE_SIZE][(map.startX + cursor.x) / TILE_SIZE] = BLANK_TILE;

		cursor.tileID = 0;
	}

    /* On fait défiler les tiles dans un sens ou dans l'autre */

	if (input.previous == 1)
	{
		cursor.tileID--;

        if (cursor.tileID < 0)
        {
            cursor.tileID = MAX_TILES - 1;
        }
        else if (cursor.tileID > MAX_TILES)
        {
            cursor.tileID = 0;
        }


		input.previous = 0;
	}

	if (input.next == 1)
	{
		cursor.tileID++;

        if (cursor.tileID < 0)
        {
            cursor.tileID = MAX_TILES - 1;
        }
        else if (cursor.tileID > MAX_TILES)
        {
            cursor.tileID = 0;
        }

		input.next = 0;
	}

    /* On copie le numéro de la tile pointée dans le curseur pour qu'il affiche et colle
    désormais cette tile */

    if (input.copy == 1)
	{
		cursor.tileID = map.tile[(map.startY + cursor.y) / TILE_SIZE] [(map.startX + cursor.x) / TILE_SIZE];

		input.copy = 0;
	}

    /* Pour réinitialiser la map, on appelle la fonction reinitMap puis on recharge la map */

    if (input.reinit == 1)
	{
		reinitMap("map/map1.txt");
		loadMap("map/map1.txt");

		input.reinit = 0;
	}

    /* Sauvegarde la map (cf. plus loin) */

	if (input.save == 1)
	{
		saveMap("map/map1.txt");

		input.save = 0;
	}

    /* Charge la map (notre bonne vieille fonction ;) ) */

	if (input.load == 1)
	{

		loadMap("map/map1.txt");



		input.load = 0;
	}

	/* On rajoute un délai entre 2 tours de boucle pour que le scrolling soit moins rapide */

	if (input.left == 1 || input.right == 1 || input.up == 1 || input.down == 1)
	{
		SDL_Delay(30);
	}

}

