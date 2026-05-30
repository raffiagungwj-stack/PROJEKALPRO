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
    "As","2","3","4","5","6","7",
    "8","9","10","Jack","Queen","King"
};

string suits[4] =
{
    "Hati",
    "Wajik",
    "Keriting",
    "Sekop"
};

int values[13] =
{
    11,2,3,4,5,6,7,8,9,10,10,10,10
};

void menuUtama();
void mulaiGame();
void tampilLeaderboard();
void cariPemain();
void tampilHistory();
void bantuanGame();
void tentangProgram();

void loginAdmin();
void menuAdmin();

void lihatSemuaData();
void hapusPemain();
void resetUangPemain();
void hapusHistory();

void loadPlayers();
void savePlayers();
void saveHistory(string text);

bool searchPlayer(string name, int &index);
void createPlayer(Player &p, string name);

Card randomCard();

void addCard(Player &p);
void calculateScore(Player &p);

void showCards(Player p, bool hideFirst);
void showCardCalculation(Player p);

void dealerTurn(Player &dealer);

void tambahMoney(int *money, int amount);
void kurangiMoney(int *money, int amount);

void sortLeaderboard();

int main()
{
    srand(time(0));

    loadPlayers();

    menuUtama();

    return 0;
}

void menuUtama()
{
    int choice;

    do
    {
        system("cls");

        cout << "========================================" << endl;
        cout << "            BLACKJACK MAX 21            " << endl;
        cout << "========================================" << endl;
       
        cout << endl;

        cout << "[1] Mulai Game" << endl;
        cout << "[2] Peringkat Pemain" << endl;
        cout << "[3] Cari Pemain" << endl;
        cout << "[4] Riwayat Permainan" << endl;
        cout << "[5] Cara Bermain" << endl;
        cout << "[6] Tentang Program" << endl;
        cout << "[7] Login Admin" << endl;
        cout << "[8] Keluar" << endl;

        cout << endl;

        cout << "Pilih Menu : ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                mulaiGame();
                break;

            case 2:
                tampilLeaderboard();
                break;

            case 3:
                cariPemain();
                break;

            case 4:
                tampilHistory();
                break;

            case 5:
                bantuanGame();
                break;

            case 6:
                tentangProgram();
                break;

            case 7:
                loginAdmin();
                break;

            case 8:
                cout << "Terima Kasih Sudah Bermain!" << endl;
                break;

            default:
                cout << "Menu Tidak Valid!" << endl;
                system("pause");
        }

    } while(choice != 8);
}

