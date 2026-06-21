# Address Book Management System

## Overview
The Address Book Management System is a C-based menu-driven application designed to manage contact information efficiently. The application allows users to create, search, edit, delete, and display contacts while ensuring data integrity through strict validation and duplicate detection mechanisms.
The project uses structures and file handling to maintain contact records and provides persistent storage, allowing data to be retained across multiple program executions.


## Supported Features

✅ Create Contact
✅ Search Contact
✅ Edit Contact
✅ Delete Contact
✅ Display All Contacts


## Problem Statement
Managing contact information manually becomes inefficient as the number of contacts increases. A reliable system is required to store, retrieve, update, and delete contact records while ensuring that invalid or duplicate entries are not added.
This project addresses these challenges by providing a menu-driven contact management system with robust validation, duplicate checking, and persistent file storage.


## File Storage

All contact information is stored in: contacts.txt
Data is automatically loaded during initialization and can be saved before exiting the application.

Example File Format:
#2
John,9876543210,john@gmail.com
Ram,9876543211,ram@gmail.com


## Project Structure
Address-Book-Management-System/ │ ├── main.c ├── contact.c ├── contact.h ├── file.c ├── file.h ├── README.md ├── contacts.txt 


## Compilation
gcc main.c contact.c file.c -o addressbook

## Execution
./addressbook


## Sample Execution

### Main Menu

Address Book Menu:

1. Create Contact
2. Search Contact
3. Edit Contact
4. Delete Contact
5. List All Contacts
6. Save and Exit
7. Exit

Enter your choice:

### Create Contact

Enter Name : John
Enter Mobile Number : 9876543210
Enter Email ID : john@gmail.com

Contact Added Successfully


### Search Contact

Enter Name : John

Name : John
Phone : 9876543210
Email : john@gmail.com


## Skills Demonstrated

* C Programming
* Structures and Arrays
* File Handling
* Data Validation
* String Manipulation
* Search Algorithms
* CRUD Operations
* Command Line Interface Design
* Modular Programming
* Software Design Principles


## Author

Sangamithra R

Electronics & Communication Engineering Graduate

GitHub: https://github.com/sangamithra-r
