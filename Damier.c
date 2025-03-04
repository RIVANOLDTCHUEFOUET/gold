// Constantes du jeu
	#define TAILLE 8 // Taille du damier (8x8)
	#define TAILLE_CASE 80 // Taille d'une case en pixels
	#define LARGEUR_FENETRE (TAILLE * TAILLE_CASE)
	#define HAUTEUR_FENETRE (TAILLE * TAILLE_CASE)
	// Définition des pièces
	#define VIDE 0
	#define PION_BLANC 1
	#define PION_NOIR 2
	#define DAME_BLANCHE 3
	#define DAME_NOIRE 4
	// structure pour représenter une position sur le damier
	typedef struct {
		int ligne;
		int colonne;
	}Position;

	// structure pour représenter un mouvement 
	typedef struct {
		position dedut;
		position fin;
	}Mouvement;
	 //Structure pour le jeu
	typedef struct {
	 int plateau[TAILLE][TAILLE]; // Représentation du plateau de jeu
	 int joueurActuel; // 1 pour blanc, 2 pour noir
	 bool partieTerminee; // Indique si la partie est terminée
	 Position pieceSel; // Position de la pièce sélectionnée (-1,-1 si aucune)
	} JeuDeDames;
	// Structure pour les ressources SDL
	typedef struct {
			SDL_Window *fenetre; // Fenêtre principale
			SDL_Renderer *rendu; // Renderer pour dessiner
			TTF_Font *police; // Police pour le texte
			SDL_Texture *texturePionBlanc;
			SDL_Texture *texturePionNoir;
			SDL_Texture *textureDameBlanc;
			SDL_Texture *textureDameNoir;
	} RessourcesSDL;

void initialiserJeu(JeuDeDame*jeu) {
 // Initialiser le plateau avec des cases vides
 for (int i = 0; i < TAILLE; i++) {
 for (int j = 0; j < TAILLE; j++) {
 jeu->plateau[i][j] = VIDE;
 }
 }
 
 // Placer les pions noirs (en haut)
 for (int i = 0; i < 3; i++) {
 for (int j = 0; j < TAILLE; j++) {
 if ((i + j) % 2 == 1) { // Cases noires uniquement
 jeu->plateau[i][j] = PION_NOIR;
 }
 }
 }
 
 // Placer les pions blancs (en bas)
 for (int i = TAILLE - 3; i < TAILLE; i++) {
 for (int j = 0; j < TAILLE; j++) {
 if ((i + j) % 2 == 1) { // Cases noires uniquement
 jeu->plateau[i][j] = PION_BLANC;
 }
 }
 }
 
 // Blanc commence
 jeu->joueurActuel = 1;
 jeu->partieTerminee = false;
}

bool initialiserSDL(RessourcesSDL *res){
// Initialiser SDL
 if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	 printf("Erreur lors de l'instalation de SDL:%s\n",SDL_GetError());
	 return false;
 }	 

//créer la fenêtre principale
res->fenetre=SDL_CreateWindow("jeu de dames",
				SDL_WINDOWPOS_UNDEFINED, 
				SDL_WINDOWPOS_UNDEFINED,
				LARGEUR_FENETRE, 
				HAUTEUR_FENETRE, 
				SDL_WINDOW_SHOWN);
				//Céer le renderer pour dessiner
				res->rendu=SDL_	CreateRenderer(res->fenetre,-1,SDL_RENDERER_ACCELERATED);			

				//Charger la police pour le texte
				res->police = TTF_OpenFont("arial.ttf", 24);
 
				// Créer les textures pour les pions et les dames
				// [Code pour créer les textures des pions et dames ici]
 
				return true;
}

