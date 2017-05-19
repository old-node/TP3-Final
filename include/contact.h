/* En-tête du fichier */
///================== */
/* Fichier:			contact.h
// Auteur:			Olivier Lemay Dostie
// Date création:	13/05/2017
// Description:		Formation, manipulation et affichage d'objets formés en SFML.
//					Comprend un enssemble d'éléments nécessaires pour former des interfaces.*/

/* Directives au pré-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <contenu.h>			// Contient contexte.h
using namespace sf;


/* Constantes pour les contacts */
///============================ */
int const NBBOUTON = 10;
int const NBTEXTE = 10;


/* Objets de contacts */ /// En construction
///================== */
class contact
{
private:
	int _etat;
	bool _visible;
	Color _peinture;
	Color _bord;
	Color _effet;
	RectangleShape _cadre;
	//condition _bouton[NBBOUTON];
	//contexte _extrait[NBTEXTE];
	static Font _police;
public:
	contact() {}
	contact(Vector2f const & pos, RenderWindow * window,
		string const & police, string const & extrait, int const & mesure,
		Color const & encre, Color const & bord, Color const & surligne)
	{ }
};