void mulaiGame()
{
    system("cls");

    string playerName;

    cout << "========================================" << endl;
    cout << "               GAME BARU" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "Masukkan Nama Pemain : ";
    cin >> playerName;

    Player player;

    int index;

    if(searchPlayer(playerName, index))
    {
        player = players[index];

        cout << endl;

        cout << "Selamat Datang Kembali "
             << player.name
             << "!" << endl;

        cout << "Uang   : $" << player.money << endl;
        cout << "Menang : " << player.win << endl;
        cout << "Kalah  : " << player.lose << endl;
    }
    else
    {
        createPlayer(player, playerName);

        players[totalPlayers] = player;
        totalPlayers++;

        savePlayers();

        cout << endl;
        cout << "Pemain Baru Berhasil Dibuat!" << endl;
        cout << "Uang Awal : $1000" << endl;
    }

    char mainLagi;

    do
    {
        if(player.money <= 0)
        {
            cout << endl;
            cout << "Uang Anda Habis! Tidak Bisa Melanjutkan." << endl;
            system("pause");
            return;
        }

        int bet;

        cout << endl;

        cout << "========================================" << endl;
        cout << "             PASANG TARUHAN" << endl;
        cout << "========================================" << endl;

        cout << endl;

        cout << "Uang : $" << player.money << endl;
        cout << "Masukkan Taruhan : $";
        cin >> bet;

        if(bet <= 0 || bet > player.money)
        {
            cout << endl;
            cout << "Taruhan Tidak Valid!" << endl;
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

            cout << "========================================" << endl;
            cout << "               BLACKJACK" << endl;
            cout << "========================================" << endl;

            cout << endl;

            cout << "Pemain  : " << player.name << endl;
            cout << "Uang    : $" << player.money << endl;
            cout << "Taruhan : $" << bet << endl;

            cout << endl;

            cout << "----------------------------------------" << endl;

            cout << endl;

            int visibleDealerScore = dealer.hand[1].value;

            cout << "Dealer "
                 << "[Skor Terlihat : "
                 << visibleDealerScore
                 << "]"
                 << endl;

            cout << endl;

            showCards(dealer, true);

            cout << endl;

            cout << "----------------------------------------" << endl;

            cout << endl;

            cout << player.name
                 << " ["
                 << player.score
                 << "]"
                 << endl;

            cout << endl;

            showCards(player, false);

            cout << endl;

            showCardCalculation(player);

            cout << endl;

            if(player.score < 17)
            {
                cout << "Status : Aman" << endl;
            }
            else if(player.score < 21)
            {
                cout << "Status : Berbahaya" << endl;
            }
            else if(player.score == 21)
            {
                cout << "BLACKJACK!" << endl;
            }
            else
            {
                cout << "MELEBIHI 21!" << endl;
            }

            if(player.score > 21)
            {
                break;
            }

            cout << endl;

            cout << "[1] Ambil Kartu" << endl;
            cout << "[2] Berhenti" << endl;

            cout << endl;

            cout << "Pilih : ";
            cin >> choice;

            if(choice == 1)
            {
                addCard(player);
            }

        } while(choice != 2);

        system("cls");

        cout << "========================================" << endl;
        cout << "             HASIL AKHIR" << endl;
        cout << "========================================" << endl;

        cout << endl;

        if(player.score <= 21)
        {
            dealerTurn(dealer);
        }

        cout << "Dealer"
             << " ["
             << dealer.score
             << "]"
             << endl;

        cout << endl;

        showCards(dealer, false);

        cout << endl;

        showCardCalculation(dealer);

        cout << endl;

        cout << "----------------------------------------" << endl;

        cout << endl;

        cout << player.name
             << " ["
             << player.score
             << "]"
             << endl;

        cout << endl;

        showCards(player, false);

        cout << endl;

        showCardCalculation(player);

        cout << endl;

        string history;

        if(player.score > 21)
        {
            cout << "ANDA KALAH!" << endl;

            kurangiMoney(&player.money, bet);

            player.lose++;

            history = player.name + " | KALAH | MELEBIHI 21";
        }
        else if(dealer.score > 21)
        {
            cout << "Dealer MELEBIHI 21!" << endl;
            cout << "ANDA MENANG!" << endl;

            tambahMoney(&player.money, bet);

            player.win++;

            history = player.name + " | MENANG | DEALER BUST";
        }
        else if(player.score > dealer.score)
        {
            cout << "ANDA MENANG!" << endl;

            tambahMoney(&player.money, bet);

            player.win++;

            history = player.name + " | MENANG";
        }
        else if(player.score < dealer.score)
        {
            cout << "ANDA KALAH!" << endl;

            kurangiMoney(&player.money, bet);

            player.lose++;

            history = player.name + " | KALAH";
        }
        else
        {
            cout << "SERI!" << endl;

            history = player.name + " | SERI";
        }

        cout << endl;

        cout << "Uang Sekarang : $"
             << player.money
             << endl;

        if(searchPlayer(player.name, index))
        {
            players[index] = player;
        }

        savePlayers();
        saveHistory(history);

        cout << endl;
        cout << "========================================" << endl;
        cout << "Ingin Main Lagi? (y/n) : ";
        cin >> mainLagi;

    } while(mainLagi == 'y' || mainLagi == 'Y');

    cout << endl;
    cout << "Terima kasih sudah bermain, " << player.name << "!" << endl;
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

        if(p.hand[i].rank == "As")
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
            cout << "[Kartu Tertutup] ";
        }
        else
        {
            cout << "["
                 << p.hand[i].rank
                 << " "
                 << p.hand[i].suit
                 << "] ";
        }
    }

    cout << endl;
}

void showCardCalculation(Player p)
{
    for(int i = 0; i < p.totalCard; i++)
    {
        cout << p.hand[i].value;

        if(i != p.totalCard - 1)
        {
            cout << " + ";
        }
    }

    cout << " = " << p.score << endl;
}

void dealerTurn(Player &dealer)
{
    if(dealer.score < 17)
    {
        addCard(dealer);

        dealerTurn(dealer);
    }
}

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
             << players[i].lose
             << endl;
    }

    file.close();
}

void saveHistory(string text)
{
    ofstream file("history.txt", ios::app);

    file << text << endl;

    file.close();
}

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

void tampilLeaderboard()
{
    system("cls");

    sortLeaderboard();

    cout << "========================================" << endl;
    cout << "          PERINGKAT PEMAIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << left
         << setw(5)  << "No"
         << setw(15) << "Nama"
         << setw(10) << "Uang"
         << setw(10) << "Menang"
         << setw(10) << "Kalah"
         << endl;

    cout << endl;

    for(int i = 0; i < totalPlayers; i++)
    {
        cout << left
             << setw(5)  << i + 1
             << setw(15) << players[i].name
             << setw(10) << players[i].money
             << setw(10) << players[i].win
             << setw(10) << players[i].lose
             << endl;
    }

    cout << endl;

    system("pause");
}

void cariPemain()
{
    system("cls");

    string name;

    cout << "========================================" << endl;
    cout << "             CARI PEMAIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "Masukkan Nama Pemain : ";
    cin >> name;

    int index;

    cout << endl;

    if(searchPlayer(name, index))
    {
        cout << "Pemain Ditemukan!" << endl;

        cout << endl;

        cout << "Nama   : " << players[index].name << endl;
        cout << "Uang   : $" << players[index].money << endl;
        cout << "Menang : " << players[index].win << endl;
        cout << "Kalah  : " << players[index].lose << endl;
    }
    else
    {
        cout << "Pemain Tidak Ditemukan!" << endl;
    }

    cout << endl;

    system("pause");
}

