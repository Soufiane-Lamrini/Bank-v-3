#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";



// Structure du client
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

// Structure du user
struct sUser
{
    string Name;
    string PinCode;
    int Permession = 0;
    bool MarkForDelete = false;
};

enum enTransactionOption
{
    enDeposer = 1, enRetirer = 2,
    enAfficherSolde = 3, enRetour = 4
};

enum enManageUserMenu {
    eListUser = 1, eAddUser = 2, eDeleteUser = 3, eUpdate = 4,
    eFindUser = 5, eRetour = 6
};

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7, eExit = 8
};

// Enum pour les permissions
enum enPermession {
    All = -1, ListClient = 1, AddClient = 2, DeleteClient = 4,
    UpdateClient = 8, FindClient = 16, Transaction = 32, ManageUser = 64
};

// Variable globale pour l'utilisateur connecté
sUser CurrentUser;


// Déclarations de fonctions
void ShowMainMenue();
void ShowTransactionsScreen();
void ShowManageUserScreen();
void GoBackToMainMenue();
void GoBackToManageUserMenue();
void GoBackToTransactionMenue();
bool CheckAccessPermission(enPermession Permission);
bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User);
bool  LoadUserInfo(string Username, string Password);
void Login();
// Fonction pour lire un numero de compte depuis l'utilisateur
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nSaisir un nombre de compte? ";
    cin >> AccountNumber;
    return AccountNumber;
}



// Fonction pour découper une chaîne en utilisant un délimiteur
vector<string> DecouperString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

// Convertir une ligne du fichier en structure client
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData = DecouperString(Line, Seperator);

    if (vClientData.size() >= 5)
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }
    return Client;
}

// Convertir une ligne du fichier en structure user
sUser ConvertLinetoRecordUser(string Line, string Seperator = "#//#")
{
    sUser User;
    vector<string> vUserData = DecouperString(Line, Seperator);

    if (vUserData.size() >= 3)
    {
        User.Name = vUserData[0];
        User.PinCode = vUserData[1];
        User.Permession = stod(vUserData[2]);
    }
    return User;
}

// Convertir un client en ligne pour le fichier
string ConvertRecordClientToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

// Convertir un utilisateur en ligne pour le fichier
string ConvertRecordUserToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.Name + Seperator;
    stUserRecord += User.PinCode + Seperator;
    stUserRecord += to_string(User.Permession);
    return stUserRecord;
}

// Vérifier si un client existe selon son numéro de compte
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    ifstream MyFile(FileName);
    if (!MyFile.is_open()) return false;

    string Line;
    while (getline(MyFile, Line))
    {
        sClient Client = ConvertLinetoRecord(Line);
        if (Client.AccountNumber == AccountNumber)
        {
            MyFile.close();
            return true;
        }
    }
    MyFile.close();
    return false;
}

// Vérifier si un utilisateur existe selon son nom
bool UserExistsByName(string Name, string FileName)
{
    ifstream MyFile(FileName);
    if (!MyFile.is_open()) return false;

    string Line;
    while (getline(MyFile, Line))
    {
        sUser User = ConvertLinetoRecordUser(Line);
        if (User.Name == Name)
        {
            MyFile.close();
            return true;
        }
    }
    MyFile.close();
    return false;
}

// Fonction pour vérifier les permissions
bool CheckAccessPermission(enPermession Permession)
{
    if (CurrentUser.Permession == -1) // Accès complet
        return true;

    return (CurrentUser.Permession & Permession) == Permession;
}

