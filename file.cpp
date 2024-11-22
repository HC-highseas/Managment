#include <iostream>
#include <fstream>
#include <string>
using namespace std;
const int MAX_STUDENTS = 100;
const int PADDING = 3;
const char DELIMITER = ';';
class UIFormat
{
    static int nameW, enrollW, roomW, fineW, statusW;
public:
    static void resetWidths()
    {
        nameW = 10 + PADDING;
        enrollW = 15 + PADDING;
        roomW = 7 + PADDING;
        fineW = 11 + PADDING;
        statusW = 11 + PADDING;
    }
    static void updateWidths(const string &name, const string &enroll)
    {
        nameW = max(nameW, (int)name.length() + PADDING);
        enrollW = max(enrollW, (int)enroll.length() + PADDING);
    }
    static void drawLine()
    {
        cout << '+' << string(nameW, '-') << '+' << string(enrollW, '-')
             << '+' << string(roomW, '-') << '+' << string(fineW, '-')
             << '+' << string(statusW, '-') << '+' << endl;
    }
    static int getW(int type) { return type == 0 ? nameW : type == 1 ? enrollW
                                                       : type == 2   ? roomW
                                                       : type == 3   ? fineW
                                                                     : statusW; }
};
int UIFormat::nameW = 13, UIFormat::enrollW = 18, UIFormat::roomW = 10,
    UIFormat::fineW = 14, UIFormat::statusW = 14;
