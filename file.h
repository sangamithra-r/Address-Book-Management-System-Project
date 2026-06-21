/*
 * File: file.h
 * Header file for file handling operations in the Address Book System.
 * Contains function declarations for saving andnloading contacts to/from a file.
 * Provides an interface between file.c and other source files.
 *
 */
#ifndef FILE_H
#define FILE_H

#include "contact.h"

void saveContactsToFile(AddressBook *addressBook);
void loadContactsFromFile(AddressBook *addressBook);

#endif
