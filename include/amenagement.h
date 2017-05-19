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
	int noJoueur,
		rang = 0,
		level = 1,
		score = 0;
};

/* Prototype des fonctions */
///======================= */
// Les pivots des blocs par défaut sont : 2,2 - 2,2 - 2.5,2.5 - 2,2 - 2,2 - 2,2 - 2,2
class amenagement;
Text initText(Font & police, string message, int mesure,
	Color couleur, Vector2f & pos, Vector2f decalage);
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
const Vector2f DIMSALLE					// Dimmension par défaut de l'aménagement
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
class amenagement
{
private:
	int _occupations					// Zones où les blocs ne doivent pas pouvoir passer (murs)
		[HAUJEU][LRGJEU] = { 0 };
	vector<carre> _tuiles;				// Carrés formant les zones infranchissables
	int _id = 0;
	vector<bloc> _blocsFixes;			// Tout les blocs qui d'ont le joueur à perdu le contrôle

	bool _defaut = true;				// Condition vérifiant si les blocs sont de formes typiques du jeu
	int _nbPiece = NBPIECE;				// Nombre de pièces disponible dans le niveau
	bloc *_blocsJeu[NBPIECE];			// La liste des blocs disponibles
	bloc _blocActif;  					// Le bloc avec lequel on joue présentement
	bloc _blocProchain; 				// Le prochain bloc du jeu

	Time _vitesseBloc = milliseconds(800); // Vitesse du bloc actif
	int _noNiveau = 1;					// Numéro du niveau actuel du jeu
	int _gradin = 100;					// Nombre de points à aquérir pour se rendre au prochain niveau.
	int _styleBlocs = 1;				// Textures, couleurs, etc. (nécessaire?)
	int _orientation = 1;				// Si on fait tourner l'aménagement

	identite _joueur;					// Joueur de l'aménagement et ses attributs
	string _nomJoueur = "Joueur";		// Nom du joueur
	bool _colle = false;				// État du jeu lorsqu'un bloc reste piégé (Passe au prochain avec colle)
	bool _permis = true;				// Si vrai, laisse le joueur faire des actions
	int _nbBombe = 1;					// Autre option lol

	RenderWindow & _window;
	Font _police;

	// Lieu principal du jeu
	Vector2f _pos = POS;				// Position de l'aménagement dans la fenêtre
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


	// Méthodes privées
	void videOccupations();
	void videBlocsJeu();

	void placeMurs();

	bool colisionActifTourne(const vector<Vector2i> & axes);
	bool defautRotation(const bool & sens, int & angle)
		// Empèche certains blocs de faire des rotations
	{
		if (_defaut)
		{
			if (_blocActif.getPiece() == 2)
				return 1;
			if (_blocActif.getPiece() == 5 || _blocActif.getPiece() == 6)
				if (sens)
				{
					if (angle == 2)
					{
						_blocActif.tourneDroite();
						angle = 0;
						return 1;
					}
				}
				else
				{
					if (angle == 3)
					{
						_blocActif.tourneGauche();
						angle = 1;
						return 1;
					}
				}
		}
		return 0;
	}
	bool pivoteActif(int angle);
	bool defautDeplacement(const int & sens)
		// Utile seulement si des blocs ne doivent pas se 
		//	déplacer dans une dirrection horizontale
	{
		if (_defaut && (_blocActif.getPlace().x < -2 ||
			_blocActif.getPlace().x >= LRGJEU - 1))
		{
			if (_blocActif.getPiece() == 5)
				return 0;
			if (_blocActif.getPiece() == 6)
				return 0;
		}
		return 0;
	}

	void prochain();

public:
	// Instanciatieurs
	amenagement(RenderWindow & window, string police, string nomJoueur);
	amenagement(RenderWindow & window, string police, Vector2f pos, int noNiveau, int orientation,
		vector<Vector2i> occupation, string nomJoueur, int noJoueur, int points, int nbBombe,
		Time vitesse, bloc blocsJeu[], const int & nbPiece);

	// Destructeurs
	~amenagement();

