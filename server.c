#include "cs537.h"
#include "request.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)> <threads> <buffers> 
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

pthread_mutex_t m;
pthread_cond_t empty;
pthread_cond_t fill;

int stored_count, buffer_size, j = 0, k = 0;
int *buffer;

// CS537: Parse the new arguments too
void getargs(int *port, int *thread_count, int argc, char *argv[])
{
    if (argc != 4) {
     fprintf(stderr, "Usage: %s <port> <threads> <buffer>\n", argv[0]);
     exit(1);
 }
 *port = atoi(argv[1]);
 *thread_count = atoi(argv[2]);
 *buffer_size = atoi(argv[3]);
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, thread_count, clientlen;
    struct sockaddr_in clientaddr;
    stored_count = 0;

    getargs(&port, &thread_count, argc, argv);

    buffer = malloc(sizeof(int)*buffer_size);
    // 
    // CS537: Create some threads...
    //
    pthread_t threads[thread_count];

    if (pthread_mutex_init(&m, NULL) < 0) {
        perror("pthread_mutex_init failed");
        return -1;
    }
    if (pthread_cond_init(&empty, NULL) < 0) {
        perror("pthread_cond_init failed");
        return -1;
    }
    if(pthread_cond_init(&fill, NULL)<0) {
        perror("pthread_cond_init failed");
        return -1;
    }
    int i;
    for(i = 0; i < thread_count; i++) {
      pthread_create(&threads[i], NULL, consumer, 0) ;
  } 

  
  listenfd = Open_listenfd(port);
  while (1) {
     clientlen = sizeof(clientaddr);
     connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
     // Producer
     // producer();
     pthread_mutex_lock(&m);
     while (stored_count == buffer_size)
       pthread_cond_wait(&empty, &m); 
     do_fill(connfd);
     pthread_cond_signal(&fill); 
     pthread_mutex_unlock(&m);
    
 }

}

// unable to call as a function

/*
// From Slides
void *producer(void *arg){
    pthread_mutex_lock(&m);
     while (stored_count == buffer_size)
       pthread_cond_wait(&empty, &m); 
     do_fill(connfd);
     pthread_cond_signal(&fill); 
     pthread_mutex_unlock(&m); 
}
*/

void *consumer(void *arg) {
  while (1) {
    pthread_mutex_lock(&m); 
    while (numfall == 0) {
      Cond_wait(&fill, &m);
  }
  int connfd = do_get(); 
  Cond_signal(&empty); 
  pthread_mutex_unlock(&m); 
  requestHandle(connfd);
  Close(connfd);
}
}

void do_fill(int connfd)
{
    buffer[j++] = connfd;
    j = j % buffer_size;
    stored_count++;
}

int do_get()
{
    int connfd = buffer[k++];
    k = k % buffer_size;
    stored_count--; 
    return connfd;
}






