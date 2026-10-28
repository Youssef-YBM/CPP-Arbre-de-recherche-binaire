#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
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

// Recherche une valeur dans l'arbre.
bool rechercher(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return false;
    }

    if (valeur == racine->valeur) {
        return true;
    }

    if (valeur < racine->valeur) {
        return rechercher(racine->gauche, valeur);
    }

    return rechercher(racine->droite, valeur);
}

// Retourne le noeud contenant la plus petite valeur d'un sous-arbre.
Noeud* trouverMinimum(Noeud* racine) {
    while (racine != nullptr && racine->gauche != nullptr) {
        racine = racine->gauche;
    }
    return racine;
}

// Supprime une valeur tout en preservant l'ordre de l'arbre.
Noeud* supprimer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return nullptr;
    }

    if (valeur < racine->valeur) {
        racine->gauche = supprimer(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droite = supprimer(racine->droite, valeur);
    } else if (racine->gauche == nullptr) {
        Noeud* droite = racine->droite;
        delete racine;
        return droite;
    } else if (racine->droite == nullptr) {
        Noeud* gauche = racine->gauche;
        delete racine;
        return gauche;
    } else {
        Noeud* minimum = trouverMinimum(racine->droite);
        racine->valeur = minimum->valeur;
        racine->droite = supprimer(racine->droite, minimum->valeur);
    }

    return racine;
}

// Le parcours gauche-racine-droite affiche les valeurs dans l'ordre croissant.
void afficherEnOrdre(Noeud* racine, ostream& sortie = cout) {
    if (racine != nullptr) {
        afficherEnOrdre(racine->gauche, sortie);
        sortie << racine->valeur << " ";
        afficherEnOrdre(racine->droite, sortie);
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

// Mesure les durees d'insertion, de recherche et de suppression.
void testerPerformances(int taille) {
    vector<int> valeurs(taille);
    iota(valeurs.begin(), valeurs.end(), 0);

    // Melanger les valeurs pour eviter de construire un arbre volontairement trie.
    random_device generateurAleatoire;
    mt19937 generateur(generateurAleatoire());
    shuffle(valeurs.begin(), valeurs.end(), generateur);

    Noeud* arbreTest = nullptr;

    auto debutInsertion = chrono::steady_clock::now();
    for (int valeur : valeurs) {
        arbreTest = inserer(arbreTest, valeur);
    }
    auto finInsertion = chrono::steady_clock::now();

    int valeursTrouvees = 0;
    auto debutRecherche = chrono::steady_clock::now();
    for (int valeur : valeurs) {
        if (rechercher(arbreTest, valeur)) {
            ++valeursTrouvees;
        }
    }
    auto finRecherche = chrono::steady_clock::now();

    // Enregistre le parcours trie dans un fichier, sans inclure cette ecriture dans les mesures.
    ofstream fichierSortie("output.txt");
    if (!fichierSortie) {
        cerr << "Erreur : impossible de creer le fichier output.txt.\n";
    } else {
        fichierSortie << "Valeurs de l'arbre de test (ordre croissant) :\n";
        afficherEnOrdre(arbreTest, fichierSortie);
        fichierSortie << "\n";
        fichierSortie.close();

        if (fichierSortie) {
            cout << "Affichage de l'arbre enregistre dans output.txt.\n";
        } else {
            cerr << "Erreur : echec lors de l'ecriture dans output.txt.\n";
        }
    }

    auto debutSuppression = chrono::steady_clock::now();
    for (int valeur : valeurs) {
        arbreTest = supprimer(arbreTest, valeur);
    }
    auto finSuppression = chrono::steady_clock::now();

    const chrono::duration<double, milli> dureeInsertion = finInsertion - debutInsertion;
    const chrono::duration<double, milli> dureeRecherche = finRecherche - debutRecherche;
    const chrono::duration<double, milli> dureeSuppression = finSuppression - debutSuppression;

    cout << fixed << setprecision(3);
    cout << "\nTest termine pour " << taille << " valeurs.\n";
    cout << "Valeurs retrouvees : " << valeursTrouvees << " / " << taille << "\n";
    cout << "Temps total d'insertion  : " << dureeInsertion.count() << " ms\n";
    cout << "Temps total de recherche : " << dureeRecherche.count() << " ms\n";
    cout << "Temps total de suppression : " << dureeSuppression.count() << " ms\n";

    libererArbre(arbreTest);
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
        cout << "3. Tester les performances\n";
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

            case 3: {
                int taille;
                cout << "Nombre de valeurs a tester : ";
                if (cin >> taille && taille > 0) {
                    testerPerformances(taille);
                } else {
                    cout << "Saisie invalide : entrez un entier positif.\n";
                    libererArbre(racine);
                    return 1;
                }
                break;
            }

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
