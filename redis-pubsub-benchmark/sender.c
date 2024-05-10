#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "hiredis/hiredis.h"

struct timeval tv;
unsigned long long now;

redisContext *c;

unsigned long long epoch_usec()
{
   gettimeofday(&tv, NULL);
   return (unsigned long long)(tv.tv_sec) * 1000000 + (unsigned long long)(tv.tv_usec) ;
}

int main(int argc, char *argv[])
{
  c = redisConnect("127.0.0.1", 6379);
  if (c->err) {
      printf("error: %s\n", c->errstr);
      return 1;
  }

  now = epoch_usec();
  char *p;

  for(long long i=0; i<strtol(argv[1], &p, 10); i++)
  {
    redisCommand(c, "PUBLISH speedtest %llu", epoch_usec());
  }

  redisCommand(c, "PUBLISH speedtest end");

  return 0;
}