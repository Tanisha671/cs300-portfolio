// Author: Tanisha Wilson //
// Projecttwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Helper function to trim whitespace
string Trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos) ? "" : str.substr(first, (last - first + 1));
}

// Helper function to convert string to uppercase
string ToUpper(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

// Course structure
struct Course {
    string courseId;
    string courseTitle;
    vector<string> prerequisites;
};

// Tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree
class BinarySearchTree {
private:
    Node* root;

    // Recursive insert
    Node* AddNode(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }
        if (course.courseId < node->course.courseId) {
            node->left = AddNode(node->left, course);
        }
        else {
            node->right = AddNode(node->right, course);
        }
        return node;
    }

    // Recursive print
    void InOrder(Node* node) {
        if (node != nullptr) {
            InOrder(node->left);
            cout << node->course.courseId << ", " << node->course.courseTitle << endl;
            InOrder(node->right);
        }
    }

    // Recursive search
    Course* Search(Node* node, const string& courseId) {
        if (node == nullptr) return nullptr;

        if (courseId == node->course.courseId) {
            return &node->course;
        }
        else if (courseId < node->course.courseId) {
            return Search(node->left, courseId);
        }
        else {
            return Search(node->right, courseId);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void Insert(Course course) {
        course.courseId = ToUpper(course.courseId);
        root = AddNode(root, course);
    }

    void PrintCourseList() {
        InOrder(root);
    }

    Course* Search(const string& courseId) {
        return Search(root, ToUpper(courseId));
    }
};

// Load courses from CSV
void LoadCourses(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(Trim(token));
        }

        if (tokens.size() < 2) continue;

        Course course;
        course.courseId = ToUpper(tokens[0]);
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(ToUpper(tokens[i]));
        }

        bst.Insert(course);
    }
}

// Display course info
void DisplayCourse(const Course& course) {
    cout << course.courseId << ", " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None" << endl;
    }
}

// Main program
int main() {
    BinarySearchTree bst;
    string fileName;
    string courseId;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;
        cin.ignore(); // clear newline

        switch (choice) {
        case 1:
            cout << "Enter file name to load (e.g., courses.csv): ";
            getline(cin, fileName);
            LoadCourses(fileName, bst);
            cout << "Courses loaded successfully." << endl;
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            bst.PrintCourseList();
            break;

        case 3:
            cout << "What course do you want to know about? ";
            getline(cin, courseId);
            courseId = ToUpper(courseId);
            if (Course* course = bst.Search(courseId)) {
                DisplayCourse(*course);
            }
            else {
                cout << "Course not found." << endl;
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}