#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include"guard/RWLock_Guard.h"
#include"queue/queue.h"
#include<random>
using namespace std;

int a = 0;
pthread_rwlock_t rw_lock;

void * worker(void *arg){
    while (true)
    {
        RWLockGuard write_lock = RWLockGuard(&rw_lock, WRLOCK);
        a++;
        printf("+1 %d\n", a);
        // sleep(1);
    }
}

void * reader(void *arg){
    while (true)
    {
        RWLockGuard read_lock = RWLockGuard(&rw_lock, RDLOCK);
        printf("a: %d\n", a);
        sleep(5);
    }
}

ThreadQueue q = ThreadQueue(2);
void * queue_worker(void *arg){
    while (true)
    {
        int test = random();
        q.push(test);
        printf("push %d \n", test);
        sleep(2);
    }
}

void * queue_reader(void *arg){
    while (true)
    {
        const int c = q.pop();
        printf("c: %d\n", c);
        sleep(5);
    }
}

int guard_test(){
        /* code */
    cout<<"test"<<endl;
    int err = pthread_rwlock_init(&rw_lock, NULL);
    if (err != 0)
    {
        cout<<"rw_lock 初始化失败"<<endl;
    }
    cout<<"rw_lock 初始化"<<endl;

    pthread_t t1,t2;
    if (pthread_create(&t1, nullptr, worker, nullptr) == -1){
        printf("create error!\n");
        return 1;
    }
    if (pthread_create(&t2, nullptr, reader, nullptr) == -1){
        printf("create error!\n");
        return 1;
    }

    sleep(10);

    if(pthread_join(t1, nullptr)){
        printf("thread is not  exit!\n");
        return -2;
    }

    if(pthread_join(t2, nullptr)){
        printf("thread is not  exit!\n");
        return -2;
    }

    err = pthread_rwlock_destroy(&rw_lock);
    if (err != 0)
    {
        cout<<"rw_lock 摧毁失败"<<endl;
    }
    cout<<"rw_lock 锁释放"<<endl;
    return 0;
}


int queue(){
    cout<<"test2"<<endl;

    pthread_t t1,t2;
    if (pthread_create(&t1, nullptr, queue_worker, nullptr) == -1){
        printf("create error!\n");
        return 1;
    }
    if (pthread_create(&t2, nullptr, queue_reader, nullptr) == -1){
        printf("create error!\n");
        return 1;
    }

    sleep(10);

    if(pthread_join(t1, nullptr)){
        printf("thread is not exit!\n");
        return -2;
    }

    if(pthread_join(t2, nullptr)){
        printf("thread is not exit!\n");
        return -2;
    }

    return 0;

}


int main(int argc, char const *argv[])
{
    /* code */
    // return guard_test();
    return queue();
    // return 0;
}
