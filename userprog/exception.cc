// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which)
		{
    		case SyscallException:
      			switch(type)
						{
      					case SC_Halt:
								{
										DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
										SysHalt();
										ASSERTNOTREACHED();
										break;
								}
      					case SC_Add:
								{
										DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
										/* Process SysAdd Systemcall*/
										int result;
										result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
												/* int op2 */(int)kernel->machine->ReadRegister(5));
										DEBUG(dbgSys, "Add returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Div:
								{
										DEBUG(dbgSys, "Div " << kernel->machine->ReadRegister(4) << " / " << kernel->machine->ReadRegister(5) << "\n");
										/* Process SysAdd Systemcall*/
										int result;
										result = SysDiv(/* int op1 */(int)kernel->machine->ReadRegister(4),
												/* int op2 */(int)kernel->machine->ReadRegister(5));
										DEBUG(dbgSys, "Div returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Exp:
								{
										DEBUG(dbgSys, "Exp " << kernel->machine->ReadRegister(4) << " ^ " << kernel->machine->ReadRegister(5) << "\n");
										/* Process SysAdd Systemcall*/
										int result;
										result = SysExp(/* int op1 */(int)kernel->machine->ReadRegister(4),
												/* int op2 */(int)kernel->machine->ReadRegister(5));
										DEBUG(dbgSys, "Exp returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Exec:
								{
										char targetName[MAX_FILENAME + 1];
										int addr = kernel->machine->ReadRegister(4);
										int tmp;
										targetName[MAX_FILENAME] = '\0';
										for (int i = 0; i < MAX_FILENAME; i++)
										{
												kernel->machine->ReadMem(addr + i, 1, &tmp);
												targetName[i] = (char)tmp;
												if (targetName[i] == '\0')
												{
													break;
												}							
										}
										DEBUG(dbgSys, "Exec " << targetName << "\n");
										int result = sysExec(targetName);
										DEBUG(dbgSys, "Exec returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Join:
								{
										DEBUG(dbgSys, "Join " << kernel->machine->ReadRegister(4) << "\n");
										/* Process SysJoin Systemcall*/
										int result;
										result = sysJoin((int)kernel->machine->ReadRegister(4));
										DEBUG(dbgSys, "Join returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Create:
								{
										char targetName[MAX_FILENAME + 1];
										int addr = kernel->machine->ReadRegister(4);
										int tmp;
										targetName[MAX_FILENAME] = '\0';
										for (int i = 0; i < MAX_FILENAME; i++)
										{
												kernel->machine->ReadMem(addr + i, 1, &tmp);
												targetName[i] = (char)tmp;
												if (targetName[i] == '\0')
												{
													break;
												}
										}
										DEBUG(dbgSys, "Create " << targetName << "\n");
										int result = sysCreate(targetName);
										DEBUG(dbgSys, "Create returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Open:
								{
										char targetName[MAX_FILENAME + 1];
										int addr = kernel->machine->ReadRegister(4);
										int tmp;
										targetName[MAX_FILENAME] = '\0';
										for (int i = 0; i < MAX_FILENAME; i++)
										{
												kernel->machine->ReadMem(addr + i, 1, &tmp);
												targetName[i] = (char)tmp;
												if (targetName[i] == '\0')
												{
													break;
												}							
										}
										int mode = kernel->machine->ReadRegister(5);
										DEBUG(dbgSys, "Open " << targetName << " with mode " << mode << "\n");
										int result = sysOpen(targetName, mode);
										DEBUG(dbgSys, "Open returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Close:
								{
										DEBUG(dbgSys, "Close " << kernel->machine->ReadRegister(4) << "\n");
										/* Process SysClose Systemcall*/
										int result;
										result = sysClose((int)kernel->machine->ReadRegister(4));
										DEBUG(dbgSys, "Close returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Read:
								{	
										int addr = kernel->machine->ReadRegister(4);
										int tmp;
										int size = kernel->machine->ReadRegister(5);
										int id = kernel->machine->ReadRegister(6);
										char *target = (char*)malloc(size);
										DEBUG(dbgSys, "Read " << id << "\n");
										int result = sysRead(target, size, id);
										for (int i = 0; i < result; i++)
										{
												tmp = (int)target[i];
												kernel->machine->WriteMem(addr + i, 1, tmp);
										}
										delete target;
										DEBUG(dbgSys, "Read returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Write:
								{
										int addr = kernel->machine->ReadRegister(4);
										int tmp;
										int size = kernel->machine->ReadRegister(5);
										int id = kernel->machine->ReadRegister(6);
										char *target = (char*)malloc(size);
										DEBUG(dbgSys, "Write " << id << "\n");
										for (int i = 0; i < size; i++)
										{
												kernel->machine->ReadMem(addr + i, 1, &tmp);
												target[i] = (char)tmp;
										}
										int result = sysWrite(target, size, id);
										delete target;
										DEBUG(dbgSys, "Write returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
								case SC_Delete:
								{
										char targetName[MAX_FILENAME + 1];
										int addr = kernel->machine->ReadRegister(4);
										int tmp;
										targetName[MAX_FILENAME] = '\0';
										for (int i = 0; i < MAX_FILENAME; i++)
										{
												kernel->machine->ReadMem(addr + i, 1, &tmp);
												targetName[i] = (char)tmp;
												if (targetName[i] == '\0')
												{
													break;
												}							
										}
										DEBUG(dbgSys, "Delete " << targetName << "\n");
										int result = sysDelete(targetName);
										DEBUG(dbgSys, "Delete returning with " << result << "\n");
										/* Prepare Result */
										kernel->machine->WriteRegister(2, (int)result);
										/* Modify return point */
										{
										  /* set previous programm counter (debugging only)*/
										  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
										  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
										  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
										  /* set next programm counter for brach execution */
										  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
										}
										return;
										ASSERTNOTREACHED();
										break;
								}
      					default:
								{
										cerr << "Unexpected system call " << type << "\n";
										break;
								}
      			}
      			break;
    		default:
    		  cerr << "Unexpected user mode exception" << (int)which << "\n";
    		  break;
    }
    ASSERTNOTREACHED();
}
