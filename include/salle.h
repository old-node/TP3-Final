/* En-tête du fichier */
///================== */
/* Fichier:			pieces.h
// Auteur:			Olivier Lemay Dostie & Jean-Allain Sainton
// Date création:	27/04/2017
// Description:		header pour le TP3 de C++ pour la création d'objets "salles" permettant
.					l'utilisateur de joueur à une version personnalisé de Tetris. */

/* Directives au pré-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
#include <math.h>				
#include <cstdlib>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <bloc.h>				// Contiens carre.h
using namespace sf;


// Données relative à un joueur.
struct identite
{
	string nomJoueur;
	bool nouveauJoueur = true;
	int level = 1,
		score = 0,
		rang = 0;
};

/* Prototype des fonctions */
///======================= */
// Les pivots des blocs par défaut sont : 2,2 - 2,2 - 2.5,2.5 - 2,2 - 2,2 - 2,2 - 2,2
class salle;
bool initPieces(bloc * blocsJeu, const int & nbPiece, const vector<Vector2i> pieces[NBPIECE][4]);
bool modifieEncrageBlocs();
int alleatoire(const int & max, const int & indice);


/* Constantes de blocs */
///=================== */
const bool ok = initPieces(tetris, NBPIECE, PIECES);	// Instancie les blocs 'tetris'
const bool ko = modifieEncrageBlocs();
/// Les septs blocs par défauts du jeu: 
// 0 (L),	1 (ſ),		2 (⬜),	3 (|),		4 (T),	 5 (S),		6 (Z)
// Sept,	Pendu,	Carré,	Ligne,	Plateau, Croche,	Plié
const bloc TETRIS = *tetris;


/* Constantes pour les salles */
///========================== */
const int LRGPOLICE = 24;
const int NBCHARMAX = 30;				// Nb de caractère maximum dans un message
const Vector2f DIMSALLE					// Dimmension par défaut de la salle
/**/(DIMCARRE.x * LRGJEU, DIMCARRE.y * HAUJEU);
const Vector2i PIVOTSALLE				// Point pivot des salles
/**/(DIMSALLE.x / 2, DIMSALLE.y / 2);
const Vector2f POSAFFICHE = Vector2f(DIMSALLE.x + POS.x - 40, LRGPOLICE);
const Keyboard::Key CONTROLES[3][6] =	// Contrôles des joueurs en mode Multijoueur
{// haut		bas				gauche			droite			tourne gauche	tourne droite
{ Keyboard::Up,	Keyboard::Down,	Keyboard::Left,	Keyboard::Right,Keyboard::Z,	Keyboard::X },		// Mode normal
{ Keyboard::W,	Keyboard::S,	Keyboard::A,	Keyboard::D,	Keyboard::F,	Keyboard::G },		// 1er Joueur
{ Keyboard::Up,	Keyboard::Down,	Keyboard::Left,	Keyboard::Right,Keyboard::Num0,	Keyboard::Period }	// 2e Joueur
};



/* Objet salle pour afficher ouvrir, jouer et manipuler le jeu principal */
///===================================================================== */
class salle
{
private:
	int _occupations					// Zones où les blocs ne doivent pas pouvoir passer (murs)
		[HAUJEU][LRGJEU] = { 0 };
	vector<carre> _tuiles;				// Carrés formant les zones infranchissables
	int _id = 0;
	vector<bloc> _blocsFixes;			// Tout les blocs qui d'ont le joueur à perdu le contrôle

	int _nbPiece = NBPIECE;				// Nombre de pièces disponible dans le niveau
	bloc *_blocsJeu[NBPIECE];			// La liste des blocs disponibles
	bloc _blocActif;  					// Le bloc avec lequel on joue présentement
	bloc _blocProchain; 				// Le prochain bloc du jeu

	Time _vitesseBloc = milliseconds(800); // Vitesse du bloc actif
	int _noNiveau = 1;					// Numéro du niveau actuel du jeu
	int _styleBlocs = 1;				// Textures, couleurs, etc. (nécessaire?)
	int _orientation = 1;				// Si on fait tourner la salle

