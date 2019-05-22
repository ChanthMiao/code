#include "syscall.h"

int main(void)
{
    OpenFileId scu;
    Create("scu.txt");
    Create("scuTmp.txt");
    scu = Open("scu.txt", RW);
    Write("test", 5, scu);
    Close(scu);
    Delete("scuTmp.txt");
    Halt();
}