// Fonction pour afficher message de refus
void TuNAPasAcces(){
    
    cout << "\n\nAcces refuse, Vous n'avez pas la permission pour cette operation.\n";
    system("pause>0");
    return;
}
// Fonctions de navigation
void GoBackToMainMenue()
{
    cout << "\n\nAppuyer sur une touche pour retourner au Menu Principal...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionMenue()
{
    cout << "\n\nAppuyer sur une touche pour retourner au Menu Transaction...";
    system("pause>0");
    ShowTransactionsScreen();
}

void GoBackToManageUserMenue()
{
    cout << "\n\nAppuyer sur une touche pour retourner au Menu Gere Utilisateur...";
    system("pause>0");
    ShowManageUserScreen();
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    ifstream MyFile(FileName);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            sClient Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<sUser> LoadUsersDataFromFile(string FileName)
{
    vector<sUser> vUser;
    ifstream MyFile(FileName);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            sUser User = ConvertLinetoRecordUser(Line);
            vUser.push_back(User);
        }
        MyFile.close();
    }
    return vUser;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(15) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintUSerRecordLine(sUser User)
{
    cout << "| " << setw(40) << left << User.Name;
    cout << "| " << setw(10) << left << User.PinCode;
    cout << "| " << setw(12) << left << User.Permession;
}

void PrintClientCard(sClient Client)
{
    cout << "\nVoici les details du client:\n";
    cout << "-----------------------------------";
    cout << "\nNumero de compte: " << Client.AccountNumber;
    cout << "\nCode Pin        : " << Client.PinCode;
    cout << "\nNom             : " << Client.Name;
    cout << "\nTelephone       : " << Client.Phone;
    cout << "\nSolde du compte : " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintUserCard(sUser User)
{
    cout << "\nVoici les informations de l'utilisateur:\n";
    cout << "-----------------------------------";
    cout << "\nNom           : " << User.Name;
    cout << "\nPassword      : " << User.PinCode;
    cout << "\nPermissions   : " << User.Permession;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindUserByUserName(string UserName, vector<sUser> vUser, sUser& User)
{
    for (sUser U : vUser)
    {
        if (U.Name == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}

int LirePermession()
{
    int Permession = 0;
    char reponse = 'o';
    cout << "Voulez-vous donner a cet utilisateur l'acces a tout le programme? o/n" << endl;
    cin >> reponse;
    if (reponse == 'o' || reponse == 'O')
    {
        return -1;
    }
    else
    {
        cout << "Voulez-vous donner a cet utilisateur l'acces a afficher la liste des clients? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::ListClient;
        }
        cout << "Voulez-vous donner a cet utilisateur l'acces a ajouter des clients? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::AddClient;
        }
        cout << "Voulez-vous donner a cet utilisateur l'acces a supprimer des clients? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::DeleteClient;
        }
        cout << "Voulez-vous donner a cet utilisateur l'acces a mettre a jour les informations des clients? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::UpdateClient;
        }
        cout << "Voulez-vous donner a cet utilisateur l'acces a chercher des clients? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::FindClient;
        }
        cout << "Voulez-vous donner a cet utilisateur l'acces au Menu Transaction? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::Transaction;
        }
        cout << "Voulez-vous donner a cet utilisateur l'acces au Menu Gerer Utilisateur? o/n" << endl;
        cin >> reponse;
        if (reponse == 'o' || reponse == 'O')
        {
            Permession = Permession | enPermession::ManageUser;
        }
        return Permession;
    }
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Entrer le numero de compte? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient avec [" << Client.AccountNumber << "] existe deja, Entrer un autre numero de compte? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Entrer le code pin? ";
    getline(cin, Client.PinCode);

    cout << "Entrer le nom? ";
    getline(cin, Client.Name);

    cout << "Entrer le numero de telephone? ";
    getline(cin, Client.Phone);

    cout << "Entrer le solde? ";
    cin >> Client.AccountBalance;

    return Client;
}

sUser ReadNewUser()
{
    sUser User;
    cout << "Entrer le nom de l'utilisateur? ";
    getline(cin >> ws, User.Name);

    while (UserExistsByName(User.Name, UsersFileName))
    {
        cout << "\nUtilisateur avec [" << User.Name << "] existe deja, Entrer un nouveau nom? ";
        getline(cin >> ws, User.Name);
    }

    cout << "Entrer le code pin? ";
    getline(cin, User.PinCode);

    User.Permession = LirePermession();

    return User;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEntrer le nouveau code pin: ";
    getline(cin >> ws, Client.PinCode);

    cout << "Entrer le nouveau nom? ";
    getline(cin, Client.Name);

    cout << "Entrer le nouveau numero de telephone? ";
    getline(cin, Client.Phone);

    cout << "Entrer le nouveau solde? ";
    cin >> Client.AccountBalance;
    return Client;
}

