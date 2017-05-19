/* En-tête du fichier */
///================== */
/* Fichier:			SFML_contexte.h
// Auteur:			Olivier Lemay Dostie
// Date création:	13/05/2017
// Description:		Utilisation de la structure Text de SFML.
//					Permet la formation, manipulation et l'affichage d'objets à texte.*/

/* Directives au pré-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
using namespace std;
#include <SFML/Graphics.hpp>	
using namespace sf;


/* Constantes pour les contextes */
///============================= */



/* Objets à contextes */ /// En construction https://github.com/SFML/SFML/issues/216
///================== */
struct contexte
{
private:
	int _etat;
	bool _visible;
	Color _encre;
	Color _bord;
	Color _surligne;
	RectangleShape _fond;
	Text _extrait;
	static Font _police;
public:
	contexte() {}
	contexte(Vector2f const & pos, RenderWindow * window,
		string const & police, string const & extrait, int const & mesure,
		Color const & encre, Color const & bord, Color const & surligne)
	{
		_police.loadFromFile(police);
		_extrait.setFont(_police);
		_extrait.setString(extrait);
		_extrait.setCharacterSize(mesure);
		_encre = encre;
		_extrait.setFillColor(_encre);
		_bord = bord;
		_extrait.setOutlineColor(_bord);
		_extrait.setPosition(pos.x,
			pos.y*_extrait.getGlobalBounds().height -
			_extrait.getGlobalBounds().top);

		int c = 0;
		while (extrait[c] != '\0') {
			_fond.setPosition(_extrait.findCharacterPos(c).x +
				_police.getGlyph(extrait[c], 30, 0).bounds.left,
				_extrait.getGlobalBounds().top +
				(_extrait.getGlobalBounds().height + _police.getGlyph(extrait[c], 30, 0).bounds.top));
			_fond.setSize(Vector2f(_police.getGlyph(extrait[c], 30, 0).bounds.width,
				_police.getGlyph(extrait[c], 30, 0).bounds.height));
			_fond.setFillColor(surligne);
			window->draw(_fond);
			c++;
		}

		window->draw(_extrait);
	}
};