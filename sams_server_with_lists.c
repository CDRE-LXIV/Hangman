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
#define ARRAY_SIZE 8
#define STRING_LENGTH 10
#define BACKLOG 10     /* how many pending connections queue will hold */
#define RETURNED_ERROR -1
#define ARGUMENTS_EXPECTED 2
#define LETTERS_IN_APLHABET 26
#define usernameMaxLength 10
#define passwordMaxLength 10
#define wordMaxLength 15

//function declarations
char *ReceiveData(int socket_identifier, int size);
char ReceiveCharacter(int socket_identifier);
int TestCharacter(char receivedChar);

//define a structure-linked list to hold word pairs
typedef struct list wordPair;

struct list {
    char firstWord [wordMaxLength];
    char secondWord [wordMaxLength];
    unsigned int wordPairNumber;
    wordPair *next;
};

//define a structure-linked list to hold users
typedef struct node userNode;

struct node {
  char username [usernameMaxLength];
  char password [passwordMaxLength];
  int gamesWon;
  int gamesPlayed;
  userNode *next;
};

//print the authentication details for a single user
// void PrintUser(userNode *user) {
//   printf("Username=%s \t Password=%s\n", user->username, user->password);
// }

//add a userNode node
userNode * AddUserNode(userNode *head, char username[], char password[]) {
    // if the user is not "Username" and password is not "password"
    char defaultUsername[] = "Username";
    char defaultPassword[] = "Password";
    if ((strcmp(username, defaultUsername) !=0)&&(strcmp(password, defaultPassword) !=0)) {
    // create new userNode to add to list
    userNode *newNode = (userNode *)malloc(sizeof(userNode));
    if (newNode == NULL) {
        return NULL;
    }
    // insert new node
    strcpy(newNode->username, username);
    strcpy(newNode->password, password);
    newNode->gamesWon = 0;
    newNode->gamesPlayed = 0;
    newNode->next = head;
    // printf("Added: Username: %s \t Password: %s\n", newNode->username, newNode->password);
    return newNode;
  }
}

//print the authentication details for every user
void PrintAllUserDetails(userNode *head) {
    for ( ; head != NULL; head = head->next) {
        printf("Username=%s \t\t Password=%s\n", head->username, head->password);
    }
}
//****************************

//add a wordPair node
wordPair * AddWordPair(wordPair *head, char firstWord[], char secondWord[], unsigned int wordPairCounter) {
    // create new wordPair to add to list
    wordPair *newPair = (wordPair *)malloc(sizeof(wordPair));
    if (newPair == NULL) {
        return NULL;
    }
    // insert new node
    strcpy(newPair->firstWord, firstWord);
    strcpy(newPair->secondWord, secondWord);
    newPair->wordPairNumber = wordPairCounter;
    newPair->next = head;
    // printf("Added: Word pair: %d \t First word: %s \t Second word: %s \n", newPair->wordPairNumber, newPair->firstWord, newPair->secondWord);
    return newPair;
}

//print every wordPair
void PrintAllWordPairs(wordPair *head) {
    for ( ; head != NULL; head = head->next) {
      printf("First Word=%s \t Second Word=%s \t Word Pair Number:%d\n ", head->firstWord, head->secondWord, head->wordPairNumber);
    }
}
//**************************