sUser ChangeUserRecord(string UserName)
{
    sUser User;
    User.Name = UserName;

    cout << "\n\nEntrer le nouveau code pin: ";
    getline(cin >> ws, User.PinCode);

    User.Permession = LirePermession();
    return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool MarkUserForDeleteByName(string UserName, vector<sUser>& vUser)
{
    for (sUser& U : vUser)
    {
        if (U.Name == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    ofstream MyFile(FileName);
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                MyFile << ConvertRecordClientToLine(C) << endl;
            }
        }
        MyFile.close();
    }
}

void SaveUsersDataToFile(string FileName, vector<sUser> vUser)
{
    ofstream MyFile(FileName);
    if (MyFile.is_open())
    {
        for (sUser U : vUser)
        {
            if (U.MarkForDelete == false)
            {
                MyFile << ConvertRecordUserToLine(U) << endl;
            }
        }
        MyFile.close();
    }
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    ofstream MyFile(FileName, ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enPermession::ListClient))
    {
        TuNAPasAcces();
        return;

    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tListe des Clients (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Numero compte";
    cout << "| " << left << setw(10) << "Code pin";
    cout << "| " << left << setw(40) << "Nom du client";
    cout << "| " << left << setw(15) << "Numero de tel";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tIl n'y a pas de clients disponibles dans le systeme";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowAllUsersScreen()
{
    vector<sUser> vUser = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tListe des Utilisateurs (" << vUser.size() << ") Utilisateur(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(40) << "Nom utilisateur";
    cout << "| " << left << setw(10) << "Code pin";
    cout << "| " << left << setw(12) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUser.size() == 0)
        cout << "\t\t\t\tIl n'y a pas d'utilisateurs disponibles dans le systeme";
    else
        for (sUser User : vUser)
        {
            PrintUSerRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void AddNewClient()
{
    sClient Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordClientToLine(Client));
}

void AddNewClients()
{


    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Ajouter nouveau client:\n\n";
        AddNewClient();
        cout << "\nClient ajoute avec succes, voulez-vous ajouter un autre? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void AddNewUser()
{
    sUser User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordUserToLine(User));
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Ajouter nouvel utilisateur:\n\n";
        AddNewUser();
        cout << "\nUtilisateur ajoute avec succes, voulez-vous ajouter un autre? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nEtes-vous sur de vouloir supprimer ce client? o/n ? ";
        cin >> Answer;
        if (Answer == 'o' || Answer == 'O')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient supprime avec succes.";
            return true;
        }
    }
    else
    {
        cout << "\nClient avec ce numero (" << AccountNumber << ") est introuvable!";
        return false;
    }
    return false;
}

bool DeleteUserByName(string UserName, vector<sUser>& vUser)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUser, User))
    {
        PrintUserCard(User);
        cout << "\n\nEtes-vous sur de vouloir supprimer cet utilisateur? o/n ? ";
        cin >> Answer;
        if (Answer == 'o' || Answer == 'O')
        {
            MarkUserForDeleteByName(UserName, vUser);
            SaveUsersDataToFile(UsersFileName, vUser);
            cout << "\n\nUtilisateur supprime avec succes.";
            return true;
        }
    }
    else
    {
        cout << "\nUtilisateur avec ce nom (" << UserName << ") est introuvable!";
        return false;
    }
    return false;
}

bool UpdateClientBalance(string AccountNumber, double Amount, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.AccountBalance += Amount;
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nOperation effectuee avec succes. Le nouveau solde est: " << C.AccountBalance;
            return true;
        }
    }
    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nConfirmer la mise a jour de ce client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient mis a jour avec succes.";
            return true;
        }
    }
    else
    {
        cout << "\nClient avec ce numero (" << AccountNumber << ") est introuvable!";
        return false;
    }
    return false;
}

