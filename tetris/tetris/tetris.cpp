/* En-tête du programme */
///==================== */
/* Programme:		tutoGitsSFML.cpp
// Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
// Date création:	16/04/2017
// Description:		Programme des essais SFML et de connection Git pour le projet final en C++
// .				Espace de développement pour l'application.*/

/* Directives au pré-processeur */
///============================ */
#include <saisieSecurisee.h>	//Pour la fonction ouvrirFichier
#include <iomanip>				
#include <locale>				
#include <string>				
#include <vector>				
#include <math.h>				
#include <cstdlib>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <SFML/Audio.hpp>		
#include <salle.h>				// Contient bloc.h qui contient carre.h
using namespace sf;


/* Prototypes des fonctions */

int saisie(RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3, identite & identite);
int menuOption(RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3, identite & identite);
int choixMenu(RenderWindow & windowMenu, RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3, identite & identite);
void afficherMenu(RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3,
	Texture & texture, Font & font, identite & identite);

string saisirNomJoueur(RenderWindow &window2, Font font, Texture texture, identite & identite);
int questionOuiNonSFML(RenderWindow & window2, identite & identite);
void initTitre(Text titre[6], Font & font, float posx, float posy, float size);
void afficheTitre(RenderWindow & window, Text titre[6]);
int listeEnregistrement(RenderWindow & windowMenu, identite & identite);
int questionEnregistrement(RenderWindow & window2, Font font, identite & identite);
void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille);
void enregistrerScore(identite & identite);
void afficherScore(RenderWindow & windowMenu);
void afficherInfo(RenderWindow &window, identite & joueur, Font font,Text text );

/* Constantes pour le programme principal */
///====================================== */
const double PI = atan(1) * 4;	// Valeur de pi (nécessaire?)

struct boutonClick
{
	int posXRect = 350,
		posYRect = 50,
		hauteur = 50,
		largeur = 300,
		posXTexte = 375,
		posYTexte = 75;
	RectangleShape rectangle;
	Texture fondImage;
	Text text;
	Font font;
};

