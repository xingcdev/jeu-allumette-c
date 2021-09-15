#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Nombre d'allumettes initial
#define NB_JOUEURS 2
#define NB_ALLUMETTES 30
#define SAISIE_MIN 1
#define SAISIE_MAX 3
#define TAILLE_NOM 20

void jeu();

/**
 * Affichage du menu du jeu
 */
void afficherMenu();

/**
 * Affichage du choix du mode de jeu (humain vs humain, humain vs ordi (naïf),
 * humain vs ordi (expert))
 */
void afficheChoixMode();

/**
 * Affichage des règles du jeu
 */
void afficheRegles();

/**
 * Affichage des crédits (réalisateurs du jeu et adresses e-mail
 */
void afficheCredits();

/**
 * Retrait d'allumettes au compteur d'allumettes restantes
 * @param nbAllumettesRestantes Nombre d'allumettes restantes
 * @param retrait Nombre d'allumettes à retirer
 */
void prendreAllumettes(int* nbAllumettesRestantes, int retrait);

/**
 * Change le numéro du joueur à jouer en faisant alterner 0 et 1 (affiché 1 et
 * 2)
 * @param numJoueur Numéro du joueur ayant réalisé le dernier tour
 */
void changerTourJoueur(int* numJoueur);

/**
 * Vérification de la saisie d'un nombre d'allumettes
 * @param nbAllumetteSaisi Le nombre d'allumettes que l'on souhaite retirer
 * @param nbAllumetteRestante Le nombre d'allumettes avant retrait
 * @return 1 si la saisie est conforme, 0 sinon
 */
int saisieEstConforme(int nbAllumetteSaisi, int nbAllumetteRestante);

/**
 * Saisie d'un nombre aléatoire d'allumettes à retirer (min 1, max : plus petit
 * nombre entre le nb d'allumettes restantes et SAISIE_MAX
 * @param nbAlumetteRestantes Le nombre d'allumettes avant retrait
 * @return
 */
int saisieRandom(int nbAlumetteRestantes);

/**
 * Saisie d'un nombre intelligent d'allumettes à retirer (faire en sorte que le
 * nombre d'allumettes restant soit congru à 1 modulo 4
 * @param nbAllumettesRestantes Le nombre d'allumettes avant retrait
 * @return Le nombre d'allumettes à retirer (
 */
int saisieIntelligente(int nbAllumettesRestantes);

void gestionSaisie(int mode, int numJoueur, int nbAllumetteRestante,
                   int* nbAllumetteSaisi);

int main() {
  int choix = 0;
  while (1) {
    afficherMenu();
    scanf("%d", &choix);

    switch (choix) {
      case 1:
        jeu();
        break;
      case 2:
        afficheRegles();
        break;
      case 3:
        afficheCredits();
        break;
      case 4:
        exit(0);
        break;
      default:
        printf("Veuillez effectuer une saisie correcte.\n");
    }
  }

  return 0;
}

void jeu() {
  // 1. humain 2. ordinateur (naïf) 3. ordinateur (expert)
  int mode = 0;

  do {
    afficheChoixMode();
    scanf("%d", &mode);
  } while (mode < 1 || mode > 3);

  // 0. joueur 1
  // 1. joueur 2
  int numJoueur = 0;

  char nomsJoueurs[NB_JOUEURS][TAILLE_NOM];
  printf("Joueur 1, entrez votre nom : ");

  getchar();  // Sinon le gets ne marche pas
  fflush(stdin);
  gets(nomsJoueurs[0]);

  if (mode == 2 || mode == 3) {
    strcpy(nomsJoueurs[1], "Ordinateur");
  } else {  // sinon mode humain donc on demande le nom au deuxième joueur
    printf("Joueur 2, entrez votre nom : ");
    fflush(stdin);
    gets(nomsJoueurs[1]);
  }

  int nbAllumetteSaisi = 0;
  int nbAllumetteRestante = NB_ALLUMETTES;

  // TODO phrase à adapter en fonction de si humain / robot
  switch (mode) {
    case 1:
      printf("Jeu contre humain mode %d, avec %d allumettes au depart\n", mode,
             NB_ALLUMETTES);
      break;
    case 2:
      printf(
          "Jeu contre ordinateur mode %d (naif), avec %d allumettes au "
          "depart\n",
          mode, NB_ALLUMETTES);
      break;
    case 3:
      printf(
          "Jeu contre ordinateur mode %d (expert), avec %d allumettes au "
          "depart\n",
          mode, NB_ALLUMETTES);
      break;
    default:
      break;
  }

  do {
    // Générer le message sur le nombre des allumettes
    for (int i = 1; i <= nbAllumetteRestante; i++) {
      printf("| ");
    }
    printf("\n");

    printf("J%d - Nombre d'allumettes a enlever : %d\n", numJoueur + 1,
           nbAllumetteRestante);

    printf("Au tour de %s\n", nomsJoueurs[numJoueur]);

    if (numJoueur == 0 ||
        mode == 1) {  // On n'affiche pas ce message quand l'ordi joue
      printf("Vous devez donner un entier compris entre 1 et 3.\n");
    }

    gestionSaisie(mode, numJoueur, nbAllumetteRestante, &nbAllumetteSaisi);

    // Test de saisi des joueurs
    while (!saisieEstConforme(nbAllumetteSaisi, nbAllumetteRestante)) {
      printf("Erreur ! Prise invalide : %d\n", nbAllumetteSaisi);
      printf("Recommencez !\n");
      gestionSaisie(mode, numJoueur, nbAllumetteRestante, &nbAllumetteSaisi);
    }

    printf("%s prend %d allumette(s)\n", nomsJoueurs[numJoueur],
           nbAllumetteSaisi);
    prendreAllumettes(&nbAllumetteRestante, nbAllumetteSaisi);
    changerTourJoueur(&numJoueur);

  } while (nbAllumetteRestante > 0);

  printf("%s a perdu\n", nomsJoueurs[1 - numJoueur]);
}

