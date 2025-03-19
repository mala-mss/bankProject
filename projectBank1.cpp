#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
enum choice
{
    ShowClientsList = 1,
    NewClient = 2,
    DeleteClient = 3,
    updateClient = 4,
    FindClient = 5,
    Transection = 7,
    Exit = 8,
    clearAll = 6
};

struct client
{
    string NumACC;
    string PinCode;
    string ClientName;
    string Phone;
    string Balance;
};
struct Node
{
    client Client;
    Node *next;
};
void ClearFile(string fileName)
{
    fstream file;
    file.open(fileName, ios::out);
    file.close();
}
client fillClient()
{

    client client;
    cout << "Account Number\t ";
    cin >> client.NumACC;
    cout << "Pin Code \t";
    cin >> client.PinCode;
    cout << "Client Name\t";
    //   cin.ignore();
    getline(cin >> ws, client.ClientName);
    cout << "Phone Number\t";
    cin >> client.Phone;
    cout << "Balance \t";
    cin >> client.Balance;

    return client;
}
typedef Node *list;
list creatList(client x)
{
    list p = new Node();
    p->Client = x;
    p->next = NULL;
    return p;
}

void addLastList(list &l, client x)
{
    if (l == NULL)
    {
        l = creatList(x);
    }
    else
    {
        list p = l;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = creatList(x);
    }
}

vector<string> splitString(string str, string Delim)
{
    int ps = 0;
    string sword;
    vector<string> result;
    if (str.length() > 1e6)
    { // Example limit of 1 million characters
        throw std::length_error("String is too large to process.");
    }

    while ((ps = str.find(Delim)) != string::npos)
    { // ida kayen delim m3naha mazal kayen klmat mch m9somin basically

        result.push_back(str.substr(0, ps));
        str.erase(0, ps + Delim.length());
        if (str.empty())
            break;
    }
    // to add the last word;
    if (!str.empty())
        result.push_back(str.substr(0, ps));

    return result;
}
client ConvertDataLineToRecord(string line)
{
    client c;

    vector<string> Vrecord = splitString(line, "//#//");

    if (Vrecord.size() != 5)
    {
        throw std::runtime_error("Malformed data line: " + line);
    }

    c.NumACC = Vrecord[0];

    c.PinCode = (Vrecord[1]);
    c.ClientName = Vrecord[2];
    c.Phone = Vrecord[3];
    c.Balance = (Vrecord[4]);

    return c;
}
string ConvertRecordToLine(client C, string delim = "//#//")
{
    string record = "";
    record += C.NumACC + delim;
    record += (C.PinCode) + delim;
    record += C.ClientName + delim;
    record += C.Phone + delim;
    record += (C.Balance) + delim;
    return record;
}

void showClientsList(string FileName)
{
    fstream clientFile;
    client Client;
    clientFile.open(FileName, ios::in);
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    cout << "                                    **  Clients List  **\n";
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "------------------------------------------------------------------------------------------------\n";

    cout << setw(20) << left << "Account Number"
         << setw(10) << left << "Pin Code"
         << setw(20) << left << "Client Name"
         << setw(15) << left << "Phone Number"
         << setw(10) << left << "Balance" << endl;

    if (clientFile.is_open())
    {
        string line;
        while (getline(clientFile, line))
        {
            client c = ConvertDataLineToRecord(line);
            cout << setw(20) << left << c.NumACC
                 << setw(10) << left << c.PinCode
                 << setw(20) << left << c.ClientName
                 << setw(15) << left << c.Phone
                 << setw(10) << left << c.Balance << endl;
        }

        cout << "\n------------------------------------------------------------------------------------------------\n";

        clientFile.close();
    }
}

void addNewClient(string FileName, client Client)
{
    fstream ClientsFile;
    ClientsFile.open(FileName, ios::out | ios::app);

    if (ClientsFile.is_open())
    {

        ClientsFile << Client.NumACC << "//#//" << Client.PinCode << "//#//" << Client.ClientName << "//#//"
                    << Client.Phone << "//#//" << Client.Balance << endl;

        ClientsFile.close();
    }
}

void fromListToFile(string &fileName, list clients)
{
    ofstream ClientsFile(fileName, ios::out | ios::app);
    if (!ClientsFile.is_open())
    {
        cout << "Error opening file!\n";
        return;
    }

    list p = clients;
    while (p != NULL)
    {
        ClientsFile << p->Client.NumACC << "//#//"
                    << p->Client.PinCode << "//#//"
                    << p->Client.ClientName << "//#//"
                    << p->Client.Phone << "//#//"
                    << p->Client.Balance << endl;
        p = p->next;
    }

    ClientsFile.close();
}

void fromFileToList(string FileName, list &clients)
{
    fstream clientFile;
    clientFile.open(FileName, ios::in);
    if (clientFile.is_open())
    {

        string line;
        while (getline(clientFile, line))
        {

            client c = ConvertDataLineToRecord(line);
            addLastList(clients, c);
        }

        clientFile.close();
    }
}