	identite _joueur;					// Joueur de la salle et ses attributs
	string _nomJoueur = "Joueur";		// Nom du joueur
	int _noJoueur = 0;					// En mode multijoueur de 1 à 2. Permet d'utiliser les bon contrôles
	int _points = 0;					// Score que le joueur à accumulé
	bool _colle = false;				// État du jeu lorsqu'un bloc reste piégé (Passe au prochain avec colle)
	bool _permis = true;				// Si vrai, laisse le joueur faire des actions
	int _nbBombe = 1;					// Autre option lol

	RenderWindow & _window;
	Font _police;

	// Lieu principal du jeu
	Vector2f _pos = POS;				// Position de la salle dans la fenêtre
	RectangleShape _boite;				// Lieu	où l'on affiche l'espace de jeu

	// Lieu des statistiques
	Vector2f _posAffiche;
	RectangleShape _statistiques;
	Text _prochaine;
	RectangleShape _fenetrePiecesSuivante;
	Text _textLevel;
	Text _textNom;
	Text _textLigne;
	Text _textScore;
	Text _textAide;


	// Méthodes privates
	void videOccupations();
	void videBlocsJeu();

	void placeMurs();

	bool pivoteActif(int angle);
	bool colisionActifTourne(const vector<Vector2i>& axes);

	void prochain();

public:
	// Instanciatieurs
	salle(RenderWindow & window, string police, string nomJoueur);
	salle(RenderWindow & window, string police, Vector2f pos, int noNiveau, int orientation,
		vector<Vector2i> occupation, string nomJoueur, int noJoueur, int points, int nbBombe,
		Time vitesse, const bloc blocsJeu[], const int & nbPiece);

	// Destructeurs
	~salle();

	// Préparations de la salle
	void init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, Time vitesse,
		const bloc blocsJeu[], const int & nbPiece);
	void initBlocsJeu(const bloc pieces[], const int & nbPiece);
	void initFont(string police);
	void initStatistiques();
	void recharche(int noNiveau);
	void demare();

	// Changements des attributs de la salle
	void setPos(Vector2f pos);
	void setNoNiveau(int noNiveau);
	void setId(const int & id)
	{
		_id = id;
	}
	void setOrientation(int orientation);
	void setNomJoueur(string nomJoueur);
	void setNoJoueur(int noJoueur);
	void setPoints(int points);
	void setColle(bool colision);
	void setNbBombe(int nbBombe);
	void setVitesse(Time vitesse);
	void setActif(bloc actif);
	void setProchain(bloc prochain);
	void setInnocupationLigne(const int & y);
	void setOccupationAbsolue(const vector<Vector2i> & axes);
	void setOccupationRelative(const vector<Vector2i> & axes, Vector2i const & place);
	void creeObstacle();
	void marcheArriere();
	void brasse();

	// Contrôles principales
	void tourne();
	void pause();
	void menu();
	void balaye();
	//void tetris();
	void compresse();
	void ferme();

	// Déplacements et transformations du bloc actif
	bool bougeActif(int X, int Y);
	void tombeActif();
	bool pivoteActifGauche();
	bool pivoteActifDroite();
	void colleActif();
	/// Rallentis la progression du bloc. (augmente le délais avant qu'il continu sa descente)
	//void bloc::ralenti()
	//{
	//}
	void arret(const bool & permis);


	// Récupérations des attributs de la salle
	string getNomJoueur();
	Vector2f getPos();
	int getNoNiveau();
	identite getJoueur()
	{
		return _joueur;
	}
	int getNoJoueur();
	int getPoints();
	int getNbBombe();
	bool getColle();
	int getStyleBloc();
	int getOrientation();
	Time getVitesse();
	bloc getActif();
	bloc getProchain();

	void getOccupation(vector<Vector2i> & occupation);
	void getOccupationAbsolue(vector<Vector2i> occupation, const vector<Vector2i> & axes);
	void getOccupationRelative(vector<Vector2i> occupation,
		const vector<Vector2i> & axes, Vector2i place);
	bool checkOccupationAbsolue(const vector<Vector2i> & axes);
	bool checkOccupationRelative(const vector<Vector2i> & axes, Vector2i place);

	// Affichages
	// Affiche le bloc 
	void drawActif()
	{
		_blocActif.draw(_window);
	}
	// Affiche le prochain bloc dans la fenêtre des statistiques.
	void afficherInterface();
	void afficheBlocsSalle();
};

