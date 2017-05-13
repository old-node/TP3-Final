/* En-t�te du fichier */
/// ================= */
/* Fichier:			carre.h
// Auteur:			Olivier Lemay Dostie
// Date cr�ation:	30/04/2017
// Description:		header pour le TP3 de C++ pour la cr�ation de carr�s SFML. */

/* Directives au pr�-processeur */
/// =========================== */
#pragma once								
#include <assert.h>				
using namespace std;
#include <SFML/Graphics.hpp>	
using namespace sf;


/* Constantes pour les carr�s */
///========================== */
const Vector2f CENTRECARRE(25, 25); // Dimmensions du centre des carr�s
const float BORDURECARRE = 1;		// Largeur des bordures des carr�s
const Vector2f DIMCARRE =			// Largeur des carr�s dans la fen�tre
/**/		Vector2f(CENTRECARRE.x + BORDURECARRE * 2, CENTRECARRE.y + BORDURECARRE * 2);
const Vector2f MILLIEUCARRE =		// Point central des carr�s (n�cessaire?)
/**/		Vector2f(DIMCARRE.x / 2, DIMCARRE.y / 2);
/// Une autre constante RectangleShape est pr�sente sous les prototype des fonctions

/* Constantes pour les blocs */		// (Pour la formation de la forme SFML par d�faut: RECT)
///========================= */	
const Color PALETTES[3][7] =
{
	{ Color::Blue, Color::Cyan, Color::Green, Color::Magenta, Color::Red, Color::White, Color::Yellow },
	{ Color(255,0,0,255), Color(0,255,0,255), Color(0,0,255,255), Color(255,255,0,255),
		Color(0,255,255,255), Color(255,0,255,255), Color(120,120,120,255) },
	{ Color(0,60,255,255), Color(20,220,255,255), Color(0,20,220,255), Color(180,25,255,255),
		Color(0,180,255,255), Color(200,100,255,255), Color(200,200,255,255) }
};
const int MAXCARRE = 5;				// Nb maximal en x et en y de carr�s dans un bloc
const Vector2f PIVOTBLOC =			// Point central des blocs
/**/		Vector2f((DIMCARRE.x * MAXCARRE) / 2, (DIMCARRE.y * MAXCARRE) / 2);

/* Constantes pour les salles */
///========================== */
const int LRGJEU = 20;				// Largeur par d�faut de l'occupation des salles
const int HAUJEU = 20;				// Hauteur par d�faut de l'occupation des salles
const Vector2i PLACE((LRGJEU - 2) / 2 - 1, 0);	// Position par d�faut des blocs dans la salle
const Vector2f POS =				// Position par d�faut de la salle
/**/		Vector2f(DIMCARRE.x + 45, DIMCARRE.y + 100);
const Vector2i COIN(0, 0);			// Position par d�faut en int
const Vector2f BASE(0, 0);			// Position par d�faut en float


/* Prototypes des fonctions */
///======================== */
struct carre;
RectangleShape initRectangle(const Vector2f & dimmensions, const float & bordure,
	const Vector2f & echelle, const Vector2f & pos, const Vector2i & place,
	const Vector2i & origine, const Color & couleur, const Color & couleurBord);
void setDimRect(RectangleShape & rect,
	const Vector2f & dimmensions, const float & bordure, const Vector2f & echelle);
void setPosRect(RectangleShape & rect,
	const Vector2f & pos, const Vector2i & place, const Vector2i & origine);
void setCouleurRect(RectangleShape & rect,
	const Color & couleur, const Color & couleurBord);


/// Constante d'une forme SFML
// Forme rectangulaire des carr�s avec les valeurs de base
const RectangleShape RECT = initRectangle(CENTRECARRE, BORDURECARRE, Vector2f(1, 1),
	POS, PLACE, Vector2i(2, 2), Color(255, 255, 255, 255), Color());


/* Objets carr�s d'un bloc ou d'une salle */
///====================================== */
struct carre
{
private:
	Vector2i _origine = Vector2i(2, 2);	// Les coordonn�es du carr� dans le bloc
	RectangleShape _vue = RECT;			// La forme SFML du carr�
	Color _couleur = RECT.getFillColor();		// Couleur du carr� avant sa transparence
	Color _couleurBord = RECT.getOutlineColor();// Couleur de la bordure avant sa transparence
	int _etat = 0;						// L'�tat du carr� (selon son utilisation)

public:
	/// dim		= dimenssion du bloc
	/// pos		= position de la salle de son bloc
	/// place	= coordon�es du bloc dans la salle
	/// origine = coordon�es du carr� dans le bloc

