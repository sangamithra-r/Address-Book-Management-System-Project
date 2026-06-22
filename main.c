/*
Project: Address Book Management System
Author Name: SANGAMITHRA.R

Description: 
* A menu-driven application to manage contacts efficiently.
* The system allows users to create, search, edit, delete,and display contacts in a structured format.
*
* It performs strict validation for all input fields:
*    - Name (only alphabets, minimum length)
*    - Phone number (10 digits, starts with 6–9)
*    - Email ID (proper format with '@' and ".com")
*
* The application ensures data integrity by preventing duplicate phone numbers and email IDs.             
* It supports searching using multiple fields (name, phone,or email) and handles both single and multiple matches.              
* Editing and deletion operations are performed safely by  selecting a specific contact from search results.
* A retry mechanism is implemented to limit invalid inputs,improving user interaction and robustness.              
* All contacts are stored in a file, enabling persistent storage so that data is retained between program runs.               
*
* The program is modular in design, separating logic into different functions for better readability, maintenance,
* and scalability.             
*                          
*
*/

/*
* File: main.c
*
*/

#include <stdio.h>
#include "contact.h"

/*
 * Function: main
 * Entry point of the program.
 * Displays menu, takes user input, and calls appropriate functions based on user choice.
 *
 */
int main() 
{
    int choice; // stores user choice
    AddressBook addressBook; // main data structure
    initialize(&addressBook); // Initialize the address book

    // Menu-driven loop
    do 
    {
        // Display menu
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save and Exit\n");		
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice)!=1){
            printf("Invalid input. Please enter a number.\n");
            while(getchar()!='\n');
            return 0;
        }

        // Perform operation based on choice
        switch (choice) 
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                // Save data and terminate program
                saveAndExit(&addressBook);
                break;
            case 7:
                printf("Exiting without saving...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7); // loop continues until user selects Exit
    
    return 0;
}