	// Préparations de l'aménagement
	void init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, Time vitesse,
		bloc blocsJeu[], const int & nbPiece);
	void initBlocsJeu(bloc pieces[], const int & nbPiece);
	void initFont(string police);
	void initStatistiques();
	void recharche(int noNiveau);
	void demare();

	// Changements des attributs de l'aménagement
	void setPos(Vector2f pos);
	void setNoNiveau(int noNiveau);
	void setId(const int & id)
	{
		_id = id;
	}
	void setOrientation(int orientation);
	void setNomJoueur(string nomJoueur);
	void setNoJoueur(int noJoueur);
	void setScore(int points);
	void diffScore(const int & diff)
	{
		if (_joueur.score += diff > _gradin)
		{
			diffVitesse(Time(milliseconds(-10)));
			_gradin = 100 * _noNiveau;
		}
	}
	void setColle(bool colision);
	void setNbBombe(int nbBombe);
	void diffVitesse(Time vitesse);
	void setActif(bloc actif);
	void setProchain(bloc prochain);
	void setInnocupationLigne(const int & y);
	void setOccupationAbsolue(const vector<Vector2i> & axes);
	void setOccupationRelative(const vector<Vector2i> & axes, Vector2i const & place);
	void descente(const int & Y);
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
	bool pivoteActifDroite();
	bool pivoteActifGauche();
	void colleActif();
	/// Rallentis la progression du bloc. (augmente le délais avant qu'il continu sa descente)
	//void bloc::ralenti()
	//{
	//}
	void arret(const bool & permis);

	// Récupérations des attributs de l'aménagement
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
amenagement::amenagement(RenderWindow & window, string police, string nomJoueur) : _window(window)
{
	initFont(police);
	setNomJoueur(nomJoueur);
	initBlocsJeu(tetris, 7);
	placeMurs();
	initStatistiques();
}

// Instancie l'aménagement avec toutes ses valeurs.
amenagement::amenagement(RenderWindow & window, string police, Vector2f pos, int noNiveau, int orientation,
	vector<Vector2i> occupation, string nomJoueur, int noJoueur, int points, int nbBombe,
	Time vitesse, bloc blocsJeu[], const int & nbPiece) : _window(window)
{
	initFont(police);
	init(pos, noNiveau, orientation, occupation, nomJoueur,
		noJoueur, points, nbBombe, vitesse, blocsJeu, nbPiece);
}

// Initialise la police de l'aménagement
void amenagement::initFont(string police)
{
	_police.loadFromFile(police);
}

/* Destructeurs */
///============ */
// Destructeur principal.
amenagement::~amenagement()
{
	Texture texture;
	/// Pourquoi est-ce qu'on load une texture quand on veux les enlevers?
	if (!texture.loadFromFile("Tetris-Background_3.jpg", IntRect(50, 25, 300, 100)));
	_boite.setTexture(&texture);
	_statistiques.setTexture(&texture);

	_boite = _statistiques = RectangleShape();

	_joueur.~identite();	/// Peut-on utiliser delete au lieu?
	_pos.x = _pos.y = _noNiveau = _colle = _nbBombe =
		_styleBlocs = _orientation = 0;
	_vitesseBloc.Zero;
	videOccupations();
	videBlocsJeu();
}

// Destructeur pour _occupations.
void amenagement::videOccupations()
{
	_tuiles.resize(0);
	_blocsFixes.resize(0);
	for (int i = 0; i < HAUJEU; i++)
		for (int j = 0; j < LRGJEU; j++)
			_occupations[i][j] = { 0 };
}

// Destructeur pour _blocsJeu.
void amenagement::videBlocsJeu()
{
	vector<carre> vide;
	vector<Vector2i> axes[4];
	for (int i = 0; i < NBPIECE; i++)
		(*_blocsJeu)[i].setFormes(vide, axes);
}

