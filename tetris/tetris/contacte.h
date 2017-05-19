/* En-t�te du fichier */
///================== */
/* Fichier:			SFML_ambiance.h
// Auteur:			Olivier Lemay Dostie
// Date cr�ation:	13/05/2017
// Description:		Formation, manipulation et manipulation d'objets form�s en SFML.
//					Comprend un enssemble d'�l�ments n�cessaires pour former une interface.*/

/* Directives au pr�-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <SFML_condition.h>		
using namespace sf;


/* Constantes pour les ambiances */
///============================= */
int const NBBOUTON = 10;
int const NBTEXTE = 10;


/* Objets d'ambiances */ /// En construction
///================== */
class ambiance
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
	ambiance() {}
	ambiance(Vector2f const & pos, RenderWindow * window,
		string const & police, string const & extrait, int const & mesure,
		Color const & encre, Color const & bord, Color const & surligne)
	{ }
};