bool UpdateUserByName(string UserName, vector<sUser>& vUser)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUser, User))
    {
        PrintUserCard(User);
        cout << "\n\nConfirmer la mise a jour de cet utilisateur? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& C : vUser)
            {
                if (C.Name == UserName)
                {
                    C = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUser);
            cout << "\n\nUtilisateur mis a jour avec succes.";
            return true;
        }
    }
    else
    {
        cout << "\nUtilisateur avec ce nom (" << UserName << ") est introuvable!";
        return false;
    }
    return false;
}

void ShowDeposerScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tEcran de Depot";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Le client avec ce numero " << AccountNumber << " n'existe pas!" << endl;
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double Montant = 0;
    cout << "Saisir le montant a deposer: ";
    cin >> Montant;

    char Confirm;
    cout << "\nConfirmer l'operation? (o/n): ";
    cin >> Confirm;
    if (Confirm == 'o' || Confirm == 'O')
    {
        if (UpdateClientBalance(AccountNumber, Montant, vClients))
        {
            cout << "\nDepot effectue avec succes.";
        }
    }
}

void ShowRetirerScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tEcran de Retrait";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Le client avec ce numero " << AccountNumber << " n'existe pas!" << endl;
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double Montant = 0;
    cout << "Saisir le montant a retirer: ";
    cin >> Montant;

    while (Montant > Client.AccountBalance)
    {
        cout << "Solde insuffisant, veuillez saisir un montant valide: ";
        cin >> Montant;
    }

    char Confirm;
    cout << "\nConfirmer l'operation? (o/n): ";
    cin >> Confirm;
    if (Confirm == 'o' || Confirm == 'O')
    {
        if (UpdateClientBalance(AccountNumber, -Montant, vClients))
        {
            cout << "\nRetrait effectue avec succes.";
        }
    }
}

void PrintClientRecordLineBalances(sClient Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(12) << Client.AccountBalance;
}

void ShowTotalSoldeScreen()
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tListe des Soldes (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Numero compte";
    cout << "| " << left << setw(40) << "Nom du client";
    cout << "| " << left << setw(12) << "Solde";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalSoldes = 0;
    if (vClients.size() == 0)
        cout << "\t\t\t\tAucun client dans le systeme!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordLineBalances(Client);
            TotalSoldes += Client.AccountBalance;
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total des Soldes = " << TotalSoldes << endl;
}

void ShowAddClientScreen()
{
    if (!CheckAccessPermission(enPermession::AddClient))
    {
        TuNAPasAcces();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAjouter Client\n";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowAddUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAjouter Utilisateur\n";
    cout << "\n-----------------------------------\n";

    AddNewUsers();
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enPermession::DeleteClient))
    {
        TuNAPasAcces();
        return;

    }

    cout << "\n-----------------------------------\n";
    cout << "\tSuppression Client\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tSuppression Utilisateur\n";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUser = LoadUsersDataFromFile(UsersFileName);
    string Name;
    cout << "Saisir le nom?" << endl;
    cin >> Name;
    DeleteUserByName(Name, vUser);
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enPermession::UpdateClient))
    {
        TuNAPasAcces();
        return;

    }

    cout << "\n-----------------------------------\n";
    cout << "\tMise a jour Client\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tMise a jour Utilisateur\n";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUser = LoadUsersDataFromFile(UsersFileName);
    string Name;
    cout << "Saisir le nom?" << endl;
    cin >> Name;
    UpdateUserByName(Name, vUser);
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enPermession::FindClient))
    {
        TuNAPasAcces();
        return;

    }

    cout << "\n-----------------------------------\n";
    cout << "\tRecherche Client\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient avec le numero [" << AccountNumber << "] introuvable!";
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tRecherche Utilisateur\n";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUser = LoadUsersDataFromFile(UsersFileName);
    sUser User;
    string Name;
    cout << "Saisir le nom?" << endl;
    cin >> Name;
    if (FindUserByUserName(Name, vUser, User))
        PrintUserCard(User);
    else
        cout << "\nUtilisateur avec ce nom [" << Name << "] est introuvable!";
}

