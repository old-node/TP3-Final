/* En-tête du fichier */
///================== */
/* Fichier:			SFML_condition.h
// Auteur:			Olivier Lemay Dostie
// Date création:	13/05/2017
// Description:		Formation, manipulation et manipulation d'objets formés en SFML.
//					Permet de créer des formes clickables, ou activables par d'autres méthodes.*/

/* Directives au pré-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <SFML_contexte.h>		
using namespace sf;


/* Constantes pour les conditions */
///============================== */



/* Objets à conditions */ /// En construction
///=================== */
class condition
{
private:
	int _etat;
	bool _visible;
	Color _peinture;
	Color _bord;
	Color _effet;
	RectangleShape _bouton;
	contexte _contenu;
	static Font _police;
public:
	condition();
	condition(Vector2f const & pos, RenderWindow * window,
		string const & police, string const & extrait, int const & mesure,
		Color const & encre, Color const & bord, Color const & surligne)
	{ }
};

condition::condition() {}