/* Programme principal */
///=================== */
int main()
{

	identite joueur;

	setlocale(LC_CTYPE, "fra"); //met la langue en francais
	srand(time(NULL));		// Trouve une source aléatoire

	RenderWindow window;
	RenderWindow window2;

	string nomJoueur;

	Font fontTitre;
	if (!fontTitre.loadFromFile("styles/hemi_head_bd_it.ttf"));		//Si la police n'est pas trouvable, une erreur est afficher dans la console
	Font font;
	if (!font.loadFromFile("styles/font_arial.ttf"));

	Music music;
	Music music2;
	Music music3;

	vector<Vector2i> occupations;

	salle espace(window, "styles/font_arial.ttf", POS, 1, 1, occupations, nomJoueur, 1, 0, 0, milliseconds(800), tetris, 7);

	Texture texture;
	if (!texture.loadFromFile("images/back_main.jpg"));		//Si la texture n'est pas trouvable, une erreur est afficher dans la console

	Sprite background(texture);

	Event event;
	Time moment;
	unsigned seed = time(0);			/// Même chose que srand ?
	Clock tempsEcoule;

	int mouvement = -1;
	int i = 0;
	int lvl = 1,
		score = 0;
	int identiteEnregistrer, reponse = 1;


	Text titre[6];
	Text text;
										// Fenêtre d'enregistrements
	window2.create(VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");

	do
	{
		saisirNomJoueur(window2, font, texture, joueur);
		espace.setNomJoueur(joueur.nomJoueur);

		identiteEnregistrer = listeEnregistrement(window2, espace.getJoueur());	
		if (identiteEnregistrer == 1)													//Si le joueur n'est pas dans la base de donnée, les statistiques retourne a 0
			reponse = questionEnregistrement(window2, font, joueur);
		else
		{
			reponse = 1;
			lvl = 0;
			score = 0;
		}
	} while (reponse != 1);

	window2.close();

	// Fenetre principal
	window.create(VideoMode(POSAFFICHE.x + 300 + LRGPOLICE, DIMSALLE.y + 200), "TETRIS Jeu"); 

	Image icon;
	icon.loadFromFile("images/icon.png");
	window.setIcon(128, 128, icon.getPixelsPtr()); //Affiche l'icone de Tétris
	initTitre(titre, fontTitre, 110, 10, 60);

	afficherMenu(window, titre, music, music2, music3, texture, font, joueur);
	espace.setNomJoueur(joueur.nomJoueur);
	

	// Fenêtre du jeu principal

	for (size_t j = 0; j < 5;j++)			//Positionne le titre
	{
		titre[j].setPosition(Vector2f(100 + (j* titre[j].getCharacterSize()), 10));
	}
	titre[5].setPosition(Vector2f(100 + (5* titre[5].getCharacterSize())- 25, 10));

	do
	{
		int posy, posx;
		int nbY = 0;

		do {
			Sprite background(texture);
			window.draw(background);	
		
			
			afficheTitre(window, titre);

			if (tempsEcoule.getElapsedTime() > espace.getVitesse())
			{
				tempsEcoule.restart();
				moment.asMilliseconds();
				mouvement = 0;
			}
			else
			{
				mouvement = saisie(window, titre, music, music2, music3, espace.getJoueur());
				nbY++;
			}

			switch (mouvement)
			{
				/// Utiliser Enum pour les valeurs
			case 2:	// Avec Z		(Tourner vers la gauche)		
				espace.pivoteActifGauche();
				break;
			case 3:	// Haut ou X	(Tourner vers la droite)
				espace.pivoteActifDroite();
				/// espace.bouge(0, -1);
					
				break;
			case 0: // Descente du bloc automatique
				espace.setColle(!espace.bougeActif(0, 1));
				break;
			case 5:	// Bas			(Faire tomber)
				espace.tombeActif();
			
				break;
			case 4:	// Gauche		(Bouger)
				espace.bougeActif(-1, 0);
			
				break;
			case 6:	// Droite		(Bouger)
				espace.bougeActif(1, 0);
			
				break;
			case 7:	// P (Menu option)
				menuOption(window, titre, music, music2, music3, joueur);
				espace.setNomJoueur(joueur.nomJoueur);
				break;
			case 1:	// Esc (retour au menu)	

				enregistrerScore(joueur);
				afficherMenu(window, titre, music, music2, music3, texture, font, joueur);
				espace.setNomJoueur(joueur.nomJoueur);
				break;
			default:
				break;
			}

			// Si une colision est présente
			if (espace.getColle())
			{
				// Ajoute le bloc actif aux occupation de la salle 
				// Démare ensuite le prochain bloc tout en trouvant le bloc suivant
				
				espace.colleActif();
				joueur.score += 25;		//ajoute des point au joueur.
				espace.setColle(false);
			}

			
			window.draw(background);

			espace.afficherInterface();
			espace.afficheBlocsSalle();
			afficheTitre(window, titre);
			afficherInfo(window, espace.getJoueur(), font, text);

			espace.drawActif(); //dessine la piece active	

			window.display();
			
		} while (ok);

	} while (ok);
	enregistrerScore(espace.getJoueur());
	return 0;
}

//Affiche les informations du joueur (nom, lvl, score) dans l'interface de jeu
void afficherInfo(RenderWindow &window,identite & joueur, Font font, Text text)
{
	
	font.loadFromFile("styles/font_arial.ttf");

	text.setFont(font);
	text.setString(joueur.nomJoueur);
	text.setColor(Color::Green);
	text.setCharacterSize(35);
	text.setPosition(675, 35);
	text.setStyle(sf::Text::Underlined);
	window.draw(text);

	text.setStyle(sf::Text::Regular);
	text.setColor(Color::Red);

	text.setCharacterSize(30);
	text.setPosition(650, 295);
	text.setString(to_string(joueur.level));
	window.draw(text);

	text.setPosition(655, 355);
	text.setString(to_string(joueur.score));
	window.draw(text);
}
// Trie en ordre croissant les scores des joueurs.
void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille)
{
	char nomJoueurTemp[20];
	int scoreJoueur[20];
	int i, j;
	for (i = 1; i < taille; ++i) {
		int elem = scoreMax[i];
		strcpy_s(nomJoueurTemp, 20, nomJoueur[i]);
		for (j = i; j > 0 && scoreMax[j - 1] < elem; j--) {
			scoreMax[j] = scoreMax[j - 1];
			strcpy_s(nomJoueur[j], 20, nomJoueur[j - 1]);
		}
		scoreMax[j] = elem;
		strcpy_s(nomJoueur[j], 20, nomJoueurTemp);
	}
}
// Retourne une valeur correspondant au choix saisi entre 'o' et 'n'
int questionOuiNonSFML(RenderWindow & window2, identite &joueur)
{
	Event event;
	while (window2.isOpen())
	{

		while (window2.waitEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
			case Event::Closed:
			{
				exit(0);
			}
			// key pressed
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Unknown))
					;
				else if (event.key.code == Keyboard::Escape) // Pour quitter
					return 3;
				else if (event.key.code == Keyboard::O)
					return 1;
				else if (event.key.code == Keyboard::N)
					return 2;

			} while (event.key.code == Keyboard::N && event.key.code == Keyboard::O);
		}
	}
}

