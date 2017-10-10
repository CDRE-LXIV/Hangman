#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <numeric>
#include <algorithm>

// numerical key values from keyboard
#define ENTER 13
#define ONE 49
#define TWO 50
#define THREE 51

using namespace std;

struct account // structure for accounts vector
{
    string usrn;
    string pass;
    int won;
    int played;
};

void ClearScreen(){ // cleans the screen
    if (system("CLS")) system("clear");
}

bool CompareUser(const account &a, const account &b){ // compare function used to sort vector of accounts
    if (a.played == 0){ // prevents divide by zero
        return 0;
    } else if (b.played == 0){
        return 1;
    }
    if (a.won == b.won){
        if (((double)a.won)/a.played == ((double)b.won)/b.played){ // determines percentage of games won
            return a.usrn < b.usrn;
        } else {
            return ((double)a.won)/a.played < ((double)b.won)/b.played;
        }
    } else {
        return a.won < b.won;
    }
}

void Leaderboard(vector <account> accounts) // shows the leaderboard
{
    vector <account> accs = accounts;
    sort(accs.begin(), accs.end(), CompareUser);
    for (int i=0;i<accs.capacity();i++){
        if (accs[i].played > 0){
            cout << "\n=============================================\n" << endl;
            cout << "Player - " << accs[i].usrn << endl;
            cout << "Number of games won - " << accs[i].won << endl;
            cout << "Number of games played - " << accs[i].played << endl;
            cout << "\n=============================================\n" << endl;
        }
    }
    system("pause");
}

void PlayHangman(const vector <string> & words, string user)
{
    cout << words[0] << endl;// testing chosen words
    cout << words[1] << endl;

    const int word1_len = words[0].length();// Length of words
    const int word2_len = words[1].length();

    bool obj[word1_len] = {false};// boolean array indicating whether a letter was guessed
    bool type[word2_len] = {false};

    string letters = "";
    char c;
    bool endgame = false;
    //int num_guesses = 20;
    int num_guesses = min((word1_len + word2_len + 10),26);
    while (!endgame){
        ClearScreen();
        cout << "\nGuessed Letters " << letters << "\n" << endl;
        cout << "Number of guesses left: " << num_guesses << "\n" << endl;
        cout << "Word: ";
        for (int i=0;i<2;i++){
            for (int j=0;j<(int)words[i].length();j++){
                if (!i){
                    if (!obj[j]){// iterating and deciding whether to show a letter or underscore
                        cout << "_ ";
                    } else {
                        cout << words[i][j] << " ";
                    }
                } else {
                    if (!type[j]){// same thing for second word
                        cout << "_ ";
                    } else {
                        cout << words[i][j] << " ";
                    }
                }
            }
            cout << "  ";
        }
        cout << "\n" << endl;
        cout << "Enter your guess - ";
        //scanf(" %c", &c);
        if((int)accumulate(obj, obj + word1_len, 0)==word1_len && (int)accumulate(type, type + word2_len, 0)==word2_len){// check if boolean array is all true
            endgame = true;
        } else if (num_guesses <= 0){
            endgame = true;
        }
        if (num_guesses > 0 && !endgame){
            c = getch();
            num_guesses -= 1;
        }
        letters += c;
        for (int i=0;i<2;i++){
            for (int j=0;j<(int)words[i].length();j++){
                if (c == words[i][j]){
                    if (!i){// setting boolean array depending if letter was guessed
                        obj[j] = true;
                    } else {
                        type[j] = true;
                    }
                }
            }
        }
    }
    cout << "\nGame over\n" << endl;
    if(num_guesses > 0){
        cout << "Well done " << user << " You won this round of Hangman!" << endl;
    } else {
        cout << "Bad luck " << user << " You have run out of guesses. The Hangman got you!" << endl;
    }
    system("pause");
}

