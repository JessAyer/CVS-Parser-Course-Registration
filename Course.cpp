//============================================================================
// Name        : Course.cpp
// Author      : Jessica Ayer
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Project 2
//============================================================================

#include <iostream>
#include <fstream>
#include "CSVparser.hpp"

using namespace std;
using std::cout;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// structure to hold course information: Number, Name, and prerequistes 
struct Course {
    string courseNumber; 
    string courseName;
    vector<string>preRequisites;

    Course() {
    }
};
   
// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    //defualt constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //initialize with bid
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Defines a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void printAllCourses(Node* node);

public: 
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void PrintAllCourses();
    void Insert(Course course);
    int NumberPrerequisites(Course course);
    void SearchCourse(string courseNumber);
    void FindPreReqs(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
* pass root node to private  printAllCourses method
*/

void BinarySearchTree::PrintAllCourses() {
    this->printAllCourses(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if the root equals a null pointer
    if (root == nullptr) {
        // root is equal to a new node course
        root = new Node(course);
    }
    else {
        //add Node root and course
        this->addNode(root, course);
    }
}

/**
* Determine number of prerequisite courses
*/

int BinarySearchTree::NumberPrerequisites(Course course) {
    int numPrereqs = course.preRequisites.size();
        return numPrereqs;
}
/**
* Find course prerequisites
*/
void BinarySearchTree::FindPreReqs(string courseNumber) {
    // set node to equal the root
    Node* current = root;
    // keep looping downwards until bottom reached or matching course number found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseNumber.compare(courseNumber) == 0) {
            //output course name
            cout << current->course.courseName;
            return;
        }
        // if course number is smaller than current node then traverse left
        else if (courseNumber.compare(current->course.courseNumber) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
}
/** 
*Search for a course
*/

void BinarySearchTree::SearchCourse(string courseNumber) {   
    string preReq1;
    string preReq2;
    // set node to equal the root
    Node* current = root;
    // keep looping downwards until bottom reached or matching course number found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseNumber.compare(courseNumber) == 0) {
            //output course information
            cout << endl << current->course.courseNumber << ": "
                << current->course.courseName << endl;

            unsigned int numPrereqs = NumberPrerequisites(current->course);
 
            cout << "Prerequisites: ";
            if (numPrereqs == 0) {
                cout << "none required" << endl;
            }
            for (unsigned int i = 0; i < numPrereqs; i) {
                preReq1 = current->course.preRequisites.at(i);
                cout << preReq1 << " ";
                FindPreReqs(preReq1);
                ++i;
                if (i <= numPrereqs) {
                   cout << ", ";
                   preReq2 = current->course.preRequisites.at(i);
                   cout << preReq2 << " ";
                   FindPreReqs(preReq2);
                   }
                cout << endl;
                return;
            }
        return;
        }
        // if course number is smaller than current node then traverse left
        else if (courseNumber.compare(current->course.courseNumber) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    // If course is not found
    cout << "The requested course is not found." << endl;
}



/**
* Add a course
*/

void BinarySearchTree::addNode(Node* node, Course course){
    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the right node
            this->addNode(node->right, course);
        }
    }
}


/**
* Print All Course in alphanumeric order
*/

void BinarySearchTree::printAllCourses(Node* node) {
    // if node is not equal to null ptr
    if (node != nullptr) {
        //printAll Courses left
        printAllCourses(node->left);
        // output course information
        cout << node->course.courseNumber << ": "
            << node->course.courseName << endl;
        // printAllCourses right
        printAllCourses(node->right);
    }
}


//============================================================================
// Load File
//============================================================================
/**
 * Load a file containing course into a container
*/
void loadCourses(string csvPath, BinarySearchTree* bst) {

    cout << "Loading file " << csvPath << endl;

    // Open course file, get/ separate each line & insert into BST
    try {
        ifstream file(csvPath);
      if (file.is_open()) {
            while (!file.eof()) {
                // Define a vector data structure to hold courses.
                vector<string> courseInformation;
                string courseInfo;

                getline(file, courseInfo);
                while (courseInfo.length() > 0) {

                    // Add to vector
                    unsigned int comma = courseInfo.find(',');
                    if (comma < 100) { 
                        courseInformation.push_back(courseInfo.substr(0, comma));
                        courseInfo.erase(0, comma + 1);
                    }
                    else {
                   courseInformation.push_back(courseInfo.substr(0, courseInfo.length()));
                   courseInfo = "";
                    }
                }
                // Create a data structure and add to the courses
                Course course;
                course.courseNumber = courseInformation[0];
                course.courseName = courseInformation[1];

                for (unsigned int i = 2; i < courseInformation.size(); i++) {
                    course.preRequisites.push_back(courseInformation[i]);
                }
                // push this course to the end
                bst->Insert(course);
            }
            file.close();
            return;
        }
    }

    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return;
}

//============================================================================
// Main Method
//============================================================================
/**
*/
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseNumber;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseNumber = argv[2];
        break;
    default:
        csvPath = "";
        break;
    }

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst = nullptr;
    
    Course course;

    cout << "Welcome to the course planner." << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();
             cout << "Enter course file name: ";
             cin >> csvPath;
            
                // load the courses
            loadCourses(csvPath, bst);
            cout << "File succesfully imported." << endl;
            cout << endl;
            break;

        case 2:
            cout << "Available Courses: " << endl;
            bst->PrintAllCourses();
            cout << endl;
            break;

        case 3:
            cout << "Enter course number: ";
            cin >> courseNumber;
            bst->SearchCourse(courseNumber);
            cout << endl;  
            break;
            
            // User entered invalid data or exited the program
        default:

            if (choice != 9)
                cout << choice << " is not a valid option\n" << endl;
            break;

        }
 
    }

    cout << "Good bye." << endl;

    return 0;
}