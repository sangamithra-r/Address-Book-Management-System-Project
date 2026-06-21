/*
* File: contact.c
* Contains all core functionalities of the Address Book system.
* This file handles all contact-related operations such as:
*   - Creating a new contact
*   - Searching contacts (by name, phone, or email)
*   - Editing existing contacts
*   - Deleting contacts
*   - Listing all contacts
*
* The module also includes:
*   - Input validation functions (name, phone number, email)
*   - Duplicate checking (unique phone and email)
*   - Retry handling for invalid inputs
*
* Dependencies:
*   - contact.h : Structure definitions and function prototypes
*   - file.h    : File handling functions (save/load contacts)
*
* Author      : SANGAMITHRA.R
* Project     : Address Book Management System
* Description : Menu-driven application to manage contacts with
*               proper validation and file persistence.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

/*
* Global variables
* Index[]: stores indices of matched contacts during search function
* Count : Stores number of matches found
* These are used across search, edit and delete function
*
*/
int Index[100];
int Count;
/*
* Function: listContacts
* Displays all contacts stored in the address book in a structured,
* tabular format for easy readability.
*
* Details:
*   - Checks whether the address book is empty before displaying.
*   - If no contacts are available, an appropriate message is shown.
*   - Prints a formatted header including:
*         Serial Number, Name, Mobile Number, Email ID
*   - Iterates through the contact array and displays each contact.
*   - Assigns a serial number (starting from 1) for user reference.
*
* Purpose:
*   - Provides a clear and organized view of all stored contacts.
*   - Helps users verify stored data before performing operations
*     like edit or delete.
*
*/
void listContacts(AddressBook *addressBook) 
{
    //Check if address book is empty
    if(addressBook->contactCount==0){
        printf("No Contact Available\n");
        return; //exit is no contacts are present
    }

    //Display table header
    printf("\tContact List\t\n");
    printf("%-10s %-20s %-15s %-50s\n","No.","Name","Mobile Number","Email Id");
    printf("-------------------------------------------------------------------------------\n");
    
    //Loop through all contacts and print details
    for(int i=0;i<addressBook->contactCount;i++){
        printf("%-10d %-20s %-15s %-50s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    
}

/*
 * Function: initialize
 * Initializes the address book before any operations are performed.
 *
 * Details:
 *   - Sets the contact count to 0 to ensure a clean initial state.
 *   - Loads existing contacts from the file into memory using
 *     loadContactsFromFile().
 *   - If the file exists and contains data, contacts are restored
 *     into the address book.
 *   - If the file does not exist or is empty, the address book
 *     remains empty and ready for new entries.
 *
 * Purpose:
 *   - Ensures that previously saved contacts are available when
 *     the program starts.
 *   - Prevents usage of uninitialized data.
 *
 */
void initialize(AddressBook *addressBook) {
    // Initialize contact count to 0
    addressBook->contactCount = 0;

    //load contacts from file (if file exists)
    loadContactsFromFile(addressBook);
}

/*
 * Function: saveAndExit
 * Saves all contacts from memory into a file and safely terminates
 * the program execution.
 *
 * Details:
 *   - Calls saveContactsToFile() to write all contact data into
 *     the storage file (contacts.txt).
 *   - Ensures that all newly added, edited, or deleted contacts
 *     are permanently stored before exiting.
 *   - Uses exit(EXIT_SUCCESS) to terminate the program immediately.
 *
 * Purpose:
 *   - Provides data persistence so that contacts are not lost
 *     after the program ends.
 *   - Ensures a safe and controlled program termination.
 */
void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

/*
* Function: createContact
* Creates and adds a new contact to the address book.
*
* Details:
*   - Checks whether the address book has reached its maximum capacity.
*   - Prompts the user to enter name, mobile number, and email ID.
*   - Validates each input using dedicated validation functions:
*         • Name validation (only alphabets, minimum length)
*         • Mobile number validation (10 digits, valid starting digit)
*         • Email validation (proper format with '@' and ".com")
*   - Ensures that the mobile number and email ID are unique
*     (no duplicates allowed).
*   - Implements a retry mechanism allowing up to 3 attempts
*     for each invalid input.
*   - If maximum attempts are exceeded, the operation is cancelled.
*   - On successful validation, stores the contact details into
*     the address book array.
*   - Increments the contact count after adding the new contact.
*
* Purpose:
*   - To safely add new contacts while maintaining data integrity
*     and preventing invalid or duplicate entries.
*/
void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */

    // Check if address book has reached maximum capacity
    if(addressBook->contactCount >= MAX_CONTACTS){
        printf("Address Book is full.\n");
        return;
    }
    
    int isvalid; // stores validation result
    int retry; // controls retry attempts

    char name[50];

    // NAME INPUT AND VALIDATION
    retry = -1;
    while(retry == -1){
        printf("Enter your Name : ");
        scanf(" %[^\n]",name);

        //Validate name (alphabets and spaces only)
        isvalid = validatename(name);
        //Handle retry attempts
        retry = attempt(isvalid);
    }
    //Exit is maximum attempts reached
    if (retry == 0){
        return;
    }

    char number[20];
    
    // PHONE NUMBER INPUT AND VALIDATION
    retry = -1;
    while(retry == -1){
        printf("Enter your Mobile number : ");
        scanf(" %[^\n]",number);
        //Validate phone number format
        isvalid = validatenumber(number);
        if(isvalid){
            //Check uniqueness only if vallid
            isvalid = UniqueNumber(addressBook,number);
        }
        retry = attempt(isvalid);
    }
    if(retry == 0){
        return;
    }

    char mail[50];

    // EMAIL ID INPUT AND VALIDATION
    retry = -1;
    while(retry == -1){
        printf("Enter your email-id : ");
        scanf(" %[^\n]",mail);
        //Validate email format
        isvalid = validatemail(mail);
        if(isvalid){
            //Check uniqueness only if valid
            isvalid = UniqueMail(addressBook,mail);
        }
        retry = attempt(isvalid);
    }
    //Exit is max attempts reached
    if(retry == 0){
        return;
    }

    //Store contact
    int index = addressBook->contactCount;
    strcpy(addressBook->contacts[index].name,name);
    strcpy(addressBook->contacts[index].phone,number);
    strcpy(addressBook->contacts[index].email,mail);

    //Increment total contact count
    addressBook->contactCount++;

    printf("Contact added successfully.\n");
}
/*
* Function: searchContact
* Searches for contacts in the address book based on user-selected criteria
* such as name, mobile number, or email ID.
*
* Details:
*   - Checks whether the address book contains any contacts.
*   - Displays a search menu allowing the user to choose:
*         • Search by Name
*         • Search by Mobile Number
*         • Search by Email ID
*         • Exit
*   - Accepts the search key from the user and validates it
*     based on the selected option.
*   - Uses a retry mechanism (maximum 3 attempts) for invalid inputs.
*   - Performs a linear search through the contact list:
*         • Name search may return multiple matches
*         • Phone and email searches return a single match (unique fields)
*   - Stores the indices of matched contacts in the global array Index[].
*   - Updates the global variable Count with the number of matches found.
*   - Displays all matching contacts in a formatted table with serial numbers.
*
* Purpose:
*   - Enables efficient retrieval of contacts based on different fields.
*   - Provides reusable search results for edit and delete operations.
*/
void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    int UserChoice; //Stores user's search option
    int isvalid,retry; //Validation and retry control
    char key[50]; //Stores search input

    //Check if address book is empty
    if(addressBook->contactCount==0){
        printf("No contacts available\n");
        return;
    }

    //Display search menu
    printf("Search:\n");
    printf("1.Search By Name\n");
    printf("2.Search By Mobile Number\n");
    printf("3.Search By Mail Id\n");
    printf("4.Exit\n");
    printf("Enter your choice : ");
    
    if (scanf("%d",&UserChoice)!=1){
        printf("Invalid input. Enter a valid number.\n");
        while(getchar()!='\n');
        return;
    }

    // Exit option
    if(UserChoice==4){
        printf("Exiting search...\n");
        return;
    }

    // Input and Validation
    retry=-1;
    while(retry==-1){
        printf("Enter Search Key:");
        scanf(" %[^\n]",key);

        //Validate input based on choice
        switch(UserChoice){
            case 1:
                isvalid=validatename(key);
                break;
            case 2:
                isvalid=validatenumber(key);
                break;
            case 3:
                isvalid=validatemail(key);
                break;
            default:
                printf("Invalid Choice\n");
                return;
        }

        // Handle retry attempts
        retry = attempt(isvalid);
    }

    //Exit is max attempts reached
    if(retry==0){
        return;
    }
    //Reset match count before search
    Count=0;

    //Search by Name - loop through all contacts(multiple matches possible)
    if(UserChoice==1){
        for(int i=0;i<addressBook->contactCount;i++){
            if(strcmp(addressBook->contacts[i].name,key)==0){
                Index[Count++]=i;    
            }
        }
    }

    //Search by Phone - loop through all contacts
    else if(UserChoice==2){
        for(int i=0;i<addressBook->contactCount;i++){
            if(strcmp(addressBook->contacts[i].phone,key)==0){
                Index[Count++]=i;
                break;
            }
        }
    }

    //Search by Email - loop through all contacts
    else if(UserChoice==3){
        for(int i=0;i<addressBook->contactCount;i++){
            if(strcmp(addressBook->contacts[i].email,key)==0){
                Index[Count++]=i;
                break;
            }
        }
    }
    // If no match found
    if(Count==0){
        printf("No matching contact found\n");
        return;
    }

    //Display results
    int serial=1;

    //Print table header
    printf("%-10s %-20s %-15s %-50s\n","No.","Name","Mobile Number","Email Id");
    printf("--------------------------------------------------------------------------\n");
    //Print matched contacts
    for(int i=0;i<Count;i++){
        printf("%-10d %-20s %-15s %-50s\n",serial++,addressBook->contacts[Index[i]].name,addressBook->contacts[Index[i]].phone,addressBook->contacts[Index[i]].email);
    }
}

