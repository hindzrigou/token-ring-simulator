#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main() {
    int nbr_noeuds= 5; // Nombre de nœuds dans le reseau
    int premier_jeton= 1; // Jeton initial
    int pipes[5][2];  // Pipes entre les nœuds
    int token =premier_jeton;  // Jeton initial
    pid_t pid;
    // Créer les pipes pour chaque nœud
    for (int i = 0; i < nbr_noeuds; i++) {
        pipe(pipes[i]); 
    }
    // Créer les processus pour chaque nœud
    for (int i = 0; i < nbr_noeuds; i++) {
        pid = fork();

        if (pid < 0) {
            printf("Erreur lors du creation du fils");
            
        }
        if(pid == 0) {  // Processus enfant (nœud)
            int jeton_reçus;
            while (1) {
                // Lire le jeton depuis le pipe d'entrée
                read(pipes[i][0], &jeton_reçus, sizeof(jeton_reçus));
                printf("Nœud %d : Jeton reçu = %d\n", i, jeton_reçus);

                // Simuler une action du nœud
                if (jeton_reçus > 0) {
                    printf("Nœud %d : Utilise le jeton.\n", i);
                }

                // Passer le jeton au prochain nœud
                int next_node = (i + 1) % nbr_noeuds;
                write(pipes[next_node][1], &jeton_reçus, sizeof(jeton_reçus));
                // Pause pour ralenti l'exécution
                sleep(1);
            }
            exit(0);
        }
    }
    // Processus parent (nœud initial)
    printf("Lancement du réseau Token Ring.\n");
    write(pipes[0][1], &token, sizeof(token));  // Envoyer le jeton au premier nœud
    // Attendre les processus enfants (inactif ici car la simulation est infinie)
    for (int i = 0; i < nbr_noeuds; i++) {
        wait(NULL);
    }
    return 0;
    }