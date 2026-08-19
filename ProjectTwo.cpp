//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Zahia Chaib
// Version     : 1.0
// Description : ABCU Advising Assistance Program final deliverable
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

//============================================================================
// Course structure
//============================================================================

// Stores information for one course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

//============================================================================
// Helper functions
//============================================================================

// Remove spaces from the beginning and end of a string
string trim(const string& text) {

    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}


// Convert a string to uppercase
string toUpper(string text) {

    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        });

    return text;
}


// Remove quotation marks surrounding a file path
string removeQuotes(string text) {

    text = trim(text);

    if (text.length() >= 2) {

        if ((text.front() == '"' && text.back() == '"') ||
            (text.front() == '\'' && text.back() == '\'')) {

            text = text.substr(1, text.length() - 2);
        }
    }

    return text;
}


// Split one CSV line at commas
vector<string> splitLine(const string& line) {

    vector<string> values;
    string value;
    stringstream ss(line);

    while (getline(ss, value, ',')) {
        values.push_back(trim(value));
    }

    return values;
}


//============================================================================
// Binary Search Tree
//============================================================================

class BinarySearchTree {

private:

    // Node used inside the binary search tree
    struct Node {

        Course course;
        Node* left;
        Node* right;

        Node(const Course& courseData) {
            course = courseData;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;


    // Add a course recursively to the BST
    Node* addNode(Node* node, const Course& course) {

        // If this location is empty, create a new node
        if (node == nullptr) {
            return new Node(course);
        }

        // Smaller course numbers go to the left
        if (course.courseNumber < node->course.courseNumber) {

            node->left = addNode(node->left, course);
        }

        // Larger course numbers go to the right
        else if (course.courseNumber > node->course.courseNumber) {

            node->right = addNode(node->right, course);
        }

        return node;
    }


    // Print courses using in-order traversal
    void inOrder(Node* node) const {

        if (node != nullptr) {

            // Visit left subtree
            inOrder(node->left);

            // Print current course
            cout << node->course.courseNumber
                << ", "
                << node->course.courseTitle
                << endl;

            // Visit right subtree
            inOrder(node->right);
        }
    }


    // Delete all nodes from memory
    void destroyTree(Node* node) {

        if (node != nullptr) {

            destroyTree(node->left);
            destroyTree(node->right);

            delete node;
        }
    }


public:

    // Constructor
    BinarySearchTree() {
        root = nullptr;
    }


    // Destructor
    ~BinarySearchTree() {
        destroyTree(root);
    }


    // Insert a course
    void insert(const Course& course) {

        root = addNode(root, course);
    }


    // Search for a course by course number
    Course* search(const string& courseNumber) {

        Node* current = root;

        while (current != nullptr) {

            // Course was found
            if (courseNumber == current->course.courseNumber) {

                return &(current->course);
            }

            // Search left subtree
            if (courseNumber < current->course.courseNumber) {

                current = current->left;
            }

            // Search right subtree
            else {

                current = current->right;
            }
        }

        return nullptr;
    }


    // Print all courses alphabetically/alphanumerically
    void printInOrder() const {

        inOrder(root);
    }


    // Remove all existing courses
    void clear() {

        destroyTree(root);

        root = nullptr;
    }


    // Determine whether the tree has data
    bool isEmpty() const {

        return root == nullptr;
    }
};


//============================================================================
// File loading function
//============================================================================

bool loadCourses(const string& enteredFileName,
    BinarySearchTree& courseTree) {

    string fileName = removeQuotes(enteredFileName);

    ifstream inputFile;

    // Try opening exactly what the user entered
    inputFile.open(fileName);

    // If the user forgot ".csv", try adding it automatically
    if (!inputFile.is_open()) {

        if (fileName.length() < 4 ||
            toUpper(fileName.substr(fileName.length() - 4)) != ".CSV") {

            string csvFileName = fileName + ".csv";

            inputFile.clear();
            inputFile.open(csvFileName);

            if (inputFile.is_open()) {
                fileName = csvFileName;
            }
        }
    }


    // Make sure the file opened correctly
    if (!inputFile.is_open()) {

        cout << endl;
        cout << "Error: Unable to open file \"" << fileName << "\"." << endl;
        cout << "Please check the file name or enter the full file path." << endl;

        return false;
    }


    // Temporary storage allows us to validate the entire file
    // before putting courses into the BST.
    vector<Course> courses;

    // Keeps track of every course number in the file
    unordered_set<string> courseNumbers;

    string line;

    int lineNumber = 0;


    //========================================================================
    // First pass: read and parse every course
    //========================================================================

    while (getline(inputFile, line)) {

        lineNumber++;

        line = trim(line);

        // Ignore completely empty lines
        if (line.empty()) {
            continue;
        }


        // Remove UTF-8 BOM if it exists at the beginning of the file
        if (lineNumber == 1 &&
            line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {

            line = line.substr(3);
        }


        vector<string> fields = splitLine(line);


        // Each course must have at least:
        // course number and course title
        if (fields.size() < 2) {

            cout << "Error: Invalid format on line "
                << lineNumber << "." << endl;

            inputFile.close();

            return false;
        }


        Course course;

        course.courseNumber = toUpper(trim(fields[0]));
        course.courseTitle = trim(fields[1]);


        // Check for missing course number or title
        if (course.courseNumber.empty() ||
            course.courseTitle.empty()) {

            cout << "Error: Missing course number or title on line "
                << lineNumber << "." << endl;

            inputFile.close();

            return false;
        }


        // Check for duplicate courses
        if (courseNumbers.find(course.courseNumber)
            != courseNumbers.end()) {

            cout << "Error: Duplicate course number "
                << course.courseNumber
                << " found on line "
                << lineNumber
                << "." << endl;

            inputFile.close();

            return false;
        }


        courseNumbers.insert(course.courseNumber);


        // Fields after the title are prerequisites
        for (size_t i = 2; i < fields.size(); i++) {

            string prerequisite =
                toUpper(trim(fields[i]));

            if (!prerequisite.empty()) {

                course.prerequisites.push_back(prerequisite);
            }
        }


        courses.push_back(course);
    }


    inputFile.close();


    // Make sure the file actually contained courses
    if (courses.empty()) {

        cout << "Error: The file does not contain any course data."
            << endl;

        return false;
    }


    //========================================================================
    // Second pass: validate prerequisites
    //========================================================================

    for (const Course& course : courses) {

        for (const string& prerequisite :
            course.prerequisites) {

            // Every prerequisite must also exist as a course
            if (courseNumbers.find(prerequisite)
                == courseNumbers.end()) {

                cout << "Error: Prerequisite "
                    << prerequisite
                    << " for "
                    << course.courseNumber
                    << " does not exist in the course file."
                    << endl;

                return false;
            }
        }
    }


    //========================================================================
    // Load validated courses into the BST
    //========================================================================

    // Clear old information if the user loads another file
    courseTree.clear();

    for (const Course& course : courses) {

        courseTree.insert(course);
    }


    cout << endl;
    cout << courses.size()
        << " courses successfully loaded from "
        << fileName
        << "."
        << endl;

    return true;
}


//============================================================================
// Display all courses
//============================================================================

void printCourseList(const BinarySearchTree& courseTree) {

    // User must load data before printing
    if (courseTree.isEmpty()) {

        cout << endl;
        cout << "Please load the course data first using option 1."
            << endl;

        return;
    }


    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    // In-order BST traversal automatically prints
    // the course numbers from lowest to highest.
    courseTree.printInOrder();

    cout << endl;
}


//============================================================================
// Display information for one course
//============================================================================

void printCourseInformation(BinarySearchTree& courseTree) {

    // User must load data before searching
    if (courseTree.isEmpty()) {

        cout << endl;
        cout << "Please load the course data first using option 1."
            << endl;

        return;
    }


    string courseNumber;

    cout << endl;
    cout << "What course do you want to know about? ";

    getline(cin, courseNumber);

    courseNumber = toUpper(trim(courseNumber));


    // Search for the requested course
    Course* course = courseTree.search(courseNumber);


    if (course == nullptr) {

        cout << endl;
        cout << "Course "
            << courseNumber
            << " was not found."
            << endl;

        return;
    }


    // Print course number and title
    cout << endl;

    cout << course->courseNumber
        << ", "
        << course->courseTitle
        << endl;


    // Print prerequisites
    cout << "Prerequisites: ";


    if (course->prerequisites.empty()) {

        cout << "None";
    }

    else {

        for (size_t i = 0;
            i < course->prerequisites.size();
            i++) {

            string prerequisiteNumber =
                course->prerequisites[i];

            Course* prerequisite =
                courseTree.search(prerequisiteNumber);


            // Print prerequisite course number and title
            if (prerequisite != nullptr) {

                cout << prerequisite->courseNumber
                    << ", "
                    << prerequisite->courseTitle;
            }

            else {

                cout << prerequisiteNumber;
            }


            // Add comma between multiple prerequisites
            if (i < course->prerequisites.size() - 1) {

                cout << ", ";
            }
        }
    }


    cout << endl;
}


//============================================================================
// Main program
//============================================================================

int main() {

    BinarySearchTree courseTree;

    int choice = 0;


    cout << "Welcome to the course planner." << endl;


    // Continue displaying the menu until the user chooses option 9
    while (choice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << endl;

        cout << "What would you like to do? ";


        // Validate numeric menu input
        if (!(cin >> choice)) {

            cout << endl;
            cout << "Invalid input. Please enter 1, 2, 3, or 9."
                << endl;

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            continue;
        }


        // Remove the newline remaining after cin
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );


        //====================================================================
        // Option 1: Load course data
        //====================================================================

        if (choice == 1) {

            string fileName;

            cout << endl;
            cout << "Enter the course data file name or full path: ";

            getline(cin, fileName);

            loadCourses(fileName, courseTree);
        }


        //====================================================================
        // Option 2: Print all courses
        //====================================================================

        else if (choice == 2) {

            printCourseList(courseTree);
        }


        //====================================================================
        // Option 3: Print information about one course
        //====================================================================

        else if (choice == 3) {

            printCourseInformation(courseTree);
        }


        //====================================================================
        // Option 9: Exit
        //====================================================================

        else if (choice == 9) {

            cout << endl;
            cout << "Thank you for using the course planner!"
                << endl;
        }


        //====================================================================
        // Invalid menu option
        //====================================================================

        else {

            cout << endl;
            cout << choice
                << " is not a valid option."
                << endl;

            cout << "Please enter 1, 2, 3, or 9."
                << endl;
        }
    }


    return 0;
}