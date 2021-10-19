#include <pthread.h>
#include <semaphore.h>
#include "../guard/Mutex_Guard.h"
#include <condition_variable>
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
	void print();
	pthread_cond_t push_cond = PTHREAD_COND_INITIALIZER;
	pthread_cond_t pop_cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	size_t count;
    size_t mSize;
};

ThreadQueue::ThreadQueue(size_t size)
	: mSize(size),
	  count(0),
	  mBuf(size),
	  mReadIndex(0),
	  mWriteIndex(0)
{}


void ThreadQueue::push(const int &value)
{
	MutexGuard mutex = MutexGuard(&this->mutex);
	while (count >= this->mSize){
		pthread_cond_wait(&this->push_cond, &this->mutex);
	}
	mBuf[mWriteIndex] = value;
	mWriteIndex = (mWriteIndex + 1) % mSize;
	++count;
	// this->print();
	pthread_cond_signal(&pop_cond);
}

const int ThreadQueue::pop()
{
	MutexGuard mutex = MutexGuard(&this->mutex);
	while (count <= 0)
	{
		pthread_cond_wait(&this->pop_cond, &this->mutex);
	}
	int value = mBuf[mReadIndex];
	mReadIndex = (mReadIndex + 1) % mSize;
	--count;
	// this->print();
	pthread_cond_signal(&push_cond);
	return value;
}

void ThreadQueue::print()
{
	std::cout<<"queue"<<':';
	for (auto i = this->mBuf.begin(); i != this->mBuf.end(); i++) {
    	std::cout << *i << ' ';
	}
	std::cout<<std::endl;
}

ThreadQueue::~ThreadQueue()
{
	pthread_mutex_destroy(&this->mutex);
	pthread_cond_destroy(&this->push_cond);
	pthread_cond_destroy(&this->pop_cond);
}



