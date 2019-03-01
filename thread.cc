#include <ucontext.h>
#include <queue>
#include <vector>
#include "thread.h"
#include "interrupt.h"
#include <tuple>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

typedef vector< tuple<ucontext_t*, int> > threadQ;

static threadQ readyQueue;
static threadQ wait;
static threadQ lock;
tuple<ucontext_t*, int> running;

void ending_output(){
  cout << "Thread library exiting." << endl;
}

static void start(thread_startfunc_t func, void *arg){
  func(arg);
}

int thread_libinit(thread_startfunc_t func, void *arg)
//We create a new thread and give access from default thread to this new thread
// Whenever we thread init with a function(usually a service thread), and a number,
// We create this service thread and set it as running and we put it in the readyQueue
// At the end of this function, we loop infinitely
{
  
  ucontext_t * ucontext_ptr;
  getcontext(ucontext_ptr);
  char *stack = new char [STACK_SIZE];
  ucontext_ptr->uc_stack.ss_sp = stack;
  ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
  ucontext_ptr->uc_stack.ss_flags =0;
  ucontext_ptr->uc_link = NULL;
  makecontext(ucontext_ptr, (void (*)()) start, 2, func, arg);
  swapcontext(NULL, ucontext_ptr);
  tuple<ucontext_t *, int> thread = make_tuple (ucontext_ptr, arg);
  readyQueue.push_back(thread);
  //running = ucontext_prt;

  ending_output();
  exit(1);
  
}

int thread_create(thread_startfunc_t func, void*arg)
/*Whenever a thread is create, we create the context, and push it onto the running queue*/
{
  ucontext_t *ucontext_ptr;
  getcontext(ucontext_ptr);
  char *stack = new char [STACK_SIZE];
  ucontext_ptr->uc_stack.ss_sp = stack;
  ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
  ucontext_ptr->uc_stack.ss_flags = 0;
  ucontext_ptr->uc_link = NULL;
  makecontext(ucontext_ptr, (void (*)()) start, 2, func, arg);
  tuple<ucontext_t *, int> thread = make_tuple (ucontext_ptr, arg);
  //TODO: making the thread wait for calls, and only when we decide to signal
  readyQueue.push_back(thread);
}

int thread_yield(void){
  /*Sets the running as the next item of the queue,as running, and then pushes the current running back into the queue*/
}

int thread_lock(unsigned int lock){}
int thread_unlock(unsigned int lock){}
int thread_wait(unsigned int lock, unsigned int cond){}
int thread_signal(unsigned int lock, unsigned int cond){}
int thread_broadcast(unsigned int lock, unsigned int cond){}

