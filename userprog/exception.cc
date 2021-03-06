// exception.cc
	//	Entry point into the Nachos kernel from user programs.
	//	There are two kinds of things that can cause control to
	//	transfer back to here from user code:
	//
	//	syscall -- The user code explicitly requests to call a procedure
	//	in the Nachos kernel. Right now, the only function we support is
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
	// Copyright (c) 1992-1993 The Regents of the University of California.
	// All rights reserved. See copyright.h for copyright notice and limitation
	// of liability and disclaimer of warranty provisions.
	
	#include "copyright.h"
	#include "system.h"
	#include "syscall.h"
	#include "math.h"
	#define MAX 200
	
	// increase program counter
	
	void IncreasePC()
	{
		int cur = machine->ReadRegister(PCReg);
		machine->WriteRegister(PrevPCReg, cur);
		//increase
		cur = machine->ReadRegister(NextPCReg);
		machine->WriteRegister(PCReg, cur);
	
		machine->WriteRegister(NextPCReg, cur + 4); //A MIPS instruction is 32 bits or 4 bytes (always).
	}
	
	//----------------------------------------------------------------------
	// ExceptionHandler
	// Entry point into the Nachos kernel. Called when a user program
	//	is executing, and either does a syscall, or generates an addressing
	//	or arithmetic exception.
	//
	// For system calls, the following is the calling convention:
	//
	// system call code -- r2
	//	arg1 -- r4
	//	arg2 -- r5
	//	arg3 -- r6
	//	arg4 -- r7
	//
	//	The result of the system call, if any, must be put back into r2.
	//
	// And don't forget to increment the pc before returning. (Or else you'll
	// loop making the same system call forever!
	//
	//	"which" is the kind of exception. The list of possible exceptions
	//	are in machine.h.
	//----------------------------------------------------------------------
	
	void
	ExceptionHandler(ExceptionType which)
	{
		int type = machine->ReadRegister(2);
	
		int op1, op2, result;
		int size;
		char s[200];
		char *s2;
		int a;
	
		switch(which)
		{
			case NoException:
				return;

			case PageFaultException:
				printf("PageFaultException dang dien ra");
				DEBUG('a', "No valid translation found\n");
				printf("No valid translation found\n");
				interrupt->Halt();
				break;
		
			case ReadOnlyException:
				DEBUG('a', "Write attempted to page marked read-only\n");
				printf("Write attempted to page marked read-only\n");
				interrupt->Halt();
				break;
		
			case BusErrorException:
				DEBUG('a', "Translation resulted invalid physical address\n");
				printf("Translation resulted invalid physical address\n");
				interrupt->Halt();
				break;
		
			case AddressErrorException:
				DEBUG('a', "Unaligned reference or one that was beyond the end of the address space\n");
				printf("Unaligned reference or one that was beyond the end of the address space\n");
				interrupt->Halt();
				break;
		
			case OverflowException:
				DEBUG('a', "Integer overflow in add or sub.\n");
				printf("Integer overflow in add or sub.\n");
				interrupt->Halt();
				break;
		
			case IllegalInstrException:
				DEBUG('a', "Unimplemented or reserved instr.\n");
				printf("Unimplemented or reserved instr.\n");
				interrupt->Halt();
				break;
		
			case NumExceptionTypes:
				DEBUG('a', "Number exception types\n");
				printf("Number exception types\n");
				interrupt->Halt();
				break;
		

			case SyscallException:
			{
				switch(type)
				{
					case SC_Halt:
						DEBUG('a', "Shutdown, initiated by user program.\n");
						interrupt->Halt();
						break;


					case SC_ReadInt:
						ReadInt();
						break;
	
					case SC_PrintInt:
						PrintInt();
						break;
	
					/*case SC_ReadChar:
						ReadChar();
						break;
	
					case SC_PrintChar:
						PrintChar();
						break;
	
					case SC_ReadString:
						ReadString();
						break;
	
					case SC_PrintString:
						PrintString();
						break;*/
				}
				IncreasePC();
	
				break;
			}
			default:
				printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
		}
	
	
	}

	
	int ReadInt()
	{
		char* buf = new char[MAX+1];
		int numBytes = gSynchConsole->Read(buf, 256);
		int cur = 0;
		int res = 0;
		bool isValid = true;
		bool dot = false;
		for(int i = cur; i < numBytes; i++)
		{
			if (buf[i] == '.')
			{
				dot = true;
				cur = i + 1;
				break;
			}
		}
		if (dot)
		{
			for (int i = cur; i < numBytes; i++)
			{
				if (buf[i] != '0')
				{
					isValid = false;
					break;
				}
			}
		}
		if (!isValid)
		{
			printf("\n\n The number is invalid");
			machine->WriteRegister(2, 0);
		}
		else
		{
			res = atoi(buf);
			machine->WriteRegister(2,res);
		}
		delete buf;

		return res;
	}

	void PrintInt()
	{
		bool isNega = false;
		
		int number = machine->ReadRegister(4);
		int len = (int)log10(number) + 1; // dem so chu so
		int i = len - 1;

		char* fal = new char[1];
		fal[0] = '0';
		

		if (number == 0)
		{
			gSynchConsole->Write(fal,1);		
			IncreasePC();
			return;
		}

		
		if(number < 0)
		{
			isNega = true;
			number *= (-1);
		}
		
		if (isNega) len += 1;
		char* buf = new char[len+1];
		buf[len] = '\0';
		
		while(number > 0)
		{
			buf[i] = (char)((number % 10) + '0');
			number /= 10;
			i--;
		}
		if(isNega) buf[0] = '-';
		gSynchConsole->Write(buf, len +1);
		return;
	}
	
	
	/*
	void ReadChar(){
		char* buffer = new char[MAX+1];
		int numBytes = gSynchConsole->Read(buffer, MAX+1);
		if (numBytes == -1)
		{
			printf("\n Error reading character \n")
			break;
		}
		else (numBytes > 1)
		{
			printf("\n Error too big for char \n")
			break;
		}
		machine->WriteRegister(2, buffer[0]);
		delete buffer;
		return;
	}
	
	void PrintChar(){
		char c = (char)machine->ReadRegister(4);
		gSynchConsole->Write(&c, 1);
		return;
	}*/