/* Modificateurs de l'aménagement */
///========================= */
// Initialise l'aménagement avec toutes ses valeurs.
void amenagement::init(Vector2f pos, int noNiveau, int orientation,
	vector<Vector2i> occupation, string nomJoueur, int noJoueur, int points, int nbBombe,
	Time vitesse, bloc blocsJeu[], const int & nbPiece)
{
	setPos(pos);
	setNoNiveau(noNiveau);
	setOrientation(orientation);
	setOccupationAbsolue(occupation);
	setNoJoueur(noJoueur);
	setScore(points);
	setNbBombe(nbBombe);
	diffVitesse(vitesse);
	initBlocsJeu(blocsJeu, nbPiece);
	initStatistiques();

	placeMurs();
}

// Remet l'aménagement à ses valeurs de base en changeant son numéro.
void amenagement::recharche(int noNiveau)
{
	videOccupations();
	placeMurs();

	setNoNiveau(noNiveau);
	initBlocsJeu(tetris, 7);
}

// Charge les blocs par défaut dans l'aménagement puis choisi les premiers blocs du jeu.
void amenagement::initBlocsJeu(bloc pieces[], const int & nbPiece)
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
void amenagement::demare()
{

}

// Ajoude des tuiles aux murs et à la base pour qu'ils deviennent infranchissables.
void amenagement::placeMurs()
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

/*Change les attributs de l'aménagement*/
/// On devrait séparer l'initialisation et l'affichage pur n'avoir qu'à réinitialiser les choses qui changent.
void amenagement::initStatistiques()
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

// Change les coordonnées de l'espace dans la fenêtre.
void amenagement::setPos(Vector2f pos)
{
	_pos = pos;
}

// Modifie le numéro du niveau de l'aménagement.
void amenagement::setNoNiveau(int noNiveau)
{
	_noNiveau = noNiveau;
}

// Change l'orientation de l'aménagement.
void amenagement::setOrientation(int orientation)
{
	_orientation = orientation;
}

// Modifie le nom du joueur.
void amenagement::setNomJoueur(string nomJoueur)
{
	_joueur.nomJoueur = nomJoueur;
}

// Donne au joueur un numéro pour l'identifier.
void amenagement::setNoJoueur(int noJoueur)
{
	assert(noJoueur >= 0 && noJoueur <= 3);
	_joueur.noJoueur = noJoueur;
}

// Modifie le score du joueur.
void amenagement::setScore(int score)
{
	_joueur.score = score;
}

// Change l'attribut des blocs pour qu'ils collent.
void amenagement::setColle(bool colision)
{
	_colle = colision;
}

// Modifie le nombre de bombe disponible.
void amenagement::setNbBombe(int nbBombe)
{
	assert(nbBombe >= 0);
	_nbBombe = nbBombe;
}

// Change le délais entre chaque descente.
void amenagement::diffVitesse(Time vitesse)
{
	if (_vitesseBloc + vitesse < milliseconds(5))
		_vitesseBloc = milliseconds(5);
	else
		_vitesseBloc += vitesse;
}

// Change le bloc actif par un autre.
void amenagement::setActif(bloc actif)
{
	_blocActif = actif;
}

// Change le bloc suivant par un autre.
void amenagement::setProchain(bloc prochain)
{
	_blocProchain = prochain;
}

// Libère une des ligne des occupations de l'aménagement.
void amenagement::setInnocupationLigne(const int & y)
{
	for (int i = 1; i < LRGJEU; i++)
	{
		_occupations[y][i] = 0;
	}
}

// Ajoute des occupations selon des axes dans le tableau
void amenagement::setOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		_occupations[element.y][element.x] = 1;
}

// Ajoute des occupations selon des axes relatifs dans le tableau
void amenagement::setOccupationRelative(vector<Vector2i> const & axes, Vector2i const & place)
{
	for (Vector2i const & axe : axes)
	{
		bool complete = true;

		_occupations[place.y + axe.y][place.x + axe.x] = 1;
		for (int i = 1; i < LRGJEU - 1; i++)
		{
			if (_occupations[place.y + axe.y][i] == 0)
			{
				complete = false;
				break;
			}
		}
		if (complete)
		{
			diffScore(25);
			for (bloc & piece : _blocsFixes)
				piece.separe(_pos, place.y + axe.y, _id);
			complete = false;
		}
	}
}

