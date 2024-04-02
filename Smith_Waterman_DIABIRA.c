/*
 * Nom du fichier : mon_fichier.c
 * Description : Ce fichier contient le code source pour l'algorithme d’alignement de séquence de Smith-Waterman.
 * Auteur : Assa DIABIRA
 * Dernière modification : 02/04/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 1000
#define MATRIX_SIZE 24

/*
 * mesure_len_seq - Cette fonction mesure la longueur de la séquence dans un fichier FASTA.
 * Arguments :
 *   - filename : le nom du fichier FASTA à lire
 * Sortie :
 *   - Le nombre de caractères alphabétiques dans la séquence.
 */
int mesure_len_seq(char *filename){
    FILE *fr; 
    int n = 0; // Compteur pour les acides amines
    char c;
    
    fr = fopen(filename, "r"); // Ouvrir le fichier en mode lecture
    if (fr == NULL) { // Verification ouverture du fichier (erreur puis fin programme)
        printf("Erreur : Impossible d'ouvrir le fichier !'%s'\n", filename);
        return -1;
    }

     while ((c = fgetc(fr)) != EOF) { // Lecture des caractères jusqu'a END OF FILE
        if (c == '>') { // Si le caractère est '>' passage a la ligne suivante
            while ((c = fgetc(fr)) != EOF && c != '\n') {
            }
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) { // Incrémentation du caractère si c'est une lettre (min ou maj)
            n++;
        }
    }

    fclose(fr); // Fermeture du fichier 
    return n;  // // Retourne la taille de la séquence
}

/*
 * read_sequence - Cette fonction prend en entrée le nom d’un fichier FASTA et retourne une chaîne de caractères representant la sequence.
 * Arguments :
 *   - filename : le nom du fichier FASTA a lire
 * Sortie :
 *   - Une chaîne de caractères representant la sequence.
 */
char *read_sequence(char *filename){
    FILE *fr;      
    int n = 0, i = 0; // n: compteur acides amines , i: indice pour parcourir la sequence
    char *t, c;       // t: pointeur vers la sequence de protéines

    fr = fopen(filename, "r"); // Ouverture du fichier en mode lecture
    if (fr == NULL) { // Verification ouverture du fichier (erreur puis fin programme)
        printf("Erreur : Impossible d'ouvrir le fichier !\n");
        return NULL;
    }

    n = mesure_len_seq(filename);  // Appel de mesure_len_seq pour déterminer la longueur de la séquence
    t = malloc((n + 1) * sizeof(char)); // Allocation de mémoire pour stocker la séquence

    // Ignorer les lignes qui commencent par '>'
    while ((c = fgetc(fr)) != EOF && c != '>') {}  
    while ((c = fgetc(fr)) != EOF && c != '\n') {}

    // Lecture de la séquence
    while ((c = fgetc(fr)) != EOF) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            t[i++] = c;
        }
    }

    t[i] = '\0'; // Terminer la séquence de protéines avec un caractère nul
    printf("La séquence de protéine du fichier %s est : %s (%d acides aminés).\n", filename, t,n);
    fclose(fr); // Fermeture du fichier
    return t; // Retourne la séquence de protéines
}

/*
 * read_substitution_matrix - Cette fonction prend en entrée le nom d’un fichier texte contenant la matrice et retourne une matrice de pointeurs, où chaque pointeur pointe vers un tableau d'entiers contenant une ligne de la matrice de substitution.
 * Arguments :
 *   - filename : le nom du fichier de matrice de substitution.
 * Sortie :
 *   - Un pointeur vers la matrice de substitution.
 */
