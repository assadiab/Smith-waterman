# Projet Smith-Waterman

Ce projet consiste à implémenter un programme en langage C pour aligner deux séquences d’acides aminés en utilisant la technique Smith-Waterman. L'algorithme de Smith-Waterman est un algorithme de programmation dynamique utilisé pour l'alignement local de séquences. Il fonctionne en comparant toutes les sous-séquences possibles de deux séquences données et en trouvant la sous-séquence ayant le meilleur score de similarité.

## Fonctionnalités

Le programme fonctionne en  plusieurs étapes :

1. `mesure_len_seq`: Lecture de la longueur de la séquence à partir d'un fichier FASTA.
2. `read_sequence`: Lecture de la séquence à partir d'un fichier FASTA.
3. `read_substitution_matrix`: Lecture de la matrice de substitution à partir d'un fichier texte.
4. `initialize_matrix `: Création d'une matrice avec une valeur initiale donnée.
5. `compute_score`: Calcul du score de similarité entre deux caractères en utilisant la matrice de substitution.
6. `max`: Calcul du  maximum entre deux entiers.
7. `create_alignment_matrix`: Création d'une matrice contenant les scores d'alignements entre deux séquences
8. `calculate_alignment`: Calcul de l'alignement optimal entre deux séquences.
9. `print_alignment`: Affichage des séquences alignées.

## Utilisation et résultats

Pour utiliser ce programme, exécutez la commande suivante dans votre terminal :

```bash
gcc Smith_Waterman_DIABIRA.c -o Smith_Waterman_DIABIRA
./Smith_Waterman_DIABIRA fichier1.fasta fichier2.fasta
```

Assurez-vous d'avoir les fichiers FASTA contenant les séquences à aligner ainsi qu'un fichier de matrice de substitution pour exécuter le programme. (disponible dans les dossiers Blosum_matrix et FASTA_sequences)

Vous serez invité à saisir la pénalité pour un gap ainsi que le nom du fichier de la matrice de substitution lors de l'exécution du programme.
```bash
Entrez la pénalité pour un gap : -2
Entrez le nom du fichier de la matrice de substitution : blosum50.txt
```

## Exemple 1 : 

Nous utilisons les  fichiers FASTA `sequence1.fasta` et `sequence2.fasta` contenant vos séquences à aligner. Pour exécuter le programme, vous pouvez faire comme suit :

```bash
gcc Smith_Waterman_DIABIRA.c -o Smith_Waterman_DIABIRA 
./Smith_Waterman_DIABIRA sequence1.fasta sequence2.fasta
Veuillez saisir la pénalité pour un gap : -5
Veuillez saisir le nom du fichier de la matrice Blosum : blosum50.txt
```
Vous obtiendrez : 
```bash
La séquence de protéine du fichier sequence1.fasta est : GRAT (4 acides aminés).
La séquence de protéine du fichier sequence2.fasta est : PRIST (5 acides aminés).

Nous utilisons la matrice blosum50.txt.

La matrice 5 x 6 a bien été initialisée avec une valeur initiale de 0.

La matrice des scores de dimensions 5 x 6 remplie est la suivante :
0 -5 -10 -15 -20
-5 -2 -7 -11 -16
-10 -7 5 0 -5
-15 -12 0 4 -1
-20 -15 -5 1 6
-25 -20 -10 -4 6

Nous obtenons les séquences alignées suivantes :
Sequence 1: GR-AT
Sequence 2: PRIST
Le score de similarité est de 6.
```

## Exemple 2 : 

Nous utilisons les  fichiers FASTA `sequence2_1.fasta` et `sequence2_2.fasta` contenant vos séquences à aligner. Pour exécuter le programme, vous pouvez faire comme suit :

```bash
gcc Smith_Waterman_DIABIRA.c -o Smith_Waterman_DIABIRA
./Smith_Waterman_DIABIRA sequence2_1.fasta sequence2_2.fasta
Veuillez saisir la pénalité pour un gap : -8
Veuillez saisir le nom du fichier de la matrice Blosum : blosum50.txt
```
Vous obtiendrez : 
```bash
La séquence de protéine du fichier sequence2_1.fasta est : HEAGAWGHEE (10 acides aminés).
La séquence de protéine du fichier sequence2_2.fasta est : PGAWHEAE (8 acides aminés).

Nous utilisons la matrice blosum50.txt.

La matrice 11 x 9 a bien été initialisée avec une valeur initiale de 0.

La matrice des scores de dimensions 11 x 9 remplie est la suivante :
0 -8 -16 -24 -32 -40 -48 -56 -64 -72 -80
-8 -2 -9 -17 -25 -33 -41 -49 -57 -65 -73
-16 -10 -5 -9 -9 -17 -25 -33 -41 -49 -57
-24 -18 -11 0 -8 -4 -12 -20 -28 -36 -44
-32 -26 -19 -8 -3 -11 11 3 -5 -13 -21
-40 -22 -26 -16 -10 -5 3 9 13 5 -3
-48 -30 -16 -24 -18 -11 -5 1 9 19 11
-56 -38 -24 -11 -19 -13 -13 -5 1 11 18
-64 -46 -32 -19 -14 -20 -16 -13 -5 7 17

Nous obtenons les séquences alignées suivantes :
Sequence 1: HEAGAWGHE-E
Sequence 2: --PGAW-HEAE
Le score de similarité est de 17.
```
## Références
- Article de Smith et Waterman de 1981 : [Identification of Common Molecular Subsequences](https://www.ncbi.nlm.nih.gov/pubmed/7265238)
- [Wikipedia - Smith-Waterman algorithm](https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm)
