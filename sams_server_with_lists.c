#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

//magic numbers and constants
#define DEFAULT_PORT_NO 54321
#define ARRAY_SIZE 10
#define STRING_LENGTH 10
#define BACKLOG 10     /* how many pending connections queue will hold */
#define RETURNED_ERROR -1
#define ARGUMENTS_EXPECTED 2
#define usernameMaxLength 10
#define passwordMaxLength 10
#define wordMaxLength 15

<<<<<<< HEAD
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
=======
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

//function declarations
char * ReceiveData(int socket_identifier, int size);
int TestCharacter(char receivedChar);

//define a structure-linked list to hold word pairs
//typedef struct list wordPair;

typedef struct list {
    char firstWord[wordMaxLength];
    char secondWord[wordMaxLength];
    unsigned int wordPairNumber;
    struct list * next;
}
wordPair;

//define a structure-linked list to hold users
//typedef struct node userNode;

typedef struct node {
    char username[usernameMaxLength];
    char password[passwordMaxLength];
    int gamesWon;
    int gamesPlayed;
    struct node * next;
}
userNode;

typedef void( * callbackUser)(userNode * data, int socket_id);
typedef void( * callbackWord)(wordPair * data);

//add a userNode node
userNode * AddUserNode(userNode * next, char username[], char password[]) {
    // if the user is not "Username" and password is not "password"
    /*
    char defaultUsername[] = "Username";
    char defaultPassword[] = "Password";
    if ((strcmp(username, defaultUsername) !=0)&&(strcmp(password, defaultPassword) !=0)) {*/
    // create new userNode to add to list
    userNode * newNode = (userNode * ) malloc(sizeof(userNode));
    if (newNode == NULL) {
        return (0);
    }
    // insert new node
    strcpy(newNode-> username, username);
    strcpy(newNode-> password, password);
    newNode-> gamesWon = 0;
    newNode-> gamesPlayed = 0;
    newNode-> next = next;
    // printf("Added: Username: %s \t Password: %s\n", newNode->username, newNode->password);
    return newNode;
    //}
<<<<<<< HEAD
}

userNode * prependUser(userNode * head, char usrn[], char pass[]) {
    userNode * new_node = AddUserNode(head, usrn, pass);
    head = new_node;
    return head;
}

userNode * appendUser(userNode * head, char usrn[], char pass[]) {
    if (head == NULL)
        return NULL;
    /* go to the last node */
    userNode * cursor = head;
    while (cursor-> next != NULL)
        cursor = cursor-> next;

    /* create a new node */
    userNode * new_node = AddUserNode(NULL, usrn, pass);
    cursor-> next = new_node;

    return head;
}

userNode* searchUser(userNode* head, char usrn[]){
    userNode *cursor = head;
    while(cursor != NULL){
        if (strcmp(cursor->username, usrn)==0){
            return cursor;
        } else {
            cursor = cursor->next;
        }
    }
    return NULL;
=======
}

userNode * prependUser(userNode * head, char usrn[], char pass[]) {
    userNode * new_node = AddUserNode(head, usrn, pass);
    head = new_node;
    return head;
}

userNode * appendUser(userNode * head, char usrn[], char pass[]) {
    if (head == NULL)
        return NULL;
    /* go to the last node */
    userNode * cursor = head;
    while (cursor-> next != NULL)
        cursor = cursor-> next;

    /* create a new node */
    userNode * new_node = AddUserNode(NULL, usrn, pass);
    cursor-> next = new_node;

    return head;
}

userNode* searchUser(userNode* head, char usrn[]){
    userNode *cursor = head;
    while(cursor != NULL){
        if (strcmp(cursor->username, usrn)==0){
            return cursor;
        } else {
            cursor = cursor->next;
        }
    }
    return NULL;
>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
}

//add a wordPair node
wordPair * AddWordPair(wordPair * next, char firstWord[], char secondWord[], unsigned int wordPairCounter) {
    // create new wordPair to add to list
    wordPair * newPair = (wordPair * ) malloc(sizeof(wordPair));
    if (newPair == NULL) {
        return (0);
    }
    // insert new node
    strcpy(newPair-> firstWord, firstWord);
    strcpy(newPair-> secondWord, secondWord);
    newPair-> wordPairNumber = wordPairCounter;
    newPair-> next = next;
    // printf("Added: Word pair: %d \t First word: %s \t Second word: %s \n", newPair->wordPairNumber, newPair->firstWord, newPair->secondWord);
    return newPair;
}