int **read_substitution_matrix(char *filename) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int row = 0, col = 0, value;
    int **matrix;

    matrix = (int **)malloc(MATRIX_SIZE * sizeof(int *)); // Allocation de la mémoire pour la matrice
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
    }

    file = fopen(filename, "r"); // Ouverture du fichier matrice
    if (file == NULL) {
         printf("Erreur : Impossible d'ouvrir le fichier matrice !\n");
        return NULL;
    }
    
    while (fgets(line, MAX_LINE_LENGTH, file)) { // Lecture des lignes du fichier en ignorant les lignes de commentaires
        if (line[0] == '#' || line[0] == '\n') {
            continue; 
        }
        if (row > MATRIX_SIZE) {
            break; 
        }
        col = 0; 
        char *token = strtok(line, " "); // La tokenisation permet d'extraire les nb et d'assurer le bon positionnement
        while (token != NULL) {
            if (col == 0) {
                col++;
                token = strtok(NULL, " ");
                continue;
            }
            sscanf(token, "%d", &value);
            if (row != 0) { 
                matrix[row - 1][col - 1] = value;
            }
            col++;
            token = strtok(NULL, " ");
        }
        row++;
    }

    fclose(file); // Fermeture du fichier

    // Vérification de la matrice
    /*printf("Voici la matrice obtenue à partir de %s\n", filename); 
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }*/
    printf("\n");
    printf("Nous utilisons la matrice %s.\n",filename);
    
    return matrix;
}

/*
 * initialize_matrix - Cette fonction prend en entrée des entiers n et m afin de créer une matrice avec une valeur initiale de "value".
 * Arguments :
 *   - n : un entier représentant le nombre de lignes de la matrice.
 *   - m : un entier représentant le nombre de colonnes de la matrice.
 *   - value : un entier représentant la valeur initiale de la matrice.
 * Sortie :
 *   - Un pointeur vers la matrice initialisée.
 */