//Demande au joueur s'il veux enregistrrer son score et attend une reponse de QuestionOuiNon 
void enregistrerScore(identite &joueur) {


	RenderWindow window3(VideoMode(633, 460), "TETRIS v1.2 Enregistrement");
	Texture texture3;
	Font font;
	if (!font.loadFromFile("styles/font_arial.ttf"));
	Text text;
	if (!texture3.loadFromFile("images/tetris_end.jpg"));	//va chercher le fond d'écran
	Sprite background(texture3);
	
	

	int reponse = 0;
	int longueurNom = 1;
	
	text.setColor(Color::Green);
	text.setFillColor(Color::Green);
	text.setFont(font);
	text.setPosition(100, 50);
	text.setCharacterSize(22);
	window3.draw(background);

	do {

		if (joueur.nouveauJoueur == false) {
			
			text.setString("Voulez vous enregistrer par-dessus \n votre ancienne sauvegarde? \n (o / n)? : ");
			window3.draw(text);
			window3.display();

		}
		else {
		
			text.setString("Voulez vous enregistrer votre score? \n (o / n)? : ");
			window3.draw(text);
			window3.display();


		}
		reponse = questionOuiNonSFML(window3, joueur);
	} while (reponse != 1 && reponse != 2);

	if (reponse == 1) {						//Enregistre tout les score dans un tableau

		ifstream vieuxscore;
		ofstream nouveauscore;

		vieuxscore.open("score.txt", ifstream::in | ifstream::out | ifstream::app);
		nouveauscore.open("score.txt", ofstream::in | ofstream::out | ofstream::app);

		char listeNomJoueur[20][20] = { '\0' };				//maximum de 30 identite enregistrer
		int levelMax[20];
		int scoreMax[20];

		int cptJoueur = 0;

		if (joueur.nouveauJoueur == false) {					//si le identite existe, remplace sont score par le nouveau
			while (vieuxscore >> listeNomJoueur[cptJoueur])
			{
				vieuxscore >> levelMax[cptJoueur];
				vieuxscore >> scoreMax[cptJoueur];
				cptJoueur++;
			}

			vieuxscore.close();
			vieuxscore.open("score.txt", ifstream::out | ifstream::trunc);
			vieuxscore.close();

			trierInsertion(listeNomJoueur, scoreMax, cptJoueur);




			for (int i = 0; i < joueur.rang - 1; i++)		// Écrit tout les autre identite avant
			{
				nouveauscore << left << setw(20) << listeNomJoueur[i] << setw(20) << levelMax[i] << scoreMax[i] << endl;
			}
			nouveauscore << left << setw(20) << joueur.nomJoueur << setw(20) << joueur.level << joueur.score;

			for (int i = 0 + joueur.rang; i < cptJoueur; i++) // Écrit tout les autre identite apres
			{
				nouveauscore << endl << left << setw(20) << listeNomJoueur[i] << setw(20) << levelMax[i] << scoreMax[i];
			}
		}

		else										//inscrit le nouveau identite a la fin de la liste de score
		{
			nouveauscore << endl << left << setw(20) << joueur.nomJoueur << setw(20) << joueur.level << joueur.score;
			joueur.nouveauJoueur = false;
		}
		nouveauscore.close();
	}
}

// Affiche le nom, le level et le score des 10 meilleurs joueurs dans un fichier score
void afficherScore(RenderWindow &windowMenu)
{

	Text text;
	Font font;
	Texture texture;
	if (!font.loadFromFile("styles/font_arial.ttf"));		//va chercher la police pour le texte
	if (!texture.loadFromFile("images/back_main.jpg"));	//va chercher le fond d'écran

	Sprite background(texture);
	windowMenu.draw(background);		//affiche le fond d'écran	

	text.setFont(font);
	text.setColor(Color::White);
	string nomJoueur;
	int lvl;
	int score;
	ifstream fichierScore;
	if (!ouvrirFichier(fichierScore, "score.txt", cout));

	text.setOutlineThickness(2);
	text.setOutlineColor(Color::Black);
	text.setPosition(400, 50);
	text.setString("10 Meilleurs Scores!");
	windowMenu.draw(text);

	text.setPosition(300, 100);
	text.setString("Joueur");
	windowMenu.draw(text);

	text.setPosition(475, 100);
	text.setString("Level");
	windowMenu.draw(text);
	text.setPosition(675, 100);
	text.setString("Score");
	windowMenu.draw(text);

	for (size_t i = 0; i < 10; i++)
	{
		fichierScore >> nomJoueur >> lvl >> score;

		text.setPosition(300, 150 + (50 * i));
		text.setString(nomJoueur);
		windowMenu.draw(text);

		text.setPosition(500, 150 + (50 * i));

		text.setString(to_string(lvl));
		windowMenu.draw(text);

		text.setPosition(700, 150 + (50 * i));
		text.setString(to_string(score));
		windowMenu.draw(text);
	}
	windowMenu.display();

	Event event;
	while (windowMenu.isOpen())
	{


		while (windowMenu.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
			case Event::Closed:
			{
				exit(0);
			}
			// key pressed
			case Event::KeyPressed:

				if (event.key.code == Keyboard::Escape) // Pour quitter
				{
					return;

				}

			}
		}
	}
}

// Demande au joueur s'il veut reprendre le profil d'un joueur enregistrer dans le fichier score
int questionEnregistrement(RenderWindow & window2, Font font, identite & identite)
{
	window2.clear();
	setlocale(LC_CTYPE, "fra");
	Event event;

	window2.display();
	Text text;

	
	text.setFont(font);
	text.setString("Ce nom appartient a un identite contenant \nles statistiques suivante :");
	text.setCharacterSize(22);
	text.setPosition(25, 130);
	text.setColor(Color::Black);
	window2.draw(text);


	char levelMax[15] = "Level max : ";
	levelMax[12] = identite.level + '0';
	text.setString(levelMax);
	text.setPosition(25, 185);
	window2.draw(text);

	char textScoreMax[15] = "Score max : \0";
	char scoreMax[15];
	sprintf(scoreMax, "%d", identite.score);
	strcat_s(textScoreMax, scoreMax);
	text.setString(textScoreMax);
	text.setPosition(25, 225);
	window2.draw(text);

	text.setString("Voulez vous continer avec cet enregistrement? \n Appuyez sur O ou sur N");
	text.setPosition(25, 275);
	window2.draw(text);
	/*identiteEnregistrer.setPosition(25, 25);
	identiteEnregistrer.setFillColor(Color::Black);*/
	/*window2.draw(identiteEnregistrer);*/
	window2.display();
	if (questionOuiNonSFML(window2, identite) != 1)
	{

		return 2;
	}
	identite.nouveauJoueur = false;
	return 1;
}

