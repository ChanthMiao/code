#include "ksyscall.h"
#include "main.h"
#include "errno.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysExp(int op1, int op2)
{
  int result = 1;
  int tmp_rt;
  while (op2 > 1)
  {
    tmp_rt = op1;
    int i;
    for (i = 2; i <= op2; i *= 2)
    {
      tmp_rt *= tmp_rt;
    }
    result *= tmp_rt;
    op2 -= i / 2;
  }
  if (op2 == 1)
  {
    result *= op1;
  }
  return result;
}

int SysDiv(int op1, int op2)
{
  return op1 / op2;
}
#ifndef NACHOS_THREAD_COMPELETED
/* This user program is done (status = 0 means exited normally). */
void sysExit(int status)
{
    exit(status);
}

/* Run the specified executable, with no args */
/* This can be implemented as a call to ExecV.
 */ 
SpaceId sysExec(char* exec_name)
{
    pid_t child;
    child = vfork();
    if(child == 0)
    {
	execl (SHELL, SHELL, "-c", exec_name, NULL);
	_exit (EXIT_FAILURE);
    }
    else if(child < 0) 
	return EPERM;
    return (SpaceId) child;
}

/* Run the executable, stored in the Nachos file "argv[0]", with
 * parameters stored in argv[1..argc-1] and return the 
 * address space identifier
 */
SpaceId sysExecV(int argc, char* argv[])
{
    pid_t child;
    child = vfork();
    if(child == 0){
	execl (SHELL, SHELL, "-c", argv, NULL);
	_exit (EXIT_FAILURE);
    }
    else if(child < 0) 
	return EPERM;
    return (SpaceId) child;
}
 
/* Only return once the user program "id" has finished.  
 * Return the exit status.
 */
int sysJoin(SpaceId id)
{
    return waitpid((pid_t) id, (int*) 0, 0);
}

#else
//TODO: the nachos thread based syscall implements are unavailable
#endif

/*Since the nachos filesys based syscall implements are currently unavailable, use platform api instead.*/
#ifndef NACHOS_FILESYS_COMPELETED

/* Create a Nachos file, with name "name" */
/* Note: Create does not open the file.   */
/* Return 1 on success, negative error code on failure */
int sysCreate(char *name)
{
    int fd;
    fd=open(name, O_TRUNC | O_CREAT |  O_WRONLY, 0666);
    if (fd>0) {
	      close(fd);
	      return 1;
    }else return fd;
}

/* Remove a Nachos file, with name "name" */
int sysRemove(char *name)
{
    return remove(name);
}

/* 
 * Open the Nachos file "name", and return an "OpenFileId" that can 
 * be used to read and write to the file. "mode" gives the requested 
 * operation mode for this file.
 */

OpenFileId sysOpen(char *name, int mode)
{
    int pmode;
    switch(mode){
	case RO: pmode=O_RDONLY;
	    break;
	case RW: pmode=O_RDWR;
	    break;
	case APPEND:
	    pmode=O_APPEND;
	    break;
	default: return -1;
    }
    return open(name, pmode);
}

/* 
 * Write "size" bytes from "buffer" to the open file. 
 * Return the number of bytes actually read on success.
 * On failure, a negative error code is returned.
 */
int sysWrite(char *buffer, int size, OpenFileId id)
{
    return write(id, buffer, (size_t) size);
}

/* 
 * Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int sysRead(char *buffer, int size, OpenFileId id)
{
    return read(id, buffer, (size_t) size);
}

/* 
 * Set the seek position of the open file "id"
 * to the byte "position".
 */
int sysSeek(int position, OpenFileId id)
{
    return (int) lseek(id, SEEK_SET, position);
}

/* 
 * Deletes a file with the filename given by "name".
 * An error is returned if file does not exist or other wicked things happen.
 */
int sysDelete(char* name)
{
    return unlink(name);
}

/* Close the file, we're done reading and writing to it.
 * Return 1 on success, negative error code on failure
 */
int sysClose(OpenFileId id)
{
    return close(id);
}

#else
//TODO: the syscall based on nachos's filesys is unavailable now.
#endif