/*
 * Function: editContact
 * Allows the user to modify an existing contact in the address book.
 *
 * Details:
 *   - Invokes searchContact() to locate matching contact(s) based on
 *     user-selected criteria.
 *   - Displays all matched results with serial numbers for selection.
 *   - Prompts the user to choose a specific contact to edit.
 *   - Validates the selected serial number to ensure correctness.
 *   - Provides edit options:
 *         • Edit Name
 *         • Edit Mobile Number
 *         • Edit Email ID
 *         • Exit
 *   - Accepts new input for the selected field and validates it:
 *         • Name → alphabets and minimum length
 *         • Mobile → 10 digits, valid starting digit, uniqueness check
 *         • Email → proper format, uniqueness check
 *   - Uses retry mechanism (maximum 3 attempts) for invalid inputs.
 *   - Updates the selected contact field with validated data.
 */
void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    // Perform search to get matching contacts
    searchContact(addressBook);

    // If no results found, exit
    if(Count==0){
        return;
    }
    int serialNo;

    // Ask user to choose which contact to edit
    printf("Enter the serial number to edit : ");

    if (scanf("%d",&serialNo) !=1){
        printf("Invalid input. Enter a valid number.\n");
        while(getchar()!='\n');
        return;
    }

    // Validate serial number
    if(serialNo<1 || serialNo>Count){
        printf("Error : Invalid serial number.\n");
        return;
    }
    int UserChoice;

    // Display edit options
    printf("Edit:\n");
    printf("1.Edit by Name\n");
    printf("2.Edit by Mobile Number\n");
    printf("3.Edit by Mail Id\n");
    printf("4.Exit\n");
    printf("Enter your choice: ");
    if (scanf("%d",&UserChoice)!=1){
        printf("Invalid input.Enter a valid number.\n");
        while(getchar()!='\n');
        return;
    }
    // Exit option
    if(UserChoice==4){
        printf("Exiting Edit\n");
        return;
    }
    char input[50];
    int retry,isvalid;

    //INPUT AND VALIDATION
    retry=-1;
    while(retry==-1){
        printf("Enter Edit value:");
        scanf(" %[^\n]",input);
        switch(UserChoice){
            case 1:
                isvalid=validatename(input);
                break;
            case 2:
                isvalid=validatenumber(input);
                // Ensure new number is unique
                if(isvalid){
                    isvalid=UniqueNumber(addressBook,input);
                }
                break;
            case 3:
                isvalid=validatemail(input);
                // Ensure new email is unique
                if(isvalid){
                    isvalid=UniqueMail(addressBook,input);
                }
                break;
            default:
                printf("Invalid Choice\n");
                return;
        }
        retry = attempt(isvalid);
    }
    // Exit if max attempts reached
    if(retry==0){
        return;
    }
    // Update contact
    if(UserChoice==1){
        strcpy(addressBook->contacts[Index[serialNo-1]].name,input);
    }
    else if(UserChoice==2){
        strcpy(addressBook->contacts[Index[serialNo-1]].phone,input);
    }
    else if(UserChoice==3){
        strcpy(addressBook->contacts[Index[serialNo-1]].email,input);
    }
    printf("Contact Updated Successfully\n");
}