// Instanciations de base */
///====================== */
// Instancie une salle avec seulement le nom du joueur.
salle::salle(RenderWindow & window, string police, string nomJoueur) : _window(window)
{
	initFont(police);
	setNomJoueur(nomJoueur);
	initBlocsJeu(tetris, 7);
	placeMurs();
	initStatistiques();
}

// Instancie la salle avec toutes ses valeurs.
salle::salle(RenderWindow & window, string police, Vector2f pos, int noNiveau, int orientation,
	vector<Vector2i> occupation, string nomJoueur, int noJoueur, int points, int nbBombe,
	Time vitesse, const bloc blocsJeu[], const int & nbPiece) : _window(window)
{
	initFont(police);
	init(pos, noNiveau, orientation, occupation, nomJoueur,
		noJoueur, points, nbBombe, vitesse, blocsJeu, nbPiece);
}

// Initialise la police de la salle
void salle::initFont(string police)
{
	_police.loadFromFile(police);
}

/* Destructeurs */
///============ */
// Destructeur principal.
salle::~salle()
{
	Texture texture;
	/// Pourquoi est-ce qu'on load une texture quand on veux les enlevers?
	if (!texture.loadFromFile("Tetris-Background_3.jpg", IntRect(50, 25, 300, 100)));
	_boite.setTexture(&texture);
	_statistiques.setTexture(&texture);

	_boite = _statistiques = RectangleShape();

	_joueur.~identite();	/// Peut-on utiliser delete au lieu?
	_nomJoueur = "";
	_pos.x = _pos.y = _noNiveau = _noJoueur = _points = _colle = _nbBombe =
		_styleBlocs = _orientation = 0;
	_vitesseBloc.Zero;
	videOccupations();
	videBlocsJeu();
}

// Destructeur pour _occupations.
void salle::videOccupations()
{
	_tuiles.resize(0);
	_blocsFixes.resize(0);
	for (int i = 0; i < HAUJEU; i++)
		for (int j = 0; j < LRGJEU; j++)
			_occupations[i][j] = { 0 };
}

// Destructeur pour _blocsJeu.
void salle::videBlocsJeu()
{
	vector<carre> vide;
	vector<Vector2i> axes[4];
	for (int i = 0; i < NBPIECE; i++)
		(*_blocsJeu)[i].setFormes(vide, axes);
}

/* Modificateurs de la salle */
///========================= */
// Initialise la salle avec toutes ses valeurs.
void salle::init(Vector2f pos, int noNiveau, int orientation,
	vector<Vector2i> occupation, string nomJoueur, int noJoueur, int points, int nbBombe,
	Time vitesse, const bloc blocsJeu[], const int & nbPiece)
{
	setPos(pos);
	setNoNiveau(noNiveau);
	setOrientation(orientation);
	setOccupationAbsolue(occupation);
	setNoJoueur(noJoueur);
	setPoints(points);
	setNbBombe(nbBombe);
	setVitesse(vitesse);
	initBlocsJeu(blocsJeu, nbPiece);
	initStatistiques();

	placeMurs();
}

// Remet la salle à ses valeurs de base en changeant son numéro.
void salle::recharche(int noNiveau)
{
	videOccupations();
	placeMurs();

	setNoNiveau(noNiveau);
	initBlocsJeu(tetris, 7);
}

// Charge les blocs par défaut dans la salle puis choisi les premiers blocs du jeu.
void salle::initBlocsJeu(const bloc pieces[], const int & nbPiece)
{
	bloc test;
	for (int i = 0; i < NBPIECE; i++)
	{
		*(_blocsJeu + i) = new bloc(pieces[i]);
	}
	setActif(**(_blocsJeu + (alleatoire(NBPIECE, -1))));
	_blocActif.setId(++_id);
	setProchain(_blocActif);
	prochain();
}

// Active les procédures du démarage du jeu.
void salle::demare()
{

}

