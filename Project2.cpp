//============================================================================
// Name        : Project2.cpp
// Author      : Allen Jhane Dela Cruz
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Program that takes a file of courses, puts information into
//               a hash table, and prints course info using the hash table
//               data structure
// Source      : Project 2 that is based off of hash table bid assignment
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// define a structure to hold course information
struct Course{
    string courseNumber; // unique identifier
    string courseName;
    vector<string> prerequisites;
    Course() {
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            // key value set UINT_MAX since it's the best value to test a key to know if it was used or not
            key = UINT_MAX; //c++ expression largest unsigned int max value
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(string courseNumber);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void Remove(string courseNumber);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
HashTable::HashTable() {

    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);

}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 *
 * @param size - size of hashtable to be constructed
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    this->tableSize = size;

    // resize nodes size
    nodes.resize(tableSize);
}


/**
 * Destructor - to free storage when class is destroyed
 */
HashTable::~HashTable() {

    // erase nodes beginning
    nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param courseNumber - the course that contains the key to hash
 * @return hashedKey - the calculated hash
 */
unsigned int HashTable::hash(string courseNumber) {
    unsigned int hashedKey = 0;
    
    for (int i = 0; i < courseNumber.length(); i++) {
        hashedKey += courseNumber.at(i);
    }

    hashedKey = hashedKey % tableSize; // string to int since data was input as string when added to hash table
        
    return hashedKey;
}

/**
 * Insert a course into the hash table
 *
 * @param course - the course to insert into the hash table
 */
void HashTable::Insert(Course course) {

    // create the key for the given course
    unsigned int key = hash(course.courseNumber);

    // retrieve node using key
    // created new node with the address of course's key
    // also checks to see if there is a node at the indicated point/key position
    Node* node = &(nodes.at(key));

    // if no entry was found for the key
    // insert a new node into the nodes vector at the given course's key position
    if (node == nullptr) {
        // assign this node to the key position
        Node* insertNode = new Node(course, key);
        nodes.insert(nodes.begin() + key, *insertNode);
    }

    // else if node is not used (node is found but is not used)
    else if (node->key == UINT_MAX) {
        // assign old node key of UNIT_MAX to key, set old node course to course and old node next to null pointer
        // dont need to create new node since the given node is already in the vector and just needs the values implemented into the node
        node->key = key;
        node->course = course;
        node->next = nullptr;

    }

    // else find the next open node at the given key (used for key collision)
    else {
        // add new newNode to end
        // add new newNode to next available slot in the table
        while (node->next != nullptr) {
            node = node->next;
        }

        node->next = new Node(course, key);
    }

}

/**
 * Print all courses in the hash table
 */
void HashTable::PrintAll() {

    // Initialize variables that will hold information of hash table for printing
    Node* node;
    Course course;

    // for node begin to end iterate
    for (int i = 0; i < tableSize; ++i) {
        // assign node to the node address at vector based on i
        node = &(nodes.at(i));
        // get the node's current course and save for printing
        course = node->course;

        //   if key not equal to UINT_MAX
        if (node->key != UINT_MAX) {
            // output course number and course name
            cout << course.courseNumber << " | " << course.courseName << endl;

            // node is equal to next iter
            node = node->next;

            // while node not equal to nullptr
            while (node != nullptr) {
                course = node->course; // reset course to new value throughout each loop
                
                // output course number and course name
                cout << course.courseNumber << " | " << course.courseName << endl;

                // node is equal to next node
                node = node->next;
            }
        }
    }
    cout << endl;
}

/**
 * Remove a course
 *
 * @param courseNumber - the course number to search for
 */
void HashTable::Remove(string courseNumber) {

    // set key equal to hash of courseNumber
    unsigned int key = hash(courseNumber);

    // erase node begin and key
    nodes.erase(nodes.begin() + key);
}

/**
 * Search for the specified courseNumber
 *
 * @param courseNumber - The course number to search for
 * @return course - the course that was found
 */
Course HashTable::Search(string courseNumber) {
    Course course;

    // create the key for the given course
    unsigned int key = hash(courseNumber);

    // retrieve node using key
    // created new node with the address of course's key
    // checks to see if there is a node at the indicated point/key position
    Node* node = &(nodes.at(key));

    // if entry found for the key
    if (node != nullptr && (node->key != UINT_MAX) && (node->course.courseNumber == courseNumber)) {
        //return node course
        return node->course;
    }

    // if no entry found for the key
    if (node == nullptr || (node->key == UINT_MAX)) {
        return course;
    }

    // while node not equal to nullptr
    while (node != nullptr) {
        // if the current node matches, return it
        if ((node->key != UINT_MAX) && (node->course.courseNumber == courseNumber)) {
            return node->course;
        }

        //node is equal to next node
        node = node->next;
    }

    return course;
}

/**
 * Display the course information to the console (std::out)
 *
 * @param course - struct containing the course info
 */
void displayCourse(Course course, HashTable* courseTable) {
    Course prereq;
    // output course number, course name, and prerequisites if any
    cout << "Course Number: " << course.courseNumber << endl;
    cout << "Course Name: " << course.courseName << endl;
    cout << "Prerequisite(s): " << endl;

    // check if prerequisites for the course is empty
    if (!course.prerequisites.empty()) {
        for (int i = 0; i < course.prerequisites.size(); ++i) {
            cout << "- " << course.prerequisites.at(i) << ": "; // print prereq course num
            prereq = courseTable->Search(course.prerequisites.at(i)); // find prereq with course num to get info
            cout << prereq.courseName << endl; // print prereq's name
        }
        cout << endl;
    }
    else {
        cout << "None\n" << endl;
    }

    return;
}

/**
 * Load a txt file containing courses into a container
 *
 * @param file the path to the txt file to load
 * @param hashTable to load file contents into
 */
void loadCourses(string file, HashTable* hashTable) {
    ifstream courseFile(file);
    string fileline;
    vector<string> courseInfo;
    
    Course course;
    
    // open file if able to
    if (courseFile.is_open()){
        
        // look at each line in the file
        while (getline(courseFile, fileline)) {
            stringstream ss(fileline); // needed to split string
            string info;

            // split string at ','
            while (getline(ss, info, ',')) {
                courseInfo.push_back(info);
            }

            // store course info in appropriate variables
            course.courseNumber = courseInfo.at(0);
            course.courseName = courseInfo.at(1);

            // if there are remaining items in the string store as prereqs
            if (courseInfo.size() >= 3) {
                courseInfo.erase(courseInfo.begin());
                courseInfo.erase(courseInfo.begin());
                course.prerequisites = courseInfo;
            }

            // clear courseInfo vector for the next loop
            courseInfo.clear();

            // insert the course that was made into the hash table
            hashTable->Insert(course);
        }
        courseFile.close();
    }
    else { // error message if file could not be opened
        cout << "Unable to open file and place contents into a hash table";
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "courses.txt";
        courseKey = "98269";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the courses
    HashTable* courseTable;

    Course course;
    courseTable = new HashTable();
    string filename;

    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        // Load courses from given file to hash table
        case 1:

            //method call to load the courses
            cout << "Enter a course file to use: ";
            cin >> filename;
            loadCourses(filename, courseTable);
            cout << "\nCourses have been loaded into the data structure.\n" << endl;
            break;

        // print all the courses that are in the hash table
        case 2:
            cout << "\nBelow is the schedule of courses:\n" << endl;
            courseTable->PrintAll();
            break;

        // search for a certain course in the hash table based on user input
        case 3:
            cout << "\nPlease enter the course number you would like to know about: ";
            cin >> courseKey;
            course = courseTable->Search(courseKey);

            if (!course.courseNumber.empty()) {
                cout << "\nCOURSE INFORMATION" << endl;
                displayCourse(course, courseTable);
            }
            else {
                cout << "Course" << courseKey << " not found." << endl;
            }

            break;

        // tell user when incorrect input is given
        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    // option 9 to end program
    cout << "Good bye." << endl;

    return 0;
}