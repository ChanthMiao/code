/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#define SHELL "/bin/sh"

void SysHalt();

int SysAdd(int op1, int op2);

int SysExp(int op1, int op2);

int SysDiv(int op1, int op2);

/* A unique identifier for an executing user program (address space) */
typedef int SpaceId;	

/* A unique identifier for a thread within a task */
typedef int ThreadId;

/* Address space control operations: Exit, Exec, Execv, and Join */

/* This user program is done (status = 0 means exited normally). */
void sysExit(int status);

/* Run the specified executable, with no args */
/* This can be implemented as a call to ExecV.
 */ 
SpaceId sysExec(char* exec_name);

/* Run the executable, stored in the Nachos file "argv[0]", with
 * parameters stored in argv[1..argc-1] and return the 
 * address space identifier
 */
SpaceId sysExecV(int argc, char* argv[]);
 
/* Only return once the user program "id" has finished.  
 * Return the exit status.
 */
int sysJoin(SpaceId id);
 
/* File system operations: Create, Remove, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * Note that the Nachos file system has a stub implementation, which
 * can be used to support these system calls if the regular Nachos
 * file system has not been implemented.
 */
 
/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;

/* when an address space starts up, it has two open files, representing 
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define ConsoleInput	0  
#define ConsoleOutput	1  
 
/* Create a Nachos file, with name "name" */
/* Note: Create does not open the file.   */
/* Return 1 on success, negative error code on failure */
int sysCreate(char *name);

/* Remove a Nachos file, with name "name" */
int sysDelete(char *name);

/* 
 * Open the Nachos file "name", and return an "OpenFileId" that can 
 * be used to read and write to the file. "mode" gives the requested 
 * operation mode for this file.
 */
#define RO 1
#define RW 2
#define APPEND 3
OpenFileId sysOpen(char *name, int mode);

/* 
 * Write "size" bytes from "buffer" to the open file. 
 * Return the number of bytes actually read on success.
 * On failure, a negative error code is returned.
 */
int sysWrite(char *buffer, int size, OpenFileId id);

/* 
 * Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int sysRead(char *buffer, int size, OpenFileId id);

/* 
 * Set the seek position of the open file "id"
 * to the byte "position".
 */
int sysSeek(int position, OpenFileId id);

/* 
 * Deletes a file with the filename given by "name".
 * An error is returned if file does not exist or other wicked things happen.
 */
int sysDelete(char* name);

/* Close the file, we're done reading and writing to it.
 * Return 1 on success, negative error code on failure
 */
int sysClose(OpenFileId id);


#endif /* ! __USERPROG_KSYSCALL_H__ */
