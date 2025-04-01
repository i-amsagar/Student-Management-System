#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <regex>
#include <limits>

using namespace std;

// Constants
const string FILENAME = "studentRecord.txt";
const string TEMP_FILENAME = "tempRecord.txt";
const string DELIMITER = "|||";

// Email Validation Function
bool isValidEmail(const string& email) {
    const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}

// Roll Number Validation Function
bool isValidRollNo(const string& roll_no) {
    const regex pattern(R"([A-Za-z0-9]{3,20})");
    return regex_match(roll_no, pattern);
}

// Contact Number Validation Function
bool isValidContactNo(long long int contact_no) {
    return (contact_no >= 1000000000 && contact_no <= 9999999999);
}

class Student {
private:
    string name, roll_no, course, address, email_id;
    long long int contact_no;

    void getValidContactNo() {
        while (true) {
            cout << "\tEnter Contact No (10 digits): ";
            cin >> contact_no;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\tInvalid input. Please enter numbers only.\n";
                continue;
            }
            
            if (isValidContactNo(contact_no)) {
                break;
            }
            cout << "\tInvalid Contact Number! Please enter a valid 10-digit number.\n";
        }
    }

public:
    void menu();
    void insert();
    void display();
    void modify();
    void search();
    void deleteStudent();
};

// Menu Function
void Student::menu() {
    int choice;
    char x;

    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        
        cout << "\t\t\t-----------------------------\n";
        cout << "\t\t\t| STUDENT MANAGEMENT SYSTEM |\n";
        cout << "\t\t\t-----------------------------\n";
        cout << "\t\t\t 1. Enter New Record\n";
        cout << "\t\t\t 2. Display Record\n";
        cout << "\t\t\t 3. Modify Record\n";
        cout << "\t\t\t 4. Search Record\n";
        cout << "\t\t\t 5. Delete Record\n";
        cout << "\t\t\t 6. Exit\n";
        cout << "\t\t\tChoose an Option: ";
        
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0; // Force default case
        }

        switch (choice) {
            case 1:
                do {
                    insert();
                    cout << "\n\n\t\t\t Add Another Student Record (Y/N): ";
                    cin >> x;
                } while (x == 'y' || x == 'Y');
                break;
            case 2: 
                display(); 
                cout << "\n\t\t\tPress Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            case 3: modify(); break;
            case 4: search(); break;
            case 5: deleteStudent(); break;
            case 6:
                cout << "\n\t\t\t Exiting Program...\n";
                exit(0);
            default:
                cout << "\n\t\t\t Invalid Choice... Try Again.\n";
                cin.ignore();
                cin.get();
        }
    }
}

// Insert New Student Record
void Student::insert() {
    fstream file;
    
    cout << "\n\tEnter Name: ";
    cin.ignore();
    getline(cin, name);
    
    do {
        cout << "\tEnter Roll No.: ";
        getline(cin, roll_no);
        if (!isValidRollNo(roll_no)) {
            cout << "\tInvalid Roll Number! Try Again.\n";
        }
    } while (!isValidRollNo(roll_no));

    cout << "\tEnter Course: ";
    getline(cin, course);

    do {
        cout << "\tEnter Email Id (name@gmail.com): ";
        getline(cin, email_id);
        if (!isValidEmail(email_id)) {
            cout << "\tInvalid Email! Try Again.\n";
        }
    } while (!isValidEmail(email_id));

    getValidContactNo();

    cout << "\tEnter Address: ";
    getline(cin, address);

    file.open(FILENAME, ios::app);
    if (!file) {
        cout << "\n\tError opening file for writing!\n";
        return;
    }
    
    file << name << DELIMITER << roll_no << DELIMITER << course << DELIMITER 
         << email_id << DELIMITER << contact_no << DELIMITER << address << "\n";
    file.close();
    
    cout << "\n\t\t\tRecord Added Successfully!\n";
}