wordPair * prependWord(wordPair * head, char word1[], char word2[]) {
    wordPair * new_node = AddWordPair(head, word1, word2, 0);
    head = new_node;
    return head;
}

wordPair * appendWord(wordPair * head, char word1[], char word2[], unsigned int counter) {
    if (head == NULL)
        return NULL;
    /* go to the last node */
    wordPair * cursor = head;
    while (cursor-> next != NULL)
        cursor = cursor-> next;

    /* create a new node */
    wordPair * new_node = AddWordPair(NULL, word1, word2, counter);
    cursor-> next = new_node;
<<<<<<< HEAD

    return head;
}

wordPair * searchWord(wordPair* head, int number){
    wordPair *cursor = head;
    while(cursor != NULL){
        if (cursor->wordPairNumber == number){
            return cursor;
        } else {
            cursor = cursor->next;
        }
    }
    return NULL;
}

=======

    return head;
}

wordPair * searchWord(wordPair* head, int number){
    wordPair *cursor = head;
    while(cursor != NULL){
        if (cursor->wordPairNumber == number){
            return cursor;
        } else {
            cursor = cursor->next;
        }
    }
    return NULL;
}

>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
int wordCount(wordPair* head){
    wordPair * cursor = head;
    int c = 0;
    while(cursor != NULL){
        c++;
        cursor = cursor-> next;
    }
    return c;
}

//write function to take contents of Authentication.txt to linked list
userNode * AuthenticationImport() {
    userNode * head = NULL;
    FILE * authenticationText;
    //authenticationArray holds fget string before parsing
    char authenticationArray[passwordMaxLength + usernameMaxLength];
    //parsed username from authenticationArray
    char usernameArray[usernameMaxLength];
    //parsed password from authenticationArray
    char passwordArray[passwordMaxLength];
    //initialise arrays with zeros
    for (int i = 0; i < usernameMaxLength; i++) {
        usernameArray[i] = 0;
        passwordArray[i] = 0;
        authenticationArray[i] = 0;
        authenticationArray[i + 10] = 0;
    }

    //open Authentication.txt in read mode
    authenticationText = fopen("Authentication.txt", "r");

    if (authenticationText == NULL) {
        perror("Error opening Authentication file:");
        return (-1);
    }

    //copy values in Authentication.txt to usernameArray and passwordArray, remove spaces somehow

    fgets(authenticationArray, (usernameMaxLength + passwordMaxLength), authenticationText);
    //head = prependUser(head, usernameArray, passwordArray);

    int cnt = 0;
    while (fgets(authenticationArray, (usernameMaxLength + passwordMaxLength), authenticationText)) {
        //populate usernameArray using first 10 values (or until a space or indent) of authenticationArray
        for (int i = 0; i < usernameMaxLength; i++) {
            //read the username one char at a time until hitting space or indent
            if ((authenticationArray[i] != ' ') && (authenticationArray[i] != '\t')) {
                usernameArray[i] = authenticationArray[i];
            }
            //terminate string
            else {
                usernameArray[i] = '\0';
                break;
            }
            //turn read chars on line into spaces
            authenticationArray[i] = ' ';
        }
        //populate passwordArray using final values (not spaces or indents) of authenticationArray
        int passwordCounter = 0;
        for (int i = 0; i < (usernameMaxLength + passwordMaxLength); i++) {
            if ((authenticationArray[i] != ' ') && (authenticationArray[i] != '\t')) {
                passwordArray[passwordCounter] = authenticationArray[i];
                authenticationArray[i] = ' ';
                passwordCounter++;
            }
        }

        //print extracted values for test purposes
        //printf("Username: %s \t\t Password: %s \n", usernameArray, passwordArray);

        //add this user to a a new node
        //userNode *head = NULL;
        //AddUserNode(head, usernameArray, passwordArray);
        if (!cnt){
            head = prependUser(head, usernameArray, passwordArray);
            cnt++;
        }
        else{
            head = appendUser(head, usernameArray, passwordArray);
        }
        //newPair = AddWordPair(newPair, firstWordArray, secondWordArray, wordPairCounter);
    }
    fclose(authenticationText);
    return head;
}