short ReadMainTransactionMenueOption()
{
    cout << "Choisir ce que vous voulez [1 a 4]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

short ReadMainManageUserMenueOption()
{
    cout << "Choisir ce que vous voulez [1 a 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

short ReadMainMenueOption()
{
    cout << "Choisir ce que vous voulez [1 a 8]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromMainTransactionMenueOption(enTransactionOption MainTransactionMenueOption)
{
    switch (MainTransactionMenueOption)
    {
    case enTransactionOption::enDeposer:
        system("cls");
        ShowDeposerScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionOption::enRetirer:
        system("cls");
        ShowRetirerScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionOption::enAfficherSolde:
        system("cls");
        ShowTotalSoldeScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionOption::enRetour:
        system("cls");
        ShowMainMenue();
        break;
    }
}

void PerfromMainManageUserMenueOption(enManageUserMenu MainManageUserOption)
{
    switch (MainManageUserOption)
    {
    case enManageUserMenu::eListUser:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUserMenue();
        break;

    case enManageUserMenu::eAddUser:
        system("cls");
        ShowAddUserScreen();
        GoBackToManageUserMenue();
        break;

    case enManageUserMenu::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUserMenu::eUpdate:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUserMenu::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUserMenu::eRetour:
        system("cls");
        ShowMainMenue();
        break;
    }
}

void ShowTransactionsScreen()
{
    if (!CheckAccessPermission(enPermession::Transaction))
    {
        TuNAPasAcces();
        return;

    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMenu Transaction\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposer.\n";
    cout << "\t[2] Retirer.\n";
    cout << "\t[3] Afficher tous les soldes.\n";
    cout << "\t[4] Retour au Menu principal\n";
    cout << "===========================================\n";

    PerfromMainTransactionMenueOption((enTransactionOption)ReadMainTransactionMenueOption());
}

void ShowManageUserScreen()
{
    if (!CheckAccessPermission(enPermession::ManageUser))
    {
        TuNAPasAcces();
        return;

    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMenu Gerer Utilisateur\n";
    cout << "===========================================\n";
    cout << "\t[1] Lister Utilisateurs.\n";
    cout << "\t[2] Ajouter Utilisateur.\n";
    cout << "\t[3] Supprimer Utilisateur.\n";
    cout << "\t[4] Mise a jour Utilisateur\n";
    cout << "\t[5] Trouver Utilisateur.\n";
    cout << "\t[6] Retour au Menu principal.\n";
    cout << "===========================================\n";

    PerfromMainManageUserMenueOption((enManageUserMenu)ReadMainManageUserMenueOption());
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTransactionsScreen();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUserScreen();
        break;
    case enMainMenueOptions::eExit:
        system("cls");
        Login();
        exit(0);
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMenu Principal\n";
    cout << "===========================================\n";
    cout << "\t[1] Afficher la liste des clients\n";
    cout << "\t[2] Ajouter un nouveau client\n";
    cout << "\t[3] Supprimer un client\n";
    cout << "\t[4] Mettre a jour un client\n";
    cout << "\t[5] Rechercher un client\n";
    cout << "\t[6] Transactions\n";
    cout << "\t[7] Gerer Utilisateur\n";
    cout << "\t[8] Quitter\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User)
{

    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser U : vUsers)
    {

        if (U.Name == Username && U.PinCode == Password)
        {
            User = U;
            return true;
        }

    }
    return false;

}

bool  LoadUserInfo(string Username, string Password)
{

    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;

}


void Login()
{
    bool LoginFaild = false;

    string Username, Password;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenue();

}
int main()
{
    Login();
    return 0;
}