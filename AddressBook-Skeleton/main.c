#include "contact.h"

int main()
{
    /*all function calls should be inside the main*/
    // Declare an AddressBook            
    AddressBook addressBook_var;  
    // Initialize the address book
    initialize(&addressBook_var);
    // Variable to store user menu choice       
    int choice;
    do
    {
        /* code */
        printf("\033[1;31mMenu : \n");
        printf("\033[1;37m1. Create contact\n2. Delete contact\n3. Search contact\n4. Edit contact\n5. List contact\n6. Save contacts\n7. Exit\n");
        printf("\033[1;35mChoice : \033[0m");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                //creat contact
                createContact(&addressBook_var); 
                break;
            case 2:
                //delete contact
                deleteContact(&addressBook_var);
                break;
            case 3:
                //search contact
                searchContact(&addressBook_var);
                break;
            case 4:
                //edit contact
                editContact(&addressBook_var);
                break;
            case 5:
                //list contact
                listContacts(&addressBook_var); 
                break; 
            case 6:
                //save contact
                saveContactsToFile(&addressBook_var);
                break;     
            case 7:
                //exit main
                break; 
            default:
                printf("Invalid choice please try again.\n");

        }
    } while (choice!=7);
    return 0;
}