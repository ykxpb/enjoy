#ifndef SPINLOCK
#define SPINLOCK

class Spinlock {
public:
    Spinlock():
        lock_(0)
    {
    }

    inline void lock()
    {
        while(__sync_lock_test_and_set(&lock_, 1)) {}
    }

    inline bool trylock()
    {
        return __sync_lock_test_and_set(&lock_, 1) == 0;
    }

    inline void unlock()
    {
        __sync_lock_release(&lock_);
    }

private:
    int lock_;
};



#endif