
# copy_my - File Copy Program using Linux System Calls



## Overview
This program implements a file copy utility written in C that uses only Linux
kernel system calls. The purpose of the assignment is to understand how the
operating system handles basic file input and output operations without using
the C standard library file functions.

The program copies the contents of a source file into a destination file while
handling errors, user input, and file overwrite scenarios correctly and
efficiently.

## Development Environment
- Operating System: Linux (via WSL on Windows or native macOS terminal)
- Compiler: gcc
- Tools: bash terminal
- Language: C

## Compilation

gcc c.copy_my -o copy_my

## Execution

./copy_my <source_file> <destination_file>

Example:

./copy_my input.txt output.txt

## Program Objectives
- Copy a file using only Linux system calls.
- Handle invalid input and error cases safely.
- Detect if the destination file already exists and ask the user whether to
  overwrite it.
- Perform the copy operation efficiently using a fixed size buffer.
- Minimize the number of system calls.

## Functional and Technical Requirements Compliance

### Use of System Calls Only
The program uses only Linux system calls for file operations and I O:
- open
- read
- write
- close
- access

No C standard library file functions such as fopen, fread, fwrite, or fprintf
are used.

### Argument Validation
The program checks that exactly two command line arguments are provided:
1. Source file
2. Destination file

If the number of arguments is incorrect, the program prints a usage message to
standard error and exits.

### Source File Handling
The source file is opened using open() with the O_RDONLY flag.
If the file does not exist or cannot be opened due to permission issues, the
program prints an error message to stderr and exits.

### Destination File Handling and Overwrite Logic
Before opening the destination file, the program checks whether it already
exists using the access() system call.

If the destination file exists:
- The program prints a warning message to the user.
- The user is asked whether to continue (y) or cancel (n).
- The program waits for valid input.
- Any input other than y or n causes the prompt to repeat.
- If the user enters n, the program exits immediately without modifying files.
- If the user enters y, the destination file is overwritten.

All input and output operations for this process use system calls only.

### Opening the Destination File
The destination file is opened using open() with the following flags:
- O_WRONLY: open for writing
- O_CREAT: create the file if it does not exist
- O_TRUNC: truncate the file if overwrite was approved

The file permissions are set to 0644.

### Buffer Management and Efficiency
The program copies the file using a fixed size buffer of 4096 bytes.

This buffer size was chosen because:
- It matches a common memory page size in Linux systems.
- It significantly reduces the number of read() and write() system calls
  compared to copying byte by byte.
- It provides a good balance between memory usage and performance.

The program reads as much data as possible in each read() call and writes it
using write(). This minimizes context switches and kernel transitions.

### Handling Partial Writes
The write() system call may write fewer bytes than requested.
To handle this correctly, the program uses an inner loop that continues writing
until all bytes from the current buffer have been written to the destination
file.

This ensures data integrity and correctness of the copy operation.

### Error Handling
All system calls are checked for errors:
- open
- read
- write
- close

If an error occurs:
- A clear error message is printed to stderr using write().
- Any open file descriptors are closed.
- The program exits with a non zero status code.

### Single Process Execution
The program runs as a single process and does not use threads or child
processes, as required by the assignment.

## Program Flow Description
1. Validate command line arguments.
2. Open the source file for reading.
3. Check whether the destination file exists.
4. If it exists, prompt the user for overwrite confirmation.
5. Open or create the destination file.
6. Repeatedly read from the source file into a buffer.
7. Write the buffer contents to the destination file.
8. Continue until end of file is reached.
9. Close all file descriptors.
10. Exit with appropriate return value.

## Return Values
- Returns 0 on successful completion.
- Returns 1 on error.
- Returns 0 if the user cancels the overwrite operation.

## Files in Repository
- c.copy_my : source code file
- copy_my : compiled executable
- Makefile : optional
- README.md : this document

## Conclusion
This program fulfills all functional, technical, and efficiency requirements of
the assignment. It demonstrates proper use of Linux system calls, correct error
handling, efficient buffer based file copying, and safe interaction with the
user.

