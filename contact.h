/*
 * File: contact.h
 * Header file for the Address Book System.
 * Contains:
 * - Structure definitions for Contact and AddressBook
 * - Macro definitions
 * - Function declarations for all contact operations
 * - Validation and uniqueness function prototypes
 *
 * This file is included in source files to provide a common interface 
 * and ensure modular programming.
 *
 */

#ifndef CONTACT_H
#define CONTACT_H
/*
 * Macro: MAX_CONTACTS
 * Defines the maximum number of contacts that can be stored in the address book.
 *
 */
#define MAX_CONTACTS 100

/*
 * Structure: Contact
 * Stores individual contact details.
 * Fields:
 *   name  → Contact name
 *   phone → Mobile number
 *   email → Email ID
 *
 */
typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

/*
 * Structure: AddressBook
 * Stores all contacts and total count.
 * Fields:
 *   contacts      → Array of Contact structures
 *   contactCount  → Number of stored contacts
 *
 */
typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;

// FUNCTION DECLARATIONS

/* Create a new contact */
void createContact(AddressBook *addressBook);

/* Search for contacts */
void searchContact(AddressBook *addressBook);

/* Edit existing contact */
void editContact(AddressBook *addressBook);

/* Delete a contact */
void deleteContact(AddressBook *addressBook);

/* Display all contacts */
void listContacts(AddressBook *addressBook);

/* Initialize address book */
void initialize(AddressBook *addressBook);

/* Save contacts and exit program */
void saveAndExit(AddressBook *addressBook);

// Validation functions

/* Controls retry attempts */
int attempt(int isvalid);

/* Validate name format */
int validatename(char name[]);

/* Validate mobile number */
int validatenumber(char number[]);

/* Validate email format */
int validatemail(char mail[]);

// Uniqueness check functions

/* Check duplicate phone number */
int UniqueNumber(AddressBook *addressBook, char number[]);
/* Check duplicate phone number */
int UniqueMail(AddressBook *addressBook,char mail[]);




#endif
