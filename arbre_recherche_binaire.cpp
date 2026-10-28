#include <iostream>
using namespace std;

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droite;

    Noeud(int nouvelleValeur) {
        valeur = nouvelleValeur;
        gauche = nullptr;
        droite = nullptr;
    }
};

Noeud* inserer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return new Noeud(valeur);
    }

    if (valeur < racine->valeur) {
        racine->gauche = inserer(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droite = inserer(racine->droite, valeur);
    } else {
        cout << "Cette valeur existe deja dans l'arbre.\n";
    }

    return racine;
}

void afficherEnOrdre(Noeud* racine) {
    if (racine != nullptr) {
        afficherEnOrdre(racine->gauche);
        cout << racine->valeur << " ";
        afficherEnOrdre(racine->droite);
    }
}

void libererArbre(Noeud* racine) {
    if (racine != nullptr) {
        libererArbre(racine->gauche);
        libererArbre(racine->droite);
        delete racine;
    }
}

int main() {
    Noeud* racine = nullptr;
    int choix;
    int valeur;

    do {
        cout << "\n--- Arbre de recherche binaire ---\n";
        cout << "1. Inserer une valeur\n";
        cout << "2. Afficher l'arbre (ordre croissant)\n";
        cout << "0. Quitter\n";
        cout << "Votre choix : ";

        if (!(cin >> choix)) {
            cout << "Saisie invalide. Fin du programme.\n";
            break;
        }

        switch (choix) {
            case 1:
                cout << "Valeur a inserer : ";
                if (cin >> valeur) {
                    racine = inserer(racine, valeur);
                } else {
                    cout << "Saisie invalide. Fin du programme.\n";
                    libererArbre(racine);
                    return 1;
                }
                break;

            case 2:
                if (racine == nullptr) {
                    cout << "L'arbre est vide.\n";
                } else {
                    cout << "Valeurs : ";
                    afficherEnOrdre(racine);
                    cout << "\n";
                }
                break;

            case 0:
                cout << "Au revoir !\n";
                break;

            default:
                cout << "Choix invalide. Reessayez.\n";
        }
    } while (choix != 0);

    libererArbre(racine);
    return 0;
}
