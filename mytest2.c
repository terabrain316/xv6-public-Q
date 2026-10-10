#include "types.h"
#include "stat.h"
#include "user.h"

// Spin forever so the scheduler has CPU-bound work to share.
static void
spin(void)
{
  volatile int x = 0;
  for(;;)
    x++;
}

int
main(int argc, char *argv[])
{
  int me = getpid();
  int nices[3] = {15, 20, 25};   // weights 3121, 1024, 335
  int pids[3];
  int i, c;

  printf(1, "\n=== TEST 1: fork inherits nice, runtime, vruntime ===\n");
  setnice(me, 15);
  printf(1, "-- parent before fork --\n");
  ps(me);
  c = fork();
  if(c == 0){
    printf(1, "-- child right after fork (expect nice 15, runtime/vruntime\n"
              "   equal to or just above the parent's values) --\n");
    ps(getpid());
    exit();
  }
  wait();
  setnice(me, 20);

  printf(1, "\n=== TEST 2: CPU share follows weight ===\n");
  for(i = 0; i < 3; i++){
    c = fork();
    if(c == 0){
      setnice(getpid(), nices[i]);
      spin();
    }
    pids[i] = c;
  }

  sleep(500);   // let the children compete for ~5 seconds

  printf(1, "-- ps(0) after 500 ticks --\n");
  ps(0);
  printf(1, "Expect for the three spinning children:\n"
            "  runtime ratio nice15 : nice20 : nice25 ~= 9.3 : 3 : 1\n"
            "  vruntime values close to each other\n"
            "TEST 3 (wake-up rule): this parent just woke from sleep(500),\n"
            "  so its vruntime should be about (min child vruntime - 1000).\n");

  for(i = 0; i < 3; i++)
    kill(pids[i]);
  for(i = 0; i < 3; i++)
    wait();

  printf(1, "\n=== done ===\n");
  exit();
}
