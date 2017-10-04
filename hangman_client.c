#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

//define a structure for linked list to hold usernames and passwords

struct list {
    char *username;
    char *password;
    // unsigned int *highscore;         //might not be needed?
    struct list *next;
};
typedef struct list userIDs;
//**************************

//write function to take contents of Authentication.txt to linked list
void AuthenticationImport {
FILE *users_list;
char usernameArray[8];
char passwordArray[6];
userIDs *current, *head;

head = current = NULL;
//open Authentication.txt in read mode
users_list = fopen("Authentication.txt", "r");

if (users_list == NULL) {
  perror("Error opening file");
  return(-1);
}

//copy values in Authentication.txt to usernameArray and passwordArray, remove spaces somehow
while(fgets(usernameArray, sizeof(usernameArray), users_list)) {
    userIDs *node = malloc(sizeof(userIDs));
    if (userIDs != NULL) {
    node->username = strdup(usernameArray);
    node->password = strdup(passwordArray);
    node->next = NULL;
    }
    else {
      free(userIDs);
      userIDs = NULL;
    }
    if(head == NULL){
        current = head = node;
    } else {
        current = current->next = node;
    }
}
fclose(users_list);

//go through the linked list and printf for test
for(current = head; current; current = current->next){
    printf("%s, %s", current->username, current->password);
}

return 0;
}
