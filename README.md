# CC1
Rouibi Rayane 3SI4

# SQL Client

Ce projet implémente un simple client SQL qui supporte les commandes suivantes :

    INSERT <id> <username> <email> : Insère une ligne dans une table.
    SELECT * : Affiche toutes les lignes de la table.
    SELECT WHERE id=<id> : Recherche et affiche une ligne spécifique par son id.
    DELETE WHERE id=<id> : Supprime une ligne de la table par son id.

Le stockage des données est réalisé à l'aide d'un arbre binaire AVL pour garantir un accès rapide et un équilibre optimal.

# Compilation

Ce projet utilise gcc comme compilateur avec les flags suivants :
Flags utilisés :

    -o : Définit le nom du fichier objet généré.
    -c : Compile uniquement le fichier source sans créer d'exécutable, générant un fichier objet.
        Utilisé pour compiler les fichiers main.c, repl.c, et btree.c séparément.

Compilateur :

    GCC version : GCC 13.3.0.

# Execution
Compilation:
make ou make all :

    Compile le projet complet et génère un exécutable sql_client.

Execution du programme :
    ./sql_client