struct Student
{
    string name, enrollmentNo;
    int roomNo;
    double fineAmount;
    bool finePaid;
    Student() : roomNo(0), fineAmount(0), finePaid(true) {}
    string toString() const
    {
        return name + DELIMITER + enrollmentNo + DELIMITER +
               to_string(roomNo) + DELIMITER + to_string(fineAmount) +
               DELIMITER + (finePaid ? "1" : "0");
    }
    void fromString(const string &s)
    {
        size_t pos = 0;
        string t = s;
        int field = 0;
        while ((pos = t.find(DELIMITER)) != string::npos)
        {
            string token = t.substr(0, pos);
            switch (field++)
            {
            case 0:
                name = token;
                break;
            case 1:
                enrollmentNo = token;
                break;
            case 2:
                roomNo = stoi(token);
                break;
            case 3:
                fineAmount = stod(token);
                break;
            case 4:
                finePaid = (token == "1");
                break;
            }
            t = t.substr(pos + 1);
        }
        UIFormat::updateWidths(name, enrollmentNo);
    }
    void display() const
    {
        cout << '|' << name << string(UIFormat::getW(0) - name.length(), ' ')
             << '|' << enrollmentNo << string(UIFormat::getW(1) - enrollmentNo.length(), ' ')
             << '|' << roomNo << string(UIFormat::getW(2) - to_string(roomNo).length(), ' ')
             << '|' << fineAmount << string(UIFormat::getW(3) - to_string(fineAmount).length() + 7, ' ')
             << '|' << (finePaid ? "Paid" : "Unpaid") << string(UIFormat::getW(4) - (finePaid ? 4 : 6), ' ')
             << '|' << endl;
    }
};
class HostelManagement
{
    Student students[MAX_STUDENTS];
    int studentCount;
    string filename;
    void displayHeader()
    {
        UIFormat::drawLine();
        cout << "|Name" << string(UIFormat::getW(0) - 4, ' ')
             << "|Enrollment No" << string(UIFormat::getW(1) - 13, ' ')
             << "|Room No" << string(UIFormat::getW(2) - 7, ' ')
             << "|Fine Amount" << string(UIFormat::getW(3) - 11, ' ')
             << "|Fine Status" << string(UIFormat::getW(4) - 11, ' ')
             << '|' << endl;
        UIFormat::drawLine();
    }
public:
    HostelManagement(string fname = "hostel_data.txt") : filename(fname), studentCount(0)
    {
        UIFormat::resetWidths();
        ifstream file(filename);
        if (file.is_open())
        {
            file >> studentCount;
            file.ignore();
            for (int i = 0; i < studentCount; i++)
            {
                string line;
                getline(file, line);
                students[i].fromString(line);
            }
            file.close();
        }
    }
    void saveData()
    {
        ofstream file(filename);
        if (file.is_open())
        {
            file << studentCount << endl;
            for (int i = 0; i < studentCount; i++)
                file << students[i].toString() << endl;
            file.close();
        }
        else
            cout << "\nError: Unable to save data!";
    }
    void addStudent()
    {
        if (studentCount >= MAX_STUDENTS)
        {
            cout << "\nMaximum student limit reached!";
            return;
        }
        cout << "\nEnter Name: ";
        cin.ignore();
        getline(cin, students[studentCount].name);
        cout << "Enter Enrollment No: ";
        cin >> students[studentCount].enrollmentNo;
        cout << "Enter Room No: ";
        cin >> students[studentCount].roomNo;
        UIFormat::updateWidths(students[studentCount].name, students[studentCount].enrollmentNo);
        studentCount++;
        saveData();
        cout << "\nStudent added successfully!";
    }
    void viewAllStudents()
    {
        if (studentCount == 0)
        {
            cout << "\nNo students registered yet!";
            return;
        }
        cout << "\nStudent Records:\n";
        displayHeader();
        for (int i = 0; i < studentCount; i++)
            students[i].display();
        UIFormat::drawLine();
    }
    void searchStudent()
    {
        if (studentCount == 0)
        {
            cout << "\nNo students registered yet!";
            return;
        }
        cout << "\nSearch by:\n1. Name\n2. Enrollment No\n3. Room No\nEnter choice: ";
        int choice;
        cin >> choice;
        string term;
        cout << "Enter search term: ";
        cin.ignore();
        getline(cin, term);
        bool found = false;
        displayHeader();
        for (int i = 0; i < studentCount; i++)
        {
            bool match = (choice == 1 && students[i].name == term) ||
                         (choice == 2 && students[i].enrollmentNo == term) ||
                         (choice == 3 && to_string(students[i].roomNo) == term);
            if (match)
            {
                students[i].display();
                found = true;
            }
        }
        if (found)
            UIFormat::drawLine();
        else
            cout << "\nNo matching records found!";
    }
    void manageFines()
    {
        if (studentCount == 0)
        {
            cout << "\nNo students registered yet!";
            return;
        }
        cout << "\nFine Management:\n1. Add/Update Fine\n2. Mark Fine as Paid"
             << "\n3. View All Fines\nEnter choice: ";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            string enroll;
            cout << "\nEnter enrollment no: ";
            cin >> enroll;
            for (int i = 0; i < studentCount; i++)
                if (students[i].enrollmentNo == enroll)
                {
                    cout << "Enter fine amount: ";
                    cin >> students[i].fineAmount;
                    students[i].finePaid = false;
                    cout << "\nFine updated successfully!";
                    saveData();
                    return;
                }
            cout << "\nStudent not found!";
            break;
        }
        case 2:
        {
            string enroll;
            cout << "\nEnter enrollment no: ";
            cin >> enroll;
            for (int i = 0; i < studentCount; i++)
                if (students[i].enrollmentNo == enroll)
                {
                    if (!students[i].finePaid)
                    {
                        students[i].finePaid = true;
                        cout << "\nFine marked as paid!";
                    }
                    else
                        cout << "\nNo pending fines for this student!";
                    saveData();
                    return;
                }
            cout << "\nStudent not found!";
            break;
        }
        case 3:
        {
            cout << "\nFine Records:\n";
            bool hasFines = false;
            displayHeader();
            for (int i = 0; i < studentCount; i++)
                if (students[i].fineAmount > 0)
                {
                    students[i].display();
                    hasFines = true;
                }
            if (hasFines)
                UIFormat::drawLine();
            else
                cout << "\nNo fine records found!";
            break;
        }
        default:
            cout << "\nInvalid choice!";
        }
    }
};
int main()
{
    HostelManagement hostel;
    while (true)
    {
        cout << "\n\nHostel Management System\n======================\n"
             << "1. Add New Student\n2. View All Students\n3. Search Student\n"
             << "4. Manage Fines\n5. Exit\nEnter your choice: ";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            hostel.addStudent();
            break;
        case 2:
            hostel.viewAllStudents();
            break;
        case 3:
            hostel.searchStudent();
            break;
        case 4:
            hostel.manageFines();
            break;
        case 5:
            cout << "\nThank you for using Hostel Management System!\n";
            return 0;
        default:
            cout << "\nInvalid choice! Please try again.";
        }
        cin.ignore();
        cin.get();
    }
}