// Verifie si le nom saisie est déja dans le fichier score
int listeEnregistrement(RenderWindow & window2, identite & identite)
{
	ifstream labyrinthe;
	char sauvegardeNomJoueur[15];
	int lvlEnregistrer,
		scoreEnregistrer,
		rangJoueur = 0;
	char continuerPartie;
	bool sauvegardeExiste = false,
		charIdentique = false;
	int longueur = NBCHARMAX;
	RectangleShape identiteEnregistrer(Vector2f(300, 125));
	Font font;
	if (!font.loadFromFile("styles/font_arial.ttf")); // choix de la police à utiliser
	Text text;

	//strcpy_s(jeu.w.police, 30, nomsPolices[7]);
	//jeu.w.mesure -= 5;
	//saisirTabCharSDL(identite.info.nom, longueur, "Quel est le nom de votre personnage ? : ", jeu);
	if (!ouvrirFichier(labyrinthe, "score.txt", cout))
		exit(0);

	while (labyrinthe >> sauvegardeNomJoueur && sauvegardeExiste == false) {			//lis le fichier de score au complet

		labyrinthe >> lvlEnregistrer >> scoreEnregistrer;
		rangJoueur++;
		strcat(sauvegardeNomJoueur, "\0");
		int longeurNom = strlen(sauvegardeNomJoueur);

		if (identite.nomJoueur == sauvegardeNomJoueur)
		{//si le nom entrer existe dans le fichier score

			sauvegardeExiste = true;
			break;
		}
		else {

			sauvegardeExiste = false;
		}

	}

	if (sauvegardeExiste == true) {
		identite.level = lvlEnregistrer;
		identite.score = scoreEnregistrer;
		identite.rang = rangJoueur;
		return 1;

	}
	else
		return 2;


}

