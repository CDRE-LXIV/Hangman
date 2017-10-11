#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void ClearScreen(){
    system("cls||clear");
}

typedef struct node{
    char usrn[10];
    char pass[10];
    int played;
    int won;
    struct node* next;
} node;

typedef void (*callback)(node* data);

node* create(char usrn[], char pass[], node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL){
        printf("Error\n");
        return(0);
    }
    //new_node->usrn = usrn;
    //new_node->pass = pass;
    strcpy(new_node->usrn, usrn);
    strcpy(new_node->pass, pass);
    new_node->played = 0;
    new_node->won = 0;
    new_node->next = next;

    return new_node;
}

node* prepend(node* head, char usrn[], char pass[]){
    node* new_node = create(usrn, pass, head);
    head = new_node;
    return head;
}

node* append(node* head, char usrn[], char pass[])
{
    if(head == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    /* create a new node */
    node* new_node =  create(usrn, pass, NULL);
    cursor->next = new_node;

    return head;
}

node* search(node* head, char usrn[]){
    node *cursor = head;
    while(cursor != NULL){
        if (strcmp(cursor->usrn, usrn)==0){
            return cursor;
        } else {
            cursor = cursor->next;
        }
    }
    return NULL;
}

int count(node *head)
{
    node *cursor = head;
    int c = 0;
    while(cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}

void traverse(node* head, callback f){
    node* cursor = head;
    while(cursor != NULL){
        f(cursor);
        cursor = cursor->next;
    }
}

void display(node* n){
    if (n != NULL){
        printf("%s ", n->usrn);
        printf("%s\n", n->pass);
    }
}

void PlayHangman(char word[], int size, node *user)
{
    char name[10], word1[size];
    if (user == NULL){
        printf("ERROR");
    }
    node* acc = user;
    strcpy(name, acc->usrn);
    printf("user: %s", name);
    strcpy(word1, word);
    printf("word: %s", word1);

    int obj[size];// boolean array testing whether a letter was guessed
    memset(obj, 0, size*sizeof(int));
    //int type[word2_len] = {0};

    char letters[10];
    memset(letters, 0, 10*sizeof(char));

    char c[2];
    int endgame = 0;
    //int num_guesses = 20;
    int num_guesses = 20;//MIN((word1_len + word2_len + 10),26);
    while (!endgame){
        ClearScreen();
        printf("\nGuessed Letters %s\n", letters);
        printf("Number of guesses left: %d\n", num_guesses);
        printf("Word: ");
        for (int i=0;i<size;i++){
            if (!obj[i]){
                printf("_ ");
            } else {
                printf("%c ", word1[i]);
            }
        }
        printf("\n");
        printf("Enter your guess - ");
        //c[0] = getchar();
        scanf(" %c", &c[0]);
        num_guesses -= 1;
        strcat(letters, c);
        for (int i=0;i<size;i++){
            if (c[0] == word1[i]){
                // setting boolean array depending if letter was guessed
                obj[i] = 1;
            }
        }
        if (!num_guesses){
            endgame = 1;
        }
    }
    printf("\nGame over\n");
    if(num_guesses > 0){
        printf("Well done %s You won this round of Hangman!\n", name);
    } else {
        printf("Bad luck %s You have run out of guesses. The Hangman got you!\n", name);
    }
    system("pause");
}

int main()
{
    node* head = NULL;
    callback disp = display;

    head = prepend(head, "Player1", "54321");
    head = append(head, "Player2", "12345");
    traverse(head, disp);
    printf("\n");

    char word[] = "snek";
    int size = strlen(word);

    char usrn[10], pass[10];
    printf("=============================================\n");
    printf("Welcome to the Online Hangman Gaming System\n");
    printf("=============================================\n\n");
    printf("You are required to logon with your registered Username and Password\n");
    printf("Please enter your username-->");
    scanf("%s", usrn);
    printf("Please enter your password-->");
    scanf("%s", pass);

    ClearScreen();

    while (1){
        ClearScreen();
        char key = 0;
        printf("\nWelcome to the Hangman Gaming System\n\n");
        printf("Please enter a selection\n");
        printf("<1> Play Hangman\n");
        printf("<2> Show Leaderboard\n");
        printf("<3> Quit\n");
        printf("Select option 1-3 ->");
        //scanf(" %c", &key);
        key = getchar();
        switch (key)
        {
        case '1':
            printf("option was 1");// testing to see if this option was chosen
            node* player = search(head, "Player1");
            PlayHangman(word, size, player);
            break;
        case '2':
            printf("option was 2");
            break;
        case '3':
            printf("option was 3");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Invalid option");
        }
    }

    return 0;
}
