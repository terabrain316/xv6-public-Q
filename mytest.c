#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int pid = getpid();
  int child;

  printf(1, "== getnice ==\n");
  printf(1, "getnice(self)  = %d (expect 20)\n", getnice(pid));
  printf(1, "getnice(1)     = %d (expect 20)\n", getnice(1));
  printf(1, "getnice(2)     = %d (expect 20)\n", getnice(2));
  printf(1, "getnice(0)     = %d (expect -1)\n", getnice(0));
  printf(1, "getnice(-1)    = %d (expect -1)\n", getnice(-1));
  printf(1, "getnice(999)   = %d (expect -1)\n", getnice(999));

  printf(1, "== setnice ==\n");
  printf(1, "setnice(self,10)  = %d (expect 0)\n", setnice(pid, 10));
  printf(1, "getnice(self)     = %d (expect 10)\n", getnice(pid));
  printf(1, "setnice(self,0)   = %d (expect 0)\n", setnice(pid, 0));
  printf(1, "setnice(self,39)  = %d (expect 0)\n", setnice(pid, 39));
  printf(1, "setnice(self,-1)  = %d (expect -1)\n", setnice(pid, -1));
  printf(1, "setnice(self,40)  = %d (expect -1)\n", setnice(pid, 40));
  printf(1, "getnice(self)     = %d (expect 39, unchanged)\n", getnice(pid));
  printf(1, "setnice(999,10)   = %d (expect -1)\n", setnice(999, 10));
  printf(1, "setnice(0,10)     = %d (expect -1)\n", setnice(0, 10));

  printf(1, "== ps(0): all processes ==\n");
  ps(0);
  printf(1, "== ps(self) ==\n");
  ps(pid);
  printf(1, "== ps(999): expect nothing below ==\n");
  ps(999);
  printf(1, "== (end of ps(999)) ==\n");

  printf(1, "== child process ==\n");
  child = fork();
  if(child == 0){
    sleep(200);
    exit();
  }
  printf(1, "getnice(child)    = %d (expect 20)\n", getnice(child));
  printf(1, "setnice(child,5)  = %d (expect 0)\n", setnice(child, 5));
  ps(0);
  kill(child);
  wait();

  exit();
}