/*
 * Function: deleteContact
 * Deletes a contact from the address book.
 *
 * Details:
 *   - Invokes searchContact() to locate matching contact(s).
 *   - Displays all matched contacts with serial numbers.
 *   - Provides a delete menu with an option to proceed or exit.
 *   - Prompts the user to select a specific contact to delete.
 *   - Validates the entered serial number to ensure it is within range.
 *   - Optionally asks for user confirmation before deletion.
 *   - Deletes the selected contact by shifting subsequent elements
 *     in the array to the left (to maintain continuity).
 *   - Decrements the total contact count after deletion.
 *
 */
void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    // Perform search to locate contact(s)
    searchContact(addressBook);

    // If no results found, exit
    if(Count==0){
        return;
    }
    int serialno;
    // Ask user to choose contact to delete
    printf("Enter the serial number to delete: ");

    if (scanf("%d",&serialno)!=1){
        printf("Invalid input. Enter a valid number.\n");
        while(getchar()!='\n');
        return;
    }
    // Validate serial number
    if(serialno<1 || serialno>Count){
        printf("Error : Invalid serial no.\n");
        return;
    }

    //confirms if the user wants to delete the contact
    char confirm;
    printf("Confirm delete (y/n): ");
    scanf(" %c", &confirm);

    if(confirm!='y' && confirm!='Y'){
        printf("Deletion cancelled.\n");
        return;
    }

    // Get actual index from global array and shift contacts left
    for(int i=Index[serialno-1];i<addressBook->contactCount-1;i++){
        addressBook->contacts[i]=addressBook->contacts[i+1];
    }
    // Reduce total count
    addressBook->contactCount--;
    printf("Contact deleted successfully.\n");
   
}
/*
 * Function: attempt
 * Controls the retry mechanism for user input validation.
 *
 * Details:
 *   - Maintains a static counter to track the number of remaining attempts.
 *   - Allows a maximum of 3 attempts for invalid input.
 *   - If the input is valid:
 *         • Resets the attempt counter to 3
 *         • Returns success immediately
 *   - If the input is invalid:
 *         • Decrements the attempt counter
 *         • Displays remaining attempts (if any)
 *   - When attempts reach zero:
 *         • Displays a termination message
 *         • Resets the counter for future operations
 *         • Stops further retries for the current input
 *
 * Purpose:
 *   - Prevents infinite loops caused by continuous invalid input.
 *   - Enhances user experience by limiting retry attempts.
 *   - Ensures controlled flow of input handling across functions.
 *
 * Input:
 *   - isvalid : Integer flag (1 = valid input, 0 = invalid input)
 *
 * Output:
 *   - Returns:
 *         1  → input is valid, proceed further
 *        -1  → input is invalid, retry allowed
 *         0  → maximum attempts reached, stop operation
 *
 */
