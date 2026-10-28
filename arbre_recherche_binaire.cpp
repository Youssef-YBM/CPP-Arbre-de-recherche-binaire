#include <iostream>
using namespace std;

// Chaque noeud contient une valeur et deux pointeurs vers ses enfants.
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

// Insere la valeur a la bonne place en respectant l'ordre de l'arbre.
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

// Le parcours gauche-racine-droite affiche les valeurs dans l'ordre croissant.
void afficherEnOrdre(Noeud* racine) {
    if (racine != nullptr) {
        afficherEnOrdre(racine->gauche);
        cout << racine->valeur << " ";
        afficherEnOrdre(racine->droite);
    }
}

// Libere tous les noeuds, en commencant par les enfants.
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

    // Le menu reste affiche jusqu'a ce que l'utilisateur choisisse de quitter.
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
