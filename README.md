🏦 Système de Gestion Bancaire en C++
📋 Description
Un système complet de gestion bancaire développé en C++ avec interface console. Le programme offre une gestion complète des clients, des transactions bancaires et des utilisateurs avec un système de permissions avancé.

✨ Fonctionnalités
🔐 Authentification et Sécurité
Système de login avec vérification des identifiants

Gestion des permissions par utilisateur

Accès contrôlé aux différentes fonctionnalités

👥 Gestion des Clients
✅ Afficher la liste complète des clients

➕ Ajouter de nouveaux clients

🗑️ Supprimer des clients avec confirmation

✏️ Modifier les informations clients

🔍 Rechercher un client par numéro de compte

💰 Transactions Bancaires
📥 Dépôt d'argent sur les comptes

📤 Retrait d'argent avec vérification du solde

📊 Affichage de tous les soldes et total général

👨‍💼 Gestion des Utilisateurs
👥 Lister tous les utilisateurs

➕ Ajouter de nouveaux utilisateurs

🗑️ Supprimer des utilisateurs

✏️ Modifier les informations utilisateurs

🔍 Rechercher des utilisateurs

🛠️ Structure Technique
📁 Fichiers de Données
Clients.txt - Base de données des clients

Users.txt - Base de données des utilisateurs

🏗️ Structures de Données
cpp
struct sClient {
    string AccountNumber;    // Numéro de compte
    string PinCode;         // Code PIN
    string Name;            // Nom complet
    string Phone;           // Téléphone
    double AccountBalance;  // Solde du compte
    bool MarkForDelete;     // Marqueur suppression
};

struct sUser {
    string Name;           // Nom d'utilisateur
    string PinCode;        // Mot de passe
    int Permession;        // Permissions
    bool MarkForDelete;    // Marqueur suppression
};
🔑 Système de Permissions
Permission	Valeur	Description
All	-1	Accès complet
ListClient	1	Lister les clients
AddClient	2	Ajouter des clients
DeleteClient	4	Supprimer des clients
UpdateClient	8	Modifier les clients
FindClient	16	Rechercher des clients
Transaction	32	Accès aux transactions
ManageUser	64	Gérer les utilisateurs
🚀 Installation et Exécution
Prérequis
Compilateur C++ (g++, clang++, etc.)

Système d'exploitation : Windows, Linux ou macOS

Compilation
bash
g++ -std=c++11 main.cpp -o gestion_bancaire
Exécution
bash
./gestion_bancaire
📊 Format des Fichiers
Format Client
text
NumeroCompte#//#CodePin#//#NomClient#//#NumeroTele#//#Solde
Exemple :

text
A100#//#1234#//#Ahmed Karim#//#0654321987#//#1500.75
B200#//#7777#//#Fatima Zahra#//#0612345678#//#980.00
Format Utilisateur
text
NomUtilisateur#//#MotDePasse#//#Permissions
Exemple :

text
admin#//#admin123#//#-1
caissier#//#pass123#//#33
🎯 Menu Principal
text
===========================================
        Menu Principal
===========================================
    [1] Afficher la liste des clients
    [2] Ajouter un nouveau client
    [3] Supprimer un client
    [4] Mettre a jour un client
    [5] Rechercher un client
    [6] Transactions
    [7] Gerer Utilisateur
    [8] Quitter
===========================================
🔧 Fonctionnalités Avancées
🛡️ Validation des Données
Vérification de l'unicité des numéros de compte

Contrôle des soldes pour les retraits

Confirmation des opérations critiques

💾 Persistance des Données
Sauvegarde automatique après chaque modification

Chargement des données au démarrage

Gestion robuste des fichiers

👥 Gestion des Permissions
Attribution granularisée des droits

Vérification des permissions avant chaque opération

Interface de configuration intuitive

🎓 Objectifs Pédagogiques
Ce projet permet de maîtriser :

Programmation orientée objet en C++

Gestion des fichiers et persistance des données

Structures de données complexes

Système d'authentification et de permissions

Interface utilisateur en mode console

Gestion d'erreurs et validation

Architecture logicielle modulaire

🔮 Améliorations Futures
Interface graphique avec Qt

Sauvegarde automatique et historique

Journalisation des transactions

Export des données en PDF/Excel

API REST pour intégration web

Chiffrement des données sensibles

📝 Notes de Développement
Le code suit les bonnes pratiques de programmation :

Séparation des préoccupations

Fonctions modulaires et réutilisables

Gestion propre de la mémoire

Interface utilisateur intuitive

Code commenté et maintenable