void afficherMenu() {
  printf("\nQue faire ?\n\n");
  printf("1 - Jouer\n");
  printf("2 - Voir les regles du jeu\n");
  printf("3 - Voir les credits\n");
  printf("4 - Quitter\n");

  printf("\nVotre choix : ");
};

void afficheChoixMode() {
  printf("Choisissez le mode de jeu : \n");
  printf("1 - Humain contre humain\n");
  printf("2 - Humain contre ordinateur (ordi naif)\n");
  printf("3 - Humain contre ordinateur (ordi expert)\n");
  printf("\nVotre choix : ");
}

void afficheRegles() {
  printf("\nLe but de ce jeu est simple:\n");
  printf("Ce jeu se joue a deux. Vous avez un nombre n d'allumettes.\n");
  printf(
      "A tour de role, chacun des deux joueurs enleve entre 1 et 3 allumettes "
      "selon ce qu'il souhaite. Le perdant est celui qui doit enlever la "
      "derniere allumette.\n");
  printf(
      "\nAttention, il existe 3 niveaux de difficulte en version humain vs "
      "ordi.\n");
  printf(
      "Le 1er est plutot simple a battre, donc pour les debutants. Mais le "
      "dernier...");
  printf(
      "Contactez-nous si vous avez reussi (screenshot evidemment ^^), nos "
      "emails sont dans les credits (choix 3).\n");
}

void afficheCredits() {
  printf("Jeu developpe par Xing Chen et Antoine Despres.\n");
  printf("Contact : xing.chen@efrei.net et antoine.despres@efrei.net\n");
}

int saisieEstConforme(int nbAllumetteSaisi, int nbAllumetteRestante) {
  return (nbAllumetteSaisi > 0 && nbAllumetteSaisi <= 3 &&
          nbAllumetteRestante - nbAllumetteSaisi >= 0);
}

void prendreAllumettes(int* nbAllumettes, int nbRetrait) {
  *nbAllumettes -= nbRetrait;
}

void changerTourJoueur(int* numJoueur) {
  // Alternance entre 1 et 0
  *numJoueur = 1 - *numJoueur;
}

int saisieRandom(int nbAllumettesRestantes) {
  srand(time(NULL));
  /* L'ordinateur peut saisir un nombre compris entre 1 et 3.
  Si le nombre d'allumettes restantes < 3, un nombre compris entre 1 et le
  nombre d'allumettes restantes.*/
  int retraitMax =
      (nbAllumettesRestantes < SAISIE_MAX ? nbAllumettesRestantes : SAISIE_MAX);
  return (rand() % (retraitMax - SAISIE_MIN + 1)) + SAISIE_MIN;
}

int saisieIntelligente(int nbAllumettesRestantes) {
  int retraitMax =
      (nbAllumettesRestantes < SAISIE_MAX ? nbAllumettesRestantes : SAISIE_MAX);

  for (int i = SAISIE_MIN; i <= retraitMax; ++i) {
    if ((nbAllumettesRestantes - i) % 4 == 1) {
      return i;
    }
  }
  return 1;
}

void gestionSaisie(int mode, int numJoueur, int nbAllumetteRestante,
                   int* nbAllumetteSaisi) {
  if (numJoueur == 0 || mode == 1) {
    printf("Combien prenez-vous d'allumettes ?\n");
    scanf("%d", nbAllumetteSaisi);
  } else if (mode == 2 && numJoueur == 1) {
    *nbAllumetteSaisi = saisieRandom(nbAllumetteRestante);
  } else if (mode == 3 && numJoueur == 1) {
    *nbAllumetteSaisi = saisieIntelligente(nbAllumetteRestante);
  }
}