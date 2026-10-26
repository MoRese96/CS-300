/*
 * Name: Mo Witherspoon
 * Course: CS 300
 * Project: Project Two - Advising Assistance Program
 * Date: 10/18/2026
 *
 * Description:
 * This program loads course data from a CSV file into a
 * Binary Search Tree (BST). Users can print a sorted
 * list of courses and search for a specific course to
 * view its title and prerequisites.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

//=====================================================
// Course Structure
//=====================================================

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() {
        courseNumber = "";
        courseTitle = "";
    }
};

//=====================================================
// BST Node
//=====================================================

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

//=====================================================
// Binary Search Tree Class
//=====================================================

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course) {

        if (course.courseNumber < node->course.courseNumber) {

            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {

            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {

        if (node != nullptr) {

            inOrder(node->left);

            cout << node->course.courseNumber
                << ", "
                << node->course.courseTitle
                << endl;

            inOrder(node->right);
        }
    }

    void destroyTree(Node* node) {

        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:

    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void Insert(Course course) {

        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    void InOrder() {
        inOrder(root);
    }

    Course Search(string courseNumber) {

        Node* current = root;

        while (current != nullptr) {

            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        Course emptyCourse;
        return emptyCourse;
    }
};

//=====================================================
// Helper Functions
//=====================================================

string toUpperCase(string text) {

    transform(text.begin(),
        text.end(),
        text.begin(),
        ::toupper);

    return text;
}

//=====================================================
// Load CSV Course Data
//=====================================================

void loadCourses(string fileName,
    BinarySearchTree& bst) {

    ifstream file(fileName);

    if (!file.is_open()) {

        cout << "Unable to open file: "
            << fileName << endl;

        return;
    }

    string line;

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        Course course;
        string token;

        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseTitle, ',');

        while (getline(ss, token, ',')) {

            if (!token.empty()) {
                course.prerequisites.push_back(token);
            }
        }

        bst.Insert(course);
    }

    file.close();

    cout << "Data loaded successfully." << endl;
}

//=====================================================
// Main Program
//=====================================================

int main() {

    BinarySearchTree bst;

    bool dataLoaded = false;

    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        cout << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {

        case 1:

            cout << "Enter file name: ";
            cin >> fileName;

            loadCourses(fileName, bst);
            dataLoaded = true;

            break;

        case 2:

            if (!dataLoaded) {

                cout << "Please load the data file first."
                    << endl;

                break;
            }

            cout << endl;
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            bst.InOrder();

            break;

        case 3:

            if (!dataLoaded) {

                cout << "Please load the data file first."
                    << endl;

                break;
            }

            cout << "What course do you want to know about? ";
            cin >> courseNumber;

            courseNumber = toUpperCase(courseNumber);

            {
                Course course = bst.Search(courseNumber);

                if (course.courseNumber.empty()) {

                    cout << "Course not found." << endl;
                }
                else {

                    cout << endl;
                    cout << course.courseNumber
                        << ", "
                        << course.courseTitle
                        << endl;

                    if (course.prerequisites.empty()) {

                        cout << "Prerequisites: None"
                            << endl;
                    }
                    else {

                        cout << "Prerequisites: ";

                        for (size_t i = 0;
                            i < course.prerequisites.size();
                            i++) {

                            cout << course.prerequisites[i];

                            if (i < course.prerequisites.size() - 1) {
                                cout << ", ";
                            }
                        }

                        cout << endl;
                    }
                }
            }

            break;

        case 9:

            cout << "Thank you for using the course planner!"
                << endl;

            break;

        default:

            cout << choice
                << " is not a valid option."
                << endl;
        }
    }

    return 0;
}