// Student Management System (compact) - compile: g++ -std=c++11 student_system.cpp -o sms
// Module 1: Add/Update  | Module 2: Search/Display | Module 3: Marks & Grades | Module 4: Reports
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

const int N = 5;
const string SUB[N] = {"Maths", "Physics", "Chemistry", "English", "Computer"};

struct Student {
    int roll = 0, age = 0;
    string name, course;
    double m[N] = {0};
    bool has = false;  // marks entered?
    double total() const { double t = 0; for (double x : m) t += x; return t; }
    double pct() const { return total() / N; }
    char grade() const {
        double p = pct();
        return p >= 90 ? 'A' : p >= 75 ? 'B' : p >= 60 ? 'C' : p >= 50 ? 'D' : p >= 40 ? 'E' : 'F';
    }
    bool pass() const { for (double x : m) if (x < 40) return false; return true; }
};
vector<Student> db;

// ---------- file storage: roll|name|age|course|has m1 m2 m3 m4 m5 ----------
void save() {
    ofstream f("students.txt");
    for (auto& s : db) {
        f << s.roll << '|' << s.name << '|' << s.age << '|' << s.course << '|' << s.has;
        for (double x : s.m) f << ' ' << x;
        f << '\n';
    }
}
void load() {
    ifstream f("students.txt");
    string l, r, a;
    while (getline(f, l)) {
        istringstream in(l);
        Student s;
        if (!getline(in, r, '|') || !getline(in, s.name, '|') || !getline(in, a, '|') ||
            !getline(in, s.course, '|'))
            continue;
        s.roll = stoi(r); s.age = stoi(a);
        in >> s.has;
        for (double& x : s.m) in >> x;
        db.push_back(s);
    }
}

// ---------- input helpers ----------
string ask(const string& p) {
    string s;
    do { cout << p; getline(cin, s); } while (s.empty());
    replace(s.begin(), s.end(), '|', ' ');
    return s;
}
double askNum(const string& p, double lo, double hi) {
    while (true) {
        try { double v = stod(ask(p)); if (v >= lo && v <= hi) return v; } catch (...) {}
        cout << "  Enter a number between " << lo << " and " << hi << "\n";
    }
}
Student* byRoll(int r) { for (auto& s : db) if (s.roll == r) return &s; return nullptr; }
Student* askStudent() {
    Student* s = byRoll(askNum("Roll number: ", 1, 999999));
    if (!s) cout << "  Student not found.\n";
    return s;
}

// ---------- Module 1: add and update ----------
void addStudent() {
    Student s;
    s.roll = askNum("Roll number: ", 1, 999999);
    if (byRoll(s.roll)) { cout << "  Roll already exists.\n"; return; }
    s.name = ask("Name: "); s.age = askNum("Age: ", 5, 100); s.course = ask("Course: ");
    db.push_back(s); save();
    cout << "  Student added.\n";
}
void updateStudent() {
    Student* s = askStudent();
    if (!s) return;
    int c = askNum("1.Name  2.Age  3.Course : ", 1, 3);
    if (c == 1) s->name = ask("New name: ");
    else if (c == 2) s->age = askNum("New age: ", 5, 100);
    else s->course = ask("New course: ");
    save(); cout << "  Record updated.\n";
}

// ---------- Module 2: display and search ----------
void showRow(const Student& s) {
    cout << left << setw(8) << s.roll << setw(20) << s.name << setw(6) << s.age << setw(14)
         << s.course << (s.has ? "Entered" : "Pending") << "\n";
}
void displayAll() {
    cout << left << setw(8) << "Roll" << setw(20) << "Name" << setw(6) << "Age" << setw(14)
         << "Course" << "Marks\n" << string(55, '-') << "\n";
    for (auto& s : db) showRow(s);
    if (db.empty()) cout << "  No records.\n";
}
void searchStudent() {
    string key = ask("Enter roll number or name: ");
    string k = key; transform(k.begin(), k.end(), k.begin(), ::tolower);
    bool found = false;
    for (auto& s : db) {
        string n = s.name; transform(n.begin(), n.end(), n.begin(), ::tolower);
        if (to_string(s.roll) == key || n.find(k) != string::npos) {
            if (!found) cout << left << setw(8) << "Roll" << setw(20) << "Name" << setw(6) << "Age"
                             << setw(14) << "Course" << "Marks\n";
            showRow(s); found = true;
        }
    }
    if (!found) cout << "  No match.\n";
}

