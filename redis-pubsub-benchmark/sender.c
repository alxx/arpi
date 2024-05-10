#include <stdio.h>
#include <sys/time.h>

#include "hiredis/hiredis.h"

struct timeval tv;
unsigned long long now, lastPulseAt, lastHookChangeAt, loopNow, firstPulseAt, numberDuration;

redisContext *c;

unsigned long long epoch_usec()
{
   gettimeofday(&tv, NULL);
   return (unsigned long long)(tv.tv_sec) * 1000000 + (unsigned long long)(tv.tv_usec) ;
}

int main()
{
  c = redisConnect("127.0.0.1", 6379);
  if (c->err) {
      printf("error: %s\n", c->errstr);
      return 1;
  }

  now = epoch_usec();

  for(long long i=0; i<=10000; i++)
  {
    redisCommand(c, "PUBLISH speedtest %llu", epoch_usec());
  }

  return 0;
}