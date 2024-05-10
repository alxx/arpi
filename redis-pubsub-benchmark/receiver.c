#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include "hiredis/adapters/libevent.h"

// called by hiredis when there's a new message posted on the SUBSCRIBEd channel
void onMessage(redisAsyncContext *c, void *reply, void *privdata) {
    redisReply *r = reply;
    if (reply == NULL) return;

    if (r->type == REDIS_REPLY_ARRAY) {
        printf("On channel %s: %s\n", r->element[1]->str, r->element[2]->str);

        char *pEnd;
        unsigned long long int received;
        received = strtoull(r->element[2]->str, &pEnd, 16);
        printf("\t%llu", received);
//        if(strcmp(r->element[1]->str, "speedtest") == 0 && r->element[2]->str != NULL)
//        {
//            char *cmd = strtok(r->element[2]->str, " ");
//            char *param = strtok(NULL, " ");
//
//            printf("Command: %s, param: %s\n", cmd, param);
//        }
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

  redisLibeventAttach(c, base);
  redisAsyncCommand(c, onMessage, NULL, "SUBSCRIBE speedtest");
  event_base_dispatch(base);

  return 0;
}