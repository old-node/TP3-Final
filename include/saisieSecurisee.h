/* En-tête du fichier
======================
Fichier:	    saisieSecurisee.h
Auteur:         Olivier Lemay Dostie
Date création:  16/02/2017
Description:    Fichier qui regroupe toutes les fonctions des laboratoires 
.				de saisie et de validation en console, ainsi que l’ouverture 
.				de fichier et le test de fichier vide. */

/*Directives au pré-processeur.
==========================*/
#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

/* Fonctions.
==========*/
//Vide le buffer avant chaque saisie.
void viderBuffer() {
	cin.clear();
	cin.seekg(0, ios::end);

	//Si le flux est dans un état invalide, on le remet valide
	if (!cin.fail())	//Flux valide, donc le buffer est non vide
		cin.ignore(numeric_limits<streamsize>::max());
	else				//Flux invalide, donc le buffer est vide
		cin.clear();
}
//Attend que l'utilisateur pèse une touche.
void pause() {
	viderBuffer();
	cin.ignore();
}
//Attend que l'utilisateur pèse Enter.
void enter(ostream &sortie) {
	viderBuffer();
	sortie << "Pesez Enter pour continuer.";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
//Saisie un nombre entier.
int saisirInt(string message, ostream &sortie) {
	int naturel = 0;

	viderBuffer();
	sortie << message;
	cin >> naturel;

	while (cin.fail()) {
		viderBuffer();
		sortie << "\nVeillez entrer un nombre entier.\n" + message;
		cin >> naturel;
	}

	return naturel;
}
//Vérifie que la saisie est entier et positive.
int saisirIntPositif(string message, ostream &sortie) {
	int naturel;

	naturel = saisirInt(message, sortie);
	while (naturel < 0/*limit*/) {
		sortie << "\nVeillez entrer une entier positif et plus grand que " << 0/*limit*/ << ".";
		naturel = saisirInt(message, sortie);
	}

	return naturel;
}
//Saisie un nombre réel.
float saisirFloat(string message, ostream &sortie) {
	char saisie[1000] = { '\0' };	//Valeur saisie
	float rationnel;		//À retourner

	viderBuffer();
	sortie << message;
	cin >> rationnel;

	while (cin.fail()) {
		viderBuffer();
		sortie << "\nVeillez entrer un nombre réel.\n" + message;
		cin >> rationnel;
	}

	return rationnel;
}
//Vérifie que la saisie est réel et positive.
float saisirFloatPositif(string message, ostream &sortie) {
	float rationnel;

	rationnel = saisirFloat(message, sortie);
	while (rationnel < 0) {
		sortie << "\nVeillez entrer une valeur positive et non nulle.";
		rationnel = saisirFloat(message, sortie);
	}

	return rationnel;
}

//Saisie une chaine de caractère non vide.
string saisirStringNonVide(string message, ostream &sortie) {
	string chaine;		//Valeur saisie

	viderBuffer();
	sortie << endl << message;
	getline(cin, chaine);

	while (chaine.empty()) {
		viderBuffer();
		sortie << "\nVeillez entrer une chaine non vide.\n" + message;
		getline(cin, chaine);
	}
	sortie << endl;
	return chaine;
}
//Retourne la réponse à la question.
char saisirCharValide(string question, char choix1, char choix2, ostream &sortie) {
	char choix;			//Réponse saisie

	viderBuffer();
	sortie << endl << question;
	choix = cin.get();
	choix = tolower(choix);

	while (choix != choix1 && choix != choix2) {
		viderBuffer();
		sortie << "\nChoisissez entre " << choix1 << " ou " << choix2 << ".\n" + question;
		choix = cin.get();
		choix = tolower(choix);
	}

	return choix;
}
//Demande à l'utilisateur s'il veut rejouer
char questionOuiNon(string question, ostream &sortie) {
	char choix;			//Réponse saisie

	viderBuffer();
	sortie << endl << question;
	choix = cin.get();
	choix = tolower(choix);

	while (choix != 'n' && choix != 'o') {
		viderBuffer();
		sortie << "\nChoisissez 'o' ou 'n'." + question;
		choix = cin.get();
		choix = tolower(choix);
	}

	return choix;
}
//Retourne le stream si le fichier de lecture trouvé.
bool ouvrirFichier(ifstream &entree, string fichier, ostream &sortie) {
	entree.open(fichier.c_str());
	if (entree.is_open())
		return true;

	//Si le nom n'est pas adéquat
	sortie << "Le fichier '" << fichier << "' n’a pas été trouvé.\n";
	pause();
	return false;
}
//Retourne le stream si le fichier de lecture est non vide.
bool siFichierVide(ifstream& entree, ostream &sortie) {
	if (entree.eof()) { 	//Si la fin du fichier est atteinte
		sortie << "Le fichier sélectionné ne contient aucune valeur.\n";
		pause();
		return true;
	}
	return false;
}