// Demande au joueur de saisir un nom et attend qu'il appuie sur la touche enter
string saisirNomJoueur(RenderWindow &window2, Font font, Texture texture, identite & joueur)
{
	Image icon;
	Text text;
	text.setFont(font);
	icon.loadFromFile("images/icon.png");			//va chercher l'image de l'icone
	window2.setIcon(128, 128, icon.getPixelsPtr()); // Affiche l'icone

	if (!texture.loadFromFile("images/back_option.jpg"));	//va chercher le fond d'écran

	RectangleShape nomJoueur(Vector2f(400, 100));
	Texture textureNom;
	if (!textureNom.loadFromFile("images/back_option.jpg", IntRect(100, 100, 400, 100)));
	nomJoueur.setTexture(&textureNom);
	nomJoueur.setPosition(100, 100);

	Sprite background(texture);
	window2.draw(background);		//affiche le fond d'écran	

	text.setString("Quel est votre nom? \n (10 caractere Max)"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(100, 15);		// position du texte
	text.setColor(Color::Red);   // choix de la couleur du texte
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(2);

	window2.draw(text);
	window2.display();

	std::string s;
	while (window2.isOpen())
	{
		//Event processing.
		Event event;
		while (window2.pollEvent(event)) {
		}
		while (window2.waitEvent(event)) {
			Sprite background(texture);
			window2.draw(background);		//affiche le fond d'écran	
			text.setCharacterSize(30);
		
			text.setString("Quel est votre nom? \n (10 caractere Max)"); 	// choix de la chaîne de caractères à afficher
			text.setPosition(100, 15);		// position du texte
			text.setColor(Color::Red);   // choix de la couleur du texte
			text.setOutlineColor(Color::Black);
			text.setOutlineThickness(2);

			window2.draw(text);
			if (event.type == Event::Closed)
				window2.close();
		
			if (event.type == Event::TextEntered){
				if (event.key.code == Keyboard::Escape)
						window2.close();
				if (event.key.code == 8 && s.size() != 0)		//touche backspace
				{
					s.pop_back();  //Enleve le dernier char dans la string
				}
				else if (event.key.code == 13 ) //touche enter
				{
					if (s.size() != 0)
					{
						joueur.nomJoueur = s;
						return joueur.nomJoueur;
					}
					else
					{
						text.setColor(Color::Cyan);
						text.setCharacterSize(20);
						text.setString("Veuillez saisir au minimum une lettre ou un chiffre"); 	// choix de la chaîne de caractères à afficher
						text.setPosition(0, 400);		// position du texte
						window2.draw(text);
					}
				}
				else if (
					((event.key.code >= 97 && event.key.code <= 122)
					|| event.key.code == 32 || (event.key.code == Keyboard::RShift || event.key.code == Keyboard::LShift)
					|| (event.key.code >= 48 && event.key.code <= 57))
					&& (s.size() <= 10))	// caractere ASCII (A - Z, 0 - 9 )
				{
					s.push_back((char)event.text.unicode); // ajoute un char a la string
				}

				if (s.size() != 0)
				{
					
					window2.draw(nomJoueur);	//Affiche un rectangle vide

					text.setString(s); 	// choix de la chaîne de caractères à afficher
					text.setPosition(100, 100);		// position du texte
					text.setColor(Color::Green);   // choix de la couleur du texte

					window2.draw(text);
				}
				window2.display();

			}
			
		}
	
	}

}

// Saisies durant le jeu.
int saisie(RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3, identite & joueur)
{
	Event event;
	//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z .20-46								26
	//Num0 Num1 Num2 Num3 Num4 Num5 Num6 Num7 Num8 Num9 .0-9									10
	//Numpad0 Numpad1 Numpad2 Numpad3 Numpad4 Numpad5 Numpad6 Numpad7 Numpad8 Numpad9 .10-19	10
	//Tilde SemiColon Comma Period Quote Unknown .-1 
	//LControl RControl LShift RShift LAlt RAlt LSystem RSystem .47-56							9
	//LBracket RBracket Slash BackSlash //LParentesis RParentesis .58-							9
	//Equal Dash Space Return BackSpace Tab PageUp PageDown End Home Insert Delete				12
	//Add Substract Multiply Divide Left Right Up Down											8
	//F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 F13 F14 F15 Pause KeyCount (toujours 102?)			15+2

	while (window.pollEvent(event))
		// Check the type of the event...
		switch (event.type)
		{
			// Window closed
		case Event::Closed:
		{
			window.close();
			break;
		}
		// Key pressed
		case Event::KeyPressed:
			/// Faire la vérification Keyboard::_ en switch au lieu?
			if (Keyboard::isKeyPressed(Keyboard::Unknown))
				;
			// Pour retourner dans les options
			else if (event.key.code == Keyboard::Escape)
				return 1;
			// Tourne à gache
			else if (event.key.code == Keyboard::Z)
				return 2;
			// Faire tomber la pièce
			else if (event.key.code == Keyboard::Down)
				return 5;
			// Faire tourner la pièce vers la droite
			else if (event.key.code == Keyboard::X || event.key.code == Keyboard::Up)
				return 3;
			// Bouger la pièce vers la gauche
			else if (event.key.code == Keyboard::Left)
				return 4;
			// Bouger la pièce vers la droite
			else if (event.key.code == Keyboard::Right)
				return 6;
			// Arrête le jeu... menu?
			else if (event.key.code == Keyboard::Pause || event.key.code == Keyboard::P)
			{
				return 7;
			}
			// Met la musique sur stop
			else if (event.key.code == Keyboard::M)
			{
				//Met la musique sur stop	/ ??
				music.setVolume(0);
				music2.setVolume(0);
				music3.setVolume(0);
			}
			// Remet la musique au volume normal
			else if (event.key.code == Keyboard::Q)
			{
				music.setVolume(100);
				music2.setVolume(100);
				music3.setVolume(100);
			}
			else if (event.key.code == Keyboard::T) // Fait jouer la prochaine musique dans la liste
			{
				if (music.getStatus() == sf::Music::Playing)
				{
					music.stop();
					music2.openFromFile("styles/music2.ogg");
					music2.play();
				}
				else if (music2.getStatus() == sf::Music::Playing)
				{
					music2.stop();
					music3.openFromFile("styles/music3.ogg");
					music3.play();

				}
				else if (music3.getStatus() == sf::Music::Playing)
				{
					music3.stop();
					music.openFromFile("styles/music.ogg");
					music.play();
				}
				else
				{
					music.openFromFile("styles/music.ogg");
					music.play();
				}
			}
		}
	return -1;
}

// Affiche le menu des option
int menuOption(RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3, identite &joueur)
{
	window.setVisible(false);	//Cache le Menu principal;


	RenderWindow windowOption(VideoMode(1000, 800), "TETRIS Option"); // Crée une nouvelle fenetre
	Event event;

	for (size_t i = 0; i < 5; i++)
	{
		titre[i].setPosition(Vector2f(350 + (i* titre[i].getCharacterSize()), 50));
	}
	titre[5].setPosition(Vector2f(350 + (5 * titre[5].getCharacterSize()) - 25, 50));
	while (window.isOpen())
	{
		Texture texture;
		Image icon;
		icon.loadFromFile("images/icon.png");		//cherche l'image de l'icone
		windowOption.setIcon(128, 128, icon.getPixelsPtr());	//Défini une icone pour la fenetre

		Font font;
		Text text;

		if (!texture.loadFromFile("images/back_main.jpg"));  //Si la texture est introuvable, envoie un message d'erreur

		if (!font.loadFromFile("styles/font_arial.ttf")); // choix de la police à utiliser

		Sprite background(texture);

		windowOption.draw(background); // Affiche le fond d'écran



		const int nbrBouton = 10;

		boutonClick bouton[nbrBouton];

		for (size_t i = 0; i < nbrBouton; i++)
		{
			bouton[i].rectangle.setSize(Vector2f(bouton[i].hauteur, bouton[i].largeur));
			bouton[i].rectangle.setOutlineThickness(5);  //défini la grosseur de la bordure
			bouton[i].rectangle.setOutlineColor(Color::Red); //défini la couleur de la bordure
			bouton[i].text.setCharacterSize(30);
			bouton[i].text.setStyle(Text::Bold);
			bouton[i].text.setColor(Color::Black);
			bouton[i].fondImage.setSmooth(true);
			bouton[i].text.setFont(font);
		}

		afficheTitre(windowOption, titre);

		bouton[0].hauteur = 100;
		bouton[0].largeur = 300;
		bouton[0].rectangle.setSize(Vector2f(bouton[0].largeur, bouton[0].hauteur));
		bouton[0].rectangle.setPosition(350, 150); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(0, 200, 300, 100))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[0].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[0].text.setString("Option"); 	// choix de la chaîne de caractères à afficher
		bouton[0].text.setPosition(450, 188);		// position du texte

		windowOption.draw(bouton[0].rectangle);		//Dessine le bouton mussique
		windowOption.draw(bouton[0].text);	//Dessine le texte de bouton mussique

		bouton[7].hauteur = 100;
		bouton[7].largeur = 200;
		bouton[7].rectangle.setSize(Vector2f(bouton[7].largeur, bouton[7].hauteur));
		bouton[7].rectangle.setPosition(250, 275); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(0, 300, 225, 100))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[7].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[7].text.setString("Volume"); 	// choix de la chaîne de caractères à afficher
		bouton[7].text.setPosition(290, 305);		// position du texte
		windowOption.draw(bouton[7].rectangle);		//Dessine le bouton mussique
		windowOption.draw(bouton[7].text);	//Dessine le texte de bouton mussique

		bouton[1].hauteur = 40;
		bouton[1].largeur = 50;
		bouton[1].rectangle.setSize(Vector2f(bouton[1].largeur, bouton[1].hauteur));
		bouton[1].rectangle.setPosition(585, 310); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[1].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[1].text.setString("On"); 	// choix de la chaîne de caractères à afficher
		bouton[1].text.setPosition(590, 275);		// position du texte

		bouton[2].hauteur = 40;
		bouton[2].largeur = 50;
		bouton[2].rectangle.setSize(Vector2f(bouton[2].largeur, bouton[2].hauteur));
		bouton[2].rectangle.setPosition(760, 310); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[2].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[2].text.setString("Off"); 	// choix de la chaîne de caractères à afficher
		bouton[2].text.setPosition(765, 275);		// position du texte

		bouton[1].text.setColor(Color::Green);
		bouton[2].text.setColor(Color::Green);

		if (music.getVolume() == 0)
		{
			bouton[1].rectangle.setFillColor(Color::White);
			bouton[2].rectangle.setFillColor(Color::Red);
		}
		else
		{
			bouton[1].rectangle.setFillColor(Color::Red);
			bouton[2].rectangle.setFillColor(Color::White);
		}


		windowOption.draw(bouton[1].rectangle);		//Dessine le bouton mussique
		windowOption.draw(bouton[1].text);	//Dessine le texte de bouton mussique
		windowOption.draw(bouton[2].rectangle);		//Dessine le bouton mussique
		windowOption.draw(bouton[2].text);	//Dessine le texte de bouton mussique



		bouton[8].hauteur = 200;
		bouton[8].largeur = 200;
		bouton[8].rectangle.setSize(Vector2f(bouton[8].largeur, bouton[8].hauteur));
		bouton[8].rectangle.setPosition(250, 400); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(150, 150, 200, 200))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[8].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[8].text.setString("Musique"); 	// choix de la chaîne de caractères à afficher
		bouton[8].text.setPosition(290, 475);		// position du texte
		windowOption.draw(bouton[8].rectangle);		//Dessine le bouton mussique
		windowOption.draw(bouton[8].text);	//Dessine le texte de bouton mussique
		

		bouton[3].hauteur = 50;
		bouton[3].largeur = 400;
		bouton[3].rectangle.setSize(Vector2f(bouton[3].largeur, bouton[3].hauteur));
		bouton[3].rectangle.setPosition(500, 400); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[3].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[3].text.setString("Snack-Bar Chez Remond"); 	// choix de la chaîne de caractères à afficher
		bouton[3].text.setPosition(505, 405);		// position du texte

		bouton[4].hauteur = 50;
		bouton[4].largeur = 400;
		bouton[4].rectangle.setSize(Vector2f(bouton[4].largeur, bouton[4].hauteur));
		bouton[4].rectangle.setPosition(500, 475); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[4].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[4].text.setString("The Turntable Turnabout"); 	// choix de la chaîne de caractères à afficher
		bouton[4].text.setPosition(505, 480);		// position du texte

		bouton[5].hauteur = 50;
		bouton[5].largeur = 400;
		bouton[5].rectangle.setSize(Vector2f(bouton[4].largeur, bouton[4].hauteur));
		bouton[5].rectangle.setPosition(500, 550); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[5].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[5].text.setString("Tetris - Theme 'A' Acapella"); 	// choix de la chaîne de caractères à afficher
		bouton[5].text.setPosition(505, 555);		// position du texte

		if (music.getStatus() == sf::Music::Playing)
		{
			bouton[3].rectangle.setFillColor(Color::Magenta);
			bouton[4].rectangle.setFillColor(Color::White);
			bouton[5].rectangle.setFillColor(Color::White);
		}
		else if (music2.getStatus() == sf::Music::Playing)
		{
			bouton[3].rectangle.setFillColor(Color::White);
			bouton[4].rectangle.setFillColor(Color::Magenta);
			bouton[5].rectangle.setFillColor(Color::White);
		}
		else if (music3.getStatus() == sf::Music::Playing)
		{
			bouton[3].rectangle.setFillColor(Color::White);
			bouton[4].rectangle.setFillColor(Color::White);
			bouton[5].rectangle.setFillColor(Color::Magenta);
		}


		windowOption.draw(bouton[3].rectangle);		//Dessine le bouton musique
		windowOption.draw(bouton[3].text);	//Dessine le texte de bouton musique
		windowOption.draw(bouton[4].rectangle);		//Dessine le bouton musique
		windowOption.draw(bouton[4].text);	//Dessine le texte de bouton musique
		windowOption.draw(bouton[5].rectangle);		//Dessine le bouton musique
		windowOption.draw(bouton[5].text);	//Dessine le texte de bouton musique

		bouton[6].text.setCharacterSize(28);
		bouton[6].hauteur = 100;
		bouton[6].largeur = 200;
		bouton[6].rectangle.setSize(Vector2f(bouton[6].largeur, bouton[6].hauteur));
		bouton[6].rectangle.setPosition(250, 625); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(50, 400, 225, 100))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[6].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[6].text.setString("Nom du joueur"); 	// choix de la chaîne de caractères à afficher
		bouton[6].text.setPosition(250, 660);		// position du texte


		windowOption.draw(bouton[6].rectangle);		//Dessine le bouton musique
		windowOption.draw(bouton[6].text);	//Dessine le texte de bouton musique

		bouton[9].text.setCharacterSize(30);
		
		bouton[9].hauteur = 75;
		bouton[9].largeur = 225;
		bouton[9].text.setColor(Color::Red);
		bouton[9].text.setOutlineColor(Color::Black);
		bouton[9].text.setOutlineThickness(2);
		bouton[9].rectangle.setSize(Vector2f(bouton[9].largeur, bouton[9].hauteur));
		bouton[9].rectangle.setPosition(585, 633); //défini la position de la bordure
		if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
		bouton[9].rectangle.setTexture(&texture);	//défini la texture du bouton
		bouton[9].text.setString(joueur.nomJoueur); 	// choix de la chaîne de caractères à afficher
		bouton[9].text.setPosition(625, 655);		// position du texte


		windowOption.draw(bouton[9].rectangle);		//Dessine le bouton musique
		windowOption.draw(bouton[9].text);	//Dessine le texte de bouton musique


		while (windowOption.pollEvent(event))
		{
			windowOption.display();
			switch (event.type)
			{
			case Event::Closed:
				exit(0);
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{// S l'utilisateur appuis que la touche echap, ferme le menu d'option et retourne au menu ou au jeu
					window.setVisible(true);
					return 0;
				}
			case Event::MouseButtonPressed:
				Mouse::getPosition(windowOption);
				// Si le joueur appuie sur le bouton On, met le volume de la musique a 100 et change le fond du bouton
				if ((event.mouseButton.x > 585 && event.mouseButton.x < 635) && (event.mouseButton.y > 310 && event.mouseButton.y < 360))
				{
					
					//Option musique on

					music.setVolume(100);
					music2.setVolume(100);
					music3.setVolume(100);
					bouton[1].rectangle.setFillColor(Color::Red);
					bouton[2].rectangle.setFillColor(Color::White);
				}
				// Si le joueur appuie sur le bouton Off, Met le volume de la musique a 0 et change le fond du bouton
				else if ((event.mouseButton.x > 760 && event.mouseButton.x < 800) && (event.mouseButton.y > 310 && event.mouseButton.y < 360))
				{

					music.setVolume(0);
					music2.setVolume(0);
					music3.setVolume(0);
					bouton[1].rectangle.setFillColor(Color::White);
					bouton[2].rectangle.setFillColor(Color::Red);

				}
				// Si le joueur appuie sur le bouton Musique 1, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 500 && event.mouseButton.x < 900) && (event.mouseButton.y > 400 && event.mouseButton.y < 450))
				{
					if (music.getStatus() != sf::Music::Playing)
					{
						music2.stop();
						music3.stop();
						music.openFromFile("styles/music.ogg");
						music.play();
					}

				}
				// Si le joueur appuie sur le bouton Musique 2, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 500 && event.mouseButton.x < 900) && (event.mouseButton.y > 475 && event.mouseButton.y < 525))
				{

					if (music2.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music3.stop();
						music2.openFromFile("styles/music2.ogg");
						music2.play();
					}


				}
				// Si le joueur appuie sur le bouton Musique 3, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 500 && event.mouseButton.x < 900) && (event.mouseButton.y > 550 && event.mouseButton.y < 600))
				{
					if (music3.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music2.stop();
						music3.openFromFile("styles/music3.ogg");
						music3.play();
					}

				}
				// Si l'utilisateur appuie sur le bouton du Nom du joueur, ouvre une fenetre et demande a l'utilisateur d'entrer un nom
				else if ((event.mouseButton.x > 585 && event.mouseButton.x < 815) && (event.mouseButton.y > 633 && event.mouseButton.y < 708))
				{
					RenderWindow window2(VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");
					int joueurEnregistrer;
					int reponse = 1;
					do
					{
						saisirNomJoueur(window2, font, texture, joueur);
						
						joueurEnregistrer = listeEnregistrement(window2, joueur);
						if (joueurEnregistrer == 1)
						{

							reponse = questionEnregistrement(window2, font, joueur);
						}
						else
						{
							joueur.level = 0;
							joueur.score = 0;
						}
					} while (reponse != 1);
					

				}

				break;
			}

		}
	}
}

