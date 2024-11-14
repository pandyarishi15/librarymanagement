#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

struct Book {
    string title;
    string author;
    int id;
    bool isIssued;
    string dueDate;
    Book* next;
};

class Library {
private:
    Book* head;

public:
    Library() {
        head = nullptr;
    }

    void addBook(string title, string author, int id) {
        Book* newBook = new Book();
        newBook->title = title;
        newBook->author = author;
        newBook->id = id;
        newBook->isIssued = false;
        newBook->dueDate = "";
        newBook->next = nullptr;

        if (head == nullptr) {
            head = newBook;
        } else {
            Book* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newBook;
        }
        cout << "Book added: " << title << " by " << author << endl;
    }

    void removeBook(int id) {
        Book* current = head;
        Book* previous = nullptr;

        while (current != nullptr) {
            if (current->id == id) {
                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                cout << "Book removed with ID: " << id << endl;
                return;
            }
            previous = current;
            current = current->next;
        }
        cout << "Book with ID: " << id << " not found." << endl;
    }

    void issueBook(int id) {
        Book* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                if (!current->isIssued) {
                    current->isIssued = true;
                    current->dueDate = getCurrentDate(14);
                    cout << "Book issued: " << current->title << ", Due date: " << current->dueDate << endl;
                } else {
                    cout << "Book already issued: " << current->title << endl;
                }
                return;
            }
            current = current->next;
        }
        cout << "Book with ID: " << id << " not found." << endl;
    }

    void returnBook(int id) {
        Book* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                if (current->isIssued) {
                    current->isIssued = false;
                    current->dueDate = "";
                    cout << "Book returned: " << current->title << endl;
                } else {
                    cout << "Book was not issued: " << current->title << endl;
                }
                return;
            }
            current = current->next;
        }
        cout << "Book with ID: " << id << " not found." << endl;
    }

    void searchBook(string title) {
        Book* current = head;
        while (current != nullptr) {
            if (current->title == title) {
                cout << "Book found: " << current->title << " by " << current->author 
                     << " (ID: " << current->id << ", Issued: " << (current->isIssued ? "Yes" : "No") 
                     << ", Due Date: " << current->dueDate << ")" << endl;
                return;
            }
            current = current->next;
        }
        cout << "Book titled \"" << title << "\" not found." << endl;
    }

    void displayBooks() {
        Book* current = head;
        if (current == nullptr) {
            cout << "No books in the library." << endl;
            return;
        }
        cout << "\nBooks in the library:" << endl;
        cout << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" 
             << setw(10) << "Issued" << setw(15) << "Due Date" << endl;
        cout << string(80, '-') << endl;
        while (current != nullptr) {
            cout << setw(5) << current->id 
                 << setw(30) << current->title 
                 << setw(20) << current->author 
                 << setw(10) << (current->isIssued ? "Yes" : "No") 
                 << setw(15) << current->dueDate << endl;
            current = current->next;
        }
    }

    void saveToFile() {
        ofstream outFile("library_data.txt");
        Book* current = head;
        while (current != nullptr) {
            outFile << current->id << "," << current->title << "," << current->author << ","
                    << current->isIssued << "," << current->dueDate << endl;
            current = current->next;
        }
        outFile.close();
        cout << "Library data saved to file." << endl;
    }

    void loadFromFile() {
        ifstream inFile("library_data.txt");
        if (!inFile) {
            cout << "No existing data found. Starting with an empty library." << endl;
            return;
        }
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            Book* newBook = new Book();
            string isIssuedStr;

            getline(ss, isIssuedStr, ',');
            newBook->id = stoi(isIssuedStr);
            getline(ss, newBook->title, ',');
            getline(ss, newBook->author, ',');
            getline(ss, isIssuedStr, ',');
            newBook->isIssued = (isIssuedStr == "1");
            getline(ss, newBook->dueDate);

            newBook->next = nullptr;

            if (head == nullptr) {
                head = newBook;
            } else {
                Book* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newBook;
            }
        }
        inFile.close();
        cout << "Library data loaded from file." << endl;
    }

    string getCurrentDate(int days) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        ltm->tm_mday += days;
        mktime(ltm);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return string(buffer);
    }

    ~Library() {
        Book* current = head;
        while (current != nullptr) {
            Book* next = current->next;
            delete current;
            current = next;
        }
    }
};

bool authenticateUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    return (username == "rishi" && password == "rishi123");
}

int main() {
    Library library;
    library.loadFromFile();

    if (!authenticateUser()) {
        cout << "Invalid credentials. Exiting the system." << endl;
        return 0;
    }

    int choice;
    string title, author;
    int id;

    do {
        cout << "\nLibrary Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Issue Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Search Book" << endl;
        cout << "6. Display Books" << endl;
        cout << "7. Save Data" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                cout << "Enter ID: ";
                cin >> id;
                library.addBook(title, author, id);
                break;
            case 2:
                cout << "Enter ID to remove: ";
                cin >> id;
                library.removeBook(id);
                break;
            case 3:
                cout << "Enter ID to issue: ";
                cin >> id;
                library.issueBook(id);
                break;
            case 4:
                cout << "Enter ID to return: ";
                cin >> id;
                library.returnBook(id);
                break;
            case 5:
                cout << "Enter Title to search: ";
                cin.ignore();
                getline(cin, title);
                library.searchBook(title);
                break;
            case 6:
                library.displayBooks();
                break;
            case 7:
                library.saveToFile();
                break;
            case 8:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
