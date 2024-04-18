#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "online_shop2.h"

struct Client loggedInPerson;

int main(){


    char name[MAX_NAME_LENGTH];
    char surname[MAX_SURNAME_LENGTH];

    int choice1;
    do {
        printf("1. Log in\n");
        printf("2. Create account\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice1);

        switch(choice1){
            case 1:
                login(&loggedInPerson);
                break;

            case 2:
                printf("Enter your name: ");
                scanf("%s",name);
                printf("Enter your surname: ");
                scanf("%s",surname);
                createAccount(name,surname);
                break;

            case 3:
                printf("Exiting...\n");
                return 0;
                break;
        }
    }while(choice1<4 && choice1>0);

return 0;
}
