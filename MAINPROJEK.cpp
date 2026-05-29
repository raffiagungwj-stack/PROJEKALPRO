#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int MAX_PLAYER = 100;
const int MAX_CARD = 10;

struct Card
{
    string rank;
    string suit;
    int value;
};

struct Player
{
    string name;
    int money;
    int win;
    int lose;

    Card hand[MAX_CARD];
    int totalCard;
    int score;
};

Player players[MAX_PLAYER];
int totalPlayers = 0;

string ranks[13] =
{
    "A","2","3","4","5","6","7",
    "8","9","10","J","Q","K"
};

string suits[4] =
{
    "♥","♦","♣","♠"
};

int values[13] =
{
    11,2,3,4,5,6,7,8,9,10,10,10,10
};

void mainMenu();
void startGame();
void showLeaderboard();
void searchPlayerMenu();
void showHistory();
void helpMenu();
void aboutProgram();

void loadPlayers();
void savePlayers();
void saveHistory(string text);

bool searchPlayer(string name, int &index);
void createPlayer(Player &p, string name);

Card randomCard();
void addCard(Player &p);
void calculateScore(Player &p);
void showCards(Player p, bool hideFirst);

void dealerTurn(Player &dealer);

void tambahMoney(int *money, int amount);
void kurangiMoney(int *money, int amount);

void sortLeaderboard();

int main()
{
    srand(time(0));

    loadPlayers();

    mainMenu();

    return 0;
}

void mainMenu()
{
    int choice;

    do
    {
        system("cls");

        cout << "========================================" << endl;
        cout << "           BLACKJACK 21" << endl;
        cout << "========================================" << endl;
        cout << "       PROJECT ALPRO C++" << endl;
        cout << "========================================" << endl;
        cout << endl;

        cout << "[1] Main Game" << endl;
        cout << "[2] Leaderboard" << endl;
        cout << "[3] Search Player" << endl;
        cout << "[4] Game History" << endl;
        cout << "[5] Help / Rules" << endl;
        cout << "[6] About Program" << endl;
        cout << "[7] Exit" << endl;

        cout << endl;
        cout << "Choose Menu : ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                startGame();
                break;

            case 2:
                showLeaderboard();
                break;

            case 3:
                searchPlayerMenu();
                break;

            case 4:
                showHistory();
                break;

            case 5:
                helpMenu();
                break;

            case 6:
                aboutProgram();
                break;

            case 7:
                cout << "Thank You For Playing!" << endl;
                break;

            default:
                cout << "Invalid Menu!" << endl;
                system("pause");
        }

    } while(choice != 7);
}