wordPair * WordListImport() {
    wordPair * head = NULL;
    FILE * hangmanText;
    //awordArray holds fget string before parsing
    char wordArray[2 * wordMaxLength];
    //parsed first word from wordArray
    char firstWordArray[wordMaxLength];
    ////parsed password from authenticationArray
    char secondWordArray[wordMaxLength];
    //initialise arrays with zeros
    for (int i = 0; i < wordMaxLength; i++) {
        firstWordArray[i] = 0;
        secondWordArray[i] = 0;
        wordArray[i] = 0;
        wordArray[i + wordMaxLength] = 0;
    }
<<<<<<< HEAD

    //open Authentication.txt in read mode
    hangmanText = fopen("hangman_text.txt", "r");

=======

    //open Authentication.txt in read mode
    hangmanText = fopen("hangman_text.txt", "r");

>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
    if (hangmanText == NULL) {
        perror("Error opening hangman_text file");
        return (-1);
    }

    //copy values in Authentication.txt to usernameArray and passwordArray, remove spaces somehow
    unsigned int wordPairCounter = 0;
    //fgets(wordArray, sizeof(wordArray), hangmanText);
    //head = prependWord(head, firstWordArray, secondWordArray);
    int cnt = 0;
    while (fgets(wordArray, sizeof(wordArray), hangmanText)) {

        //populate firstWordArray using char values (until a comma) of hangmanText
        for (int i = 0; i < wordMaxLength; i++) {
            //read the username one char at a time until hitting comma
            if (wordArray[i] != ',') {
                firstWordArray[i] = wordArray[i];
            }
            //when reaching the comma, change it to a space, terminate string, and break
            else {
                firstWordArray[i] = '\0';
                wordArray[i] = ' ';
                break;
            }
            //change each read char in wordArray to a space
            wordArray[i] = ' ';
        }
        //populate secondWordArray using final values (not spaces) of wordArray
        int secondWordCounter = 0;
        for (int i = 0; i < 2 * wordMaxLength; i++) {
            if (wordArray[i] != '\r' && wordArray[i] != '\n' && wordArray[i] != ' ') {
                secondWordArray[secondWordCounter] = wordArray[i];
                wordArray[i] = ' ';
                secondWordCounter++;
            }
        }
        wordPairCounter++;
        //print extracted values for test purposes
        //printf("Word pair: %d \t First word: %s \t Second word: %s \n", wordPairCounter, firstWordArray, secondWordArray);

        // //add this user to a a new node
        //wordPair *newPair = NULL;
        //newPair = AddWordPair(newPair, firstWordArray, secondWordArray, wordPairCounter);
        if (!cnt){
            head = prependWord(head, firstWordArray, secondWordArray);
            cnt++;
        }
        else {
            head = appendWord(head, firstWordArray, secondWordArray, wordPairCounter);
        }
    }
    fclose(hangmanText);
    return head;
}

void traverseUsers(userNode * head, int socket_id, callbackUser f) {
    userNode * cursor = head;
    while (cursor != NULL) {
        f(cursor, socket_id);
        cursor = cursor-> next;
    }
}

void traverseWords(wordPair * head, callbackWord f) {
    wordPair * cursor = head;
    while (cursor != NULL) {
        f(cursor);
        cursor = cursor-> next;
    }
}

void displayUsers(userNode * n) {
    if (n != NULL) {
        printf("%s ", n-> username);
        printf("%s\n", n-> password);
    }
}

void sendUser(userNode * n, int socket_id){
    if (n != NULL && n->gamesPlayed > 0){
        char name[usernameMaxLength];
        strcpy(name, n->username);
        int played = n->gamesPlayed;
        int won = n->gamesWon;
        SendInt(socket_id, 0);
        ReceiveInt(socket_id);
        send(socket_id, name, 10, 0);
        ReceiveInt(socket_id);
        SendInt(socket_id, won);
        ReceiveInt(socket_id);
        SendInt(socket_id, played);
        ReceiveInt(socket_id);
    }
}