// Display All Student Records
void Student::display() {
    fstream file;
    file.open(FILENAME, ios::in);
    
    if (!file) {
        cout << "\n\t\tNo Data is Present...\n";
        return;
    }

    int total = 0;
    string line;
    while (getline(file, line)) {
        size_t pos = 0;
        string token;
        int field = 0;
        
        while ((pos = line.find(DELIMITER)) != string::npos) {
            token = line.substr(0, pos);
            switch (field) {
                case 0: name = token; break;
                case 1: roll_no = token; break;
                case 2: course = token; break;
                case 3: email_id = token; break;
                case 4: contact_no = stoll(token); break;
            }
            line.erase(0, pos + DELIMITER.length());
            field++;
        }
        address = line;

        cout << "\n\tStudent No.: " << ++total;
        cout << "\n\tName: " << name;
        cout << "\n\tRoll No.: " << roll_no;
        cout << "\n\tCourse: " << course;
        cout << "\n\tEmail Id: " << email_id;
        cout << "\n\tContact No.: " << contact_no;
        cout << "\n\tAddress: " << address << "\n";

        if (total % 5 == 0) {
            cout << "\n\tPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    if (total == 0) {
        cout << "\n\t\tNo Data Found...\n";
    }

    file.close();
}

// Modify Student Record
void Student::modify() {
    fstream file, file1;
    string rollno;
    int found = 0;

    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Student Modify Details ------------------------------------------" << endl;
    
    file.open(FILENAME, ios::in);
    if (!file) {
        cout << "\n\t\t\tNo Data is Present..";
        file.close();
        return;
    }
    
    cout << "\nEnter Roll No. of Student which you want to Modify: ";
    cin >> rollno;
    
    file1.open(TEMP_FILENAME, ios::app | ios::out);
    
    string line;
    while (getline(file, line)) {
        size_t pos = 0;
        string token;
        int field = 0;
        string current_roll;
        
        // Extract roll number first
        size_t first_delim = line.find(DELIMITER);
        if (first_delim != string::npos) {
            current_roll = line.substr(0, first_delim);
        }

        if (rollno != current_roll) {
            file1 << line << "\n";
        } else {
            cout << "\n\t\t\tCurrent Details:\n";
            // Display current details
            pos = 0;
            field = 0;
            while ((pos = line.find(DELIMITER)) != string::npos) {
                token = line.substr(0, pos);
                switch (field) {
                    case 0: cout << "\t\t\tName: " << token << "\n"; break;
                    case 1: cout << "\t\t\tRoll No.: " << token << "\n"; break;
                    case 2: cout << "\t\t\tCourse: " << token << "\n"; break;
                    case 3: cout << "\t\t\tEmail Id: " << token << "\n"; break;
                    case 4: cout << "\t\t\tContact No.: " << token << "\n"; break;
                }
                line.erase(0, pos + DELIMITER.length());
                field++;
            }
            cout << "\t\t\tAddress: " << line << "\n";

            // Get new details
            cout << "\n\t\t\tEnter New Details:\n";
            cout << "\t\t\tEnter Name: ";
            cin.ignore();
            getline(cin, name);
            
            do {
                cout << "\t\t\tEnter Roll No.: ";
                getline(cin, roll_no);
                if (!isValidRollNo(roll_no)) {
                    cout << "\t\t\tInvalid Roll Number! Try Again.\n";
                }
            } while (!isValidRollNo(roll_no));

            cout << "\t\t\tEnter Course: ";
            getline(cin, course);

            do {
                cout << "\t\t\tEnter Email Id (name@gmail.com): ";
                getline(cin, email_id);
                if (!isValidEmail(email_id)) {
                    cout << "\t\t\tInvalid Email! Try Again.\n";
                }
            } while (!isValidEmail(email_id));

            getValidContactNo();

            cout << "\t\t\tEnter Address: ";
            getline(cin, address);

            file1 << name << DELIMITER << roll_no << DELIMITER << course << DELIMITER 
                 << email_id << DELIMITER << contact_no << DELIMITER << address << "\n";
            found++;
            cout << "\n\t\t\tRecord Updated Successfully!\n";
        }
    }

    if (found == 0) {
        cout << "\n\n\t\t\t Student Roll No. Not Found....";
    }
    
    file1.close();
    file.close();
    remove(FILENAME.c_str());
    rename(TEMP_FILENAME.c_str(), FILENAME.c_str());
}

// Search Student Record
void Student::search() {
    fstream file;
    int found = 0;

    file.open(FILENAME, ios::in);
    if (!file) {
        cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
        cout << "------------------------------------- Student Search Data --------------------------------------------" << endl;
        cout << "\n\t\t\tNo Data is Present... " << endl;
        return;
    }

    string rollno;
    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Student Search Table --------------------------------------------" << endl;
    cout << "\nEnter Roll No. of Student which you want to search: ";
    cin >> rollno;

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(DELIMITER);
        if (pos != string::npos) {
            string current_roll = line.substr(0, pos);
            if (rollno == current_roll) {
                // Display all details
                pos = 0;
                int field = 0;
                while ((pos = line.find(DELIMITER)) != string::npos) {
                    string token = line.substr(0, pos);
                    switch (field) {
                        case 0: cout << "\n\n\t\t\tName: " << token << "\n"; break;
                        case 1: cout << "\t\t\tRoll No.: " << token << "\n"; break;
                        case 2: cout << "\t\t\tCourse: " << token << "\n"; break;
                        case 3: cout << "\t\t\tEmail Id: " << token << "\n"; break;
                        case 4: cout << "\t\t\tContact No.: " << token << "\n"; break;
                    }
                    line.erase(0, pos + DELIMITER.length());
                    field++;
                }
                cout << "\t\t\tAddress: " << line << "\n";
                found++;
                break;
            }
        }
    }
    
    if (found == 0) {
        cout << "\n\t\t\t Student Roll No. Not Found....";
    }

    file.close();
}

// Delete Student Record
void Student::deleteStudent() {
    fstream file, file1;
    string roll;
    int found = 0;

    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Delete Student Details ------------------------------------------" << endl;
    file.open(FILENAME, ios::in);
    if (!file) {
        cout << "\n\t\t\tNo Data is Present..";
        file.close();
        return;
    }

    cout << "\nEnter Roll No. of Student which you want to Delete Data: ";
    cin >> roll;
    
    char confirm;
    cout << "\n\t\tAre you sure you want to delete this record? (Y/N): ";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        cout << "\n\t\tDeletion cancelled.\n";
        return;
    }

    file1.open(TEMP_FILENAME, ios::app | ios::out);

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(DELIMITER);
        if (pos != string::npos) {
            string current_roll = line.substr(0, pos);
            if (roll != current_roll) {
                file1 << line << "\n";
            } else {
                found++;
            }
        }
    }

    if (found == 0) {
        cout << "\n\t\t\t Student Roll No. Not Found....";
    } else {
        cout << "\n\t\t\tSuccessfully Deleted Data\n";
    }

    file1.close();
    file.close();
    remove(FILENAME.c_str());
    rename(TEMP_FILENAME.c_str(), FILENAME.c_str());
}

// Main Function
int main() {
    Student project;
    project.menu();
    return 0;
}