// Selectionne le choix de l'utilisateur
int choixMenu(RenderWindow & windowMenu, RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3, identite & joueur)
{
	Event event;
	while (windowMenu.isOpen())
	{
		while (windowMenu.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				exit(0);
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
					exit(0);
			case Event::MouseButtonPressed:
				Mouse::getPosition(windowMenu);
				//Bouton Play
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 150 && event.mouseButton.y < 250))
				{
					
					return 1;
				}
				//Bouton Option
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 300 && event.mouseButton.y < 400))
				{
					window.clear();
					menuOption(windowMenu, titre, music, music2, music3, joueur);
					return 0;
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 450 && event.mouseButton.y < 550))
				{

					window.clear();
					afficherScore(windowMenu);
					return 0;
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 600 && event.mouseButton.y < 700))
				{

					window.close();
					windowMenu.close();
					exit(0);
					return -1;
				}
				break;
			default:
				break;
			}
		}
	}
}

// Affiche le titre du jeu
void afficheTitre(RenderWindow & window, Text titre[6])
{
	for (int i = 0; i < 6; i++)
		window.draw(titre[i]);
}

// Initialise le titre du jeu "Tetris"
void initTitre(Text titre[6], Font & font, float posx, float posy, float size)
{
	for (int i = 0; i < 6; i++)
	{
		titre[i].setCharacterSize(size);
		titre[i].setStyle(sf::Text::Bold);
		titre[i].setFont(font);
		titre[i].setPosition(posx + (size * i), posy);
	}
	titre[0].setString("T");
	titre[0].setColor(Color::Cyan);
	titre[1].setString("E");
	titre[1].setColor(Color::Red);
	titre[2].setString("T");
	titre[2].setColor(Color::Green);
	titre[3].setString("R");
	titre[3].setColor(Color::Cyan);
	titre[4].setString("I");
	titre[4].setColor(Color::White);
	titre[5].setString("S");
	titre[5].setPosition(posx + size * 4.65, posy);
	titre[5].setColor(Color::Yellow);

}

