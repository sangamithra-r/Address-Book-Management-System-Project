/*
 * File: file.c
 * Contains file handling functions for the Address Book Management System.
 * Functions included:
 * - saveContactsToFile() → saves contacts to file
 * - loadContactsFromFile() → loads contacts from file
 * Responsible for data persistence using text file.
 *
 */

#include <stdio.h>
#include "file.h"

/*
 * Function: saveContactsToFile
 * Saves all contacts from the address book into a file named "contacts.txt".
 * Format:
 *   #<contactCount>
 *   name,phone,email
 *
 */
void saveContactsToFile(AddressBook *addressBook) {
    // Open file in write mode (overwrites old data)
    FILE *fp = fopen("contacts.txt","w");

    // Check if file opened successfully
    if(fp==NULL){
        printf("Error opening file\n");
        return;
    }

    // Write number of contacts
    fprintf(fp,"#%d\n",addressBook->contactCount);

    // Write each contact in CSV format
    for(int i=0;i<addressBook->contactCount;i++){
        fprintf(fp,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }

    // Close file
    fclose(fp);
}

/*
 * Function: loadContactsFromFile
 * Loads contacts from "contacts.txt" into memory.
 * If file does not exist or is invalid, initializes
 * address book as empty.
 *
 */
void loadContactsFromFile(AddressBook *addressBook) {
    // Open file in read mode
    FILE *fp = fopen("contacts.txt","r");

    // If file does not exist → start fresh
    if(fp==NULL){
        addressBook->contactCount=0;
        return;
    }

    // Read contact count safely
    if (fscanf(fp,"#%d\n",&addressBook->contactCount) != 1) {
        addressBook->contactCount = 0;
        fclose(fp);
        return;
    }
    
    // Read each contact
    for(int i=0;i<addressBook->contactCount;i++){
        fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }

    // Close file
    fclose(fp);
}
