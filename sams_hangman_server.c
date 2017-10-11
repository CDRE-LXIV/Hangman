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
#include <errno.h>
// #include <pthreads.h>

/*This is the pseudo code for the Hangman server, to be filled in
with real code in due course.*/

//define magic numbers and constants
#define DEFAULT_PORT_NO 12345
#define STRING_LENGTH 10
#define RETURNED_ERROR -1
#define LETTERS_IN_APLHABET 26
#define BACKLOG 10

//function declarations
char *ReceiveData(int socket_identifier, int size);
char ReceiveCharacter(int socket_identifier);
int TestCharacter(char receivedChar);

/*
Function for receiving strings, such as a username or password.
Known issues: if it does not receive the passed parameter "size" characters,
it will wait forever; solve by padding strings client-side for size uniformity.
*/


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
}

//main loop
int main (int argc, char *argv[]) {
  int portNumber;
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;

	//Get a port number from argv[1], or revert to default value
	if (argc==2){
		portNumber = atoi(argv[1]);
  } else {
			fprintf(stderr,"Defaulting to port 12345.\n");
			portNumber = DEFAULT_PORT_NO;
		}

	//generate the socket, this will be used by later listen, bind, receive and send
  //functions
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
  //note integer BACKLOG is a constant of 10, this should be modified
  //as needed for the Threadpool criteria
	if (listen(sockfd, BACKLOG) == RETURNED_ERROR) {
		perror("listen");
		exit(1);
	}

	//this message may not be necessary
	printf("Hangman Server listening on port %d...\n", portNumber);

/*
Repeat: accept, send, close the connection.
For every accepted connection, use a separate thread from the Threadpool
to serve it, if available (NOT YET IMPLEMENTED).*/
	while(1) {
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == RETURNED_ERROR) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));

      //new thread here after new connection accepted

/*
      BEGIN PSEUDOCODE:
      1.) Send string to client prompting for username, wait on valid string.
      2.) Test received string against vector of usernames (linear search), if
      no match found, send string to client, username not found, reset connection.
        ELSE:
        2.1.) Store password associated with that username in temp string,
        2.2.) Send string to client prompting for password, wait on valid string.
        2.3.) Test received string against temporarily stored password, if match,
              proceed, else send error message string and reset connection.
      3.) Username and password confirmed, send welcome message string.
      4.) Randomly select a word pair from stored pairs. Send string representing
          word pair (don't send the words themselves.)
      5.) Enter while loop, wait for valid lowercase character. Test for end game
          conditions (VICTORY, NO GUESSES LEFT, CLIENT QUIT).
      6.)
 */

 //Send string to client prompting for username
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


	} //end while loop
	//this function closes an accepted connection, put it outside the loop
	close(new_fd);  /* parent doesn't need this */
	//once thread pool is implemented, we will need to return thread to pool here
  return 0;
} //end main