//Affiche le menu et les options
void afficherMenu(RenderWindow & window, Text titre[6],
	Music & music, Music & music2, Music & music3,
	Texture & texture, Font & font, identite & joueur)
{
	window.setVisible(false);
	RenderWindow windowMenu(VideoMode(1000, 800), "TETRIS Menu");
	
	const int nbrBouton = 8;

	Text text;

	int BtnMenuChoisi;
	Sprite  background(texture);

	if (!texture.loadFromFile("images/back_main.jpg"));
	

	Image icon;
	icon.loadFromFile("images/icon.png");
	windowMenu.setIcon(128, 128, icon.getPixelsPtr());

	boutonClick bouton[nbrBouton];

	Texture textureBouton;

	for (size_t i = 0; i < 5; i++)
	{
		titre[i].setPosition(Vector2f(350 + (i* titre[i].getCharacterSize()), 50));
	}
	titre[5].setPosition(Vector2f(350 + (5 * titre[5].getCharacterSize()) - 25, 50));

	while (windowMenu.isOpen())
	{
		for (size_t i = 0; i < nbrBouton; i++)
		{
			bouton[i].rectangle.setSize(Vector2f(bouton[i].hauteur, bouton[i].largeur));
			bouton[i].rectangle.setOutlineThickness(5);  //défini la grosseur de la bordure
			bouton[i].rectangle.setOutlineColor(Color::Red); //défini la couleur de la bordure
			bouton[i].text.setCharacterSize(30);
			bouton[i].text.setColor(Color::Black);
			bouton[i].fondImage.setSmooth(true);
			bouton[i].text.setFont(font);
		}
		do
		{
			windowMenu.draw(background);

			
			text.setFont(font);
			text.setColor(Color::Black);	// choix de la couleur du texte
			text.setCharacterSize(30);		// choix de la taille des caractères exprimée en pixels, pas en points !		

			
			afficheTitre(windowMenu, titre);

			bouton[0].hauteur = 100;
			bouton[0].largeur = 300;
			bouton[0].rectangle.setSize(Vector2f(bouton[0].largeur, bouton[0].hauteur));
			bouton[0].rectangle.setPosition(350, 150);

			if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(0, 0, 200, 100)));
			bouton[0].rectangle.setTexture(&textureBouton);

			bouton[0].text.setString("Play :D");
			bouton[0].text.setPosition(450, 188);

			windowMenu.draw(bouton[0].rectangle);
			windowMenu.draw(bouton[0].text);

			bouton[1].hauteur = 100;
			bouton[1].largeur = 300;
			bouton[1].rectangle.setSize(Vector2f(bouton[1].largeur, bouton[1].hauteur));
			bouton[1].rectangle.setPosition(350, 300);

			if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(200, 500, 300, 100)));
			bouton[1].rectangle.setTexture(&textureBouton);

			bouton[1].text.setString("Options");
			bouton[1].text.setPosition(450, 333);

			windowMenu.draw(bouton[1].rectangle);
			windowMenu.draw(bouton[1].text);


			bouton[2].hauteur = 100;
			bouton[2].largeur = 300;
			bouton[2].rectangle.setSize(Vector2f(bouton[2].largeur, bouton[2].hauteur));
			bouton[2].rectangle.setPosition(350, 450);

			if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(300, 300, 300, 100)));
			bouton[2].rectangle.setTexture(&textureBouton);

			bouton[2].text.setString("Scores");
			bouton[2].text.setPosition(450, 488);

			windowMenu.draw(bouton[2].rectangle);
			windowMenu.draw(bouton[2].text);


			bouton[3].hauteur = 100;
			bouton[3].largeur = 300;
			bouton[3].rectangle.setSize(Vector2f(bouton[3].largeur, bouton[3].hauteur));
			bouton[3].rectangle.setPosition(350, 600);

			if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(100, 500, 300, 100)));
			bouton[3].rectangle.setTexture(&textureBouton);

			bouton[3].text.setString("Quitter");
			bouton[3].text.setPosition(450, 625);

			windowMenu.draw(bouton[3].rectangle);
			windowMenu.draw(bouton[3].text);

			windowMenu.display();

			BtnMenuChoisi = choixMenu(windowMenu, window, titre, music, music2, music3, joueur);


		} while (BtnMenuChoisi != 1 && BtnMenuChoisi != -1);
		windowMenu.close();
	}
	window.setVisible(true);
}


