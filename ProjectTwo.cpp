//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Deanna Talton
// Version     : 1.0
// Copyright   : © 2025 Deanna Talton
// Description : CS-300 Project Two – Advising Assistance Program
//               Loads course data from a file, stores courses in a binary
//               search tree, and allows users to view a sorted course list
//               or detailed course information including prerequisites.
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// =====================================================
// Course data model
// =====================================================
struct Course {
    string number;                 // Course number (e.g., CSCI300)
    string title;                  // Course title
    vector<string> prereqs;        // List of prerequisite course numbers
};

// =====================================================
// Binary Search Tree Node
// =====================================================
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// =====================================================
// Binary Search Tree
// =====================================================
class BinarySearchTree {
private:
    Node* root;

    // Recursive insert helper
    Node* insert(Node* node, const Course& course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.number < node->course.number) {
            node->left = insert(node->left, course);
        }
        else if (course.number > node->course.number) {
            node->right = insert(node->right, course);
        }

        return node;
    }

    // Recursive search helper
    Node* search(Node* node, const string& courseNumber) {
        if (node == nullptr || node->course.number == courseNumber) {
            return node;
        }

        if (courseNumber < node->course.number) {
            return search(node->left, courseNumber);
        }

        return search(node->right, courseNumber);
    }

    // In-order traversal helper (prints sorted list)
    void inOrder(Node* node) {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);
        cout << node->course.number << ", " << node->course.title << endl;
        inOrder(node->right);
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    // Public insert
    void Insert(const Course& course) {
        root = insert(root, course);
    }

    // Public search
    Course* Search(const string& courseNumber) {
        Node* found = search(root, courseNumber);
        if (found != nullptr) {
            return &found->course;
        }
        return nullptr;
    }

    // Print all courses in alphanumeric order
    void PrintInOrder() {
        inOrder(root);
    }
};

// =====================================================
// Load courses from CSV file into BST
// =====================================================
void LoadCourses(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split CSV line
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Must have at least course number and title
        if (tokens.size() < 2) {
            continue;
        }

        Course course;
        course.number = tokens[0];
        course.title = tokens[1];

        // Remaining tokens are prerequisites
        for (size_t i = 2; i < tokens.size(); ++i) {
            // Trim whitespace
            string prereq = tokens[i];

            // Remove leading/trailing spaces/tabs
            prereq.erase(0, prereq.find_first_not_of(" \t\r\n"));
            prereq.erase(prereq.find_last_not_of(" \t\r\n") + 1);

            // Only store real prerequisite values
            if (!prereq.empty()) {
                course.prereqs.push_back(prereq);
            }
        }


        // Insert course into BST
        bst.Insert(course);
    }

    file.close();
    cout << "Data loaded successfully." << endl;
}

// =====================================================
// Print one course's information
// =====================================================
void PrintCourseInfo(BinarySearchTree& bst) {
    string courseNumber;
    cout << "Enter course number: ";
    cin >> courseNumber;

    Course* course = bst.Search(courseNumber);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->number << ", " << course->title << endl;

    if (course->prereqs.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites:" << endl;
        for (const string& prereq : course->prereqs) {
            cout << "  " << prereq << endl;
        }
    }
}

// =====================================================
// Main Menu
// =====================================================
int main() {
    int choice = 0;
    bool dataLoaded = false;

    BinarySearchTree bst;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course Information" << endl;
        cout << "9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        // Handle non-numeric input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        if (choice == 1) {
            string filename;
            cout << "Enter file name: ";
            cin.ignore();              // clear leftover newline from previous input
            getline(cin, filename);    // allow spaces in filename

            LoadCourses(filename, bst);
            dataLoaded = true;

        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load the data first." << endl;
            }
            else {
                bst.PrintInOrder();
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load the data first." << endl;
            }
            else {
                PrintCourseInfo(bst);
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << "Invalid option. Please choose a valid menu option." << endl;
        }
    }

    return 0;
}