void dessinerPlateau(RessourcesSDL *res, JeuDeDames *jeu) {
	// Effacer l'écran avec une couleur de fond
	SDL_SetRenderDrawColor(res->rendu, 150, 100, 50, 255); // Couleur bois clair
	SDL_RenderClear(res->rendu);
 
	// Dessiner les cases du damier (alternance de cases claires et sombres)
	for (int i = 0; i < TAILLE; i++) {
		for (int j = 0; j < TAILLE; j++) {
			SDL_Rect rect = {j * TAILLE_CASE, i * TAILLE_CASE, TAILLE_CASE, 
			TAILLE_CASE};
			if ((i + j) % 2 == 0) {
				// Case blanche
				SDL_SetRenderDrawColor(res->rendu, 240, 217, 181, 255); // Beige clair
			} else {
				// Case noire
				SDL_SetRenderDrawColor(res->rendu, 100, 65, 23, 255); // Marron foncé
				}
 
				SDL_RenderFillRect(res->rendu, &rect);
 
				// Mettre en évidence la case sélectionnée
				if (jeu->pieceSel.ligne == i && jeu->pieceSel.colonne == j) {
					SDL_SetRenderDrawColor(res->rendu, 0, 255, 0, 100); // Vert transparent
					SDL_RenderFillRect(res->rendu, &rect);
				}
					// Dessiner les pièces
					if (jeu->plateau[i][j] != VIDE) {
						SDL_Texture *texture = NULL;
						switch (jeu->plateau[i][j]){
							case PION_BLANC:
							texture = res->texturePionBlanc;
							break;
							case PION_NOIR:
							texture = res->texturePionNoir;
							break;
							case DAME_BLANCHE:
							texture = res->textureDameBlanc;
							break;
							case DAME_NOIRE:
							texture = res->textureDameNoir;
							break;
							if (texture) {
								SDL_Rect destRect = {
								j * TAILLE_CASE + 10, 
								i * TAILLE_CASE + 10, 
								TAILLE_CASE - 20, 
								TAILLE_CASE - 20
							};
							SDL_RenderCopy(res->rendu, texture, NULL, &destRect);
					}
				}
			}
		}

	}
	 // Afficher le joueur actuel
	  if (res->police) {
		SDL_Color couleur = {255, 255, 255, 255};
		char texte[50];
		sprintf(texte, "Joueur: %s", (jeu->joueurActuel == 1) ? "Blanc" : "Noir");
		SDL_Surface *surface = TTF_RenderText_Solid(res->police, texte, couleur);
		if (surface) {
			SDL_Texture *texture = SDL_CreateTextureFromSurface(res->rendu, surface);
			if (texture) {
				SDL_Rect rect = {10, 10, surface->w, surface->h};
				SDL_RenderCopy(res->rendu, texture, NULL, &rect);
				SDL_DestroyTexture(texture);
			}
			SDL_FreeSurface(surface);
		}
	}
 
	// Afficher le rendu à l'écran
	SDL_RenderPresent(res->rendu);
}
bool estMouvementValide(JeuDeDames *jeu, Mouvement mouv) {
	// Vérifier que les positions sont valides
	if (!estPositionValide(mouv.debut) || !estPositionValide(mouv.fin)) {
		return false;
	}
 
	// Vérifier que la case de départ contient un pion du joueur actuel
 int pieceDepart = jeu->plateau[mouv.debut.ligne][mouv.debut.colonne];
	if (jeu->joueurActuel == 1 && (pieceDepart != PION_BLANC && pieceDepart != 
		DAME_BLANCHE)) {
		return false;
	}
		if (jeu->joueurActuel == 2 && (pieceDepart != PION_NOIR && pieceDepart != 
			DAME_NOIRE)) {
			return false;
		}
 
		// Vérifier que la case d'arrivée est vide
		if (jeu->plateau[mouv.fin.ligne][mouv.fin.colonne] != VIDE) {
			return false;
		}
		 // Vérifier que la case d'arrivée est une case noire
			if ((mouv.fin.ligne + mouv.fin.colonne) % 2 == 0) {
				return false;
 }
 
				// Calcul des différences entre positions
				int diffLigne = mouv.fin.ligne - mouv.debut.ligne;
				int diffColonne = mouv.fin.colonne - mouv.debut.colonne;
 
				// Vérification du sens du mouvement pour les pions
				if (pieceDepart == PION_BLANC && diffLigne >= 0) {
					return false; // Les pions blancs ne peuvent aller que vers le haut
				}
					if (pieceDepart == PION_NOIR && diffLigne <= 0) {
						return false; // Les pions noirs ne peuvent aller que vers le bas
					}
 
						// Mouvement simple en diagonale
						if (abs(diffLigne) == 1 && abs(diffColonne) == 1) {
							return true;
						}
 
							// Mouvement de prise (saut par-dessus un pion adverse)
							if (abs(diffLigne) == 2 && abs(diffColonne) == 2) {
								// Position du pion à prendre
								Position positionPrise;
								positionPrise.ligne = mouv.debut.ligne + diffLigne / 2;
								positionPrise.colonne = mouv.debut.colonne + diffColonne / 2;
 
								// Vérifier qu'il y a un pion adverse à prendre
								int piecePrise = jeu->plateau[positionPrise.ligne][positionPrise.colonne];
								if (jeu->joueurActuel == 1 && (piecePrise == PION_NOIR || piecePrise == 
									DAME_NOIRE)) {
									return true;
								}
									if (jeu->joueurActuel == 2 && (piecePrise == PION_BLANC || piecePrise == 
										DAME_BLANCHE)) {
										return true;
									}
							}
 
									// Mouvement invalide dans tous les autres cas
									return false;
						}
						La fonction effectuerMouvement applique un mouvement valide :