vector <vector <string> > GetWords(string filename) // pulls words from txt file
{
    vector <vector <string> > word_list;
    ifstream ifile(filename);
    while (ifile)
    {
        string word;
        if (!getline( ifile, word )) break;

        istringstream ss( word );
        vector <string> record;

        while (ss)
        {
            string word;
            if (!getline( ss, word, ',' )) break;
            record.push_back( word );// finds comma then inserts word to the list
        }

        word_list.push_back( record );
    }
    if (!ifile.eof())
    {
        cerr << "Error\n";
    }
    cout << word_list[4][0] << word_list[4][1] << endl; // testing word io
    return word_list;
}

//vector <vector <string> > GetAccs(string filename) // pulls usernames and passwords from txt file
//{
//    ifstream auth_file(filename);
//    vector <vector <string> > accs;
//
//    int counter = 0;
//    while (!auth_file.eof())
//    {
//        vector<string> acc;
//        string field;
//        for (int i=0;i<2;i++){
//            auth_file >> field;
//            acc.push_back(field);
//        }
//        accs.push_back(acc);
//        cout << accs[counter][0] << accs[counter][1] << endl; // testing accounts
//        counter++;
//    }
//    return accs;
//}

//! ^^^ OLD FUNCTION ^^^

// creating a vector of structs but not a linked list because this is easier
// the struct is declared near the top

vector <account> GetAccs(string filename) // pulls usernames and passwords from txt file
{
    ifstream auth_file(filename);
    auth_file.ignore(numeric_limits<streamsize>::max(),'\n'); // skips the first line
    //vector <vector <string> > accs;
    vector <account> accs;

    int counter = 0;
    while (!auth_file.eof())
    {
        //vector<string> acc;
        string usrn, pass;
        //for (int i=0;i<2;i++){
            auth_file >> usrn;
            auth_file >> pass;
            accs.push_back({usrn, pass, 0, 0});
        //}
        //accs.push_back(acc);
        cout << accs[counter].usrn << accs[counter].pass << endl; // testing account creds
        cout << accs[counter].won << accs[counter].played << endl; // testing account stats
        counter++;
    }
    return accs;
}

int main()
{
    vector <vector <string> > words = GetWords("hangman_text.txt"); // unused for now
    //vector <vector <string> > accs = GetAccs("Authentication.txt");
    vector <account> accs = GetAccs("Authentication.txt");

    int key;
    string usrn, pass;
    cout << "=============================================\n" << endl;
    cout << "Welcome to the Online Hangman Gaming System\n" << endl;
    cout << "=============================================\n\n" << endl;
    cout << "You are required to logon with your registered Username and Password\n" << endl;
    cout << "Please enter your username-->";
    cin >> usrn;
    cout << "Please enter your password-->";
    while (key!=ENTER){
        key = getch();
        cout << "*";
        pass += key;// append letter to password string
    }
    cout << endl;
    cout << "password is: " << pass << endl; // testing hidden password

    ClearScreen();
    while (true){
        ClearScreen();
        int n;
        cout << "\nWelcome to the Hangman Gaming System\n\n" << endl;
        cout << "Please enter a selection" << endl;
        cout << "<1> Play Hangman" << endl;
        cout << "<2> Show Leaderboard" << endl;
        cout << "<3> Quit\n" << endl;
        cout << "Select option 1-3 ->";
        //cin >> n;
        n = getch();
        switch (n)
        {
        case ONE:// key value from getch()
            cout << "option was 1" << endl;// testing to see if this option was chosen
            PlayHangman(words[4], usrn); // using some chosen words and whatever username typed
            break;
        case TWO:
            cout << "option was 2" << endl;
            accs[4].played = 10;
            accs[4].won = 8;
            accs[5].played = 10;
            accs[5].won = 8;
            accs[8].played = 100;
            accs[8].won = 8;
            accs[2].played = 20;
            accs[2].won = 17;
            Leaderboard(accs);
            break;
        case THREE:
            cout << "option was 3" << endl;
            exit(EXIT_SUCCESS);
            break;
        default:
            cout << "Invalid option" << endl;
        }
    }
    return 0;
}
