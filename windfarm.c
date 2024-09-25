#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Définition de la structure pour une éolienne
typedef struct Eolienne {
int etat; // 0 = neuf, 1 = fonctionne, 2 = en panne int joursPanne;
struct Eolienne *suivant;
} Eolienne;
// Prototypes de fonctions
Eolienne* initialiserParc(int nombreEoliennes);
void simulerJour(Eolienne *parc, int *vitesseVent, double *prodJournaliere, int
*interventionsMaintenance, int *joursPleineCapacite);
int calculerJoursPanne(Eolienne *parc);
double calculerCoutMaintenance(Eolienne *parc, int nombreJours);
double calculerProductionTotale(Eolienne *parc, int nombreJours);
void libererParc(Eolienne *parc);
int determinerVitesseVent(int vitesseVentActuelle);
double calculerCoutTotalMaintenance(int interventionsMaintenance);
double calculerProductionMinimale(Eolienne *parc, int nombreJours);
double calculerProductionMaximale(Eolienne *parc, int nombreJours);
void reinitialiserParc(Eolienne *parc, int *vitesseVent, int
*interventionsMaintenance, int *joursPleineCapacite);
void afficherMenu();
int determinerVitesseVent(int vitesseVentActuelle) {
// Matrice de transition V
int V[3][3] = {{2, 73, 7}, {11, 74, 15}, {4, 61, 35}}; int probaCumulée = 0;
int tirage = rand() % 100;
for (int i = 0; i < 3; i++) {
        probaCumulée += V[vitesseVentActuelle][i];
        if (tirage < probaCumulée) {
return i; }
}
return vitesseVentActuelle; // Retourner la vitesse actuelle par défaut }
Eolienne* initialiserParc(int nombreEoliennes) {
    Eolienne *tete = NULL, *temp;
    for (int i = 0; i < nombreEoliennes; i++) {
        temp = (Eolienne*)malloc(sizeof(Eolienne));
        if (temp == NULL) {
            perror("Erreur d'allocation de mémoire");
            exit(EXIT_FAILURE);
        }
temp->etat = 0; // état neuf temp->joursPanne = 0; temp->suivant = tete;
tete = temp;
}
    return tete;
}
void simulerJour(Eolienne *parc, int *vitesseVent, double *prodJournaliere, int
*interventionsMaintenance, int *joursPleineCapacite) {
double production[] = {0, 0.4, 0.65}; // Production en MW
Eolienne *actuelle = parc;
int enPleineCapacite = 1; // On suppose que le parc est en pleine capacité
while (actuelle != NULL) {
 if (actuelle->etat == 2) { // Éolienne en panne
if (*vitesseVent != 2 && actuelle->joursPanne < 3) { // Maintenance
possible si le vent n'est pas fort
actuelle->joursPanne++;
if (actuelle->joursPanne == 3) { // Maintenance terminée après 3 jours
actuelle->etat = 0; // Éolienne réparée et considérée comme neuve actuelle->joursPanne = 0;
*interventionsMaintenance += 1; // Suivi des interventions de
maintenance terminées
} }
enPleineCapacite = 0; // Si une éolienne est en panne, le parc n'est pas en pleine capacité
} else if (*vitesseVent == 2 && (rand() % 100) < 15) { // 15% de chance de panne si vent fort
actuelle->etat = 2;
actuelle->joursPanne = 1; // Premier jour de panne enPleineCapacite = 0;
} else {
*prodJournaliere += production[*vitesseVent]; // Production normale
}
        actuelle = actuelle->suivant;
    }
if (enPleineCapacite && *vitesseVent > 0) { // Pleine capacité uniquement si vent moyen ou fort
        (*joursPleineCapacite)++;
    }
*vitesseVent = determinerVitesseVent(*vitesseVent); // Mise à jour de la vitesse du vent
}
int calculerJoursPanne(Eolienne *parc) {
    int total = 0;
    Eolienne *actuelle = parc;
    while (actuelle != NULL) {
        total += actuelle->joursPanne;
        actuelle = actuelle->suivant;
    }
    return total;
}
double calculerCoutMaintenance(Eolienne *parc, int nombreJours) {
    double coutTotal = 0;
    int nombreEoliennes = 0;
    Eolienne *actuelle = parc;
while (actuelle != NULL) {
coutTotal += (actuelle->joursPanne / 3) * 150000; // 150k€ par intervention nombreEoliennes++;
actuelle = actuelle->suivant;
}
    return nombreEoliennes > 0 ? coutTotal / nombreEoliennes : 0;
}
double calculerProductionTotale(Eolienne *parc, int nombreJours) {
double productionTotale = 0;
int vitesseVent = 0; // Vitesse du vent initiale (nulle)
int joursPanneTotal = 0; // Suivi des jours de panne total
int interventionsMaintenance = 0; // Suivi des interventions de maintenance
    for (int jour = 0; jour < nombreJours; jour++) {
        double prodJournaliere = 0;

         simulerJour(parc, &vitesseVent, &prodJournaliere, &joursPanneTotal,
&interventionsMaintenance);
        productionTotale += prodJournaliere;
    }
    return productionTotale;
}
double calculerCoutTotalMaintenance(int interventionsMaintenance) {
const double coutIntervention = 150000.0; // Coût d'une intervention en euros return interventionsMaintenance * coutIntervention;
}
double calculerProductionMinimale(Eolienne *parc, int nombreJours) {
double productionMinimale = -1; // Initialisation à -1 pour indiquer qu'aucune
valeur n'a encore été définie
    int vitesseVent = 0;
    int joursPanneTotal = 0, interventionsMaintenance = 0;
    for (int jour = 0; jour < nombreJours; jour++) {
        double prodJournaliere = 0;
        simulerJour(parc, &vitesseVent, &prodJournaliere, &joursPanneTotal,
&interventionsMaintenance);
        if (productionMinimale == -1 || prodJournaliere < productionMinimale) {
            productionMinimale = prodJournaliere;
        }
}
    return productionMinimale;
}
double calculerProductionMaximale(Eolienne *parc, int nombreJours) {
    double productionMaximale = 0;
    int vitesseVent = 0;
    int joursPanneTotal = 0, interventionsMaintenance = 0;
    for (int jour = 0; jour < nombreJours; jour++) {
        double prodJournaliere = 0;
        simulerJour(parc, &vitesseVent, &prodJournaliere, &joursPanneTotal,
&interventionsMaintenance);
        if (prodJournaliere > productionMaximale) {
            productionMaximale = prodJournaliere;
        }
}
    return productionMaximale;
}
double calculerPourcentagePleineCapacite(int joursPleineCapacite, int nombreJours) {
    return (double)joursPleineCapacite / nombreJours * 100;
}
void reinitialiserParc(Eolienne *parc, int *vitesseVent, int
*interventionsMaintenance, int *joursPleineCapacite) {
    Eolienne *actuelle = parc;
    while (actuelle != NULL) {
actuelle->etat = 0; // État neuf actuelle->joursPanne = 0; // Aucune panne actuelle = actuelle->suivant;

 }
    *vitesseVent = 0;
    *interventionsMaintenance = 0;
    *joursPleineCapacite = 0;
}
void libererParc(Eolienne *parc) {
    Eolienne *temp;
    while (parc != NULL) {
        temp = parc;
        parc = parc->suivant;
        free(temp);
    }
}
void afficherMenu() {
printf("Menu:\n");
printf("1. Lancer une simulation\n");
printf("2. Afficher la production totale annuelle\n");
printf("3. Afficher la production journalière minimale\n");
printf("4. Afficher la production journalière maximale\n");
printf("5. Afficher le cout total de maintenance\n");
printf("6. Afficher le pourcentage de temps de fonctionnement à pleine
capacité\n");
printf("7. Réinitialiser la simulation\n"); printf("8. Quitter\n");
}
// Fonction principale
int main() {
    srand(time(NULL));
    int choix;
    int nombreEoliennes;
    int nombreJours;
    int vitesseVent = 0;
    int interventionsMaintenance = 0;
    int joursPleineCapacite = 0;
    double prodJournaliere = 0;
    Eolienne *parc = NULL;
    while(choix != 8)
    {
        afficherMenu();
        printf("Entrez votre choix: ");
        scanf("%d", &choix);
        switch(choix) {
            case 1:
                printf("Entrez le nombre de jours de la simulation: ");
                scanf("%d", &nombreJours);
                printf("Entrez le nombre d'éoliennes: ");
                scanf("%d", &nombreEoliennes);
                if (parc != NULL) {
                    libererParc(parc);
                }
                parc = initialiserParc(nombreEoliennes);
                for (int jour = 0; jour < nombreJours; jour++) {
                    double prodJournaliere = 0;
                    simulerJour(parc, &vitesseVent, &prodJournaliere,
&interventionsMaintenance, &joursPleineCapacite);
}
break; case 2:

printf("Production totale annuelle: %.2f MW\n", calculerProductionTotale(parc, nombreJours));
break; case 3:
printf("Production journalière minimale: %.2f MW\n", calculerProductionMinimale(parc, nombreJours));
break; case 4:
printf("Production journalière maximale: %.2f MW\n", calculerProductionMaximale(parc, nombreJours));
break; case 5:
            double coutTotalMaintenance =
calculerCoutTotalMaintenance(interventionsMaintenance);
printf("Cout total de maintenance: %.2f Euros\n", coutTotalMaintenance); break;
            case 6:
                double pourcentagePleineCapacite =
calculerPourcentagePleineCapacite(joursPleineCapacite, nombreJours);
                printf("Pourcentage de temps de fonctionnement à pleine capacité:
%.2f%%\n", pourcentagePleineCapacite); break;
            case 7:
                reinitialiserParc(parc, &vitesseVent, &interventionsMaintenance,
&joursPleineCapacite);
printf("Simulation réinitialisée.\n");
break; case 8:
printf("Quitter.\n");
break; default:
printf("Choix non valide. Veuillez réessayer.\n");
} }
    libererParc(parc);
return 0; }