// Effectue un nettoyage d'une ligne.
void amenagement::descente(const int & Y)
{
	for (bloc & element: _blocsFixes)
	{
		if (Y - 5 > element.getPlace().y)
			element.deplace(0, 1);
		else
		{
			;
		}
	}
}

// Alterne le bloc actif par le prochain et trouve aléatoirement le prochain différent bloc.
void amenagement::prochain()
{
	setActif(_blocProchain);
	int piece = alleatoire(NBPIECE, _blocProchain.getPiece());
	while (piece == _blocActif.getPiece())
		piece = alleatoire(NBPIECE, _blocProchain.getPiece());

	setProchain(**(_blocsJeu + piece));
	_blocProchain.setId(++_id);
}

/* Récupérations des attributs de l'aménagement */
// Retourne le nom du joueur.
string amenagement::getNomJoueur()
{
	return _nomJoueur;;
}

// Retourne la position de l'aménagement dans la fenêtre.
Vector2f amenagement::getPos()
{
	return _pos;
}

// Retourne le numéro du niveau.
int amenagement::getNoNiveau()
{
	return _noNiveau;
}

// Retourne le numéro du joueur.
int amenagement::getNoJoueur()
{
	return _joueur.noJoueur;
}

// Retourne le nombre de points du joueur.
int amenagement::getPoints()
{
	return _joueur.score;
}

// Retourne si le bloc actif se figera.
bool amenagement::getColle()
{
	return _colle;
}

// Retourne le nombre de bombe du joueur.
int amenagement::getNbBombe()
{
	return _nbBombe;
}

// Retourne le style des blocs du niveau. (utile?)
int amenagement::getStyleBloc()
{
	return _styleBlocs;
}

// Retourne l'orientation de l'aménagement.
int amenagement::getOrientation()
{
	return _orientation;
}

// Retourne le délai entre chaque descente du bloc du niveau.
Time amenagement::getVitesse()
{
	return _vitesseBloc;
}

// Retourne le bloc actif.
bloc amenagement::getActif()
{
	return _blocActif;
}

// Retourne le bloc suivant.
bloc amenagement::getProchain()
{
	return _blocProchain;
}

// Retourne les occupations de l'aménagement qui sont occupées.
void amenagement::getOccupation(vector<Vector2i> & occupation)
{
	occupation.resize(0);
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			if (_occupations[i][j] == 1)
				occupation.push_back(Vector2i(i, j));
}

// Retourne des occupations relatives de l'aménagement .m.nm.
void amenagement::getOccupationAbsolue(vector<Vector2i> occupation, vector<Vector2i> const & axes)
{
	occupation.resize(0);
	for (auto const & element : axes)
		if (_occupations[element.y][element.x] == 1)
			occupation.push_back(element);
}

// Retourne les coordonnées relatives de l'aménagement qui sont occupés.
void amenagement::getOccupationRelative(vector<Vector2i> occupation, vector<Vector2i> const& axes, Vector2i place)
{
	occupation.resize(0);
	for (auto const &element : axes)
		if (_occupations[place.y + element.y][place.x + element.x] == 1)
			occupation.push_back(element);
}

// Vérifie si l'une des tuiles spécifiés de l'aménagement est occupée.
bool amenagement::checkOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		if (_occupations[element.y][element.x] == 1)
			return true;
	return false;
}

// Vérifie si l'une des tuiles du jeu est occupé dans un endroit relatif.
bool amenagement::checkOccupationRelative(const vector<Vector2i> & axes, Vector2i place)
{
	for (Vector2i const &element : axes)
		if (_occupations[place.y + element.y][place.x + element.x] == 1)
			return true;
	return false;
}

/* Transformations de l'aménagement */
// Crée un obstacle infranchissable dans l'aménagement. 
void amenagement::creeObstacle()		//**
{}

// Remet le jeu à son état précédent le dernier bloc ajouté.
void amenagement::marcheArriere()	//**
{}

// Fait pivoter toute l'aménagement sur elle-même.
void amenagement::tourne()			//**
{}

// Fait une animation de tremblement de terre + brise certains carrés qui sont suspendus.
void amenagement::brasse()			//**
{}

/* Contrôles principales */
// Met le jeu en arrêt pour montrer les options ou le menu principal.
void amenagement::pause()
{}

