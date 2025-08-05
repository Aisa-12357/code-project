
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    string genre;  
    bool isIssued;
    string issueDate;
    string returnDate;
    int borrowerId;

    Book() : id(0), isIssued(false), borrowerId(0) {}
   
    void input() {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Genre: ";  
        getline(cin, genre);
        isIssued = false;
        issueDate = "";
        returnDate = "";
        borrowerId = 0;
    }
};

class Member {
public:
    int memberId;
    string name;

    Member() : memberId(0) {}

    void input() {
        cout << "Enter Member ID: ";
        cin >> memberId;
        cin.ignore();
        cout << "Enter Member Name: ";
        getline(cin, name);
    }

    void display() const {
        cout << "\n--- Member Information ---\n";
        cout << "Member ID: " << memberId << endl;
        cout << "Name: " << name << endl;
    }
};

class Library {
    vector<Book> books;
    vector<Member> members;
    const string bookFile = "books.txt";
    const string memberFile = "members.txt";

    void ensureFileExists(const string& filename) {
        ifstream infile(filename);
        if (!infile.good()) {
            ofstream outfile(filename);
            outfile.close();
        }
    }

    void loadBooks() {
        books.clear();
        ifstream file(bookFile);
        if (!file.is_open()) return;

        Book b;
        while (file >> b.id) {
            file.ignore();
            getline(file, b.title);
            getline(file, b.author);
            getline(file, b.genre);  
            file >> b.isIssued;
            file.ignore();
            getline(file, b.issueDate);
            getline(file, b.returnDate);
            file >> b.borrowerId;
            file.ignore();
            books.push_back(b);
        }
    }

    void saveBooks() {
        ofstream file(bookFile);
        for (const Book& b : books) {
            file << b.id << "\n"
                 << b.title << "\n"
                 << b.author << "\n"
                 << b.genre << "\n"  
                 << b.isIssued << "\n"
                 << b.issueDate << "\n"
                 << b.returnDate << "\n"
                 << b.borrowerId << "\n";
        }
    }

    void loadMembers() {
        members.clear();
        ifstream file(memberFile);
        if (!file.is_open()) return;

        Member m;
        while (file >> m.memberId) {
            file.ignore();
            getline(file, m.name);
            members.push_back(m);
        }
    }

    void saveMembers() {
        ofstream file(memberFile);
        for (const Member& m : members) {
            file << m.memberId << "\n" << m.name << "\n";
        }
    }

    string getMemberNameById(int id) const {
        for (const auto& m : members) {
            if (m.memberId == id)
                return m.name;
        }
        return "Unknown";
    }

public:
    Library() {
        ensureFileExists(bookFile);
        ensureFileExists(memberFile);
        loadBooks();
        loadMembers();
    }

    ~Library() {
        saveBooks();
        saveMembers();
    }

    void registerMember() {
        Member m;
        m.input();
        for (const auto& mem : members) {
            if (mem.name == m.name) {
                cout << "Member with this name already exists.\n";
                return;
            }
        }
        for (const auto& mem : members) {
            if (mem.memberId == m.memberId) {
                cout << "Member ID already exists.\n";
                return;
            }
        }
        members.push_back(m);
        saveMembers();
        cout << "Member registered successfully.\n";
    }

    void addBook() {
        Book b;
        b.input();
        books.push_back(b);
        saveBooks();
        cout << "Book added successfully.\n";
    }

    void showBooks() {
        if (books.empty()) {
            cout << "No books available.\n";
        } else {
            cout << "\n=== Book Records ===\n";
            for (const auto& b : books) {
                cout << "\n--- Book Information ---\n";
                cout << "ID: " << b.id << endl;
                cout << "Title: " << b.title << endl;
                cout << "Author: " << b.author << endl;
                cout << "Genre: " << b.genre << endl;  
                cout << "Status: " << (b.isIssued ? "Issued" : "Available") << endl;
                if (b.isIssued) {
                    cout << "Issue Date: " << b.issueDate << endl;
                    cout << "Return Date: " << b.returnDate << endl;
                    cout << "Borrowed by Member ID: " << b.borrowerId << endl;
                    cout << "Borrowed by Member Name: " << getMemberNameById(b.borrowerId) << endl;
                }
            }
        }
    }

    void searchBook() {
        string name;
        cin.ignore();
        cout << "Enter Book Title to search: ";
        getline(cin, name);

        bool found = false;
        for (const auto& b : books) {
            if (b.title == name) {
                cout << "\n--- Book Information ---\n";
                cout << "ID: " << b.id << endl;
                cout << "Title: " << b.title << endl;
                cout << "Author: " << b.author << endl;
                cout << "Genre: " << b.genre << endl;  
                cout << "Status: " << (b.isIssued ? "Issued" : "Available") << endl;
                if (b.isIssued) {
                    cout << "Issue Date: " << b.issueDate << endl;
                    cout << "Return Date: " << b.returnDate << endl;
                    cout << "Borrowed by Member ID: " << b.borrowerId << endl;
                    cout << "Borrowed by Member Name: " << getMemberNameById(b.borrowerId) << endl;
                }
                found = true;
            }
        }
        if (!found)
            cout << "Book not found.\n";
    }

