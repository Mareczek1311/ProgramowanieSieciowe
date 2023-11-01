#include <sys/ptrace.h>
#include <unistd.h>

int main()
{
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    while(1){
        sleep(1);
    }
}
