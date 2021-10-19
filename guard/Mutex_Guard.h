#include <pthread.h>

class MutexGuard
{
public:
	MutexGuard(pthread_mutex_t *mutex);
	~MutexGuard();
private:
	pthread_mutex_t *m_mutex;
};

MutexGuard::MutexGuard(pthread_mutex_t *mutex)
	:m_mutex(mutex)
{
	pthread_mutex_lock(m_mutex);
}

MutexGuard::~MutexGuard()
{
	pthread_mutex_unlock(m_mutex);
}
