#ifndef SPINLOCK
#define SPINLOCK

#include <atomic>

class Spinlock {
public:
    Spinlock():
        lock_(ATOMIC_FLAG_INIT)
    {
    }

    inline void lock()
    {
        while(!atomic_flag_test_and_set(&lock_)) {}
    }

    inline bool trylock()
    {
        return !atomic_flag_test_and_set(&lock_);
    }

    inline void unlock()
    {
        atomic_flag_clear(&lock_);
    }

private:
    std::atomic_flag lock_;
};



#endif
