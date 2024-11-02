#include "contact.h"
#include <string.h>
#include<ctype.h>
#include<stdio.h>
int contactCount;
AddressBook addressBook_var;
extern Contact contacts[];
int size;
extern Contact dummyContacts[];
void printing(){
    for(int i=0;i<65;i++){
        printf("\033[1;37m-\033[0m");
    }
    printf("\n");
}

void listContacts(AddressBook *addressBook){
    int count=1;
    // Sort contacts based on the chosen criteria
    printf("%50s","\033[1;33mLIST OF CONTACTS\033[0m\n");
    printing();
    printf("%-5s %-20s%-20s%-20s\n","\033[1;33mS.No","Name","Phone No","Email\033[0m");
    printing();
    for(int i=0;i<contactCount;i++){
        printf("\033[1;37m% -5d\033[1;36m%-20s%-20s%-20s\033[0m\n",count++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    printing();
}
void initialize(AddressBook *addressBook){
    /*contactCount=size;
    for (int i = 0; i < contactCount; i++) 
    {
        addressBook->contacts[i]=dummyContacts[i];
    }*/
    contactCount=0;
    // Open the file in read mode
    FILE *fptr = fopen("data.txt", "r");
    // Check if the file was opened successfully
    if (fptr == NULL) {
        printf("\033[1;31mFile not Exist\033[0m\n");
        return; // Exit the function if file cannot be opened
    }
    for(int i=0;(!feof(fptr));i++){
        fscanf(fptr,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        contactCount++;
    }
    // Close the file after reading
    fclose(fptr);
}
// Function to save contacts from the AddressBook to a file
void saveContactsToFile(AddressBook *addressBook){
    // Open the file in read/update mode ("w")
    FILE *fptr = fopen("data.txt", "w");
    // Check if the file was opened successfully
    if (fptr == NULL) {
        printf("\033[1;31mError opening file for writing.\033[0m\n");
        return; // Exit the function if file cannot be opened
    }
    // Loop through each contact and write their details to the file
    for (int i = 0; i < contactCount; i++) {
        fprintf(fptr, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    // Close the file after writing
    fclose(fptr);
    printf("\033[1;32mSave Contacts Successfully\033[0m\n");
}
void createContact(AddressBook *addressBook){
    /* Define the logic to create a Contacts */
    char name[50];
    p1:
    printf("\033[1;33mEnter name: \033[0m");
    scanf(" %[^\n]",name);
    int i=0;
    while(name[i]){
        if((name[i]>='A'&&name[i]<='Z')||(name[i]>='a'&&name[i]<='z')||name[i]==' '){
            i++;
        }
        else{
            printf("\033[1;31mEnter name only alphabets\033[0m\n");
            goto p1;
        }
    }
    for(int i=0;i<contactCount;i++){
        if((strcasecmp(addressBook->contacts[i].name,name))==0){
            printf("\033[1;31mEntered name is already exist\033[0m\n");
            goto p1;
        }
    }
    strcpy(addressBook->contacts[contactCount].name,name);
    char phone[50];
    p2:
    printf("\033[1;33mEnter phone no: \033[0m");
    scanf("%s",phone);
    int count=0;
    for(int i=0;phone[i];i++){
        if(isdigit(phone[i])==0){
            printf("\033[1;31mEnter only numbers\033[0m\n");
            goto p2;
        }
        else{
            count++;
        }
    }
    if(count!=10){
        printf("\033[1;31mEnter 10 diggits phone no\033[0m\n");
        goto p2;
    }
    for(int i=0;i<contactCount;i++){
        if((strcmp(addressBook->contacts[i].phone,phone))==0){
            printf("\033[1;31mEntered phone no is already exist\033[0m\n");
            goto p2;
        }
    }
    strcpy(addressBook->contacts[contactCount].phone,phone);
    char email[50];
    p3:
    printf("\033[1;33mEnter email: \033[0m");
    scanf("%s",email);
    if(strchr(email,'@')==0 || strstr(email,".com")==0 || strstr(email,"@")+1>=strstr(email,".com")){
        puts("\033[1;31mEntered email pattern is not currect\033[0m\n");
        goto p3;
    }
    for(int i=0;i<contactCount;i++){
        if((strcmp(addressBook->contacts[i].email,email))==0){
            puts("\033[1;31mEntered email is already exist\033[0m\n");
            goto p3;
        }           
    }
    strcpy(addressBook->contacts[contactCount++].email,email);
    puts("\033[1;32mContact Created Successfully\033[0m\n");
}
void searchContact(AddressBook *addressBook){
    /* Define the logic to search a Contacts */
    char name[50];
    char phone[20];
    char email[50];
    int ch;
    a0:
    do{
        /* code */
        printf("Menu:\n");
        printf("1.name\n2.phone no\n3.email\n4.Exit\n");
        printf("Choice: ");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                goto a1;
                break;
            case 2:
                goto a2;
                break;
            case 3:
                goto a3;
                break;
            case 4:
                break;
            default:
                printf("\033[1;31mEnter currect choice\033[0m\n");
                goto a0;
        }
    } while (ch!=4);
    goto a4;
    a1:
    printf("\033[1;33mEnter name : \033[0m");
    scanf(" %[^\n]",name);
    int count=1;
    for(int i=0;i<contactCount;i++){
        if((strstr(addressBook->contacts[i].name,name))!=0)
        printf("%d %-20s%-20s%-20s\n",count++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    if(count==1){
        printf("\033[1;31mEntered Name not exist in the list\033[0m\n");
        goto a0;
    }
    else{
        goto a0;
    }
    a2:
    printf("\033[1;33mEnter phone no : \033[0m");
    scanf("%s",phone);
    int count1=1;
    for(int i=0;i<contactCount;i++){
        if((strstr(addressBook->contacts[i].phone,phone))!=0)
        printf("%d %-20s%-20s%-20s\n",count1++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    if(count1==1){
        printf("\033[1;31mEntered Phone number not exist in the list\033[0m\n");
        goto a0;
    }
    else{
        goto a0;
    }
    a3:
    printf("\033[1;33mEnter Email : \033[0m");
    scanf("%s",email);
    int count2=1;
    for(int i=0;i<contactCount;i++){
        if((strstr(addressBook->contacts[i].email,email))!=0)
        printf("%d %-20s%-20s%-20s\n",count2++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    if(count2==1){
        printf("\033[1;31mEntered email not exist in the list\033[0m\n");
        goto a0;
    }
    a4:
    printf("\033[1;37mexit search contact\033[0m\n");
}
void editContact(AddressBook *addressBook) {
    /* Define the logic to edit a Contacts */
    char name[50];
    char phone[20];
    char email[50];
    int ch;
    a0:
    do{
        /* code */
        printf("Menu:\n");
        printf("1.name\n2.phone no\n3.email\n4.Exit\n");
        printf("Choice: ");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                goto a1;
                break;
            case 2:
                goto a2;
                break;
            case 3:
                goto a3;
                break;
            case 4:
                break;    
            default:
                printf("\033[1;31mEnter currect choice\033[0m\n");
                goto a0;
        }
    } while (ch!=4);
    goto a4;
    a1:
    printf("\033[1;33mEnter old name : \033[0m");
    scanf(" %[^\n]",name);
    int count1=1;
    for(int i=0;i<contactCount;i++){
        if((strstr(addressBook->contacts[i].name,name))!=0){
            printf("%d %-20s%-20s%-20s\n",count1++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
    }
    if(count1==1){
        printf("\033[1;31mEntered Name not exist in the list\033[0m\n");
        goto a1;
    }
    else{
        printf("\033[1;33mEnter name in the above list: \033[0m");
        scanf(" %[^\n]",&name);
        for(int j=0;j<contactCount;j++){
            if((strcmp(addressBook->contacts[j].name,name))==0){
                e1:
                printf("\033[1;33mEnter New name: \033[0m");
                scanf(" %[^\n]",&name);
                int i=0;
                while(name[i]){   
                    if((name[i]>='A'&&name[i]<='Z')||(name[i]>='a'&&name[i]<='z')||name[i]==' '){
                        i++;
                    }
                    else{
                        printf("\033[1;31mEnter name only alphabets\033[0m\n");
                        goto e1;
                    }
                }
                strcpy(addressBook->contacts[j].name,name);
                printf("\033[1;32mEntered Name successfully updated\033[0m\n");
                goto a0;
            }
        }
    }
    goto a1;
    a2:
    printf("\033[1;33mEnter old 10 digits phone number: \033[0m");
    scanf(" %[^\n]",phone);
    int count2=0;
    for(int i=0;i<contactCount;i++){
        if((strcmp(addressBook->contacts[i].phone,phone))==0){
            //char name1[50];
            printf("\033[1;33mEnter New phone number: \033[0m");
            scanf(" %[^\n]",&phone);
            strcpy(addressBook->contacts[i].phone,phone);
            count2=1;
            break;
        }
    }
    if(count2==0){
        printf("\033[1;31mEntered phone number not exist in the list\033[0m\n");
        goto a2;
    }
    else{
        printf("\033[1;32mEntered phone number successfully updated\033[0m\n");
        goto a0;
    }
    a3:
    printf("\033[1;33mEnter old email: \033[0m");
    scanf(" %[^\n]",email);
    int count3=0;
    for(int i=0;i<contactCount;i++){
        if((strcmp(addressBook->contacts[i].email,email))==0){
            //char name1[50];
            printf("\033[1;33mEnter New email: \033[0m");
            scanf(" %[^\n]",&name);
            strcpy(addressBook->contacts[i].email,email);
            count3=1;
            break;
        }
    }
    if(count3==0){
        printf("\033[1;31mEntered Email not exist in the list\033[0m\n");
        goto a3;
    }
    else{
        printf("\033[1;32mEntered Email successfully updated\033[0m\n");
        goto a0;
    }
    a4:
    printf("\033[1;37mexit edit contact\033[0m\n");
}
void deleteContact(AddressBook *addressBook) {
    /* Define the logic to delete a Contacts */
    char name[50];
    char phone[20];
    char email[50];
    int ch;
    a0:
    do{
        /* code */
        printf("Menu:\n");
        printf("1.name\n2.phone no\n3.email\n4.Exit\n");
        printf("Choice: ");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                goto a1;
                break;
            case 2:
                goto a2;
                break;
            case 3:
                goto a3;
                break;
            case 4:
                break;    
            default:
                printf("\033[1;31mEnter currect choice\033[0m\n");
                goto a0;
        }
    } while (ch!=4);
    goto a4;
    a1:
    printf("\033[1;33mEnter name : \033[0m");
    scanf(" %[^\n]",name);
    int count1=1;
    for(int i=0;i<contactCount;i++)
    {
        if((strstr(addressBook->contacts[i].name,name))!=0){
            printf("%d %-20s%-20s%-20s\n",count1++,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
    }
    if(count1==1){
        printf("\033[1;31mEntered Name not exist in the list\033[0m\n");
        goto a0;
    }
    else{
        printf("\033[1;33mEnter Name in the above list: \033[0m");
        a5:
        scanf(" %[^\n]",&name);
        for(int i=0;i<contactCount;i++){
            if((strcmp(addressBook->contacts[i].name,name))==0){
                int count=i;
                for(int j=i+1;j<=contactCount;j++){
                    addressBook->contacts[count++]=addressBook[0].contacts[j];
                    count1=1;
                }
                contactCount--;
                break;
            }
        }
        if(count1==1){
            printf("\033[1;32mContact Successfully deleted\033[0m\n");
            goto a0;
        }
        else{
            printf("\033[1;31mEntered name is wrong\033[0m\n");
            printf("\033[1;33mSo please enter correct name in the list:\033[0m");
            goto a5;
        }
    }
    a2:
    printf("\033[1;33mEnter phone number: \033[0m");
    scanf("%s",phone);
    int count2=0;
    for(int i=0;i<contactCount;i++){
        if((strcmp(addressBook->contacts[i].phone,phone))==0){
            int count=i;
            for(int j=i+1;j<=contactCount;j++){
                addressBook->contacts[count++]=addressBook->contacts[j];
                count2=1;
            }
            contactCount--;
            break;
        }
    }
    if(count2==0){
        printf("\033[1;31mEntered Phone number not exist in the list\033[0m\n");
        goto a0;
    }
    else{
        printf("\033[1;32mcontact successfully deleted\033[0m\n");
        goto a0;
    }
    a3:
    printf("\033[1;33mEnter email: \033[0m");
    scanf("%s",email);
    int count3=0;
    for(int i=0;i<contactCount;i++){
        if((strcmp(addressBook->contacts[i].email,email))==0){
            int count=i;
            for(int j=i+1;j<=contactCount;j++){
                addressBook->contacts[count++]=addressBook->contacts[j];
                count3=1;
            }
            contactCount--;
            break;
        }
    }
    if(count3==0){
        printf("\033[1;31mEntered email not exist in the list\033[0m\n");
        goto a0;
    }
    else{
        printf("\033[1;32mcontact successfully deleted\033[0m\n");
        goto a0;
    }
    a4:
    printf("\033[1;37mexit delete contact\033[0m\n");
}