void startGame()
{
    system("cls");

    string playerName;

    cout << "================================" << endl;
    cout << "           NEW GAME" << endl;
    cout << "================================" << endl;
    cout << endl;

    cout << "Input Player Name : ";
    cin >> playerName;

    Player player;

    int index;

    if(searchPlayer(playerName, index))
    {
        player = players[index];

        cout << endl;
        cout << "Welcome Back " << player.name << "!" << endl;
        cout << "Money : $" << player.money << endl;
        cout << "Win   : " << player.win << endl;
        cout << "Lose  : " << player.lose << endl;
    }
    else
    {
        createPlayer(player, playerName);

        players[totalPlayers] = player;
        totalPlayers++;

        savePlayers();

        cout << endl;
        cout << "New Player Created!" << endl;
        cout << "Starting Money : $1000" << endl;
    }

    if(player.money <= 0)
    {
        cout << endl;
        cout << "Your money is empty!" << endl;
        system("pause");
        return;
    }

    int bet;

    cout << endl;
    cout << "================================" << endl;
    cout << "          PLACE BET" << endl;
    cout << "================================" << endl;

    cout << "Money : $" << player.money << endl;
    cout << "Input Bet : $";
    cin >> bet;

    if(bet <= 0 || bet > player.money)
    {
        cout << "Invalid Bet!" << endl;
        system("pause");
        return;
    }

    Player dealer;

    dealer.name = "Dealer";
    dealer.totalCard = 0;
    dealer.score = 0;

    player.totalCard = 0;
    player.score = 0;

    addCard(player);
    addCard(player);

    addCard(dealer);
    addCard(dealer);

    int choice;

    do
    {
        system("cls");

        cout << "==================================" << endl;
        cout << "            BLACKJACK" << endl;
        cout << "==================================" << endl;
        cout << endl;

        cout << "Player : " << player.name << endl;
        cout << "Money  : $" << player.money << endl;
        cout << "Bet    : $" << bet << endl;

        cout << endl;

        cout << "----------------------------------" << endl;
        cout << endl;

        cout << "Dealer Cards" << endl;
        showCards(dealer, true);
        cout << "Dealer Total : ?" << endl;

        cout << endl;

        cout << "----------------------------------" << endl;
        cout << endl;

        cout << "Your Cards" << endl;
        showCards(player, false);

        cout << "Your Total : " << player.score << endl;

        if(player.score < 17)
        {
            cout << "Status : Aman" << endl;
        }
        else if(player.score < 21)
        {
            cout << "Status : Risky" << endl;
        }
        else if(player.score == 21)
        {
            cout << "BLACKJACK!" << endl;
        }
        else
        {
            cout << "BUST!" << endl;
        }

        if(player.score > 21)
        {
            break;
        }

        cout << endl;

        cout << "[1] Hit" << endl;
        cout << "[2] Stand" << endl;

        cout << endl;
        cout << "Choose : ";
        cin >> choice;

        if(choice == 1)
        {
            addCard(player);
        }

    } while(choice != 2);

    system("cls");

    cout << "==================================" << endl;
    cout << "           FINAL RESULT" << endl;
    cout << "==================================" << endl;
    cout << endl;

    if(player.score <= 21)
    {
        dealerTurn(dealer);
    }

    cout << "Dealer Cards" << endl;
    showCards(dealer, false);
    cout << "Dealer Total : " << dealer.score << endl;

    cout << endl;

    cout << "Your Cards" << endl;
    showCards(player, false);
    cout << "Your Total : " << player.score << endl;

    cout << endl;

    string history;

    if(player.score > 21)
    {
        cout << "YOU BUST!" << endl;
        cout << "YOU LOSE!" << endl;

        kurangiMoney(&player.money, bet);

        player.lose++;

        history = player.name + " | LOSE | BUST";
    }
    else if(dealer.score > 21)
    {
        cout << "DEALER BUST!" << endl;
        cout << "YOU WIN!" << endl;

        tambahMoney(&player.money, bet);

        player.win++;

        history = player.name + " | WIN | DEALER BUST";
    }
    else if(player.score > dealer.score)
    {
        cout << "YOU WIN!" << endl;

        tambahMoney(&player.money, bet);

        player.win++;

        history = player.name + " | WIN | SCORE";
    }
    else if(player.score < dealer.score)
    {
        cout << "YOU LOSE!" << endl;

        kurangiMoney(&player.money, bet);

        player.lose++;

        history = player.name + " | LOSE | SCORE";
    }
    else
    {
        cout << "DRAW!" << endl;

        history = player.name + " | DRAW";
    }

    cout << endl;
    cout << "Current Money : $" << player.money << endl;

    if(searchPlayer(player.name, index))
    {
        players[index] = player;
    }

    savePlayers();
    saveHistory(history);

    cout << endl;

    system("pause");
}

void createPlayer(Player &p, string name)
{
    p.name = name;
    p.money = 1000;
    p.win = 0;
    p.lose = 0;
}

bool searchPlayer(string name, int &index)
{
    for(int i = 0; i < totalPlayers; i++)
    {
        if(players[i].name == name)
        {
            index = i;
            return true;
        }
    }

    return false;
}

Card randomCard()
{
    Card c;

    int r = rand() % 13;
    int s = rand() % 4;

    c.rank = ranks[r];
    c.suit = suits[s];
    c.value = values[r];

    return c;
}

void addCard(Player &p)
{
    p.hand[p.totalCard] = randomCard();

    p.totalCard++;

    calculateScore(p);
}

void calculateScore(Player &p)
{
    p.score = 0;

    int aceCount = 0;

    for(int i = 0; i < p.totalCard; i++)
    {
        p.score += p.hand[i].value;

        if(p.hand[i].rank == "A")
        {
            aceCount++;
        }
    }

    while(p.score > 21 && aceCount > 0)
    {
        p.score -= 10;
        aceCount--;
    }
}

void showCards(Player p, bool hideFirst)
{
    for(int i = 0; i < p.totalCard; i++)
    {
        if(i == 0 && hideFirst)
        {
            cout << "[?] ";
        }
        else
        {
            cout << "[" 
                 << p.hand[i].rank
                 << p.hand[i].suit
                 << "] ";
        }
    }

    cout << endl;
}

// RECURSIVE FUNCTION
void dealerTurn(Player &dealer)
{
    if(dealer.score < 17)
    {
        addCard(dealer);

        dealerTurn(dealer);
    }
}

