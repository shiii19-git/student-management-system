#include <iostream>
#include <string>
using namespace std;

struct Student {
    int roll;
    string name;
    int age;
    int marks[3];
    bool hasMarks;
};

Student s[50];                                   // up to 50 students
int n = 0;                                       // number of students added
string subjects[3] = {"Maths", "Science", "English"};

int findStudent(int roll) {
    for (int i = 0; i < n; i++) {
        if (s[i].roll == roll) return i;
    }
    return -1;
}

char getGrade(float p) {
    if (p >= 90) return 'A';
    else if (p >= 75) return 'B';
    else if (p >= 60) return 'C';
    else if (p >= 40) return 'D';
    else return 'F';
}

int total(int i) { return s[i].marks[0] + s[i].marks[1] + s[i].marks[2]; }
float percentage(int i) { return total(i) / 3.0; }

// ---------- Module 1: Add and update ----------
void addStudent() {
    if (n == 50) { cout << "Student list is full!\n"; return; }
    int roll;
    cout << "Roll number: ";
    cin >> roll;
    if (findStudent(roll) != -1) { cout << "Roll number already exists!\n"; return; }
    s[n].roll = roll;
    cout << "Name (no spaces): ";
    cin >> s[n].name;
    cout << "Age: ";
    cin >> s[n].age;
    s[n].hasMarks = false;
    n++;
    cout << "Student added.\n";
}

void updateStudent() {
    int roll;
    cout << "Roll number to update: ";
    cin >> roll;
    int i = findStudent(roll);
    if (i == -1) { cout << "Student not found!\n"; return; }
    cout << "New name: ";
    cin >> s[i].name;
    cout << "New age: ";
    cin >> s[i].age;
    cout << "Record updated.\n";
}

// ---------- Module 2: Search and display ----------
void displayAll() {
    if (n == 0) { cout << "No students yet.\n"; return; }
    cout << "\nRoll\tName\t\tAge\n";
    for (int i = 0; i < n; i++) {
        cout << s[i].roll << "\t" << s[i].name << "\t\t" << s[i].age << "\n";
    }
}

void searchStudent() {
    int roll;
    cout << "Enter roll number: ";
    cin >> roll;
    int i = findStudent(roll);
    if (i == -1) { cout << "Student not found!\n"; return; }
    cout << "\nRoll: " << s[i].roll << "\nName: " << s[i].name << "\nAge: " << s[i].age << "\n";
    if (s[i].hasMarks) cout << "Percentage: " << percentage(i) << "%  Grade: " << getGrade(percentage(i)) << "\n";
    else cout << "Marks: not entered yet\n";
}

// ---------- Module 3: Enter marks and calculate grade ----------
void enterMarks() {
    int roll;
    cout << "Roll number: ";
    cin >> roll;
    int i = findStudent(roll);
    if (i == -1) { cout << "Student not found!\n"; return; }
    for (int j = 0; j < 3; j++) {
        do {
            cout << subjects[j] << " marks (0-100): ";
            cin >> s[i].marks[j];
        } while (s[i].marks[j] < 0 || s[i].marks[j] > 100);
    }
    s[i].hasMarks = true;
    cout << "Total: " << total(i) << "/300  Percentage: " << percentage(i)
         << "%  Grade: " << getGrade(percentage(i)) << "\n";
}

// ---------- Module 4: Result reports ----------
void studentReport() {
    int roll;
    cout << "Roll number: ";
    cin >> roll;
    int i = findStudent(roll);
    if (i == -1) { cout << "Student not found!\n"; return; }
    if (!s[i].hasMarks) { cout << "Marks not entered yet!\n"; return; }
    cout << "\n===== RESULT REPORT =====\n";
    cout << "Roll: " << s[i].roll << "\nName: " << s[i].name << "\n";
    for (int j = 0; j < 3; j++) cout << subjects[j] << ": " << s[i].marks[j] << "\n";
    cout << "Total: " << total(i) << "/300\nPercentage: " << percentage(i) << "%\n";
    cout << "Grade: " << getGrade(percentage(i)) << "\n";
    if (percentage(i) >= 40) cout << "Result: PASS\n";
    else cout << "Result: FAIL\n";
}

void classReport() {
    int pass = 0, fail = 0;
    cout << "\n===== CLASS REPORT =====\nRoll\tName\tTotal\t%\tGrade\tResult\n";
    for (int i = 0; i < n; i++) {
        if (!s[i].hasMarks) continue;
        cout << s[i].roll << "\t" << s[i].name << "\t" << total(i) << "\t" << percentage(i)
             << "\t" << getGrade(percentage(i)) << "\t";
        if (percentage(i) >= 40) { cout << "PASS\n"; pass++; }
        else { cout << "FAIL\n"; fail++; }
    }
    cout << "Passed: " << pass << "  Failed: " << fail << "\n";
}

int main() {
    int choice;
    do {
        cout << "\n==== STUDENT MANAGEMENT SYSTEM ====\n";
        cout << "1. Add student\n2. Update student\n3. Display all students\n4. Search student\n";
        cout << "5. Enter marks\n6. Student report\n7. Class report\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addStudent(); break;
            case 2: updateStudent(); break;
            case 3: displayAll(); break;
            case 4: searchStudent(); break;
            case 5: enterMarks(); break;
            case 6: studentReport(); break;
            case 7: classReport(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
    return 0;
}
