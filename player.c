   #include "player.h"


  void initializePlayer(void)
   {

       /* Charge le sprite de notre h�ros */
       player.sprite = loadImage("graphics/walkright.png");

        //Indique l'�tat et la direction de notre h�ros
        player.direction = RIGHT;
        player.etat = IDLE;

        //R�initialise le timer de l'animation et la frame
        player.frameNumber = 0;
        player.frameTimer = TIME_BETWEEN_2_FRAMES;

    /* Coordonn�es de d�marrage de notre h�ros */
    player.x = 0;
    player.y = 0;

    /* Hauteur et largeur de notre h�ros */
    player.w = PLAYER_WIDTH;
    player.h = PLAYER_HEIGTH;

   //Variables n�cessaires au fonctionnement de la gestion des collisions
    player.timerMort = 0;
    player.onGround = 0;

   }

    void drawplayer()
   {
    /* Rectangle de destination � blitter */
    SDL_Rect dest;

    // On soustrait des coordonn�es de notre h�ros, ceux du d�but de la map, pour qu'il colle
    //au scrolling :
    dest.x = player.x - map.startX;
    dest.y = player.y - map.startY;
    dest.w = PLAYER_WIDTH;
    dest.h = PLAYER_HEIGTH;

    /* Rectangle source � blitter */
    SDL_Rect src;

    //Pour conna�tre le X de la bonne frame � blitter, il suffit de multiplier
    //la largeur du sprite par le num�ro de la frame � afficher -> 0 = 0; 1 = 40; 2 = 80...
    src.x = player.frameNumber * PLAYER_WIDTH;
    src.y = 0;
    src.w = PLAYER_WIDTH;
    src.h = PLAYER_HEIGTH;

    /* Blitte notre h�ros sur l'�cran aux coordonn�es x et y */

    SDL_BlitSurface(player.sprite, &src, jeu.screen, &dest);

}

 void updatePlayer(void)
 {

   //On rajoute un timer au cas o� notre h�ros mourrait lamentablement en tombant dans un trou...
   //Si le timer vaut 0, c'est que tout va bien, sinon, on le d�cr�mente jusqu'� 0, et l�,
   //on r�initialise.
   //C'est pour �a qu'on ne g�re le joueur que si ce timer vaut 0.
  if (player.timerMort == 0)
  {

    //On r�initialise notre vecteur de d�placement lat�ral (X), pour �viter que le perso
    //ne fonce de plus en plus vite pour atteindre la vitesse de la lumi�re ! ;)
    //Essayez de le d�sactiver pour voir !
    player.dirX = 0;

    // La gravit� fait toujours tomber le perso : on incr�mente donc le vecteur Y
    player.dirY += GRAVITY_SPEED;

    //Mais on le limite pour ne pas que le joueur se mette � tomber trop vite quand m�me
    if (player.dirY >= MAX_FALL_SPEED)
    {
        player.dirY = MAX_FALL_SPEED;
    }

//Voil�, au lieu de changer directement les coordonn�es du joueur, on passe par un vecteur
    //qui sera utilis� par la fonction mapCollision(), qui regardera si on peut ou pas d�placer
    //le joueur selon ce vecteur et changera les coordonn�es du player en fonction.
     if (input.left == 1)
    {
        player.dirX -= PLAYER_SPEED;
        player.direction = LEFT;

        if(player.etat != WALK_LEFT && player.onGround == 1)
        {
            player.etat = WALK_LEFT;
            player.sprite = loadImage("graphics/walkleft.png");
            player.frameNumber = 0;
        }
    }

    else if (input.right == 1)
    {
        player.dirX += PLAYER_SPEED;
        player.direction = RIGHT;

        if(player.etat != WALK_RIGHT && player.onGround == 1)
        {
            player.etat = WALK_RIGHT;
            player.sprite = loadImage("graphics/walkright.png");
            player.frameNumber = 0;
        }
    }

    //Si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant l'inactivit� (Idle)
    else if(input.right == 0 && input.left == 0 && player.onGround == 1)
    {
        //On teste si le joueur n'�tait pas d�j� inactif, pour ne pas recharger l'animation
        //� chaque tour de boucle

        if(player.etat != IDLE)
        {
            player.etat = IDLE;
            //On change l'animation selon la direction
            if(player.direction == LEFT)
            {
                player.sprite = loadImage("graphics/IdleLeft.png");
                player.frameNumber = 0;
            }
            else
            {
                player.sprite = loadImage("graphics/IdleRight.png");
                player.frameNumber = 0;
            }

        }

    }



    //Et voici la fonction de saut tr�s simple :
    //Si on appuie sur la touche saut (C par d�faut) et qu'on est sur le sol, alors on attribue une valeur
    //n�gative au vecteur Y
    //parce que sauter veut dire se rapprocher du haut de l'�cran et donc de y=0.
if (input.jump == 1)
    {
        if(player.onGround == 1)
        {
            player.dirY = -JUMP_HEIGHT;
            player.onGround = 0;
            player.jump = 1;
        }
        /* Si on est en saut 1, on peut faire un deuxi�me bond et on remet jump1 � 0 */
        else if (player.jump == 1)
        {
            player.dirY = -JUMP_HEIGHT;
            player.jump = 0;
        }
        //On remet les input � 0 par s�curit�
        input.jump = 0;
    }

    /* R�active la possibilit� de double saut si on tombe sans sauter */
    if (player.onGround == 1)
        player.jump = 1;

    //On g�re l'anim du saut
    if(player.onGround == 0)
    {
        if(player.direction == RIGHT && player.etat != JUMP_RIGHT)
        {
            player.etat = JUMP_RIGHT;
            player.sprite = loadImage("graphics/JumpRight.png");
            player.frameNumber = 0;
        }
        else if(player.direction == LEFT && player.etat != JUMP_LEFT)
        {
            player.etat = JUMP_LEFT;
            player.sprite = loadImage("graphics/JumpLeft.png");
            player.frameNumber = 0;
        }

    }

    //On rajoute notre fonction de d�tection des collisions qui va mettre � jour les coordonn�es
    //de notre super lapin, puis on centre le scrolling comme avant.
    mapCollision(&player);
    centerScrollingOnPlayer();

  }

    //Gestion de la mort quand le h�ros tombe dans un trou :
    //Si timerMort est diff�rent de 0, c'est qu'il faut r�initialiser le joueur.
    //On ignore alors ce qui pr�c�de et on joue cette boucle (un wait en fait) jusqu'� ce que
    // timerMort == 1. A ce moment-l�, on le d�cr�mente encore -> il vaut 0 et on r�initialise
    //le jeu avec notre bonne vieille fonction d'initialisation ;) !
    if (player.timerMort > 0)
    {
        player.timerMort--;

        if (player.timerMort == 0)
        {
            /* Si on est mort */
            initializePlayer();
        }
    }

  }

void centerScrollingOnPlayer(void)
 {
    //On d�finit les coordonn�es du d�but de l'affichage de la map par rapport � celles
    //du joueur.
    //Pour centrer le joueur, la map doit donc s'afficher � un demi-�cran avant la position
    //du joueur.
    //Puis on "clamp" (limite) l'affichage de l'�cran pour ne pas sortir de la map.
    map.startX = player.x - (SCREEN_WIDTH / 2);

    if (map.startX < 0)
    {
        map.startX = 0;
    }

    else if (map.startX + SCREEN_WIDTH >= map.maxX)
    {
        map.startX = map.maxX - SCREEN_WIDTH;
    }

    map.startY = player.y - (SCREEN_HEIGHT / 2);

    if (map.startY < 0)
    {
        map.startY = 0;
    }

    else if (map.startY + SCREEN_HEIGHT >= map.maxY)
    {
        map.startY = map.maxY - SCREEN_HEIGHT;
    }
 }