// Ajoude des tuiles aux murs et à la base pour qu'ils deviennent infranchissables.
void salle::placeMurs()
{
	// Murs
	for (int y = 0; y < HAUJEU; y++)
	{
		_occupations[y][LRGJEU - 1] = 1;
		_tuiles.push_back(carre(_pos, Vector2i(LRGJEU - 1, y), COIN,
			PALETTES[2][rand() % 6], 3));
		_occupations[y][0] = 1;
		_tuiles.push_back(carre(_pos, Vector2i(0, y), COIN,
			PALETTES[2][rand() % 6], 3));
	}
	// Plancher
	for (int x = 0; x < LRGJEU; x++)
	{
		_occupations[HAUJEU - 1][x] = 1;
		_tuiles.push_back(carre(_pos, Vector2i(x, HAUJEU - 1), COIN,
			PALETTES[2][rand() % 6], 3));
	}
}

Text initText(Font & police, string message, int mesure,
	Color couleur, Vector2f & pos, Vector2f decalage)
{
	Text text(message, police, mesure);
	text.setFillColor(couleur);
	pos.x += decalage.x;
	pos.y += decalage.y;
	text.setPosition(pos);
	return text;
}

/*Change les attributs de la salle*/
/// On devrait séparer l'initialisation et l'affichage pur n'avoir qu'à réinitialiser les choses qui changent.
void salle::initStatistiques()
{
	Vector2f posAffiche(POSAFFICHE.x - 15, POSAFFICHE.y);
	Vector2f decalage = BASE;

	Color couleurBordStat = Color::Red;
	Color couleurStat = Color(255, 255, 255, 80);
	float bordure = 10;

	_statistiques.setSize(Vector2f(300, DIMSALLE.y + LRGPOLICE * 5));
	_statistiques.setPosition(posAffiche);
	_statistiques.setFillColor(couleurStat);
	_statistiques.setOutlineThickness(bordure);
	_statistiques.setOutlineColor(couleurBordStat);

	decalage.x += LRGPOLICE;
	_prochaine = initText(_police, "Prochaine piece", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE * 2;
	_fenetrePiecesSuivante.setSize(Vector2f(PIVOTBLOC.x * 2.4, PIVOTBLOC.y * 1.9));
	_fenetrePiecesSuivante.setPosition(posAffiche.x, posAffiche.y + decalage.y);
	_fenetrePiecesSuivante.setFillColor(Color::Black);
	_fenetrePiecesSuivante.setOutlineThickness(bordure);
	_fenetrePiecesSuivante.setOutlineColor(couleurBordStat);

	decalage = BASE;
	decalage.y = PIVOTBLOC.y * 2.5 + LRGPOLICE;
	_textLevel = initText(_police, "Niveau : ", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE;
	_textNom = initText(_police, _nomJoueur, LRGPOLICE,
		Color::Black, posAffiche, decalage);

	_textLigne = initText(_police, "Lignes faites : ", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE * 2;
	_textScore = initText(_police, "Score : ", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE * 1.5;
	_textAide = initText(_police,
		"     Controles \n Z : Tourne a gauche \n X : Tourne a droite \n P : Pause \n M : Sourdine \n T : Musique \n Q : Options \n Esc : Menu",
		LRGPOLICE, Color::Black, posAffiche, decalage);
}

// 
void salle::setPos(Vector2f pos)
{
	_pos = pos;
}

// 
void salle::setNoNiveau(int noNiveau)
{
	_noNiveau = noNiveau;
}

// 
void salle::setOrientation(int orientation)
{
	_orientation = orientation;
}

// 
void salle::setNomJoueur(string nomJoueur)
{
	_nomJoueur = nomJoueur;
}

// 
void salle::setNoJoueur(int noJoueur)
{
	assert(noJoueur >= 0 && noJoueur <= 3);
	_noJoueur = noJoueur;
}

// 
void salle::setPoints(int points)
{
	_points = points;
}

// 
void salle::setColle(bool colision)
{
	_colle = colision;
}

// 
void salle::setNbBombe(int nbBombe)
{
	assert(nbBombe >= 0);
	_nbBombe = nbBombe;
}

// Change le délais entre chaque descente.
void salle::setVitesse(Time vitesse)
{
	assert(vitesse >= milliseconds(0));
	_vitesseBloc = vitesse;
}

// Change le bloc actif par un autre.
void salle::setActif(bloc actif)
{
	_blocActif = actif;
}

// Change le bloc suivant par un autre.
void salle::setProchain(bloc prochain)
{
	_blocProchain = prochain;
}

// Libère une des ligne des occupations de la salle.
void salle::setInnocupationLigne(const int & y)
{
	for (int i = 1; i < LRGJEU; i++)
	{
		_occupations[y][i] = 0;
	}
}

// Ajoute des occupations selon des axes dans le tableau
void salle::setOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		_occupations[element.y][element.x] = 1;
}

// Ajoute des occupations selon des axes relatifs dans le tableau
void salle::setOccupationRelative(vector<Vector2i> const & axes, Vector2i const & place)
{
	for (Vector2i const & element : axes)
		_occupations[place.y + element.y][place.x + element.x] = 1;
}

// Alterne le bloc actif par le prochain et trouve aléatoirement le prochain différent bloc.
void salle::prochain()
{
	setActif(_blocProchain);
	int piece = alleatoire(NBPIECE, _blocProchain.getPiece());
	while (piece == _blocActif.getPiece())
		piece = alleatoire(NBPIECE, _blocProchain.getPiece());

	setProchain(**(_blocsJeu + piece));
	_blocProchain.setId(++_id);
}

/* Récupérations des attributs de la salle */
// Retourne le nom du joueur.
string salle::getNomJoueur()
{
	return _nomJoueur;;
}

// Retourne la position de la salle dans la fenêtre.
Vector2f salle::getPos()
{
	return _pos;
}

// Retourne le numéro du niveau.
int salle::getNoNiveau()
{
	return _noNiveau;
}

// Retourne le numéro du joueur.
int salle::getNoJoueur()
{
	return _noJoueur;
}

// Retourne le nombre de points du joueur.
int salle::getPoints()
{
	return _points;
}

// Retourne si le bloc actif se figera.
bool salle::getColle()
{
	return _colle;
}

// Retourne le nombre de bombe du joueur.
int salle::getNbBombe()
{
	return _nbBombe;
}

// Retourne le style des blocs du niveau. (utile?)
int salle::getStyleBloc()
{
	return _styleBlocs;
}

// Retourne l'orientation de la salle.
int salle::getOrientation()
{
	return _orientation;
}

// Retourne le délai entre chaque descente du bloc du niveau.
Time salle::getVitesse()
{
	return _vitesseBloc;
}

// Retourne le bloc actif.
bloc salle::getActif()
{
	return _blocActif;
}

// Retourne le bloc suivant.
bloc salle::getProchain()
{
	return _blocProchain;
}

// Retourne les occupations de la salle qui sont occupées.
void salle::getOccupation(vector<Vector2i> & occupation)
{
	occupation.resize(0);
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			if (_occupations[i][j] == 1)
				occupation.push_back(Vector2i(i, j));
}

// Retourne des occupations relatives de la salle .m.nm.
void salle::getOccupationAbsolue(vector<Vector2i> occupation, vector<Vector2i> const & axes)
{
	occupation.resize(0);
	for (auto const & element : axes)
		if (_occupations[element.y][element.x] == 1)
			occupation.push_back(element);
}

// Retourne les coordonnées relatives de la salle qui sont occupés.
void salle::getOccupationRelative(vector<Vector2i> occupation, vector<Vector2i> const& axes, Vector2i place)
{
	occupation.resize(0);
	for (auto const &element : axes)
		if (_occupations[place.y + element.y][place.x + element.x] == 1)
			occupation.push_back(element);
}

// Vérifie si l'une des tuiles spécifiés de la salle est occupée.
bool salle::checkOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		if (_occupations[element.y][element.x] == 1)
			return true;
	return false;
}

// Vérifie si l'une des tuiles du jeu est occupé dans un endroit relatif.
bool salle::checkOccupationRelative(const vector<Vector2i> & axes, Vector2i place)
{
	for (Vector2i const &element : axes)
		if (_occupations[place.y + element.y][place.x + element.x] == 1)
			return true;
	return false;
}

/* Transformations de la salle */
// Crée un obstacle infranchissable dans la salle. 
void salle::creeObstacle()		//**
{}

// Remet le jeu à son état précédent le dernier bloc ajouté.
void salle::marcheArriere()	//**
{}

// Fait pivoter toute la salle sur elle-même.
void salle::tourne()			//**
{}

// Fait une animation de tremblement de terre + brise certains carrés qui sont suspendus.
void salle::brasse()			//**
{}

/* Contrôles principales */
// Met le jeu en arrêt pour montrer les options ou le menu principal.
void salle::pause()
{}

// Accède au menu principal.
void salle::menu()
{}

// Vérifie si des lignes sont complétés pour vider ces lignes.
void salle::balaye()
{}

/// // ??
//void salle::tetris()
//{}

// Déplace tout les carrés en suspenssions d'un nombre requis vers le bas.
void salle::compresse()
{}

// Ferme le jeu actif avant de retourner au menu principal.
void salle::ferme()
{}

/* Déplacements et transformations du bloc actif */
// Bouge le bloc d'une distance en x si elle n'entre pas en conflit avec la salle.
bool salle::bougeActif(int X, int Y)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;
	///vector<Vector2i> axes = _blocActif.getAxes(_blocActif.getAngle());

	// Vérifie si le bloc peut se déplacer
	for (Vector2i const & element : _blocActif.getAxes(_blocActif.getAngle()))
	{
		/// Question: Pourquoi quand "element" est constant, on ne peut utiliser de méthodes ?
		if (_occupations[y + element.y + Y][x + element.x + X] == 1)
			return 0;
	}

	// Déplace le bloc
	_blocActif.deplace(X, Y);

	// Tente de redéplacer le bloc dans la même dirrection pour savoir s'il y a eu une colision
	/// À enlever si l'on veut que les blocs ne collent pas automatiquement
	//for (Vector2i const & element : axes)
	//	if (_occupations[y + element.y + Y][x + element.x + X] == 1)
	//		return 0;
	return 1;
}

// Trouve l'angle suivant au sens antihoraire et effectue la maneuvre si possible.
bool salle::pivoteActifGauche()
{
	int angle = _blocActif.getAngle();
	if (++angle > 3)
		angle = 0;

	if (!colisionActifTourne(_blocActif.getAxes(angle)))
	{
		_blocActif.tourneGauche(angle);
		return 1;
	}

	return 0;
}

// Trouve l'angle suivant au sens horaire et effectue la maneuvre si possible.
bool salle::pivoteActifDroite()
{
	int angle = _blocActif.getAngle();
	if (--angle < 0)
		angle = 3;

	// Vérifie si le bloc peut tourner sans être en conflit avec le reste
	if (!colisionActifTourne(_blocActif.getAxes(angle)))
	{
		// Fait pivoter la pièce
		_blocActif.tourneDroite(angle);
		return 1;
	}
	return 0;
}

// Vérifie si le bloc colisionnerais sous un angle différent.
bool salle::colisionActifTourne(const vector<Vector2i> & axes)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;

	for (Vector2i const & element : axes)
		if (_occupations[y + element.y][x + element.x] == 1)
			return 1;

	return 0;
}