int attempt(int isvalid){
    static int attempts = 3; // persists across function calls
    // If input is valid → reset attempts
    if(isvalid){
        attempts = 3;
        return 1;
    }
    // Reduce attempt count
    attempts--;
    // If attempts still remain
    if(attempts>=1){
        printf("Attempts left : %d\n",attempts);
        return -1;
    }
    else{
        // No attempts left
        printf("Maximum attempts reached.\n");
        // Reset for future use
        attempts = 3;
        return 0;
    }
}
/*
 * Function: validatename
 * Validates the name entered by the user based on predefined rules.
 *
 * Details:
 *   - Ensures the name contains at least 4 characters.
 *   - Allows only alphabets (A–Z, a–z) and spaces.
 *   - Rejects any digits or special characters.
 *   - Displays appropriate error messages indicating the reason
 *     for invalid input.
 *
 * Purpose:
 *   - Ensures that only meaningful and valid names are stored
 *     in the address book.
 *   - Maintains data consistency and avoids incorrect entries.
 *
 */
int validatename(char name[]){
    // Checks if name contains at least 4 characters
    if(strlen(name)<4){
        printf("Error : Name must contain at least 4 characters.\n");
        return 0;
    }
    for(int i=0;name[i];i++){
        // Allow only alphabets and space
        if(!(isalpha(name[i]) || name[i] == ' ')){
            printf("Error : Name should contain only alphabets.\n");
            return 0;
        }
        
    }
    return 1;
}

