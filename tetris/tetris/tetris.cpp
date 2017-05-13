/* En-tête du programme */
///==================== */
/* Programme:		tutoGitsSFML.cpp
// Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
// Date création:	16/04/2017
// Description:		Programme des essais SFML et de connection Git pour le projet final en C++
// .				Espace de développement pour l'application.*/

/* Directives au pré-processeur */
///============================ */
#include <saisieSecurisee.h>	
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

void initTitre(Text titre[6], Font & font, float posx, float posy, float size);
void afficheTitre(RenderWindow & window, Text titre[6]);

int listeEnregistrement(RenderWindow & windowMenu, identite & identite);
void saisirNomJoueur(RenderWindow &window2, Font font, Texture texture, identite & joueur);
int questionOuiNonSFML(RenderWindow & window2, identite & identite);
int questionEnregistrement(RenderWindow & window2, Font font, identite & identite);
void afficherScore(RenderWindow & windowMenu);
void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille);
void enregistrerScore(RenderWindow & window, identite & identite);


/* Constantes pour le programme principal */
///====================================== */
const double PI = atan(1) * 4;	// Valeur de pi (nécessaire?)


/* Programme principal */
///=================== */
int main()
{
	/// Tests
	//teStruct test;
	RectangleShape f(Vector2f(100, 100));
	f.setFillColor(Color(120, 210, 100, 130));

	/// Essais de blocs
	//bloc active = espace.getBloc();
	//int angle = active.getAngle();

	setlocale(LC_CTYPE, "fra");
	srand(time(NULL));		// Trouve une source aléatoire

	RenderWindow window;
	RenderWindow window2;

	Font font;

	if (!font.loadFromFile("styles/hemi_head_bd_it.ttf"))
		cout << "Erreur à afficher pour chaque font, quitter le programme en plus?";
	Text titre[6];
	initTitre(titre, font, 110, 10, 60);

	font.loadFromFile("styles/font_arial.ttf");// Choix de la police à utiliser
	Text textAngle;
	textAngle.setFont(font);
	textAngle.setCharacterSize(LRGPOLICE);	// Taille des caractères exprimée en pixels, pas en points !						 
	textAngle.setColor(Color::Black);		// Couleur du texte
	textAngle.setPosition(Vector2f(60, DIMSALLE.y + 150));		// Position du texte
	Text textPos = textAngle;
	textPos.setPosition(Vector2f(120, DIMSALLE.y + 150));

	//RenderWindow windowTest(VideoMode(500, 500), "TETRIS v1.2 Nom du identite");

	//windowTest.display();

	string nomJoueur;
	Music music2;
	Music music3;
	vector<Vector2i> occupations;
	salle espace(window, "styles/font_arial.ttf", POS, 1, 1, occupations, nomJoueur, 1, 0, 0, milliseconds(800), tetris, 7);

	Music music;
	music.stop();
	music.openFromFile("styles/music3.ogg");

	Texture texture;
	if (!texture.loadFromFile("images/back_main.jpg"));
	Sprite background(texture);
	window.draw(background);

	Event event;
	Time moment;
	unsigned seed = time(0);			/// Même chose que srand ?
	Clock tempsEcoule;

	int mouvement = -1;

	int i = 0;
	int lvl = 1,
		score = 0;
	int identiteEnregistrer, reponse = 1;

	font.loadFromFile("styles/font_arial.ttf");// Choix de la police à utiliser

										// Fenêtre d'enregistrements
	window2.create(VideoMode(500, 500), "TETRIS v1.2 Nom du identite");
	do
	{
		saisirNomJoueur(window2, font, texture, espace.getJoueur());
		identiteEnregistrer = listeEnregistrement(window2, espace.getJoueur());
		if (identiteEnregistrer == 1)
			reponse = questionEnregistrement(window2, font, espace.getJoueur());
		else
		{
			reponse = 1;
			lvl = 0;
			score = 0;
		}
	} while (reponse != 1);

	window2.close();

	window.create(VideoMode(POSAFFICHE.x + 300 + LRGPOLICE, DIMSALLE.y + 200), "TETRIS Jeu");
	Image icon;
	icon.loadFromFile("images/icon.png");
	window.setIcon(128, 128, icon.getPixelsPtr()); //Affiche l'icone de Tétris

	afficherMenu(window, titre, music, music2, music3, texture, font, espace.getJoueur());

	/// Peut être gossant quand on fait des tests XD
	//if (music.getStatus() == false)	
	//	music.play();

	window.draw(background);

	window.display();

	/// Ajouter une condition tant que la fenêtre est active, ne ferme pas le programme
	///			sinon la console reste ouverte quand on ferme avec le X de la fenêtre

	// Fenêtre du jeu principal
	do
	{
		int posy, posx;
		int nbY = 0;

		//espace.afficherInterface(window);
		//espace.modifierInterface(window, prochain, profil, identite.nomJoueur, identite.level, identite.score);

		do {
			Sprite background(texture);
			window.draw(background);

			espace.afficherInterface();
			//espace.modifierInterface(window, prochain, profil, identite.nomJoueur, identite.level, identite.score);
			//active.drawBloc(window, active.getAngle());

			if (tempsEcoule.getElapsedTime() > espace.getVitesse())
			{
				tempsEcoule.restart();
				moment.asMilliseconds();	/// Utile ? Clock peut faire la job je crois
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
				f.move(Vector2f(0, -20));					/////
				break;
			case 0: // Descente du bloc automatique
				espace.setColle(!espace.bougeActif(0, 1));
				break;
			case 5:	// Bas			(Faire tomber)
				espace.tombeActif();
				f.move(Vector2f(0, 20));					/////
				break;
			case 4:	// Gauche		(Bouger)
				espace.bougeActif(-1, 0);
				f.move(Vector2f(-20, 0));					/////
				break;
			case 6:	// Droite		(Bouger)
				espace.bougeActif(1, 0);
				f.move(Vector2f(20, 0));					/////
				break;
			case 7:	// P
				menuOption(window, titre, music, music2, music3, espace.getJoueur());
				break;
			case 1:	// Esc
				afficherMenu(window, titre, music, music2, music3, texture, font, espace.getJoueur());
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
				espace.setColle(false);
			}

			//window.clear();
			window.draw(background);
			//afficheTitre(window, titre);
			window.draw(f);
			espace.afficherInterface();
			//espace.montreProchain();
			espace.afficheBlocsSalle();
			afficheTitre(window, titre);

			espace.drawActif();
			//espace.getBloc().draw(window);
			textPos.setString(to_string(espace.getActif().getAngle())); 	// Chaîne de caractères à afficher
			window.draw(textPos);
			string place = to_string(espace.getActif().getPlace().x)
				+ "," + to_string(espace.getActif().getPlace().y);
			textAngle.setString(place); 	// Chaîne de caractères à afficher
			window.draw(textAngle);

			window.display();
			sleep(seconds(0));

		} while (ok);

	} while (ok);
	enregistrerScore(window2, espace.getJoueur());
	return 0;
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

// 
void enregistrerScore(RenderWindow & window, identite &joueur) {

	window.clear();

	int reponse = 0;
	int longueurNom = 1;
	Text text;

	text.setColor(Color::Green);
	text.setFillColor(Color::Green);
	Font font;
	if (!font.loadFromFile("styles/font_arial.ttf"));
	text.setFont(font);
	text.setPosition(100, 50);
	text.setCharacterSize(22);

	do {

		if (joueur.nouveauJoueur == false) {
			window.display();
			text.setString("Voulez vous enregistrer par-dessus votre ancienne sauvegarde? (o / n)? : ");
			window.draw(text);
			window.display();

		}
		else {
			window.display();
			text.setString("Voulez vous enregistrer votre score? (o / n)? : ");
			window.draw(text);
			window.display();


		}
		reponse = questionOuiNonSFML(window, joueur);
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

// 
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
	text.setColor(Color::Red);
	string nomJoueur;
	int lvl;
	int score;
	ifstream fichierScore;
	if (!ouvrirFichier(fichierScore, "score.txt", cout));

	text.setOutlineThickness(2);
	text.setOutlineColor(Color::Black);
	text.setPosition(400, 50);
	text.setString("10 Meilleurs score!");
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

// 
int questionEnregistrement(RenderWindow & window2, Font font, identite & identite)
{
	window2.clear();
	setlocale(LC_CTYPE, "fra");
	Event event;

	window2.display();
	Text text;

	if (!font.loadFromFile("styles/font_arial.ttf")); // choix de la police à utiliser
	text.setFont(font);
	text.setString("Ce nom appartient a un identite contenant \nles statistiques suivante :");
	text.setCharacterSize(22);
	text.setPosition(25, 100);
	text.setColor(Color::Black);
	window2.draw(text);


	char levelMax[15] = "Level max : ";
	levelMax[12] = identite.level + '0';
	text.setString(levelMax);
	text.setPosition(25, 160);
	window2.draw(text);

	char textScoreMax[15] = "Score max : \0";
	char scoreMax[15];
	sprintf(scoreMax, "%d", identite.score);
	strcat_s(textScoreMax, scoreMax);
	text.setString(textScoreMax);
	text.setPosition(25, 200);
	window2.draw(text);

	text.setString("Voulez vous continer avec cet enregistrement? \n Appuyez sur O ou sur N");
	text.setPosition(25, 250);
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

// 
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

// 
void saisirNomJoueur(RenderWindow &window2, Font font, Texture texture, identite & joueur)
{
	Image icon;
	Text text;
	icon.loadFromFile("images/icon.png");			//va chercher l'image de l'icone
	window2.setIcon(128, 128, icon.getPixelsPtr()); // Affiche l'icone

	if (!font.loadFromFile("styles/font_arial.ttf"));		//va chercher la police pour le texte
	if (!texture.loadFromFile("images/back_option.jpg"));	//va chercher le fond d'écran

	Sprite background(texture);
	window2.draw(background);		//affiche le fond d'écran	

	text.setFont(font);
	text.setString("Quel est votre nom?"); 	// choix de la chaîne de caractères à afficher
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
			if (event.type == Event::Closed)
				window2.close();
			if (event.type == Event::TextEntered) {
				if (event.key.code == 8 && s.size() != 0)		//touche backspace
				{
					s.pop_back();  //Enleve le dernier char dans la string
				}
				else if (event.key.code == 13 && s.size() != 0) //touche enter
				{
					joueur.nomJoueur = s;
					return;
				}
				else if ((event.key.code >= 97 && event.key.code <= 122)
					|| event.key.code == 32
					|| (event.key.code >= 48 && event.key.code <= 57))	// caractere ASCII (A - Z, 0 - 9 )
				{
					s.push_back((char)event.text.unicode); // ajoute un char a la string
				}

				RectangleShape nomJoueur(Vector2f(400, 50));
				window2.display();

				if (!texture.loadFromFile("images/back_option.jpg", IntRect(100, 50, 500, 50)));
				nomJoueur.setTexture(&texture);

				nomJoueur.setPosition(100, 50);
				window2.draw(nomJoueur);	//Affiche un rectangle vide

				text.setString(s); 	// choix de la chaîne de caractères à afficher
				text.setPosition(100, 50);		// position du texte
				text.setColor(Color::Green);   // choix de la couleur du texte

				window2.draw(text);

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
				/// Pause if false; if true: Unpause
				menuOption(window, titre, music, music2, music3, joueur);
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

	Texture texture;
	Image icon;
	icon.loadFromFile("images/icon.png");		//cherche l'image de l'icone
	windowOption.setIcon(128, 128, icon.getPixelsPtr());	//Défini une icone pour la fenetre

	RectangleShape btnMusique(Vector2f(300, 100));
	RectangleShape btnMusiquePowerOn(Vector2f(50, 50));
	RectangleShape btnMusiquePowerOff(Vector2f(50, 50));
	RectangleShape btnMusiqueChoix1(Vector2f(200, 50));
	RectangleShape btnMusiqueChoix2(Vector2f(200, 50));
	RectangleShape btnMusiqueChoix3(Vector2f(200, 50));
	RectangleShape btnRetour(Vector2f(100, 50));
	RectangleShape btnNomJoueur(Vector2f(300, 125));

	RectangleShape btnAutre(Vector2f(300, 100));

	Font font;
	Text text;

	if (!texture.loadFromFile("images/back_main.jpg"));  //Si la texture est introuvable, envoie un message d'erreur

	Sprite background(texture);

	windowOption.draw(background); // Affiche le fond d'écran

	if (!font.loadFromFile("styles/font_arial.ttf")); // choix de la police à utiliser

											   //afficheTitre(windowOption, titre);	/// changer les valeur pour 340, 50, 60 avant

	text.setFont(font);
	text.setCharacterSize(30); // choix de la taille des caractères exprimée en pixels, pas en points !					
	text.setStyle(Text::Bold); 	// choix du style du texte

	btnMusique.setOutlineThickness(5);  //défini la grosseur de la bordure
	btnMusique.setOutlineColor(Color::Red); //défini la couleur de la bordure
	btnMusique.setPosition(350, 150); //défini la position de la bordure

	if (!texture.loadFromFile("images/back_option.jpg", IntRect(0, 190, 300, 100))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
	btnMusique.setTexture(&texture);	//défini la texture du bouton
	texture.setSmooth(true);			//addoucie la texture de l'image

	text.setString("Musique"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(450, 188);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusique);		//Dessine le bouton mussique
	windowOption.draw(text);			//Dessine le texte de bouton mussique

	btnMusiquePowerOn.setOutlineThickness(5);
	btnMusiquePowerOn.setOutlineColor(Color::Red);
	btnMusiquePowerOn.setPosition(350, 275);

	if (!texture.loadFromFile("images/back_option.jpg", IntRect(150, 150, 50, 50)));
	btnMusiquePowerOn.setTexture(&texture);
	texture.setSmooth(true);
	if (music.getVolume() == 0)
	{
		btnMusiquePowerOn.setFillColor(Color::White);
	}
	else
	{
		btnMusiquePowerOn.setFillColor(Color::Magenta);
	}


	text.setString("On"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(352, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusiquePowerOn);
	windowOption.draw(text);


	btnMusiquePowerOff.setOutlineThickness(5);
	btnMusiquePowerOff.setOutlineColor(Color::Red);
	btnMusiquePowerOff.setPosition(600, 275);
	if (!texture.loadFromFile("images/back_option.jpg", IntRect(150, 150, 50, 50)));
	texture.setSmooth(true);

	btnMusiquePowerOff.setTexture(&texture);
	if (music.getVolume() == 0)
	{
		btnMusiquePowerOff.setFillColor(Color::Magenta);
	}

	text.setString("Off"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(605, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusiquePowerOff);
	windowOption.draw(text);

	btnMusiqueChoix1.setOutlineThickness(5);
	btnMusiqueChoix1.setOutlineColor(Color::Red);
	btnMusiqueChoix1.setPosition(350, 355);

	if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50)));
	texture.setSmooth(true);
	btnMusiqueChoix1.setTexture(&texture);
	if (music.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix1.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix1.setFillColor(Color::White);
	}

	text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 355);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusiqueChoix1);
	windowOption.draw(text);


	btnMusiqueChoix2.setOutlineThickness(5);
	btnMusiqueChoix2.setOutlineColor(Color::Red);
	btnMusiqueChoix2.setPosition(350, 420);
	btnMusiqueChoix2.setTexture(&texture);

	if (music2.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix2.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix2.setFillColor(Color::White);
	}

	text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 420);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusiqueChoix2);
	windowOption.draw(text);


	btnMusiqueChoix3.setOutlineThickness(5);
	btnMusiqueChoix3.setOutlineColor(Color::Red);
	btnMusiqueChoix3.setPosition(350, 485);
	btnMusiqueChoix3.setTexture(&texture);

	text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 485);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	if (music3.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix3.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix3.setFillColor(Color::White);
	}
	windowOption.draw(btnMusiqueChoix3);
	windowOption.draw(text);

	btnRetour.setOutlineThickness(5);
	btnRetour.setOutlineColor(Color::Red);
	btnRetour.setPosition(10, 10);

	if (!texture.loadFromFile("images/back_option.jpg", IntRect(190, 310, 50, 50)));
	texture.setSmooth(true);
	btnRetour.setTexture(&texture);

	text.setString("Retour"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(10, 15);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnRetour);
	windowOption.draw(text);

	btnNomJoueur.setOutlineThickness(5);
	btnNomJoueur.setOutlineColor(Color::Red);
	btnNomJoueur.setPosition(350, 550);
	if (!texture.loadFromFile("images/back_option.jpg", IntRect(90, 290, 300, 100)));
	texture.setSmooth(true);
	btnNomJoueur.setTexture(&texture);

	if (joueur.nomJoueur != "Nouveau Joueur")
	{
		btnNomJoueur.setFillColor(Color::Magenta);
	}
	text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 565);		// position du texte
	windowOption.draw(btnNomJoueur);
	windowOption.draw(text);

	text.setString(joueur.nomJoueur); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 625);		// position du texte

	windowOption.draw(text);
	windowOption.display();



	Event event;
	while (window.isOpen())
	{
		while (windowOption.pollEvent(event))
		{

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
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 400) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					//Option musique on
					if (!texture.loadFromFile("images/back_option.jpg", IntRect(150, 150, 50, 50)));
					windowOption.display();
					music.setVolume(100);
					music2.setVolume(100);
					music3.setVolume(100);
					btnMusiquePowerOn.setFillColor(Color::Magenta);
					btnMusiquePowerOn.setTexture(&texture);
					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOn);
					windowOption.draw(text);

					btnMusiquePowerOff.setFillColor(Color::White);
					btnMusiquePowerOff.setTexture(&texture);
					/*if (!texture.loadFromFile("images/back_button.jpg", IntRect(0, 0, 300, 100)));*/
					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOff);
					windowOption.draw(text);

					windowOption.display();
				}
				// Si le joueur appuie sur le bouton Off, Met le volume de la musique a 0 et change le fond du bouton
				else if ((event.mouseButton.x > 600 && event.mouseButton.x < 650) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					if (!texture.loadFromFile("images/back_option.jpg", IntRect(150, 150, 50, 50)));
					btnMusiquePowerOn.setTexture(&texture);
					windowOption.display();
					music.setVolume(0);
					music2.setVolume(0);
					music3.setVolume(0);
					btnMusiquePowerOn.setFillColor(Color::White);
					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOn);
					windowOption.draw(text);

					btnMusiquePowerOff.setTexture(&texture);
					btnMusiquePowerOff.setFillColor(Color::Magenta);
					/*if (!texture.loadFromFile("images/back_button.jpg", IntRect(0, 0, 300, 100)));*/
					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOff);
					windowOption.draw(text);


					windowOption.display();
				}
				// Si le joueur appuie sur le bouton Musique 1, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 355 && event.mouseButton.y < 405))
				{
					if (music.getStatus() != sf::Music::Playing)
					{
						music2.stop();
						music3.stop();
						music.openFromFile("styles/music.ogg");
						music.play();
					}

					windowOption.display();
					if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50)));

					btnMusiqueChoix1.setFillColor(Color::Magenta);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix1);
					windowOption.draw(text);


					btnMusiqueChoix2.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix2);
					windowOption.draw(text);


					btnMusiqueChoix3.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix3);
					windowOption.draw(text);

					windowOption.display();
				}
				// Si le joueur appuie sur le bouton Musique 2, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 420 && event.mouseButton.y < 470))
				{

					if (music2.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music3.stop();
						music2.openFromFile("styles/music2.ogg");
						music2.play();
					}

					windowOption.display();
					if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50)));
					btnMusiqueChoix1.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix1);
					windowOption.draw(text);


					btnMusiqueChoix2.setFillColor(Color::Magenta);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix2);
					windowOption.draw(text);


					btnMusiqueChoix3.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix3);
					windowOption.draw(text);

					windowOption.display();
				}
				// Si le joueur appuie sur le bouton Musique 3, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 485 && event.mouseButton.y < 535))
				{
					if (music3.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music2.stop();
						music3.openFromFile("styles/music3.ogg");
						music3.play();
					}
					if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50)));
					windowOption.display();
					btnMusiqueChoix1.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix1);
					windowOption.draw(text);


					btnMusiqueChoix2.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix2);
					windowOption.draw(text);


					btnMusiqueChoix3.setFillColor(Color::Magenta);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix3);
					windowOption.draw(text);


					windowOption.display();
				}
				// Si l'utilisateur appuie sur le bouton du Nom du joueur, ouvre une fenetre et demande a l'utilisateur d'entrer un nom
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 550 && event.mouseButton.y < 700))
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





					windowOption.display();

					if (!texture.loadFromFile("images/back_option.jpg", IntRect(90, 290, 300, 100)));
					btnNomJoueur.setTexture(&texture);
					texture.setSmooth(true);

					btnNomJoueur.setFillColor(Color::Magenta);
					btnNomJoueur.setOutlineThickness(0);
					btnNomJoueur.setOutlineColor(Color::Red);
					btnNomJoueur.setPosition(350, 550);

					text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 565);		// position du texte

					windowOption.draw(btnNomJoueur);
					windowOption.draw(text);

					text.setColor(Color::Black);
					text.setString(joueur.nomJoueur); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 625);		// position du texte

					windowOption.draw(text);
					windowOption.display();

					if (!texture.loadFromFile("images/back_option.jpg", IntRect(450, 450, 200, 50)));
					btnMusique.setTexture(&texture);
				}
				// Si le joueur appuie sur le bouton retour
				else if ((event.mouseButton.x > 15 && event.mouseButton.x < 105) && (event.mouseButton.y > 15 && event.mouseButton.y < 65))
				{
					window.setVisible(true);
					return 0;
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

	RectangleShape btnCommencer(Vector2f(300, 100));
	RectangleShape btnOption(Vector2f(300, 100));
	RectangleShape btnScore(Vector2f(300, 100));
	RectangleShape btnQuitter(Vector2f(300, 100));

	Text text;
	int BtnMenuChoisi;
	Sprite  background(texture);

	if (!texture.loadFromFile("images/back_main.jpg"));
	if (!font.loadFromFile("styles/font_arial.ttf"));

	Image icon;
	icon.loadFromFile("images/icon.png");
	windowMenu.setIcon(128, 128, icon.getPixelsPtr());

	do
	{
		windowMenu.draw(background);

		//afficheTitre(windowMenu, titre);
		Texture textureBouton;

		if (!font.loadFromFile("styles/font_arial.ttf"));
		text.setFont(font);
		text.setColor(Color::Black);	// choix de la couleur du texte
		text.setCharacterSize(30);		// choix de la taille des caractères exprimée en pixels, pas en points !		

		btnCommencer.setFillColor(Color::White);
		btnCommencer.setOutlineThickness(5);
		btnCommencer.setOutlineColor(Color::Red);
		btnCommencer.setPosition(350, 150);
		windowMenu.draw(btnCommencer);

		if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(0, 0, 200, 100)));
		btnCommencer.setTexture(&textureBouton);
		textureBouton.setSmooth(true);

		windowMenu.draw(btnCommencer);

		text.setString("Play :D"); 		// choix de la chaîne de caractères à afficher
		text.setPosition(450, 188);		// position du texte
		windowMenu.draw(text);

		btnOption.setFillColor(Color::White);
		btnOption.setOutlineThickness(5);
		btnOption.setOutlineColor(Color::Red);
		btnOption.setPosition(350, 300);

		if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(200, 500, 300, 100)));
		btnOption.setTexture(&textureBouton);

		windowMenu.draw(btnOption);

		text.setString("Options "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 333);		// position du texte
		windowMenu.draw(text);

		btnScore.setFillColor(Color::White);
		btnScore.setOutlineThickness(5);
		btnScore.setOutlineColor(Color::Red);
		btnScore.setPosition(350, 450);

		if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(300, 300, 300, 100)));
		btnScore.setTexture(&textureBouton);
		textureBouton.setSmooth(true);

		windowMenu.draw(btnScore);

		text.setString("Scores ");		// choix de la chaîne de caractères à afficher
		text.setPosition(450, 488);		// position du texte
		windowMenu.draw(text);

		btnQuitter.setFillColor(Color::White);
		btnQuitter.setOutlineThickness(5);
		btnQuitter.setOutlineColor(Color::Red);
		btnQuitter.setPosition(350, 600);

		if (!textureBouton.loadFromFile("images/back_button.jpg", IntRect(100, 500, 300, 100)));
		btnQuitter.setTexture(&textureBouton);
		textureBouton.setSmooth(true);

		windowMenu.draw(btnQuitter);

		text.setString("Quitter "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 625);		// position du texte
		windowMenu.draw(text);

		windowMenu.display();

		BtnMenuChoisi = choixMenu(windowMenu, window, titre, music, music2, music3, joueur);


	} while (BtnMenuChoisi != 1 && BtnMenuChoisi != -1);

	window.setVisible(true);
}


/// Réparer l'allocation de la classe des blocs avec new, *, &, etc ? 
/// j'ai de la difficulté à rendre ça fonctionnel
/// ???
/// Profit
/// 