void tampilHistory()
{
    system("cls");

    ifstream file("history.txt");

    string text;

    cout << "========================================" << endl;
    cout << "          RIWAYAT PERMAINAN" << endl;
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

void bantuanGame()
{
    system("cls");

    cout << "========================================" << endl;
    cout << "             CARA BERMAIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "- Tujuan game mendekati angka 21" << endl;
    cout << "- Jika lebih dari 21 maka kalah" << endl;
    cout << "- Dealer otomatis mengambil kartu jika <17" << endl;
    cout << "- Ambil Kartu untuk menambah kartu" << endl;
    cout << "- Berhenti untuk stop kartu" << endl;

    cout << endl;

    cout << "Nilai Kartu :" << endl;
    cout << "As = 1 atau 11" << endl;
    cout << "Jack/Queen/King = 10" << endl;

    cout << endl;

    system("pause");
}

void tentangProgram()
{
    system("cls");

    cout << "========================================" << endl;
    cout << "            TENTANG PROGRAM" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "BLACKJACK 21" << endl;
    cout << "Project Algoritma dan Pemrograman" << endl;

    cout << endl;

    cout << "Fitur :" << endl;
    cout << "- Struct" << endl;
    cout << "- Rekursif" << endl;
    cout << "- Searching" << endl;
    cout << "- Sorting" << endl;
    cout << "- Pointer" << endl;
    cout << "- File I/O" << endl;

    cout << endl;

    cout << "Dibuat Oleh :" << endl;
    cout << "Muhammad Zenitho Mawla Pasha Munif" << endl;

    cout << endl;

    system("pause");
}

void loginAdmin()
{
    system("cls");

    string username;
    string password;

    cout << "========================================" << endl;
    cout << "              LOGIN ADMIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    if(username == "adminjudol" && password == "blackjack123")
    {
        cout << endl;
        cout << "Login Berhasil!" << endl;

        system("pause");

        menuAdmin();
    }
    else
    {
        cout << endl;
        cout << "Username atau Password Salah!" << endl;

        system("pause");
    }
}

void menuAdmin()
{
    int choice;

    do
    {
        system("cls");

        cout << "========================================" << endl;
        cout << "               MENU ADMIN" << endl;
        cout << "========================================" << endl;

        cout << endl;

        cout << "[1] Lihat Semua Data Pemain" << endl;
        cout << "[2] Hapus Data Pemain" << endl;
        cout << "[3] Reset Uang Pemain" << endl;
        cout << "[4] Hapus Riwayat Permainan" << endl;
        cout << "[5] Kembali" << endl;

        cout << endl;

        cout << "Pilih : ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                lihatSemuaData();
                break;

            case 2:
                hapusPemain();
                break;

            case 3:
                resetUangPemain();
                break;

            case 4:
                hapusHistory();
                break;

            case 5:
                break;

            default:
                cout << "Menu Tidak Valid!" << endl;
                system("pause");
        }

    } while(choice != 5);
}

void lihatSemuaData()
{
    system("cls");

    cout << "========================================" << endl;
    cout << "           DATA SELURUH PEMAIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << left
         << setw(5)  << "No"
         << setw(15) << "Nama"
         << setw(10) << "Uang"
         << setw(10) << "Menang"
         << setw(10) << "Kalah"
         << endl;

    cout << endl;

    for(int i = 0; i < totalPlayers; i++)
    {
        cout << left
             << setw(5)  << i + 1
             << setw(15) << players[i].name
             << setw(10) << players[i].money
             << setw(10) << players[i].win
             << setw(10) << players[i].lose
             << endl;
    }

    cout << endl;

    system("pause");
}

void hapusPemain()
{
    system("cls");

    string name;

    cout << "========================================" << endl;
    cout << "             HAPUS PEMAIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "Masukkan Nama Pemain : ";
    cin >> name;

    int index;

    if(searchPlayer(name, index))
    {
        for(int i = index; i < totalPlayers - 1; i++)
        {
            players[i] = players[i + 1];
        }

        totalPlayers--;

        savePlayers();

        cout << endl;
        cout << "Data Pemain Berhasil Dihapus!" << endl;
    }
    else
    {
        cout << endl;
        cout << "Pemain Tidak Ditemukan!" << endl;
    }

    cout << endl;

    system("pause");
}

void resetUangPemain()
{
    system("cls");

    string name;

    cout << "========================================" << endl;
    cout << "           RESET UANG PEMAIN" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "Masukkan Nama Pemain : ";
    cin >> name;

    int index;

    if(searchPlayer(name, index))
    {
        players[index].money = 1000;

        savePlayers();

        cout << endl;
        cout << "Uang Pemain Berhasil Direset!" << endl;
    }
    else
    {
        cout << endl;
        cout << "Pemain Tidak Ditemukan!" << endl;
    }

    cout << endl;

    system("pause");
}

void hapusHistory()
{
    system("cls");

    ofstream file("history.txt");

    file.close();

    cout << "========================================" << endl;
    cout << "         HAPUS RIWAYAT GAME" << endl;
    cout << "========================================" << endl;

    cout << endl;

    cout << "Riwayat Berhasil Dihapus!" << endl;

    cout << endl;

    system("pause");
}