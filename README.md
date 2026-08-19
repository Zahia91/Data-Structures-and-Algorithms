# Data-Structures-and-Algorithms
# CS 300 Project Two Reflection

## What was the problem you were solving in the projects for this course?

The primary problem I addressed in CS 300 was determining how algorithms and data structures could be used to efficiently organize, search, and display academic course information for the ABCU Computer Science department. The advising application needed to read course information from a CSV file, store that information in an appropriate data structure, display the complete course list in alphanumeric order, and allow an academic advisor to search for a specific course and view its prerequisites.

In Project One, I analyzed three possible data structures: a vector, hash table, and binary search tree (BST). I compared their run-time behavior, memory requirements, searching capabilities, insertion performance, and ability to produce sorted output. Based on this analysis, I recommended a binary search tree. In Project Two, I implemented that design as a working C++ command-line application.

## How did you approach the problem? Why are data structures important to understand?

I approached the project by first considering the operations the application needed to perform rather than selecting a data structure based only on ease of implementation. Because the application needed both individual course lookup and an alphanumerically ordered course list, I evaluated how each data structure would support those operations.

I selected a binary search tree because course numbers could be used to organize the nodes. When the tree is reasonably balanced, searching and insertion can achieve approximately O(log n) performance. More importantly for this application, an in-order traversal visits the nodes from the lowest course number to the highest, allowing the complete course list to be displayed in O(n) traversal time without requiring a separate sorting algorithm.

Understanding data structures is important because the way data is organized directly affects the efficiency and complexity of the algorithms operating on it. A vector, hash table, and binary search tree can all store the same course objects, but they provide different performance characteristics for searching, insertion, traversal, and ordered output. This project demonstrated that choosing an appropriate data structure should be based on the requirements of the application and the operations that will be performed most frequently.

## How did you overcome roadblocks encountered during the activities and project?

One of the main challenges I encountered during Project Two involved reading the external CSV course-data file. The program initially could not locate the file even though the file was included with the project. Through testing, I learned that the location of a file in the Visual Studio project and the program's current working directory are not necessarily the same. I resolved the issue by verifying the file location and testing the application with both the complete file path and the filename when the CSV file was placed in the appropriate working directory.

I also used incremental testing throughout development rather than attempting to troubleshoot the entire application at once. I first verified that the program compiled successfully. I then tested loading the course data, followed by printing the complete course list, searching for an individual course, displaying prerequisite information, handling an invalid menu selection, and searching for a course that did not exist. Breaking the testing process into smaller operations made it easier to isolate problems and verify that previously completed functionality continued to work.

The final program also includes defensive checks for situations such as an inaccessible input file, invalid file data, duplicate course numbers, invalid prerequisites, invalid menu input, and course numbers that cannot be found. Working through these issues reinforced the importance of considering failure conditions as part of software development rather than designing only for successful input.

## How has your work on this project expanded your approach to designing software and developing programs?

This project expanded my approach to software development by reinforcing the importance of separating design decisions from implementation. Before writing the final application, I analyzed the expected operations and compared multiple data structures using their run-time and memory characteristics. This allowed the implementation in Project Two to follow a design that was based on the application's requirements.

I also developed a stronger understanding of how algorithms and data structures work together. For example, selecting a binary search tree did more than provide a place to store the course objects. The structure of the BST directly supported the algorithm used to produce the required sorted output. By performing an in-order traversal of the tree, the program could visit courses in ascending course-number order naturally.

This experience has encouraged me to think about scalability, algorithmic complexity, data organization, input validation, and error handling earlier in the development process. Instead of focusing only on whether a program produces the expected output, I now consider how the program organizes its data, how efficiently operations are performed, and how the design will behave as the amount of data increases.

## How has your work on this project evolved the way you write maintainable, readable, and adaptable programs?

Project Two improved the way I organize C++ programs by emphasizing modularity, descriptive naming, documentation, and separation of responsibilities. Instead of placing all functionality inside the `main()` function, I separated major operations into dedicated functions and methods for loading course data, inserting courses into the binary search tree, searching for courses, traversing the tree, printing course information, and validating input.

I also used descriptive identifiers such as `courseNumber`, `courseTitle`, `prerequisites`, `loadCourses()`, `printCourseList()`, and `printCourseInformation()`. Inline comments and clearly identified sections make the program easier to follow and provide context for important operations such as BST insertion and in-order traversal.

Another improvement was designing the program to handle unexpected conditions without terminating unnecessarily. The application validates menu selections, checks whether course data has been loaded before allowing certain operations, reports missing files, handles nonexistent courses, and validates course and prerequisite information. These practices improve reliability while also making the program easier to maintain.

Overall, Project Two strengthened my understanding that maintainable software is not simply code that works. A well-designed program should communicate its purpose through its structure, use appropriate algorithms and data structures, handle errors predictably, and be organized so that individual components can be modified or extended without requiring the entire application to be rewritten.