void effectuerMouvement(JeuDeDames *jeu, Mouvement mouv) {
		// Déplacer le pion
			jeu->plateau[mouv.fin.ligne][mouv.fin.colonne] = jeu-
			>plateau[mouv.debut.ligne][mouv.debut.colonne];
			jeu->plateau[mouv.debut.ligne][mouv.debut.colonne] = VIDE;
 
		// Vérifier si c'est une prise
	if (abs(mouv.fin.ligne - mouv.debut.ligne) == 2) {
		// Calculer la position du pion pris
		int ligneIntermediaire = (mouv.debut.ligne + mouv.fin.ligne) / 2;
		int colonneIntermediaire = (mouv.debut.colonne + mouv.fin.colonne) / 2;
 
		// Enlever le pion pris
		jeu->plateau[ligneIntermediaire][colonneIntermediaire] = VIDE;
	}
 
		// Vérifier si un pion doit être transformé en dame
		if (jeu->plateau[mouv.fin.ligne][mouv.fin.colonne] == PION_BLANC && mouv.fin.ligne 
== 0) {
		jeu->plateau[mouv.fin.ligne][mouv.fin.colonne] = DAME_BLANCHE;
		}
			if (jeu->plateau[mouv.fin.ligne][mouv.fin.colonne] == PION_NOIR && mouv.fin.ligne 
== TAILLE - 1) {
				jeu->plateau[mouv.fin.ligne][mouv.fin.colonne] = DAME_NOIRE;
			}
 
			// Changer de joueur
			jeu->joueurActuel = (jeu->joueurActuel == 1) ? 2 : 1;
}
bool verifierFinPartie(JeuDeDames *jeu) {
		int compteurBlancs = 0, compteurNoirs = 0;
 
			// Compter les pions restants
		for (int i = 0; i < TAILLE; i++) {
			for (int j = 0; j < TAILLE; j++) {
				if (jeu->plateau[i][j] == PION_BLANC || jeu->plateau[i][j] == DAME_BLANCHE) {
					compteurBlancs++;
				} else if (jeu->plateau[i][j] == PION_NOIR || jeu->plateau[i][j] == DAME_NOIRE) {
					compteurNoirs++;
				}
			}
		}
			// Si un joueur n'a plus de pions, la partie est terminée
 
		if (compteurBlancs == 0) {
		printf("Les noirs ont gagné!\n");
		return true;
		} else if (compteurNoirs == 0) {
		printf("Les blancs ont gagné!\n");
		return true;
	}
 
 return false;
}
int main(int argc, char *argv[]) { 
    JeuDeDames jeu; 
    RessourcesSDL ressources; 
    bool continuer = true; 
    SDL_Event event; 
     
    // Initialiser le jeu 
    initialiserJeu(&jeu); 
    jeu.pieceSel.ligne = -1; 
    jeu.pieceSel.colonne = -1; 
     
    // Initialiser SDL 
    if (!initialiserSDL(&ressources)) { 
        printf("Erreur lors de l'initialisation de SDL!\n"); 
        return 1; 
    } 
     
    // Boucle principale 
    while (continuer && !jeu.partieTerminee) { 
        // Dessiner le plateau 
        dessinerPlateau(&ressources, &jeu); 
         
        // Gérer les événements 
        while (SDL_PollEvent(&event)) { 
            switch (event.type) { 
                case SDL_QUIT: 
                    continuer = false; 
                    break; 
                     
                case SDL_MOUSEBUTTONDOWN: 
                    if (event.button.button == SDL_BUTTON_LEFT) { 
                        Position pos = obtenirPositionClic(event.button.x, event.button.y); 
                         
                        // Si aucune pièce n'est sélectionnée, essayer d'en sélectionner une 
                        if (jeu.pieceSel.ligne == -1 || jeu.pieceSel.colonne == -1) { 
                            selectionnerPiece(&jeu, pos); 
                        } else { 
                            // Sinon, déplacer la pièce sélectionnée 
                            deplacerPiece(&jeu, pos); 
                        } 
                    } 
                    break; 
            } 
        } 
         
        // Limiter la fréquence d'images 
        SDL_Delay(16);  // Environ 60 FPS 
    } 
     
    // Afficher le message de fin si la partie est terminée 
    if (jeu.partieTerminee) { 
        // [Code pour afficher le message de fin de partie] 
        SDL_Delay(3000);  // Afficher pendant 3 secondes 
    } 
     
    // Libérer les ressources 
    libererSDL(&ressources); 
     
    return 0; 
} 
 