    void deleteBook() {
        int id;
        cout << "Enter Book ID to delete: ";
        cin >> id;
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].id == id) {
                books.erase(books.begin() + i);
                saveBooks();
                cout << "Book deleted successfully.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void issueBook() {
        int bookId;
        cout << "Enter Book ID to issue: ";
        cin >> bookId;

        bool foundBook = false;
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].id == bookId) {
                foundBook = true;

                if (books[i].isIssued) {
                    cout << "Book already issued.\n";
                    return;
                }

                string memName;
                cin.ignore();
                cout << "Enter Member Name who is borrowing: ";
                getline(cin, memName);

                bool foundMember = false;
                for (const auto& m : members) {
                    if (m.name == memName) {
                        books[i].isIssued = true;
                        books[i].borrowerId = m.memberId;
                        cout << "Enter Issue Date (dd-mm-yyyy): ";
                        getline(cin, books[i].issueDate);
                        cout << "Enter Return Date (dd-mm-yyyy): ";
                        getline(cin, books[i].returnDate);
                        saveBooks();
                        cout << "Book issued successfully to " << m.name << ".\n";
                        return;
                    }
                }
                cout << "Member not found.\n";
                return;
            }
        }

        if (!foundBook) {
            cout << "Book not found.\n";
        }
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;

        for (auto& b : books) {
            if (b.id == id) {
                if (!b.isIssued) {
                    cout << "Book was not issued.\n";
                    return;
                }
                b.isIssued = false;
                b.issueDate = "";
                b.returnDate = "";
                b.borrowerId = 0;
                saveBooks();
                cout << "Book returned successfully.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void deleteMember() {
        int id;
        cout << "Enter Member ID to delete: ";
        cin >> id;

        for (const auto& b : books) {
            if (b.isIssued && b.borrowerId == id) {
                cout << "Cannot delete member. They have an issued book.\n";
                return;
            }
        }

        for (size_t i = 0; i < members.size(); ++i) {
            if (members[i].memberId == id) {
                members.erase(members.begin() + i);
                saveMembers();
                cout << "Member deleted successfully.\n";
                return;
            }
        }
        cout << "Member not found.\n";
    }

    void showAllRecords() {
        cout << "\n=== Member Records ===\n";
        if (members.empty()) {
            cout << "No members registered.\n";
        } else {
            for (const auto& m : members) {
                m.display();

                bool borrowedAny = false;
                for (const auto& b : books) {
                    if (b.isIssued && b.borrowerId == m.memberId) {
                        cout << "\n  --- Borrowed Book ---\n";
                        cout << "  Book ID: " << b.id << endl;
                        cout << "  Title: " << b.title << endl;
                        cout << "  Genre: " << b.genre << endl;  
                        cout << "  Issue Date: " << b.issueDate << endl;
                        cout << "  Return Date: " << b.returnDate << endl;
                        borrowedAny = true;
                    }
                }
                if (!borrowedAny) {
                    cout << "  No books borrowed.\n";
                }
            }
        }
    }

    
    void showBorrowedBooks() {
        cout << "\n=== Currently Borrowed Books ===\n";
        bool anyBorrowed = false;
        for (const auto& b : books) {
            if (b.isIssued) {
                cout << "\n--- Book Information ---\n";
                cout << "ID: " << b.id << endl;
                cout << "Title: " << b.title << endl;
                cout << "Author: " << b.author << endl;
                cout << "Genre: " << b.genre << endl;
                cout << "Issue Date: " << b.issueDate << endl;
                cout << "Return Date: " << b.returnDate << endl;
                cout << "Borrowed by Member ID: " << b.borrowerId << endl;
                cout << "Borrowed by Member Name: " << getMemberNameById(b.borrowerId) << endl;
                anyBorrowed = true;
            }
        }
        if (!anyBorrowed) {
            cout << "No books are currently borrowed.\n";
        }
    }
};

int main() {
    Library lib;
    int choice;

    do {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Register Member\n";
        cout << "2. Add Book\n";
        cout << "3. Show All Books\n";
        cout << "4. Search Book by Title\n";
        cout << "5. Delete Book\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book\n";
        cout << "8. Show All Member Records\n";
        cout << "9. Delete Member\n";
        cout << "10. Show Borrowed Books\n";  
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.registerMember(); break;
            case 2: lib.addBook(); break;
            case 3: lib.showBooks(); break;
            case 4: lib.searchBook(); break;
            case 5: lib.deleteBook(); break;
            case 6: lib.issueBook(); break;
            case 7: lib.returnBook(); break;
            case 8: lib.showAllRecords(); break;
            case 9: lib.deleteMember(); break;
            case 10: lib.showBorrowedBooks(); break;  
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}