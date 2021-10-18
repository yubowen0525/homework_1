#include <pthread.h>
#include<iostream>
using namespace std;

#define RDLOCK 0
#define WRLOCK 1

class RWLockGuard
{
public:
    RWLockGuard(pthread_rwlock_t *rw_lock, int mdoe);
    ~RWLockGuard();

private:
    void RWLockRDlock();
    void WLockWRlock();
    pthread_rwlock_t *rw_lock;
    int flag;
    int mode;
};


RWLockGuard::RWLockGuard(pthread_rwlock_t *rw_lock, int mode)
	: rw_lock(rw_lock), mode(mode)
{
    switch (mode)
    {
    case RDLOCK:
        RWLockRDlock();
        break;
    case WRLOCK:
        WLockWRlock();
        break;
    default:
        cout<<"不支持"<<endl;
        break;
    }
}

void RWLockGuard::RWLockRDlock()
{
    int err = pthread_rwlock_rdlock(rw_lock);
    if (err != 0)
    {
        printf("上读锁失败\n");
        this->flag = 0;
        return;
        /* code */
    }

    this->flag = 1;
    printf("上读锁成功\n");
}

void RWLockGuard::WLockWRlock()
{
    int err = pthread_rwlock_wrlock(rw_lock);
    if (err != 0)
    {
        printf("上写锁失败\n");
        this->flag = 0;
        return;
        /* code */
    }

    this->flag = 1;
    printf("上写锁成功\n");
}

RWLockGuard::~RWLockGuard(){
    if (flag == 1)
    {
        printf("释放锁 %d\n", this->mode);
        pthread_rwlock_unlock(rw_lock);
        return;
    }

    printf("无需释放锁\n");
}