// Vérifie si le bloc peux tourner dans la salle.
bool salle::pivoteActif(int angle)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;
	vector<Vector2i>profil = _blocActif.getAxes(angle);

	for (Vector2i const & element : profil)
	{
		if (_occupations[y + element.y][x + element.x] == 1)
			return 0;
	}

	_blocActif.setAngle(angle);
	return 1;
}

// Fait dessendre le bloc jusqu'à ce qu'il colisione.
void salle::tombeActif()
{
	int minY = HAUJEU;
	vector<Vector2i> axes = _blocActif.getAxes(_blocActif.getAngle());

	while (bougeActif(0, 1)) {}

	setColle(true);
}

// _En travaux_ Empèche de bouger, de tourner, etc pendant jusqu'à réactivation.
void salle::arret(const bool & permis)
{
	_permis = permis;
}

// Aublige le bloc de rester en place, prenant place à l'occupation de la salle.
void salle::colleActif()
{
	/// À cause de la redondance, mémoriser les axes dans la salle su lieu?
	setOccupationRelative(
		_blocActif.getAxes(_blocActif.getAngle()), 
		_blocActif.getPlace());
	_blocsFixes.push_back(_blocActif);
	prochain();
}

// Dessine les composantes de la salle.
void salle::afficherInterface()
{
	_window.draw(_boite);
	_window.draw(_statistiques);

	_window.draw(_prochaine);
	_window.draw(_fenetrePiecesSuivante);
	_blocProchain.montre(_window, Vector2f(
		POSAFFICHE.x + PIVOTBLOC.x * 0.75,
		POSAFFICHE.y + PIVOTBLOC.y * 1.6));

	_window.draw(_textLevel);
	_window.draw(_textNom);
	_window.draw(_textLigne);
	_window.draw(_textScore);
	_window.draw(_textAide);
}