	// Initialisateurs.
	carre();
	carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Vector2f & dim, const float & bordure, const Vector2f & echelle,
		const Color & couleur, const Color & couleurBord, const int & etat);
	carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Vector2f & dim, const float & bordure, const Vector2f & echelle, const int & etat);
	carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Color & couleur, const int & etat);
	carre(const Color & couleur, const Color & couleurBord, const int & etat);


	// Destructeurs.
	~carre();

	// Change les propri�t�s du rectangle.
	void setRect(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Vector2f & dim, const float & bordure, const Vector2f & echelle,
		const Color & couleur, const Color & couleurBord, const int & etat);
	void setPos(const Vector2f & pos, const Vector2i & place, const Vector2i & origine);
	void setDim(const Vector2f & dim, const float & bordure, const Vector2f & echelle);
	void setCouleur(const Color & couleur, const Color & couleurBord);
	void setVue(const RectangleShape & rect);
	void setOrigine(const Vector2i & origine);
	void setEncrage(const Vector2f & origine);
	void setEtat(const int & etat);
	void cache();
	void montre();

	// Change la position du carr� dans la salle.
	void deplace(const Vector2i axe);
	void rotate(const int & degree)
	{
		_vue.rotate(degree);
	}

	// Retourne les attributs du carr�.
	RectangleShape getVue();
	Color getColor();
	Vector2i getPos();
	int getX();
	int getY();
	int getEtat();

	// Dessine le carr� dans l'�cran
	void draw(RenderWindow & window);
	void montre(RenderWindow & window, const Vector2f & coin);
};


/* M�thodes des carr�s */
///=================== */

// Instanciation sans personalisation.
carre::carre() {}

// Instancie un carr� avec toute les attributs qu'il peut contenir.
carre::carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Vector2f & dim, const float & bordure, const Vector2f & echelle,
	const Color & couleur, const Color & couleurBord, const int & etat)
{
	setRect(pos, place, origine, dim, bordure, echelle, couleur, couleurBord, etat);
}

// Instancie un carr� avec ses dimmenssions et sa position.
carre::carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Vector2f & dim, const float & bordure, const Vector2f & echelle, const int & etat)
{
	setOrigine(origine);
	setPosRect(_vue, pos, place, _origine);
	setDimRect(_vue, dim, bordure, echelle);
	setEtat(etat);
}

// 
carre::carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Color & couleur, const int & etat)
{
	setOrigine(origine);
	setPosRect(_vue, pos, place, _origine);
	setCouleur(couleur, Color());
	setEtat(etat);
}

// Instancie un carr� avec ses couleurs.
carre::carre(const Color & couleur, const Color & couleurBord, const int & etat)
{
	setCouleur(couleur, couleurBord);
	setEtat(etat);
}

// Destructeur de carr�s.
carre::~carre()
{
	setVue(RectangleShape());
	_etat = 0;
}

// Change toules les valeurs du carr�.
void carre::setRect(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Vector2f & dim, const float & bordure, const Vector2f & echelle,
	const Color & couleur, const Color & couleurBord, const int & etat)
{
	setOrigine(origine);
	setVue(initRectangle(dim, bordure, echelle, pos, place, _origine, couleur, couleurBord));
	setEtat(etat);
}

// Change la position du carr� dans son bloc et dans la salle.
void carre::setPos(const Vector2f & pos, const Vector2i & place, const Vector2i & origine)
{
	setOrigine(origine);
	setPosRect(_vue, pos, place, _origine);
}

// Change les dimmenssions du carr�.
void carre::setDim(const Vector2f & dim, const float & bordure, const Vector2f & echelle)
{
	setDimRect(_vue, dim, bordure, echelle);
}

// Change les couleurs du carr�.
void carre::setCouleur(const Color & couleur, const Color & couleurBord)
{
	setCouleurRect(_vue, couleur, couleurBord);
	setVue(_vue);
}

// Change le carr� pour un autre.
void carre::setVue(const RectangleShape & rect)
{
	_vue = rect;
	_couleur = rect.getFillColor();
	_couleurBord = rect.getOutlineColor();
}

// Change les coordonn�s du carr� dans le bloc.
void carre::setOrigine(const Vector2i & origine)
{
	_origine = origine;
}

