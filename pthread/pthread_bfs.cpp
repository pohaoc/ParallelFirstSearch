#include "pthread_bfs.h"

static std::queue<int> q;
static std::vector<bool> visited;
pthread_mutex_t q_lock;
void (*action)(int);
adjacencyList G;
int threads_working;
int N;
static void* bfs_pt_while(void* arg){
    bool working = true;
    while(1){
        //wait if queue is empty and threads wroking< total
        pthread_mutex_lock(&q_lock);
        if(q.empty()){
            if(working){
                working=false;
                threads_working--;
            }
            if(threads_working>0){
                //wait
                // std::cout<<*threads_working<<std::endl;
                pthread_mutex_unlock(&q_lock);
                while(q.empty()&&threads_working>0);
            }
            else{
                //exit
                pthread_mutex_unlock(&q_lock);
                return 0;
            }
        }
        else{
            //continue
            if(!working){
                working=true;
                threads_working++;
            }
            //atomic
            int node = q.front();
            q.pop();
            pthread_mutex_unlock(&q_lock);
            ///endatomic
            if(action!=NULL)
                action(node);
            
            for(auto i = G[node].begin();i!=G[node].end();i++){
                int neighbor = *i;
                //atomic
                pthread_mutex_lock(&q_lock);
                if(visited[neighbor]==false){
                    q.push(neighbor);
                    visited[neighbor]=true;
                    
                }
                pthread_mutex_unlock(&q_lock);
                //endatomic
            }
        }
    }
}
void bfs_pt(const adjacencyList &g, int origin,void (*act)(int),int NUM_THREADS){
    G=g;
    N=G.size();
    q=std::queue<int>();
    visited=std::vector<bool>(N,false);
    threads_working=NUM_THREADS;
    pthread_mutex_init(&q_lock,NULL);
    //atomic
    pthread_mutex_lock(&q_lock);
    q.push(origin);
    visited[origin]=true;
    pthread_mutex_unlock(&q_lock);
    //endatomic
    action=act;
    G=g;

    pthread_t threads[NUM_THREADS];
    for(int i=0;i<NUM_THREADS;i++){
        pthread_create(&threads[i], NULL, bfs_pt_while,NULL);
    }
    for(int i=0;i<NUM_THREADS;i++){
        pthread_join(threads[i], NULL);
    }
}

void bfs(const adjacencyList &G, int origin,void (*action)(int)){
    std::queue<int> q;
    std::vector<bool>visited(G.size(),false);
    //atomic
    q.push(origin);
    visited[origin]=true;
    //endatomic

    while(!q.empty()){
        //atomic
        int node = q.front();
        q.pop();
        ///endatomic
        if(action!=NULL)
            action(node);
        
        for(auto i = G[node].begin();i!=G[node].end();i++){
            int neighbor = *i;
            //atomic
            if(visited[neighbor]==false){
                q.push(neighbor);
                visited[neighbor]=true;
            }
            //endatomic
        }
    }
}