//write function to take contents of Authentication.txt to linked list
int AuthenticationImport() {
    FILE *authenticationText;
    //authenticationArray holds fget string before parsing
    char authenticationArray[passwordMaxLength+usernameMaxLength];
    //parsed username from authenticationArray
    char usernameArray[usernameMaxLength];
    //parsed password from authenticationArray
    char passwordArray[passwordMaxLength];
    //initialise arrays with zeros
    for (int i = 0; i< usernameMaxLength; i++) {
        usernameArray[i] = 0;
        passwordArray[i] = 0;
        authenticationArray[i] = 0;
        authenticationArray[i+10] = 0;
    }

    //open Authentication.txt in read mode
    authenticationText = fopen("Authentication.txt", "r");

    if (authenticationText == NULL) {
        perror("Error opening Authentication file:");
        return(-1);
    }

    //copy values in Authentication.txt to usernameArray and passwordArray, remove spaces somehow

    while(fgets(authenticationArray, (usernameMaxLength+passwordMaxLength), authenticationText)) {
        //populate usernameArray using first 10 values (or until a space or indent) of authenticationArray
        for (int i = 0; i < usernameMaxLength; i++) {
            //read the username one char at a time until hitting space or indent
            if ((authenticationArray[i] != ' ')&&(authenticationArray[i] != '\t')) {
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
        for (int i = 0; i < (usernameMaxLength+passwordMaxLength); i++) {
            if ((authenticationArray[i] != ' ')&&(authenticationArray[i] != '\t')) {
                passwordArray[passwordCounter] = authenticationArray[i];
                authenticationArray[i] = ' ';
                passwordCounter++;
            }
        }

        //print extracted values for test purposes
        printf("Username: %s \t\t Password: %s \n", usernameArray, passwordArray);

        //add this user to a a new node
        userNode *head = NULL;
        AddUserNode(head, usernameArray, passwordArray);
        //newPair = AddWordPair(newPair, firstWordArray, secondWordArray, wordPairCounter);
    }
    fclose(authenticationText);
    return 0;
}

int WordListImport() {
    FILE *hangmanText;
    //awordArray holds fget string before parsing
    char wordArray[2*wordMaxLength];
    //parsed first word from wordArray
    char firstWordArray[wordMaxLength];
    ////parsed password from authenticationArray
    char secondWordArray[wordMaxLength];
    //initialise arrays with zeros
    for (int i = 0; i< wordMaxLength; i++) {
        firstWordArray[i] = 0;
        secondWordArray[i] = 0;
        wordArray[i] = 0;
        wordArray[i+wordMaxLength] = 0;
    }

    //open Authentication.txt in read mode
    hangmanText = fopen("hangman_text.txt", "r");

    if (hangmanText == NULL) {
        perror("Error opening hangman_text file");
        return(-1);
    }

    //copy values in Authentication.txt to usernameArray and passwordArray, remove spaces somehow
    unsigned int wordPairCounter = 0;
    while(fgets(wordArray, sizeof(wordArray), hangmanText)) {

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
        for (int i = 0; i < 2*wordMaxLength; i++) {
            if (wordArray[i] != ' ') {
                secondWordArray[secondWordCounter] = wordArray[i];
                wordArray[i] = ' ';
                secondWordCounter++;
            }
        }
        wordPairCounter++;
        //print extracted values for test purposes
        printf("Word pair: %d \t First word: %s \t Second word: %s \n", wordPairCounter, firstWordArray, secondWordArray);

        // //add this user to a a new node
        wordPair *newPair = NULL;
        newPair = AddWordPair(newPair, firstWordArray, secondWordArray, wordPairCounter);
    }
    fclose(hangmanText);
    return 0;
}


char *ReceiveData(int socket_identifier, int size) {

	int numberOfBytes = 0;
	unsigned short networkValue;
	char *receivedData = malloc(sizeof(char)*size);

	for (int i = 0; i < size; i++) {
		if ((numberOfBytes = recv(socket_identifier, &networkValue, sizeof(unsigned short), 0)) \
    == RETURNED_ERROR) {
			perror("recv");
			exit(EXIT_FAILURE);
		}
		receivedData[i] = ntohs(networkValue);
	}
  //terminate string
  // receivedData[size] = '\0';
	return receivedData;
}

/*
Function for receiving a single char, for the Hangman game itself.
Known issues: NOT YET TESTED.
Thoughts: Should we test for letter (i.e., reject numbers or punctuation)
*/
char ReceiveCharacter(int socket_identifier) {
  int numberOfBytes = 0;
  unsigned short networkValue;
  char receivedCharacters[2];
	char receivedChar;

  //this might need to be re-written so the server waits on a character before acting
	for (int i = 0; i < 2; i++) {
  	if ((numberOfBytes = recv(socket_identifier, &networkValue, sizeof(unsigned short), 0)) \
  	== RETURNED_ERROR) {
    perror("recv");
    exit(EXIT_FAILURE);
  	}
  	receivedCharacters[i] = ntohs(networkValue);
	}
	receivedChar = receivedCharacters[0];
	return receivedChar;
//   //here is where we test received character, reply to client if not valid
//   if (TestCharacter(receivedChar) == 0) {
//     send(socket_identifier, "Invalid choice. Please choose a lowercase letter.\n", 40 , 0);
//   }
//   //note that this function might return receivedChar even if it is invalid; fix this
//   else {
//     return receivedChar;
//   }
// return 0;
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
    if (receivedChar >= 'a' && receivedChar <= 'z'){
        return 1;
    } else {
        return 0;
    }
}

int main (int argc, char *argv[]) {
    int portNumber;
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;

	//If no port number argument is recieved, show message, use default port
	if (argc != ARGUMENTS_EXPECTED) {
		fprintf(stderr,"Defaulting to port 54321.\n");
        portNumber = DEFAULT_PORT_NO;
	}

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == RETURNED_ERROR) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(portNumber);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */


	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== RETURNED_ERROR) {
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
    WordListImport();
    printf("Words imported!\n");

    AuthenticationImport();
    printf("Authentication details imported!\n");

	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a separate process or thread to serve it */
	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == RETURNED_ERROR) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));

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
		 if (send(new_fd, "Please enter your username:\n", 40 , 0) \
         == RETURNED_ERROR) {
             perror("send");
           close(new_fd);
          //  exit(0);
         }
         printf("I told the guy to send a username...\n");

         //wait for string to be received
         //this might need to be in a control structure, wait until a string is received
         char *receivedUsername = ReceiveData(new_fd, STRING_LENGTH);

         //test the received username, for now use simple string comparison
         char testUsername[] = "Maolin";
         //if the received and stored usernames match
         if (strcmp(receivedUsername, testUsername) == 0){
           //send a message prompting for a password
           if (send(new_fd, "Please enter your password:\n", 40 , 0) \
           == RETURNED_ERROR) {
               perror("send");
             close(new_fd);
            //  exit(0);
           }
              printf("I told the guy to send a password...\n");
             //if the received and stored usernames DON'T match
         } else {
           if (send(new_fd, "Invalid username; closing connection.\n", 40 , 0) \
           == RETURNED_ERROR) {
             perror("send");
             close(new_fd);
            //  exit(0);
           } else {
             close(new_fd);
            //  exit(0);
           }
         }

           //receive a password
           char *receivedPassword = ReceiveData(new_fd, STRING_LENGTH);
           //test the received username, for now use simple string comparison
           char testPassword[] = "123456";
           //if the received and stored passwords match
           if (strcmp(receivedPassword, testPassword) != 0){
                 if (send(new_fd, "Invalid password; closing connection.\n", 40 , 0) \
                 == RETURNED_ERROR) {
                    perror("send");
                    close(new_fd);
                    //  exit(0);
                        } else {
                        close(new_fd);
                        //  exit(0);
                        }
            } else {
                    send(new_fd, "Welcome.\n", 40 , 0);
                    printf("Login accepted.\n");
                }

    //by this point, the username and password should have been verified against
    //stored values

    //receive integer, 1-3 choosing action
    char *choice = ReceiveData(new_fd, STRING_LENGTH);
    printf("The client chose: %s\n", choice);


	}
//this function closes an accepted connection, put it outside the loop
	close(new_fd);  /* parent doesn't need this */
	//once thread pool is implemented, we will need to return thread to pool here
  return 0;

}