// ---------- Module 3: marks and grades ----------
void enterMarks() {
    Student* s = askStudent();
    if (!s) return;
    for (int i = 0; i < N; i++) s->m[i] = askNum("  " + SUB[i] + " (0-100): ", 0, 100);
    s->has = true; save();
    cout << fixed << setprecision(2) << "  Total: " << s->total() << "/" << N * 100
         << "  Percentage: " << s->pct() << "%  Grade: " << s->grade() << "\n";
}
void gradeSheet() {
    cout << fixed << setprecision(2) << left << setw(8) << "Roll" << setw(20) << "Name" << setw(10)
         << "Percent" << "Grade\n" << string(45, '-') << "\n";
    for (auto& s : db)
        if (s.has) cout << setw(8) << s.roll << setw(20) << s.name << setw(10) << s.pct() << s.grade() << "\n";
    cout << "A>=90 B>=75 C>=60 D>=50 E>=40 F<40\n";
}

// ---------- Module 4: reports ----------
void studentReport() {
    Student* s = askStudent();
    if (!s) return;
    if (!s->has) { cout << "  Marks not entered yet.\n"; return; }
    cout << fixed << setprecision(2) << "\n===== RESULT REPORT =====\nRoll: " << s->roll
         << "\nName: " << s->name << "\nCourse: " << s->course << "\n-------------------------\n";
    for (int i = 0; i < N; i++)
        cout << left << setw(12) << SUB[i] << setw(8) << s->m[i] << (s->m[i] >= 40 ? "Pass" : "Fail") << "\n";
    cout << "-------------------------\nTotal: " << s->total() << "/" << N * 100
         << "\nPercentage: " << s->pct() << "%\nGrade: " << s->grade()
         << "\nResult: " << (s->pass() ? "PASS" : "FAIL") << "\n=========================\n";
}
void classReport() {
    vector<Student> r;
    for (auto& s : db) if (s.has) r.push_back(s);
    sort(r.begin(), r.end(), [](const Student& a, const Student& b) { return a.pct() > b.pct(); });
    cout << fixed << setprecision(2) << "\n===== CLASS REPORT =====\n" << left << setw(6) << "Rank"
         << setw(8) << "Roll" << setw(18) << "Name" << setw(9) << "Percent" << setw(7) << "Grade"
         << "Result\n";
    int passed = 0; double sum = 0;
    for (size_t i = 0; i < r.size(); i++) {
        cout << setw(6) << i + 1 << setw(8) << r[i].roll << setw(18) << r[i].name << setw(9)
             << r[i].pct() << setw(7) << r[i].grade() << (r[i].pass() ? "PASS" : "FAIL") << "\n";
        passed += r[i].pass(); sum += r[i].pct();
    }
    if (r.empty()) { cout << "  No marks entered yet.\n"; return; }
    cout << "Passed: " << passed << "  Failed: " << r.size() - passed << "  Class average: "
         << sum / r.size() << "%  Topper: " << r[0].name << "\n";
}

int main() {
    load();
    while (true) {
        cout << "\n===== STUDENT MANAGEMENT =====\n"
                " 1. Add student        (Module 1)\n 2. Update student\n"
                " 3. Display all        (Module 2)\n 4. Search student\n"
                " 5. Enter marks        (Module 3)\n 6. Grade sheet\n"
                " 7. Student report     (Module 4)\n 8. Class report\n 0. Exit\n";
        switch ((int)askNum("Choice: ", 0, 8)) {
            case 1: addStudent(); break;
            case 2: updateStudent(); break;
            case 3: displayAll(); break;
            case 4: searchStudent(); break;
            case 5: enterMarks(); break;
            case 6: gradeSheet(); break;
            case 7: studentReport(); break;
            case 8: classReport(); break;
            case 0: return 0;
        }
    }
}