void sendLeaderboard(userNode* head, int socket_id)
{
    userNode * users = head;
    while (users != NULL) {
        sendUser(users, socket_id);
        users = users-> next;
    }
    SendInt(socket_id, 1);
}
<<<<<<< HEAD

void displayWords(wordPair * n) {
    if (n != NULL) {
        printf("%s ", n-> firstWord);
        printf("%s\n", n-> secondWord);
    }
}

=======

void displayWords(wordPair * n) {
    if (n != NULL) {
        printf("%s ", n-> firstWord);
        printf("%s\n", n-> secondWord);
    }
}

>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
void SendString(int socket_id, char *string, int stringLength) {
	//pad out string with zeros as required
	if (stringLength < ARRAY_SIZE) {
		for (int i = stringLength; i < ARRAY_SIZE; i++){
			string[i] = 0;
		}
	}
	for (int i = 0; i < ARRAY_SIZE; i++) {
		unsigned short networkValue = htons(string[i]);
		send(socket_id, &networkValue, sizeof(unsigned short), 0);
		// printf("String[%d] = %c\n", i, string[i]);
	}
	fflush(stdout);
}

char * ReceiveData(int socket_identifier, int size) {

    int numberOfBytes = 0;
    unsigned short networkValue;
    char * receivedData = malloc(sizeof(char) * size);

    for (int i = 0; i < size; i++) {
        if ((numberOfBytes = recv(socket_identifier, & networkValue, sizeof(unsigned short), 0)) == RETURNED_ERROR) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        receivedData[i] = ntohs(networkValue);
    }
    //terminate string
    // receivedData[size] = '\0';
    return receivedData;
}

void SendInt(int sockfd, int integer){
    unsigned short netval = htons(integer);
    send(sockfd, &netval, sizeof(unsigned short), 0);
}

int ReceiveInt(int socket_id){
    int integer;
    unsigned short netval;
    recv(socket_id, & netval, sizeof(unsigned short), 0);
    integer = ntohs(netval);
    return integer;
}

/*
Function for rejecting any entry that isn't a lowercase letter.
Known issues: NOT YET TESTED.
*/
int TestCharacter(char receivedChar) {
    /*
    int lowerCaseChar = 0;
    char letterArray[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m', \
                        'n','o','p','q','r','s','t','u','v','w','x','y','z'};
    for (int i=0; i < LETTERS_IN_APLHABET; i++){
        if (receivedChar == letterArray[i]) {
            lowerCaseChar = 1;
            break;
        }
    }
    return lowerCaseChar;
    */
    if (receivedChar >= 'a' && receivedChar <= 'z') {
        return 1;
    } else {
        return 0;
    }
}
<<<<<<< HEAD