// POINTER FUNCTION
void tambahMoney(int *money, int amount)
{
    *money += amount;
}

void kurangiMoney(int *money, int amount)
{
    *money -= amount;
}

void loadPlayers()
{
    ifstream file("players.txt");

    totalPlayers = 0;

    while(file >> players[totalPlayers].name
               >> players[totalPlayers].money
               >> players[totalPlayers].win
               >> players[totalPlayers].lose)
    {
        totalPlayers++;
    }

    file.close();
}

void savePlayers()
{
    ofstream file("players.txt");

    for(int i = 0; i < totalPlayers; i++)
    {
        file << players[i].name << " "
             << players[i].money << " "
             << players[i].win << " "
             << players[i].lose << endl;
    }

    file.close();
}

void saveHistory(string text)
{
    ofstream file("history.txt", ios::app);

    file << text << endl;

    file.close();
}

// SORTING
void sortLeaderboard()
{
    for(int i = 0; i < totalPlayers - 1; i++)
    {
        for(int j = i + 1; j < totalPlayers; j++)
        {
            if(players[i].money < players[j].money)
            {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
}

void showLeaderboard()
{
    system("cls");

    sortLeaderboard();

    cout << "========================================" << endl;
    cout << "             LEADERBOARD" << endl;
    cout << "========================================" << endl;
    cout << endl;

    cout << left
         << setw(5) << "No"
         << setw(15) << "Name"
         << setw(10) << "Money"
         << setw(10) << "Win"
         << setw(10) << "Lose"
         << endl;

    for(int i = 0; i < totalPlayers; i++)
    {
        cout << left
             << setw(5) << i + 1
             << setw(15) << players[i].name
             << setw(10) << players[i].money
             << setw(10) << players[i].win
             << setw(10) << players[i].lose
             << endl;
    }

    cout << endl;

    system("pause");
}

void searchPlayerMenu()
{
    system("cls");

    string name;

    cout << "========================================" << endl;
    cout << "             SEARCH PLAYER" << endl;
    cout << "========================================" << endl;
    cout << endl;

    cout << "Input Player Name : ";
    cin >> name;

    int index;

    cout << endl;

    if(searchPlayer(name, index))
    {
        cout << "Player Found!" << endl;
        cout << endl;

        cout << "Name  : " << players[index].name << endl;
        cout << "Money : $" << players[index].money << endl;
        cout << "Win   : " << players[index].win << endl;
        cout << "Lose  : " << players[index].lose << endl;
    }
    else
    {
        cout << "Player Not Found!" << endl;
    }

    cout << endl;

    system("pause");
}

void showHistory()
{
    system("cls");

    ifstream file("history.txt");

    string text;

    cout << "========================================" << endl;
    cout << "             GAME HISTORY" << endl;
    cout << "========================================" << endl;
    cout << endl;

    while(getline(file, text))
    {
        cout << text << endl;
    }

    file.close();

    cout << endl;

    system("pause");
}

void helpMenu()
{
    system("cls");

    cout << "========================================" << endl;
    cout << "               HELP MENU" << endl;
    cout << "========================================" << endl;
    cout << endl;

    cout << "- Tujuan game mendekati angka 21" << endl;
    cout << "- Jika lebih dari 21 maka kalah" << endl;
    cout << "- Dealer otomatis hit jika <17" << endl;
    cout << "- Pilih HIT untuk tambah kartu" << endl;
    cout << "- Pilih STAND untuk berhenti" << endl;

    cout << endl;

    cout << "Card Value :" << endl;
    cout << "A = 1 atau 11" << endl;
    cout << "J/Q/K = 10" << endl;

    cout << endl;

    system("pause");
}

void aboutProgram()
{
    system("cls");

    cout << "========================================" << endl;
    cout << "             ABOUT PROGRAM" << endl;
    cout << "========================================" << endl;
    cout << endl;

    cout << "BLACKJACK 21" << endl;
    cout << "Project Algoritma dan Pemrograman" << endl;

    cout << endl;

    cout << "Features :" << endl;
    cout << "- Struct" << endl;
    cout << "- Rekursif" << endl;
    cout << "- Searching" << endl;
    cout << "- Sorting" << endl;
    cout << "- Pointer" << endl;
    cout << "- File I/O" << endl;

    cout << endl;

    cout << "Created By :" << endl;
    cout << "Muhammad Zenitho Mawla Pasha Munif" << endl;

    cout << endl;

    system("pause");
}