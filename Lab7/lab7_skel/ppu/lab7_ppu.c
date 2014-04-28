/* alexandru.olteanu@cs.pub.ro, 31/03/2014 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>

extern spe_program_handle_t lab7_spu;

#define SPU_THREADS 16
#define CHUNKS 80

struct arg_t {
   spe_context_ptr_t ctx;
   int* count;
   int* sum;
   pthread_mutex_t* count_lock;
   pthread_mutex_t* sum_lock;
};


/*
 * thread used to start the programs on the SPE's
 */
void *ppu_pthread_function(void* ctx) {
   unsigned int entry = SPE_DEFAULT_ENTRY;

   if (spe_context_run(*(spe_context_ptr_t*)ctx, &entry, 0, NULL, NULL, NULL) < 0) {
      perror ("Failed running context");
      exit (1);
   }

   pthread_exit(NULL);
}


/*
 * threads used to keep communication with the SPE's
 */
void *comm_pthread_function(void* argument) {
   struct arg_t arg = *(struct arg_t*)argument;
   int val = 0;
   unsigned int sum = 0;
    unsigned int mbox_data = 42;

   //TODO: Task3 - inconjoara Task1, Task2 si calculul sumei cu o bucla ca procesarea sa se faca
   // pana la primirea de SPU a unei valori > CHUNKS

      //TODO: Task1 - trimite un intreg de la PPU la SPU
    while(1){
      // scriem o intrare in mailbox; in mod sigur trimitem un singur mesaj pentru fiecare SPU asa ca nu e nevoie sa fie blocant
      spe_in_mbox_write(arg.ctx, arg.count, 1, SPE_MBOX_ANY_NONBLOCKING);

      // //TODO: Task2 - trimite un intreg de la SPU la PPU
      while (spe_out_mbox_status(arg.ctx) == 0) { ; }
      spe_out_mbox_read (arg.ctx, &mbox_data, 1);
      printf("[PPU] Recieved response=%d\n",mbox_data);
      if(arg.count > 80) break;
   }
      pthread_mutex_lock(arg.sum_lock);
      (*(arg.sum)) += (int)sum;
      pthread_mutex_unlock(arg.sum_lock);

   pthread_exit(NULL);
}



int main(void) {
   spe_context_ptr_t ctxs[SPU_THREADS];
   pthread_t threads[SPU_THREADS];
   pthread_t comm_threads[SPU_THREADS];
   struct arg_t args[SPU_THREADS];
   int i;

   int count=0;		       // this will count the chunks (from 1 to 80)
   int sum=0;                  // this will store the sum

   pthread_mutex_t count_lock; // the above two variable are shared by all threads
   pthread_mutex_t sum_lock;   // therefore they need to be protected by locks

   /* Create several SPE-threads to execute 'SPU'. */
   for(i=0; i<SPU_THREADS; i++) {
      /* Create context */
      if ((ctxs[i] = spe_context_create (0, NULL)) == NULL) {
         perror ("Failed creating context");
         exit (1);
      }

      /* Load program into context */
      if (spe_program_load (ctxs[i], &lab7_spu)) {
         perror ("Failed loading program");
         exit (1);
      }

      /* Create thread for each SPE context */
      if (pthread_create (&threads[i], NULL, &ppu_pthread_function, ctxs+i)) {
         perror ("Failed creating thread");
         exit (1);
      }
   }

   for (i=0; i<SPU_THREADS; i++) {
      args[i].ctx = ctxs[i];
      args[i].count = &count;
      args[i].count_lock = &count_lock;
      args[i].sum = &sum;
      args[i].sum_lock = &sum_lock;

      if (pthread_create (&comm_threads[i], NULL, &comm_pthread_function, args+i)) {
         perror ("Failed creating thread");
         exit (1);
      }
   }

   /* Wait for comm-thread to complete execution. */
   for (i=0; i<SPU_THREADS; i++) {
      if (pthread_join (comm_threads[i], NULL)) {
         perror("Failed pthread_join");
         exit (1);
      }
   }

   /* Wait for SPU-thread to complete execution. */
   for (i=0; i<SPU_THREADS; i++) {
      if (pthread_join (threads[i], NULL)) {
         perror("Failed pthread_join");
         exit (1);
      }

      /* Destroy context */
      if (spe_context_destroy (ctxs[i]) != 0) {
         perror("Failed destroying context");
         exit (1);
      }
   }

   printf("\nThe program has successfully executed. The sum is %d.\n",sum);
   return (0);
}