int PlayHangman(char *object[], int obj_size, char *obj_type[], int type_size, userNode *user, int new_fd)
{
    int size1 = obj_size;
    int size2 = type_size;
    char name[10], word1[size1], word2[size2];
    if (user == NULL){
        printf("ERROR");
    }
    userNode* acc = user;
    strcpy(name, acc->username);
    printf("user: %s", name);
    strcpy(word1, object);
    strcpy(word2, obj_type);
    printf("words: %s %s", word1, word2);

    int obj[size1];// boolean array testing whether a letter was guessed
    int type[size2];
    memset(obj, 0, size1*sizeof(int));
    memset(type, 0, size2*sizeof(int));
    //int type[word2_len] = {0};

    char letters[20] = { NULL };
    //memset(letters, 0, 20*sizeof(char));

    char progress[50] = { NULL };

    char str_spc[2], str_word[2];
    str_spc[1] = '\0';
    str_word[1] = '\0';
    char spc = ' ';
    str_spc[0] = spc;

    char c;
    char *raw;
    char undr[] = "_ ";
    char space[] = "  ";
    int endgame = 0;
    //int num_guesses = 20;
    int num_guesses = MIN((size1 + size2 + 10),26);
    while (!endgame){
        SendInt(new_fd, num_guesses);
        memset(progress, 0, sizeof progress);
        for (int i=0;i<size1;i++){
            if (!obj[i]){
                //printf("_ ");
                strcat(progress, undr);
            } else {
                //printf("%c ", word1[i]);
                str_word[0] = word1[i];
                strcat(progress, str_word);
                strcat(progress, str_spc);
            }
        }
        //printf("  ");
        strcat(progress, space);
        for (int i=0;i<size2;i++){
            if (!type[i]){
                //printf("_ ");
                strcat(progress, undr);
            } else {
                //printf("%c ", word2[i]);
                str_word[0] = word2[i];
                strcat(progress, str_word);
                strcat(progress, str_spc);
            }
        }
        ReceiveInt(new_fd);
        send(new_fd, progress, 60, 0);
        //c[0] = getchar();
        endgame = 1;
        for (int i=0;i<size1;i++){
            if (!obj[i]){ // checking all true for boolean arrays
                endgame = 0;
            }
        }
        for (int i=0;i<size2;i++){
            if (!type[i]){
                endgame = 0;
            }
        }
        if (!num_guesses){
            endgame = 1;
        }
        ReceiveInt(new_fd);
        SendInt(new_fd, endgame);
        if (num_guesses > 0 && !endgame){
            //scanf(" %c", &c[0]);
            //c = ReceiveData(new_fd, STRING_LENGTH);
            raw = ReceiveData(new_fd, STRING_LENGTH);
            c = raw[0];
        }
        //ClearScreen();
        num_guesses -= 1;
        for (int i=0;i<size1;i++){
            if (c == word1[i]){
                // setting boolean array depending if letter was guessed
                obj[i] = 1;
            }
        }
        for (int i=0;i<size2;i++){
            if (c == word2[i]){
                // setting boolean array depending if letter was guessed
                type[i] = 1;
            }
        }

    }
    if(num_guesses > 0){
        printf("Well done %s You won this round of Hangman!\n", name);
        return 1;
    } else {
        printf("Bad luck %s You have run out of guesses. The Hangman got you!\n", name);
        return 0;
    }
}

