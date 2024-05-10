#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include "hiredis/adapters/libevent.h"

struct timeval tv;

unsigned long long min, max, count, sum;

unsigned long long epoch_usec()
{
   gettimeofday(&tv, NULL);
   return (unsigned long long)(tv.tv_sec) * 1000000 + (unsigned long long)(tv.tv_usec) ;
}

// called by hiredis when there's a new message posted on the SUBSCRIBEd channel
void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
    redisReply *r = reply;
    if (reply == NULL) return;

    if (r->type == REDIS_REPLY_ARRAY) {
        if (r->element[2]->str != NULL)
        {
          if (strcmp(r->element[2]->str, "end") == 0)
          {
              printf("Min: %llu\tMax: %llu\tCount: %llu\tAvg: %.2f microseconds\n", min, max, count, (double)(sum/count));
              redisAsyncCommand(c, onMessage, NULL, "UNSUBSCRIBE speedtest");
              exit(0);
          } else {
              char *pEnd = NULL;
              unsigned long long int received;
              received = strtoull(r->element[2]->str, &pEnd, 10);

              long long diff = epoch_usec() - received;

              if (diff > max)
                max = diff;

              if (diff < min)
                min = diff;

              sum += diff;
              count++;
            }
        }
    }
}


int main()
{
  struct event_base *base = event_base_new();

  redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
  if (c->err) {
      printf("error: %s\n", c->errstr);
      return 1;
  }

  count = 0;
  sum = 0;
  max = 0;
  min = 0;

  redisLibeventAttach(c, base);
  redisAsyncCommand(c, onMessage, NULL, "SUBSCRIBE speedtest");
  event_base_dispatch(base);

  return 0;
}