/*
 * Function: validatenumber
 * Validates the mobile number entered by the user based on
 * predefined rules.
 *
 * Details:
 *   - Ensures that the mobile number contains only numeric digits (0–9).
 *   - Checks that the length of the number is exactly 10 digits.
 *   - Verifies that the first digit is between 6 and 9.
 *   - Displays appropriate error messages if any validation fails,
 *     clearly indicating the reason.
 *
 * Purpose:
 *   - Ensures correctness and consistency of mobile number data.
 *   - Prevents invalid or improperly formatted numbers from being stored.
 *
 *
 */
int validatenumber(char number[]){
    for(int i=0;number[i];i++){
        // Check all characters are digits
        if(!(isdigit(number[i]))){
            printf("Error : Mobile Number must contain only digits.\n");
            return 0;
        }
    }
    // Check length = 10
    if(strlen(number)!=10){
        printf("Error : Mobile Number must contain exactly 10 digits.\n");
        return 0;
    }
    // Check starting digit (6–9)
    if(number[0]<'6' || number[0]>'9'){
        printf("Error : Mobile Number First digit must be between 6 and 9.\n");
        return 0;
    }
    
    return 1;
}

/*
 * Function: UniqueNumber
 * Checks whether the given mobile number already exists in the address book.
 *
 * Details:
 *   - Iterates through all stored contacts in the address book.
 *   - Compares the input mobile number with existing numbers using strcmp().
 *   - If a match is found, it indicates a duplicate entry.
 *   - Displays an appropriate error message when a duplicate is detected.
 *   - If no match is found, the number is considered unique.
 *
 * Purpose:
 *   - Prevents duplicate mobile numbers in the address book.
 *   - Ensures data integrity and avoids redundancy.
 *
 *
 */
int UniqueNumber(AddressBook *addressBook, char number[]){
    for(int i=0;i<addressBook->contactCount;i++){
        // Compare with existing numbers
        if(strcmp(addressBook->contacts[i].phone,number)==0){
            printf("Error : Mobile number already exists.\n");
            return 0;
        }
    }
    return 1;
}