void deleteFromList(list &l, string targetAccNum)
{
    list current = l, prev = NULL;
    while (current != NULL)
    {
        if (current->Client.NumACC == targetAccNum)
        {
            if (prev == NULL)
            {
                l = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

bool searchInFile(string FileName, string targetAccNum)
{
    fstream clientFile;
    clientFile.open(FileName, ios::in);

    if (clientFile.is_open())
    {

        string line = "";
        while (getline(clientFile, line))
        {
            client c = ConvertDataLineToRecord(line);
            if (c.NumACC == targetAccNum)
            {
                clientFile.close();
                return true;
            }
        }
    }

    clientFile.close();
    return false;
}
client FindInFile(string fileName, string targetAccNum)
{
    ifstream clientsFile;
    clientsFile.open(fileName, ios::in);
    if (!clientsFile.is_open())
    {
        cout << "Error opening file!\n";
    }

    string line;
    while (getline(clientsFile, line))
    {
        client c = ConvertDataLineToRecord(line);
        if (c.NumACC == targetAccNum)
        {
            clientsFile.close();
            return c;
        }
    }

    clientsFile.close();
}

void deleteClient(string fileName, list clients, string targetAccNum)
{

    fromFileToList(fileName, clients);
    deleteFromList(clients, targetAccNum);
    ClearFile(fileName);
    fromListToFile(fileName, clients);
}
void PrintClientCard(client Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.NumACC;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.ClientName;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.Balance;
    cout << "\n-----------------------------------\n";
}
void UpdateList(list l, string targetAccNum)
{
    list current = l;
    client Client;
    while (current != NULL)
    {
        if (current->Client.NumACC == targetAccNum)
        {
            Client = fillClient();
            current->Client = Client;
            cout << "Client After Update:\n";
            PrintClientCard(Client);
        }
        current = current->next;
    }
}
void UpdateClient(string fileName, list clients, string targetAccNum)
{

    fromFileToList(fileName, clients);
    UpdateList(clients, targetAccNum);
    ClearFile(fileName);
    fromListToFile(fileName, clients);
}
// µµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµ

// deposit section

void deposit(string FileName, string NumACC, int Blnc)
{
    list clients=NULL;
   fstream clientFile;
   client Clt;
  
   clientFile.open(FileName, ios::in);

   if (clientFile.is_open())
   {
       string line = "";
       
       while (getline(clientFile, line))
       {

           Clt = ConvertDataLineToRecord(line);

           if (Clt.NumACC == NumACC)
           {

               int balance = stoi(Clt.Balance);
               balance += Blnc;
               Clt.Balance = to_string(balance);
               deleteClient(FileName, clients, NumACC);
               addNewClient(FileName, Clt);
           }
       }

       clientFile.close();
   }
}
// With Draw
void WithDraw(string FileName, string NumACC, int Blnc)

{
     list clients=NULL;
    fstream clientFile;
    client Clt;
   
    clientFile.open(FileName, ios::in);

    if (clientFile.is_open())
    {
        string line = "";
        
        while (getline(clientFile, line))
        {

            Clt = ConvertDataLineToRecord(line);

            if (Clt.NumACC == NumACC)
            {

                int balance = stoi(Clt.Balance);
                balance -= Blnc;
                Clt.Balance = to_string(balance);
                deleteClient(FileName, clients, NumACC);
                addNewClient(FileName, Clt);
            }
        }

        clientFile.close();
    }
}


// ToTal Balance
int ToTalBalance(string FileName){
    int s=0;
    client clt;
    string Line="";
    fstream ClientsFile;
    ClientsFile.open(FileName, ios::in);
    if(ClientsFile.is_open()){
        
       
        while(getline(ClientsFile,Line)){
            clt= ConvertDataLineToRecord(Line);
            s+=stoi(clt.Balance);
        }
        ClientsFile.close();

    }
    else{ 
        cout<<"file didnt open ";
    }
    return s;
}
void showClientsBalance(string FileName)
{
    fstream clientFile;
    client Client;
    clientFile.open(FileName, ios::in);
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    cout << "                                    **  Balance List  **\n";
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "------------------------------------------------------------------------------------------------\n";

    cout << setw(20) << left << "Account Number"

         << setw(20) << left << "Client Name"

         << setw(20) << left << "Balance" << endl;

    if (clientFile.is_open())
    {
        string line;
        while (getline(clientFile, line))
        {
            client c = ConvertDataLineToRecord(line);
            cout << setw(20) << left << c.NumACC

                 << setw(20) << left << c.ClientName

                 << setw(20) << left << c.Balance << endl;
        }

        cout<<"--------------------------------------------------------ToTal Balanace Is : ";
        cout<<ToTalBalance(FileName);

        cout << "\n------------------------------------------------------------------------------------------------\n";

        clientFile.close();
    }
}


void menuTransection()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransection\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit\n";
    cout << "\t[2] WithDraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Back To Menu.\n";

    cout << "===========================================\n";
}
// µµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµ
int clientChoice()
{
    int n;

    do
    {
        cout << "Enter your choice (1-8): ";
        cin >> n;

    } while (n < 1 || n > 8);
    return (choice)n;
}
void menu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] clear all.\n";
    cout << "\t[7] transection\n";
    cout << "\t[8] Exit.\n";

    cout << "===========================================\n";
}

void switchChoice(choice Choice, list Clients, string FileName)
{
    string target;
    char c = 'N';
    switch (Choice)
    {
    case choice::ShowClientsList:
        showClientsList(FileName);
        break;
    case choice::NewClient:

        do
        {
            cout << "\n***********ADDING  NEW CLIENT****************\n";
            if (!searchInFile(FileName, target))
            {
                cout << "Please Enter The Next Data:\n";
                client Client = fillClient();
                addNewClient(FileName, Client);

                cout << "Do You Want To Add More Clients??[Y\\N]\t";
                cin >> c;
            }
            else
            {
                cout << "the client you tryna add alrd exists in our systeme ^^\n";
                cout << "Do You Want To Add another Clients??[Y\\N]\t";
                cin >> c;
            }
            c = toupper(c);
        } while (c != 'N');

        break;
    case choice::DeleteClient:
        do
        {
            cout << "\n****************DELETING A CLIENT*****************\n";
            cout << "Please Enter Client Account Number:\t";
            cin >> target;
            if (searchInFile(FileName, target))
            {
                deleteClient(FileName, Clients, target);
                cout << "Do You Wanna Delete Another Account ? [Y\\N]?\t";
                cin >> c;
            }
            else
            {
                cout << "the Account Number You inserted Doesnt exist\n";
                cout << "Do You Wanna Delete Another Account ? [Y\\N]?\t";
                cin >> c;
            }
            c = toupper(c);
        } while (c != 'N');

        break;
    case choice::updateClient:
        do
        {
            cout << "\n****************UPDATING A CLIENT*****************\n";
            cout << "Please Enter Client Account Number:\t";
            cin >> target;
            if (searchInFile(FileName, target))
            {
                UpdateClient(FileName, Clients, target);

                cout << "\nDo You Wanna Update Another Account ? [Y\\N]?\t";
                cin >> c;
            }
            else
            {
                cout << "the Account Number You inserted Doesnt exist\n";
                cout << "Do You Wanna Update Another Account ? [Y\\N]?\t";
                cin >> c;
            }
        } while (c != 'N');

        break;
    case choice::FindClient:
        do
        {
            cout << "\n****************FINDING A CLIENT*****************\n";
            cout << "Please Enter Client Account Number:\t";
            cin >> target;
            if (searchInFile(FileName, target))
            {
                client Client = FindInFile(FileName, target);
                PrintClientCard(Client);
                cout << "Do You Wanna Find Another Account ? [Y\\N]?\t";
                cin >> c;
            }
            else
            {
                cout << "the Account Number You inserted Doesnt exist\n";
                cout << "Do You Wanna Find Another Account ? [Y\\N]?\t";
                cin >> c;
            }
        } while (c != 'N');

        break;

    case clearAll:
        ClearFile(FileName);
        break;
    case choice::Transection:
    
        int shoice;
        menuTransection();
        do
        {
            cout << "what is your choice ?\t ";
            cin >> shoice;
        } while (shoice < 1 || shoice > 4);

        switch (shoice)
        {
        case 1:{
            // deposit
            cout << " Whats your account Number?\t";
            string NumAcc;
            cin >> NumAcc;
            if (searchInFile(FileName, NumAcc))
            {
                cout<<"How Much Balance You wanna deposit?\t";
                int blnc;
                cin>>blnc;
                deposit(FileName,NumAcc,blnc);
            }
            else
            {
                cout<<"Sorry We dont have this client in here ";
            }
        }
            break;
        case 2: // withdraw
        {
    
        cout << " Whats your account Number?\t";
        string numAcc;
        cin >> numAcc;
        if (searchInFile(FileName, numAcc))
        {
            cout<<"How Much Balance You wanna WithDraw?\t";
            int blnc;
            cin>>blnc;
            WithDraw(FileName,numAcc,blnc);
        }
        else
        {
            cout<<"Sorry We dont have this client in here ";
        }
    }
            break;
        case 3: // Total balance
        {
           showClientsBalance(FileName);
        }
            break;
        case 4: // back to menu
        {
             menu();
             choice c=(choice)clientChoice();
            switchChoice(c, Clients, FileName);
        }
            break;
        };

        break;
    case choice::Exit:
        system("pause");
        return;
        break;
    }
}


void BankEnter()
{
    char c = 'N';
    string FileName = "ClienfFile.txt";

    do
    {
        menu();
        list ListClient = NULL;

        switchChoice((choice)clientChoice(), ListClient, FileName);
        cout << "Do you want To Enter to The Bank Again? [Y\\N]\n";

        cin >> c;
    } while (c != 'N');
    cout << "THANK YOU FOR VISITING ^^\n";
}
int main()
{
    BankEnter();
    return 0;
}