// Accède au menu principal.
void amenagement::menu()
{}

// Vérifie si des lignes sont complétés pour vider ces lignes.
void amenagement::balaye()
{}

/// // ??
//void salle::tetris()
//{}

// Déplace tout les carrés en suspenssions d'un nombre requis vers le bas.
void amenagement::compresse()
{}

// Ferme le jeu actif avant de retourner au menu principal.
void amenagement::ferme()
{}

/* Déplacements et transformations du bloc actif */
// Bouge le bloc d'une distance en x si elle n'entre pas en conflit avec l'aménagement.
bool amenagement::bougeActif(int X, int Y)
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

	if (defautDeplacement(X))
		return 0;

	// Déplace le bloc
	_blocActif.deplace(X, Y);

	// Tente de redéplacer le bloc dans la même dirrection pour savoir s'il y a eu une colision
	/// À enlever si l'on veut que les blocs ne collent pas automatiquement
	//for (Vector2i const & element : axes)
	//	if (_occupations[y + element.y + Y][x + element.x + X] == 1)
	//		return 0;
	return 1;
}

// Trouve l'angle suivant au sens horaire et effectue la maneuvre si possible.
bool amenagement::pivoteActifDroite()
{
	int angle = _blocActif.getAngle();
	if (--angle < 0)
		angle = 3;

	// Vérifie si le bloc peut tourner sans être en conflit avec le reste
	if (!colisionActifTourne(_blocActif.getAxes(angle)))
	{
		if (defautRotation(true, angle))
			return 0;
		// Fait pivoter la pièce
		_blocActif.setAngle(angle);
		_blocActif.tourneDroite();
		return 1;
	}
	return 0;
}

// Trouve l'angle suivant au sens antihoraire et effectue la maneuvre si possible.
bool amenagement::pivoteActifGauche()
{
	int angle = _blocActif.getAngle();
	if (++angle > 3)
		angle = 0;

	if (!colisionActifTourne(_blocActif.getAxes(angle)))
	{
		if (defautRotation(false, angle))
			return 0;

		_blocActif.setAngle(angle);
		_blocActif.tourneGauche();
		return 1;
	}

	return 0;
}

// Vérifie si le bloc colisionnerais sous un angle différent.
bool amenagement::colisionActifTourne(const vector<Vector2i> & axes)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;

	for (Vector2i const & element : axes)
		if (_occupations[y + element.y][x + element.x] == 1)
			return 1;

	return 0;
}

// Vérifie si le bloc peux tourner dans l'aménagement.
bool amenagement::pivoteActif(int angle)
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
void amenagement::tombeActif()
{
	int minY = HAUJEU;
	vector<Vector2i> axes = _blocActif.getAxes(_blocActif.getAngle());

	while (bougeActif(0, 1)) {}

	setColle(true);
}

// _En travaux_ Empèche de bouger, de tourner, etc pendant jusqu'à réactivation.
void amenagement::arret(const bool & permis)
{
	_permis = permis;
}

// Aublige le bloc de rester en place, prenant place à l'occupation de l'aménagement.
void amenagement::colleActif()
{
	/// À cause de la redondance, mémoriser les axes dans l'aménagement su lieu?
	setOccupationRelative(
		_blocActif.getAxes(_blocActif.getAngle()),
		_blocActif.getPlace());
	_blocsFixes.push_back(_blocActif);
	diffScore(10 * _noNiveau);

	prochain();
}

// Dessine les composantes de l'aménagement.
void amenagement::afficherInterface()
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

// Affiche les blocs et les carrés de l'aménagement.
void amenagement::afficheBlocsSalle()
{
	for (carre & element : _tuiles)
		element.draw(_window);
	for (bloc & element : _blocsFixes)
		element.draw(_window);
}


/* Fonctions */
///========= */

// Initialise du texte pour les interfaces.
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
			formes.push_back(carre(POS, PLACE, pieces[f][0][c], PALETTES[0][f], 0));

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
	//tetris[2].setEncrage(Vector2f(-.5, -.5));
	//tetris[2].setPlace();
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