int main(int argc, char * argv[]) {
    userNode * users = NULL;
    wordPair * words = NULL;
    callbackUser dispUsers = displayUsers;
    callbackWord dispWords = displayWords;

    int portNumber;
    int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
    struct sockaddr_in my_addr; /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    socklen_t sin_size;

    //If no port number argument is recieved, show message, use default port
    if (argc != ARGUMENTS_EXPECTED) {
        fprintf(stderr, "Defaulting to port 54321.\n");
        portNumber = DEFAULT_PORT_NO;
    }

    /* generate the socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == RETURNED_ERROR) {
        perror("socket");
        exit(1);
    }

    /* generate the end point */
    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(portNumber); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */

=======

int PlayHangman(char *object[], int obj_size, char *obj_type[], int type_size, userNode *user, int new_fd)
{
    int size1 = obj_size;
    int size2 = type_size;
    char name[10], word1[size1], word2[size2];
    if (user == NULL){
        printf("ERROR");
    }
    userNode* acc = user;
    strcpy(name, acc->username);
    printf("user: %s", name);
    strcpy(word1, object);
    strcpy(word2, obj_type);
    printf("words: %s %s", word1, word2);

    int obj[size1];// boolean array testing whether a letter was guessed
    int type[size2];
    memset(obj, 0, size1*sizeof(int));
    memset(type, 0, size2*sizeof(int));
    //int type[word2_len] = {0};

    char letters[20] = { NULL };
    //memset(letters, 0, 20*sizeof(char));

    char progress[50] = { NULL };

    char str_spc[2], str_word[2];
    str_spc[1] = '\0';
    str_word[1] = '\0';
    char spc = ' ';
    str_spc[0] = spc;

    char c;
    char *raw;
    char undr[] = "_ ";
    char space[] = "  ";
    int endgame = 0;
    //int num_guesses = 20;
    int num_guesses = MIN((size1 + size2 + 10),26);
    while (!endgame){
        SendInt(new_fd, num_guesses);
        memset(progress, 0, sizeof progress);
        for (int i=0;i<size1;i++){
            if (!obj[i]){
                //printf("_ ");
                strcat(progress, undr);
            } else {
                //printf("%c ", word1[i]);
                str_word[0] = word1[i];
                strcat(progress, str_word);
                strcat(progress, str_spc);
            }
        }
        //printf("  ");
        strcat(progress, space);
        for (int i=0;i<size2;i++){
            if (!type[i]){
                //printf("_ ");
                strcat(progress, undr);
            } else {
                //printf("%c ", word2[i]);
                str_word[0] = word2[i];
                strcat(progress, str_word);
                strcat(progress, str_spc);
            }
        }
        ReceiveInt(new_fd);
        send(new_fd, progress, 60, 0);
        //c[0] = getchar();
        endgame = 1;
        for (int i=0;i<size1;i++){
            if (!obj[i]){ // checking all true for boolean arrays
                endgame = 0;
            }
        }
        for (int i=0;i<size2;i++){
            if (!type[i]){
                endgame = 0;
            }
        }
        if (!num_guesses){
            endgame = 1;
        }
        ReceiveInt(new_fd);
        SendInt(new_fd, endgame);
        if (num_guesses > 0 && !endgame){
            //scanf(" %c", &c[0]);
            //c = ReceiveData(new_fd, STRING_LENGTH);
            raw = ReceiveData(new_fd, STRING_LENGTH);
            c = raw[0];
        }
        //ClearScreen();
        num_guesses -= 1;
        for (int i=0;i<size1;i++){
            if (c == word1[i]){
                // setting boolean array depending if letter was guessed
                obj[i] = 1;
            }
        }
        for (int i=0;i<size2;i++){
            if (c == word2[i]){
                // setting boolean array depending if letter was guessed
                type[i] = 1;
            }
        }

    }
    if(num_guesses > 0){
        printf("Well done %s You won this round of Hangman!\n", name);
        return 1;
    } else {
        printf("Bad luck %s You have run out of guesses. The Hangman got you!\n", name);
        return 0;
    }
}

int main(int argc, char * argv[]) {
    userNode * users = NULL;
    wordPair * words = NULL;
    callbackUser dispUsers = displayUsers;
    callbackWord dispWords = displayWords;

    int portNumber;
    int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
    struct sockaddr_in my_addr; /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    socklen_t sin_size;

    //If no port number argument is recieved, show message, use default port
    if (argc != ARGUMENTS_EXPECTED) {
        fprintf(stderr, "Defaulting to port 54321.\n");
        portNumber = DEFAULT_PORT_NO;
    }

    /* generate the socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == RETURNED_ERROR) {
        perror("socket");
        exit(1);
    }

    /* generate the end point */
    my_addr.sin_family = AF_INET; /* host byte order */
    my_addr.sin_port = htons(portNumber); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */

>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
    /* bind the socket to the end point */
    if (bind(sockfd, (struct sockaddr * ) & my_addr, sizeof(struct sockaddr)) == RETURNED_ERROR) {
        perror("bind");
        exit(1);
    }

    /* start listnening */
    if (listen(sockfd, BACKLOG) == RETURNED_ERROR) {
        perror("listen");
        exit(1);
    }

    //this message may not be necessary
    printf("server starts listnening on port %d...\n", portNumber);

    //import word list and authentication details
    words = WordListImport();
    traverseWords(words, dispWords);
    printf("Words imported!\n");

    users = AuthenticationImport();
    //traverseUsers(users, dispUsers);
    printf("Authentication details imported!\n");

    /* repeat: accept, send, close the connection */
    /* for every accepted connection, use a separate process or thread to serve it */
    while (1) { /* main accept() loop */
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr * ) & their_addr, & sin_size)) == RETURNED_ERROR) {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

        //FORK HERE, HAVE ALL COMMON VALUES LOADED ABOVE
        /*
		if (!fork()) { // this is the child process
			// Call method to recieve array data - Uncomment this line once function implemented
			char *results = ReceiveData(new_fd, ARRAY_SIZE);
			// Print out the array results sent by client
			for (int i=0; i<ARRAY_SIZE; i++){
				printf("Received value[%c]\n", results[i]);
			}
            char *testString="password";
            printf("Test string is: [%s] \n", testString);
            printf("Received string is: [%s] \n", results);
            //test received string against stored
            if (strcmp(testString, results) == 0) {
                printf("The received password matches.");
            } else {
                printf("The received password does not match.");
                send(new_fd, "That password does not match the records.\n", 40, 0);
            }
		}
		close(new_fd);  // parent doesn't need this
		while(waitpid(-1,NULL,WNOHANG) > 0); // clean up child processes
		*/
        if (send(new_fd, "Please enter your username-->", 40, 0) == RETURNED_ERROR) {
            perror("send");
            close(new_fd);
            //  exit(0);
        }
        printf("I told the guy to send a username...\n");

        //wait for string to be received
        //this might need to be in a control structure, wait until a string is received
        char * receivedUsername = ReceiveData(new_fd, STRING_LENGTH);

        //test the received username, for now use simple string comparison
        //char testUsername[] = "Maolin";
        userNode * user = searchUser(users, receivedUsername);
        //if the received and stored usernames match
        //if (strcmp(receivedUsername, testUsername) == 0) {
        if (user != NULL){
            //send a message prompting for a password
            if (send(new_fd, "Please enter your password-->", 40, 0) == RETURNED_ERROR) {
                perror("send");
                close(new_fd);
                //  exit(0);
            }
            printf("I told the guy to send a password...\n");
            //if the received and stored usernames DON'T match
        } else {
            if (send(new_fd, "Invalid username; closing connection.\n", 40, 0) == RETURNED_ERROR) {
                perror("send");
                close(new_fd);
                //  exit(0);
            } else {
                close(new_fd);
                //  exit(0);
            }
        }

        //receive a password
        char * receivedPassword = ReceiveData(new_fd, STRING_LENGTH);
        //test the received username, for now use simple string comparison
        //char testPassword[] = "123456";
        //if the received and stored passwords match
        //if (strcmp(receivedPassword, testPassword) != 0) {
        if (strcmp(receivedPassword, user->password) == 0){
            if (send(new_fd, "Invalid password; closing connection.\n", 40, 0) == RETURNED_ERROR) {
                perror("send");
                close(new_fd);
                //  exit(0);
            } else {
                close(new_fd);
                //  exit(0);
            }
        } else {
            //send(new_fd, "Welcome.\n", 40, 0);
            printf("Login accepted.\n");
        }

        //by this point, the username and password should have been verified against
        //stored values

        //receive integer, 1-3 choosing action
        //char choice = ReceiveData(new_fd, STRING_LENGTH);
        int end = 0;
        int wins;
        char choice;
        //char choices[] = "123";
        char * raw;
        while(!end){
            raw = ReceiveData(new_fd, STRING_LENGTH);
            choice = raw[0];
            //printf("The client chose: %c\n", &choice);
<<<<<<< HEAD
            switch (choice)
            {
            case '1':
            //if (choice == choices[0]){
=======
            switch (choice)
            {
            case '1':
            //if (choice == choices[0]){
>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
                printf("option was 1");// testing to see if this option was chosen
                //userNode *mao = searchUser(users, "Maolin");
                time_t t;
                srand((unsigned) time(&t));
                wordPair * word = searchWord(words, rand() % wordCount(words));
<<<<<<< HEAD
                //const char *words[] = {"snek", "snake"};
                const int sizes[] = {strlen(word->firstWord), strlen(word->secondWord)};
                wins = PlayHangman(word->firstWord, sizes[0], word->secondWord, sizes[1], user, new_fd);
                user->gamesPlayed++;
                user->gamesWon += wins;
                break;
            //}else if (choice == choices[1]){
            case '2':
                printf("option was 2");
                sendLeaderboard(users, new_fd);
                break;
            //}else if (choice == choices[2]){
            case '3':
                printf("option was 3");
                end = 1;
                break;
            //}else{
            default:
                printf("Invalid option");
=======
                //const char *words[] = {"snek", "snake"};
                const int sizes[] = {strlen(word->firstWord), strlen(word->secondWord)};
                wins = PlayHangman(word->firstWord, sizes[0], word->secondWord, sizes[1], user, new_fd);
                user->gamesPlayed++;
                user->gamesWon += wins;
                break;
            //}else if (choice == choices[1]){
            case '2':
                printf("option was 2");
                sendLeaderboard(users, new_fd);
                break;
            //}else if (choice == choices[2]){
            case '3':
                printf("option was 3");
                end = 1;
                break;
            //}else{
            default:
                printf("Invalid option");
>>>>>>> 3777ab9ade21e1719ebf2fb5457f44fe4ff1cb99
            }
        }
    }
    //this function closes an accepted connection, put it outside the loop
    close(new_fd); /* parent doesn't need this */
    //once thread pool is implemented, we will need to return thread to pool here
    return 0;

}
