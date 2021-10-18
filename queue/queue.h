#include <pthread.h>
#include <semaphore.h>
#include<iostream>
#include<vector>
using namespace std;

#define RDLOCK 0
#define WRLOCK 1

// template<typename T>
class ThreadQueue
{
public:
    explicit ThreadQueue(size_t size);
    ~ThreadQueue();

    void push(const int& a);
    const int pop();
private:
    vector<int> mBuf;
    int mReadIndex, mWriteIndex;
    sem_t mReadSem, mWriteSem;
    size_t mSize;
    int flag;
    int mode;
};

ThreadQueue::ThreadQueue(size_t size)
	: mSize(size),
	  mBuf(size),
	  mReadIndex(0),
	  mWriteIndex(0)
{
    int err = sem_init(&mReadSem, 0, 0);
	if (err != 0 ){
        printf("sem_init error %d\n", err);
    }

    err = sem_init(&mWriteSem, 0, this->mSize);
	if (err != 0){
        printf("sem_init error %d\n", err);
    }
}


void ThreadQueue::push(const int &value)
{
	sem_wait(&mWriteSem);
	mBuf[mWriteIndex] = value;
	mWriteIndex = (mWriteIndex + 1) % mSize;
	sem_post(&mReadSem);
}

const int ThreadQueue::pop()
{
	sem_wait(&mReadSem);
	int value = mBuf[mReadIndex];
	mReadIndex = (mReadIndex + 1) % mSize;
	sem_post(&mWriteSem);
	return value;
}


ThreadQueue::~ThreadQueue()
{
	sem_destroy(&mReadSem);
	sem_destroy(&mWriteSem);
}



