/* En-tête du fichier */
///================== */
/* Fichier:			SFML_contenu.h
// Auteur:			Olivier Lemay Dostie
// Date création:	13/05/2017
// Description:		Formation, manipulation et affichage d'objets formés avec SFML.
//					Permet de créer, de manipuler et d'afficher des formes clickables, 
//					ou activables par d'autres méthodes.*/

/* Directives au pré-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <contexte.h>			
using namespace sf;


/* Constantes pour les contenus */
///============================ */



/* Objets à contenus */ /// En construction
///================= */
class contenu
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
	contenu();
	contenu(Vector2f const & pos, RenderWindow * window,
		string const & police, string const & extrait, int const & mesure,
		Color const & encre, Color const & bord, Color const & surligne)
	{ }
};

contenu::contenu() {}