// Affiche les blocs et les carrés de la salle.
void salle::afficheBlocsSalle()
{
	for (carre & element : _tuiles)
		element.draw(_window);
	for (bloc & element : _blocsFixes)
		element.draw(_window);
}


/* Autres structures */
///================= */


// Crée des formes pour les manipuler des essais, des tests etc.
struct teStruct
{
	int outline = 10;
	CircleShape boule;
	RectangleShape rectangle;
	RectangleShape shape;

	teStruct()
	{
		boule.setRadius(100.f);
		boule.setFillColor(Color::Green);

		rectangle.setSize(Vector2f(120, 50));

		shape.setSize(Vector2f(100, 100));
		shape.setFillColor(Color::Green);
		// set a 10-pixel wide orange outline
		shape.setOutlineThickness(10);
		shape.setOutlineColor(Color(250, 150, 100));
		// set the absolute position of the entity
		shape.setPosition(60, 100);
		// set the absolute scale of the entity
		shape.setScale(4.0f, 1.6f);
		// set the absolute rotation of the entity
		shape.setRotation(45);
	}
};

// Manipulation des formes à essais.
void testPackPlay(teStruct &test, RenderWindow &window)
{
	window.draw(test.boule);
	window.draw(test.shape);
	window.draw(test.rectangle);

	static bool retour = false;
	int x = test.rectangle.getPosition().x;
	if (retour)
	{
		x -= 200;
		if (x < 10)
			retour = false;
	}
	else
	{
		x += 200;
		if (x > 800)
			retour = true;
	}
	test.rectangle.setPosition(x, 100);

	test.shape.setOutlineThickness(test.outline *= 3);
	// move the entity relatively to its current position
	test.shape.move(20, 5);
	test.boule.move(20, 5);

	// retrieve the absolute position of the entity
	Vector2f position = test.shape.getPosition();
	// = (15, 55)

	// rotate the entity relatively to its current orientation
	test.shape.rotate(10);

	// retrieve the absolute rotation of the entity
	float rotation = test.shape.getRotation();
	// = 55

	// scale the entity relatively to its current scale
	test.shape.scale(0.8f, 0.4f);

	// retrieve the absolute scale of the entity
	Vector2f scale = test.shape.getScale();
	// = (2, 0.8)
}

