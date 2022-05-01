#include "pthread_bfs.h"
#include <cassert>
#include <iostream>
#define PRINTOUT 0


typedef struct {
    const adjacencyList* G;//global graph, read only
    std::queue<bfs_node>* threadqueue;//local queue, managed by main thread
    std::vector<bool>* seen; /* whether each node has been added to queue*/
    std::vector<bool>* done; /* whether each node has been acted on*/
    std::vector<pthread_mutex_t>* node_locks; /*lock for each node before adding to
     queue, to redundant thread taking */
    
    pthread_mutex_t* thread_lock;/*lock for each thread, only taken
     by main thread if main thread is modifying thread's queue*/
    void (*action)(bfs_node);//action function

    bool active;//whether thread should keep running
    bool waiting;//whether the thread is waiting with an empty queue;
    int id;

} thread_block;

static void* bfs_pt_while(void* arg){
    thread_block* TB = (thread_block*)arg;
    void (*action)(bfs_node) = TB->action;
    #if PRINTOUT==1
                    std::cout<<TB->id<<" Entering subrouting\n";
    #endif
    
    while(TB->active){
        while(!TB->threadqueue->empty()){
            //take own thread's lock
            #if PRINTOUT==2
                    std::cout<<TB->id<<" Locking\n";
            #endif
            if (pthread_mutex_lock(TB->thread_lock))
                perror("failed to lock");
            else{
            #if PRINTOUT==2
                    std::cout<<TB->id<<" Locked\n";
            #endif
            }
            //double check that you have work to do
            if(!TB->threadqueue->empty()){
                //pop first node from queue
                bfs_node node = TB->threadqueue->front();
                // std::cout<<node<<std::endl;
                TB->threadqueue->pop();
                
                /*if(!pthread_mutex_trylock(&TB->node_locks->at(node)))*/{
                                
                    //check neighbors
                    for(int i = 0;i<TB->G->at(node).size();i++){
                        bfs_node neighbor = TB->G->at(node).at(i);
                        //take lock for this node, to make sure only added to queue once
                        if(0==pthread_mutex_trylock(&(TB->node_locks->at(neighbor))))
                        {
                            // assert(neighbor!=node);
                            
                            //add if not yet seen
                            #if PRINTOUT==1
                            std::cout<<"N"<<node<<" checking neighbor "<<neighbor<<std::endl;
                            #endif
                            if(TB->seen->at(neighbor) == false){
                                #if PRINTOUT==1
                                std::cout<<"N"<<node<<" enqueueing "<<neighbor;
                                #endif
                                TB->threadqueue->push(neighbor);
                                TB->seen->at(neighbor) = true;
                            }
                            pthread_mutex_unlock(&(TB->node_locks->at(neighbor)));                    
                        }
                        
                    }
                    //perform action on node
                    if(action!=NULL &&!TB->done->at(node)){
                        action(node);
                    }
                    TB->done->at(node)=true;
                }

            }
            pthread_mutex_unlock(TB->thread_lock);
            #if PRINTOUT==2
                    std::cout<<TB->id<<" Unlocking\n";
            #endif
        }
        //queue empty, so let main thread know that you want more work
        TB->waiting=true;
        #if PRINTOUT==1
        std::cout<< TB->id <<" now waiting\n";
        #endif
        while(TB->waiting);
        #if PRINTOUT==1
        std::cout<<TB->id<<" no longer waiting\n";
        #endif
        //to kill thread, first set active= false, then set waiting = false 

    }
    #if PRINTOUT==1
        std::cout<<TB->id<<" exiting\n";
    #endif
    pthread_exit(NULL);
}
void bfs_pt(const adjacencyList &g, bfs_node origin,void (*act)(bfs_node),int NUM_THREADS){
    assert(NUM_THREADS>=2);
    int N = g.size();
    std::queue<bfs_node> threadqueues[NUM_THREADS-1];
    std::vector<bool> seen(N,false);
    std::vector<bool> done(N,false);
    std::vector<pthread_mutex_t> node_locks(N);
    pthread_mutex_t thread_locks[NUM_THREADS-1];

    //init mutex locks
    for(int i = 0; i < N; i++)
        pthread_mutex_init(&(node_locks[i]),NULL);
    for(int i = 0; i < NUM_THREADS-1; i++)
        pthread_mutex_init(thread_locks+i,NULL);
    //Thread "Block"s
    thread_block TBS[NUM_THREADS-1];
    //threads
    pthread_t threads[NUM_THREADS-1];

    //put place items to all queues:

    seen[origin]=true;
    std::queue<bfs_node> tempq;
    tempq.push(origin);
    int ndone =0;
    while(tempq.size()<NUM_THREADS-1 && tempq.size()!=0){
        // std::cout<<ndone;
        bfs_node tn = tempq.front();
        tempq.pop();
        /*if(!pthread_mutex_trylock(&node_locks.at(tn)))*/{
            if (act!=NULL && !done.at(tn))
                act(tn);
            done.at(tn)=true;
            ++ndone;
            for(auto i = g.at(tn).begin();i!=g.at(tn).end();i++)
                if(false==seen[*i]){
                    tempq.push(*i);
                    seen[*i]=true;
                }
        }
        
    }
    #if PRINTOUT==4
                    std::cout<<"tempq size: "<<tempq.size()<<std::endl;
    #endif
    if (tempq.size()==0)
        return;
    
    int i = 0;
    while(tempq.size()>0){
        bfs_node tn = tempq.front();
        tempq.pop();
        threadqueues[i%(NUM_THREADS-1)].push(tn);
        ++i;
    }
    


    //init
    for(int i = 0; i < NUM_THREADS-1; i++){
        //init TB
        TBS[i].action=act;
        TBS[i].active=true;
        TBS[i].waiting=false;
        TBS[i].G=&g;
        TBS[i].node_locks=&node_locks;
        TBS[i].seen=&seen;
        TBS[i].done=&done;
        TBS[i].thread_lock=&thread_locks[i];
        TBS[i].threadqueue=&threadqueues[i];
        TBS[i].id=i;
        //make threads
        #if PRINTOUT==1
                    std::cout<<"making thread "<<i<<std::endl;
        #endif
        pthread_create(&threads[i],NULL, bfs_pt_while,(void*)&TBS[i]);
    }
    


    #if PRINTOUT==2
                    std::cout<<"Load balance time  \n";
    #endif
    //-----------------------//
     
    //now balance loads
    int nwaiting = 0;
    while(nwaiting<NUM_THREADS-1){
        nwaiting=0;
        int waiter=0;
        for(int i = 0;i < NUM_THREADS-1;i++){
            if(TBS[i].waiting){
                ++nwaiting;
                waiter=i;
            }   
        }
        //balance
        for(int i = 0;i < NUM_THREADS-1 && nwaiting>0;i++){
            if (TBS[i].threadqueue->size()>1){
                #if PRINTOUT==2
                    std::cout<<"Locking "<<i<<" \n";
                #endif
                pthread_mutex_lock(TBS[i].thread_lock);
                #if PRINTOUT==2
                    std::cout<<"Locked "<<i<<" \n";
                #endif
                #if PRINTOUT==4
                    std::cout<<"N"<<i<<" --> "<<waiter<<"\n";
                #endif
                
                if(TBS[i].threadqueue->size()>1)
                {
                   TBS[waiter].threadqueue->push(TBS[i].threadqueue->front());
                   TBS[i].threadqueue->pop();
                   --nwaiting;
                   TBS[waiter].waiting=false;
                }
                
                #if PRINTOUT==1
                    std::cout<<std::endl;
                #endif
                pthread_mutex_unlock(TBS[i].thread_lock);
                #if PRINTOUT==2
                    std::cout<<"Unlocking "<<i<<" \n";
                #endif
            }
        }
    }
    //now all threads are waiting/done so exit them
    #if PRINTOUT==1
        std::cout<<" all threads are done, exit\n";
    #endif
    for(int i = 0;i < NUM_THREADS-1 && nwaiting>0;i++){
        TBS[i].active=false;
        TBS[i].waiting=false;
        pthread_join(threads[i], NULL);
    }
    //done
    //destroy mutex locks
    for(int i = 0; i < N; i++)
        pthread_mutex_destroy(&(node_locks[i]));
    for(int i = 0; i < NUM_THREADS-1; i++)
        pthread_mutex_destroy(thread_locks+i);
    #if PRINTOUT==1
        std::cout<<" main thread returning\n";
    #endif
    return;
}