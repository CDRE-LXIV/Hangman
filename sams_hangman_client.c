#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

//magic numbers and constants
#define MAXDATASIZE 100
#define ARRAY_SIZE 10
#define RETURNED_ERROR -1
#define h_addr h_addr_list[0] /* for backward compatibility */

//send a string to hangman_server, upto 10 characters length
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

//send a char to hangman_server
void SendChar(int socket_id, char character) {
	//send as a string because I don't know how to do otherwise
	int stringLength = 2;
	char singleCharString[stringLength];
	singleCharString[1] = '\0';
	singleCharString[0] = character;

	SendString(socket_id, singleCharString, stringLength);
}

void ReceiveData(int sockfd) {
	int numberOfBytes;
	char buf[MAXDATASIZE];
	if ((numberOfBytes=recv(sockfd, buf, MAXDATASIZE, 0)) == RETURNED_ERROR) {
		perror("recv");
		exit(1);
	}
	buf[numberOfBytes] = '\0';
	printf("%s\n",buf);
}

void ClearScreen() {
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

//draw the welcome message template
void DisplayWelcomeMessage() {
	printf("\n=============================================\n");
	printf("Welcome to the Online Hangman Gaming System\n");
	printf("=============================================\n\n");
	printf("You are required to log on with your registered Username and Password\n");
}

void DisplayMainMenu() {
	printf("\nWelcome to the Online Hangman Gaming System\n\n\n\n");
	printf("Please enter a selection\n");
	printf("<1> Play Hangman\n");
	printf("<2> Show Leaderboard\n");
	printf("<3> Quit\n\n");
	printf("Select option 1-3:\n");
}

int main(int argc, char *argv[]) {
	int portNumber;
	int sockfd, numberOfBytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	//if there aren't three main arguments, throw an error message
	if (argc != 3) {
		fprintf(stderr,"Usage: client_hostname, port number\n");
		exit(1);
	}

	//if there are three or more main arguments, convert argv[2] to port number
	if (argc==3){
		portNumber = atoi(argv[2]);
	}

	//if a host name isn't specified
	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == RETURNED_ERROR) {
		perror("socket");
		exit(1);
	}


	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(portNumber);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	// bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == RETURNED_ERROR) {
		perror("connect");
		exit(1);
	}
	DisplayWelcomeMessage();

	while(1){
        //client should be connected, receive message
        ReceiveData(sockfd);

        //take username to be sent to server
        char username[10] ="";
        scanf("%9s", username);
        int usernameLength = strlen(username);
        //send username
        SendString(sockfd, username, usernameLength);
        ReceiveData(sockfd);

        //take password to be sent to server
        char password[10] ="";
        scanf("%9s", password);
        int passwordLength = strlen(password);
        //send password
        SendString(sockfd, password, passwordLength);


        //should now be logged in, draw game area
        ClearScreen();
        //receive welcome message
        ReceiveData(sockfd);
        // ClearScreen();
        //draw the menu
        DisplayMainMenu();
        //wait for key input 1-3
        char key = ' ';
        while ((key != '1')&&(key != '2')&&(key != '3')){
            key = getchar();
        }
        SendChar(sockfd, key);
        ClearScreen();
        printf("You picked: %c\n", key);
	}	//end while loop
    close(sockfd);

    return 0;
}	//end main