int** initialize_matrix(int n, int m, int value) {
    int** matrix = (int**)malloc(n * sizeof(int*)); // Allocation mémoire d'un tableau de pointeurs pour les lignes
    if (matrix == NULL) {
        printf("Erreur d'allocation de mémoire pour les lignes de la matrice\n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(m * sizeof(int)); // Allocation de la mémoire pour chaque ligne
        if (matrix[i] == NULL) {
            printf("Erreur d'allocation de mémoire pour la ligne %d de la matrice\n", i);

            for (int j = 0; j < i; j++) { // Liberation de la mémoire allouée précédemment
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }

        for (int j = 0; j < m; j++) { // Initialisation de la ligne avec la valeur donnée
            matrix[i][j] = value;
        }
    }

    /*if (matrix != NULL) { // Vérification de l'initialisation de la matrice
        printf("Matrice initialisée avec des valeurs de %d :\n", value);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }*/
    printf("\n");
    printf("La matrice %d x %d a bien été initialisée avec une valeur initiale de %d. \n", n, m, value);
    return matrix;
}

/*
 * compute_score - Cette fonction calcule le score de similarité entre deux caractères en utilisant une matrice de substitution.
 * Arguments :
 *   - a : un caractère représentant le premier caractère.
 *   - b : un caractère représentant le deuxième caractère.
 *   - substitution_matrix : un pointeur vers la matrice de substitution.
 * Sortie :
 *   - Le score de similarité (entier).
 */

int compute_score(char a, char b, int** substitution_matrix) {
    char amino_acids[] = "ARNDCQEGHILKMFPSTWYV"; // Spécification de l'ordre des AA standards

    int index_a = -1, index_b = -1; // Recherche des AA que nous avons dans la matrice de substitution
    for (int i = 0; i < sizeof(amino_acids); ++i) {
        if (a == amino_acids[i]) {
            index_a = i;
        }
        if (b == amino_acids[i]) {
            index_b = i;
        }
    }

    if (index_a == -1 || index_b == -1) { // Erreur pour les AA non standards
        printf("Erreur : Les caractères ne sont pas des acides aminés standard.\n");
        return -1;
    }

    int score = substitution_matrix[index_a][index_b]; // Valeur à l'intersection des AA = score de similarité
    return score;
}

/*
 * max - Cette fonction retourne le maximum entre deux entiers.
 * Arguments :
 *   - a : le premier entier.
 *   - b : le deuxième entier.
 * Sortie :
 *   - Le plus grand des deux entiers.
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/*
 * create_alignment_matrix - Cette fonction  crée et remplie la matrice des scores d'alignements pour chaque AA des séquences.
 * Arguments :
 *   - seq1 : une chaîne de caractères représentant la première séquence.
 *   - seq2 : une chaîne de caractères représentant la deuxième séquence.
 *   - substitution_matrix : un pointeur vers la matrice de substitution.
 ∗   - gap_penalty : un entier représentant le score de pénalité pour un gap.
 * Sortie :
 *   - Un pointeur vers la matrice des scores d’alignements.
 */
int** create_alignment_matrix(char* seq1, char* seq2, int** substitution_matrix, int gap_penalty) {
    // Obtention de la longueur des deux séquences
    int len_seq1 = strlen(seq1);
    int len_seq2 = strlen(seq2);
    

    // Allocation de mémoire pour la matrice de scores d'alignement
    int** alignment_matrix = initialize_matrix(len_seq1 + 1, len_seq2 + 1, 0);
    
    //printf("Matrice d'alignement initialisée\n");

    // Initialisation de la première colonne avec des pénalités de gap
    for (int i = 0; i <= len_seq1; ++i) {
        alignment_matrix[i][0] = i * gap_penalty;
    }

    //printf("Première colonne initialisée\n");

    // Initialisation de la première ligne avec des pénalités de gap
    for (int j = 0; j <= len_seq2; ++j) {
        alignment_matrix[0][j] = j * gap_penalty;
    }

    //printf("Première ligne initialisée\n");

    // Calcul des scores d'alignement pour les autres cases de la matrice

    for (int i = 1; i <= len_seq1; ++i) {
        for (int j = 1; j <= len_seq2; ++j) {
            int max_score;
            // Score obtenu en alignant les caractères de seq1[i-1] et seq2[j-1]
            int match_score = alignment_matrix[i-1][j-1] + compute_score(seq1[i-1], seq2[j-1], substitution_matrix);
            // Score obtenu en faisant une délétion
            int delete_score = alignment_matrix[i-1][j] + gap_penalty;
            // Score obtenu en faisant une insertion 
            int insert_score = alignment_matrix[i][j-1] + gap_penalty;

            // Take maximum score among paths, or 0 if negative
            max_score = match_score;
            if (delete_score > max_score) max_score = delete_score;
            if (insert_score > max_score) max_score = insert_score;
            
            // Update the score matrix
            alignment_matrix[i][j] = max_score;

            /* Vérification : Affichage de l'alignement pour ce caractère
            printf("Alignement pour la position (%d, %d):\n", i, j);
            printf("Seq1: %c\n", seq1[i-1]);
            printf("Seq2: %c\n", seq2[j-1]);
            printf("deletion = %d et insertion = %d et match = %d\n",delete_score,insert_score,match_score);
            printf("Score final: %d\n", max_score);
            printf("\n"); */
        }
    }

    printf("\n");
    printf("La matrice des scores de dimensions %d x %d remplie est la suivante :\n", len_seq1 + 1, len_seq2 + 1);
    for (int i = 0; i <= len_seq2; i++) {
        for (int j = 0; j <= len_seq1; j++) {
            printf("%d ", alignment_matrix[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    return alignment_matrix;
}

/*
 * calculate_alignment - Cette fonction calcule l’alignement optimal entre deux séquences.
 * Arguments :
 *   - seq1 : une chaîne de caractères représentant la première séquence.
 *   - seq2 : une chaîne de caractères représentant la deuxième séquence.
 *   - substitution_matrix : un pointeur vers la matrice de substitution.
 ∗   - gap_penalty : un entier représentant le score de pénalité pour un gap.
 ∗   - aligned_seq1 : un pointeur vers une chaîne de caractères représentant la première séquence alignée.
 ∗   - aligned_seq2 : un pointeur vers une chaîne de caractères représentant la deuxième séquence alignée.
 * Sortie :
 *   - La fonction retourne un entier représentant le score de similarité entre les deux séquence.
 ∗   - Les deux séquences alignées sont retournées dans les variables aligned_seq1 et aligned_seq2.
 */
int calculate_alignment(char* seq1, char* seq2, int** substitution_matrix,
                        int gap_penalty, char** aligned_seq1, char** aligned_seq2) {
    int len_seq1 = strlen(seq1);
    int len_seq2 = strlen(seq2);
    
    // Appel à la fonction create_alignment_matrix pour obtenir la matrice des scores d'alignement
    int** alignment_matrix = create_alignment_matrix(seq1, seq2, substitution_matrix, gap_penalty);

    // Récupération du score 
    int max_similarity_score = alignment_matrix[len_seq1][len_seq2];

    // Reconstruction des séquences alignées en remontant dans la matrice
    *aligned_seq1 = (char*)malloc((len_seq1 + len_seq2 + 1) * sizeof(char));
    *aligned_seq2 = (char*)malloc((len_seq1 + len_seq2 + 1) * sizeof(char));
    int current_i = len_seq1;
    int current_j = len_seq2;
    int align_seq1_index = 0;
    int align_seq2_index = 0;

    // Parcours de la matrice pour remonter le chemin optimal
    while (current_i > 0 || current_j > 0) {
        if (current_i > 0 && current_j > 0 && alignment_matrix[current_i][current_j] == alignment_matrix[current_i - 1][current_j - 1] + compute_score(seq1[current_i - 1], seq2[current_j - 1], substitution_matrix)) {
            // Correspondance entre les deux caractères
            (*aligned_seq1)[align_seq1_index++] = seq1[current_i - 1];
            (*aligned_seq2)[align_seq2_index++] = seq2[current_j - 1];
            current_i--;
            current_j--;
        } else if (current_i > 0 && alignment_matrix[current_i][current_j] == alignment_matrix[current_i - 1][current_j] + gap_penalty) {
            // Décalage dans seq1 (délétion dans seq1)
            (*aligned_seq1)[align_seq1_index++] = seq1[current_i - 1];
            (*aligned_seq2)[align_seq2_index++] = '-';
            current_i--;
        } else if (current_j > 0 && alignment_matrix[current_i][current_j] == alignment_matrix[current_i][current_j - 1] + gap_penalty) {
            // Décalage dans seq2 (insertion dans seq2)
            (*aligned_seq1)[align_seq1_index++] = '-';
            (*aligned_seq2)[align_seq2_index++] = seq2[current_j - 1];
            current_j--;
        } else {
            // Correspond à la situation où l'un des indices current_i ou current_j atteint 0
            if (current_i == 0) {
                // Si current_i est 0, cela signifie qu'il n'y a plus de caractères dans seq1 mais qu'il reste des caractères dans seq2
                // Par conséquent, nous devons insérer un gap dans seq1 et aligner le caractère correspondant de seq2
                (*aligned_seq1)[align_seq1_index++] = '-';
                (*aligned_seq2)[align_seq2_index++] = seq2[current_j - 1];
                current_j--;
            } else {
                // Si current_j est 0, cela signifie qu'il n'y a plus de caractères dans seq2 mais qu'il reste des caractères dans seq1
                // Par conséquent, nous devons insérer un gap dans seq2 et aligner le caractère correspondant de seq1
                (*aligned_seq1)[align_seq1_index++] = seq1[current_i - 1];
                (*aligned_seq2)[align_seq2_index++] = '-';
                current_i--;
            }
        }
    }

    // Terminaison des chaînes alignées
    (*aligned_seq1)[align_seq1_index] = '\0';
    (*aligned_seq2)[align_seq2_index] = '\0';

    // Inversion des séquences alignées
    for (int i = 0; i < align_seq1_index / 2; ++i) {
        char temp = (*aligned_seq1)[i];
        (*aligned_seq1)[i] = (*aligned_seq1)[align_seq1_index - i - 1];
        (*aligned_seq1)[align_seq1_index - i - 1] = temp;
    }
    for (int i = 0; i < align_seq2_index / 2; ++i) {
        char temp = (*aligned_seq2)[i];
        (*aligned_seq2)[i] = (*aligned_seq2)[align_seq2_index - i - 1];
        (*aligned_seq2)[align_seq2_index - i - 1] = temp;
    }

    // Libération de la mémoire allouée à la matrice d'alignement
    for (int i = 0; i <= len_seq1; ++i) {
        free(alignment_matrix[i]);
    }
    free(alignment_matrix);

    // Retour du score de similarité
    return max_similarity_score;
}

/*
 * print_alignment - Cette fonction imprime deux séquences alignées à l'écran.
 * Arguments :
 *   - aligned_seq1 : une chaîne de caractères représentant la première séquence alignée.
 *   - aligned_seq2 : une chaîne de caractères représentant la deuxième séquence alignée.
 * Sortie :
 *   - La fonction ne retourne rien.
 */
void print_alignment(char* aligned_seq1, char* aligned_seq2) {
    // Imprimer la première séquence alignée
    printf("Sequence 1: %s\n", aligned_seq1);
    
    // Imprimer la deuxième séquence alignée
    printf("Sequence 2: %s\n", aligned_seq2);
}

/*--------------------------------------------------------------------------------------------
        Programme principal :
---------------------------------------------------------------------------------------------*/ 

int main(int argc, char *argv[]) {
    if (argc != 3) { // Vérification de la présence des fichiers
        printf("Erreur : Veuillez saisir les deux fichiers FASTA\n");
        return -1;
    }
    
    char *input_file1 = argv[1];
    char *input_file2 = argv[2];
    
    // Saisie de la pénalité de gap au clavier
    int gap_penalty;
    printf("Veuillez saisir la pénalité pour un gap : ");
    scanf("%d", &gap_penalty);

    // Saisie du nom du fichier de la matrice Blosum au clavier
    char matrix_filename[100];
    printf("Veuillez saisir le nom du fichier de la matrice Blosum : ");
    scanf("%s", matrix_filename);

    // Lecture des séquences à partir des fichiers FASTA
    char* seq1 = read_sequence(argv[1]);
    char* seq2 = read_sequence(argv[2]);

    // Lecture de la matrice de substitution à partir du fichier
    int** substitution_matrix = read_substitution_matrix(matrix_filename);

    if (seq1 == NULL || seq2 == NULL || substitution_matrix == NULL) {
        printf("Erreur : Impossible de lire les séquences ou la matrice de substitution.\n");
        return -1; // Quitter le programme avec un code d'erreur
    }

    // Variables pour stocker les séquences alignées
    char* aligned_seq1;
    char* aligned_seq2;

    // Calcul de l'alignement optimal et affichage des séquences alignées
    int similarity_score = calculate_alignment(seq1, seq2, substitution_matrix, gap_penalty, &aligned_seq1, &aligned_seq2);
    if (similarity_score == -1) {
        printf("Erreur : Impossible de calculer l'alignement optimal.\n");
        return -1; // Quitter le programme avec un code d'erreur
    }

    // Affichage des séquences alignées
    printf("Nous obtenons les séquences alignées suivantes :\n");
    print_alignment(aligned_seq1, aligned_seq2);

    // Affichage du score de similarité
    printf("Le score de similarité est de %d.\n", similarity_score);

    // Libération de la mémoire allouée
    free(seq1);
    free(seq2);

    free(aligned_seq1);
    free(aligned_seq2);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        free(substitution_matrix[i]);
    }
    free(substitution_matrix);

    return 0; // Fin du programme
}