/* Fonctions */
///========= */

// Construit les pièces par défaut à partir des coordonnées du tableau PIECES.
bool initPieces(bloc * blocsJeu, const int & nbPiece, const vector<Vector2i> pieces[NBPIECE][4])
{
	vector<carre> formes;		// Carrés de la forme
	vector<Vector2i> axes[4];	// Coordonnées des carrés sous chaque angles

	// Crée un bloc avec les coordonnés du tableau donné
	for (int f = 0; f < nbPiece; f++)
	{
		// Forme les carrés de la pièce (selon l'angle 1)
		for (int c = 0; c < 4; c++)
			formes.push_back(carre(POS, PLACE, pieces[f][0][c], PALETTES[2][f], 0));

		// Ajoute les coordonnées des carrés de pièce aux axes
		for (int a = 0; a < 4; a++)
			for (int c = 0; c < 4; c++)
				axes[a].push_back(pieces[f][a][c]);

		// Initialise des blocs avec les vectors formés
		*(blocsJeu + f) = bloc(PLACE, 0, 1, 0, f, 0, formes, axes);

		// Vide la mémoire des vectors avant la prochaine pièce
		formes.resize(0);
		for (int i = 0; i < 4; i++)
			axes[i].resize(0);
	}
	return 1;
}

// Modifie le point d'encrage des blocs par défaut.
bool modifieEncrageBlocs()
{
	// Change le point pivot du cube (3e forme) pour qu'il pivote sur son centre
	tetris[2].setEncrage(Vector2f(2.5, 2.5));
	return false;
}

// Obtien un nomre aléatoire différent du précédent.
int alleatoire(const int & max, const int & indice)
{
	int nouveau = rand() % max;
	while (nouveau == indice)
		nouveau = rand() % max;

	return nouveau;
}