/*
 * Function: validatemail
 * Validates the email ID entered by the user based on multiple rules.
 *
 * Details:
 *   - Ensures the email starts with a lowercase alphabet.
 *   - Checks that the email contains exactly one '@' symbol.
 *   - Verifies that only allowed characters are used:
 *         • Lowercase alphabets (a–z)
 *         • Digits (0–9)
 *         • Special symbols '@' and '.'
 *   - Validates the local part (before '@'):
 *         • Must contain only lowercase alphabets and digits
 *   - Ensures that a '.' (dot) is present after the '@' symbol.
 *   - Ensures there is at least one character between '@' and '.' 
 *     (valid domain name exists).
 *   - Validates the domain name (between '@' and '.'):
 *         • Must contain only alphabets or digits
 *   - Ensures the email ends strictly with ".com".
 *   - Prevents any extra characters after ".com".
 *
 * Purpose:
 *   - Ensures correct email format and prevents invalid entries.
 *   - Maintains consistency and reliability of stored email data.
 *
 */
int validatemail(char mail[])
{
    int len = strlen(mail); // Get length of email

    // Check starting character
    if(!islower(mail[0])){
        printf("Error: Mail must start with a lowercase alphabet.\n");
        return 0;
    }
    // Count '@' and store its index
    int atcount = 0, atindex = -1;
    for(int i = 0; mail[i]; i++){
        if(mail[i] == '@'){
            atcount++;
            atindex = i;
        }
    }
    // Validate '@' presence
    if(atcount == 0){
        printf("Error: Mail must contain '@'.\n");
        return 0;
    }
    // Ensure only one '@'
    if(atcount > 1){
        printf("Error: Mail must contain only one '@'.\n");
        return 0;
    }

    int dotfound = 0;
    // Check at least one '.' exists
    for(int i=0; mail[i]; i++){
        if(mail[i]=='.'){
            dotfound = 1;
            break;
        }
    }
    if(!dotfound){
        printf("Error: Mail must contain at least one '.'.\n");
        return 0;
    }  
    // Check allowed characters  
    for(int i = 0; mail[i]; i++){
        if(!(islower(mail[i]) || isdigit(mail[i]) || mail[i]=='@' || mail[i]=='.')){
            printf("Error: Invalid character in email.\n");
            return 0;
        }
    }
    // Validate local part (before '@')
    for(int i = 0; i < atindex; i++){
        if(!(islower(mail[i]) || isdigit(mail[i]))){
            printf("Error: Invalid characters before '@'.\n");
            return 0;
        }
    }
    // Check email ends with ".com"
    if(len < 4 ||
       mail[len-4] != '.' ||
       mail[len-3] != 'c' ||
       mail[len-2] != 'o' ||
       mail[len-1] != 'm'){
        printf("Error: Mail must end with '.com'.\n");
        return 0;
    }

    int dotindex = -1;
    // Find '.' after '@'
    for(int i = atindex + 1; mail[i]; i++){
        if(mail[i] == '.'){
            dotindex = i;
            break;
        }
    }
    // Ensure '.' is after '@'
    if(dotindex == -1){
        printf("Error: '.' must be present after '@'.\n");
        return 0;
    }

    // Ensure domain name exists  
    if(dotindex == atindex + 1){
        printf("Error: Domain name missing between '@' and '.'.\n");
        return 0;
    }

    // Validate domain name (between '@' and '.')   
    for(int i = atindex + 1; i < dotindex; i++){
        if(!(islower(mail[i]) || isdigit(mail[i]))){
            printf("Error: Invalid domain name.\n");
            return 0;
        }
    }

    return 1;
}

/*
 * Function: UniqueMail
 * Checks whether the given email ID already exists in the address book.
 *
 * Details:
 *   - Iterates through all contacts stored in the address book.
 *   - Compares the input email ID with existing email IDs using strcmp().
 *   - If a match is found, it indicates a duplicate email entry.
 *   - Displays an appropriate error message when a duplicate is detected.
 *   - If no match is found, the email ID is considered unique.
 *
 * Purpose:
 *   - Prevents duplicate email IDs from being stored.
 *   - Ensures data integrity and avoids redundancy in contact records.
 *
 */
int UniqueMail(AddressBook *addressBook,char mail[]){
    // Compare with existing emails
    for(int i=0;i<addressBook->contactCount;i++){
        if(strcmp(addressBook->contacts[i].email,mail)==0){
            printf("Error : Mail Id already exists.\n");
            return 0;
        }
    }
    return 1;
}