// Change le point des transformations faites sur le carr�.
void carre::setEncrage(const Vector2f & origine)

{
	// Point d'encrage du carr� dans le bloc
	_vue.setOrigin(Vector2f(
		DIMCARRE.x * origine.x + 100,
		DIMCARRE.y * origine.y));
}

// Change l'�tat du carr�.
void carre::setEtat(const int & etat)
{
	_etat = etat;
}

// Met transparent les couleurs du carr�.
void carre::cache()
{
	_vue.setFillColor(Color::Transparent);
	_vue.setOutlineColor(Color::Transparent);
}

// Remet les couleurs du carr� d'avant s'�tre cach�.
void carre::montre()
{
	_vue.setFillColor(_couleur);
	_vue.setOutlineColor(_couleurBord);
}

// D�place le carr�.
void carre::deplace(const Vector2i axe)
{
	_vue.move(Vector2f(axe.x * DIMCARRE.x, axe.y * DIMCARRE.y));
}

// Rotate


// Retourne la forme du carr�.
RectangleShape carre::getVue()
{
	return _vue;
}

// Retourne la couleur du centre du carr�.
Color carre::getColor()
{
	return _vue.getFillColor();
}

// Retourne les coordonn�es du carr� dans son bloc
Vector2i carre::getPos()
{
	return _origine;
}

// Retourne la position du carr� en x dans son bloc.
int carre::getX()
{
	return _origine.x;
}

// Retourne la position du carr� en y dans son bloc.
int carre::getY()
{
	return _origine.y;
}

// Retourne l'�tat du carr�.
int carre::getEtat()
{
	return _etat;
}

// Dessine le carr� dans l'�cran.
void carre::draw(RenderWindow & window)
{
	window.draw(_vue);
}

// Dessine le carr� dans l'�cran � la coordon�e d�sir�.
void carre::montre(RenderWindow & window, const Vector2f & coin)
{
	Vector2f ancien = _vue.getPosition();

	setPosRect(_vue, coin, Vector2i(1, 0), _origine);
	window.draw(_vue);

	_vue.setPosition(ancien);
}


/* Fonctions pour les carr�s */
///========================= */
// Forme un carr� selon les crit�res voulu.
RectangleShape initRectangle(const Vector2f & dimmensions, const float & bordure,
	const Vector2f & echelle, const Vector2f & pos, const Vector2i & place,
	const Vector2i & origine, const Color & couleur, const Color & couleurBord)
{
	RectangleShape rect;
	setDimRect(rect, dimmensions, bordure, echelle);
	setPosRect(rect, pos, place, origine);
	setCouleurRect(rect, couleur, couleurBord);

	return rect;
}

// Modifie la dimmension, la bordure et l'�chelle du carr�.
void setDimRect(RectangleShape & rect,
	const Vector2f & dimmensions, const float & bordure, const Vector2f & echelle)
{
	rect.setSize(dimmensions);			// Dimmenssion du centre du carr�
	rect.setOutlineThickness(bordure);	// �paisseur de la bordure
	rect.setScale(echelle);				// Transforme ses dimmensions selon une �chelle
}

/// Modifie la position et le point pivot du carr�.
// / pos =		position de la salle dans la fen�tre
// / place =		coordon�es du bloc dans la salle
// / origine =	coordon�es du carr� dans le bloc
void setPosRect(RectangleShape & rect,
	const Vector2f & pos, const Vector2i & place, const Vector2i & origine)
{
	assert(origine.x >= 0 && origine.x < MAXCARRE &&
		origine.y >= 0 && origine.y < MAXCARRE &&
		place.x > -2 && place.x <= LRGJEU - 1 &&
		place.y >= 0 && place.y <= HAUJEU - 1);

	rect.setPosition(Vector2f(			// Position absolue
		pos.x + (DIMCARRE.x * place.x),
		pos.y + (DIMCARRE.y * place.y)));
	rect.setOrigin(Vector2f(			// Point d'encrage du carr� dans le bloc
		PIVOTBLOC.x - (DIMCARRE.x * origine.x),
		PIVOTBLOC.y - (DIMCARRE.y * origine.y)));
}

// Modifie les couleurs du carr�.
void setCouleurRect(RectangleShape & rect,
	const Color & couleur, const Color & couleurBord)
{
	rect.setFillColor(couleur);			// Couleur principale
	rect.setOutlineColor(couleurBord);	// Couleur de la bordure
}
