#include <thread>
#include <iostream>

#define Memory_Barrier __asm__ __volatile__("" : : : "memory")
#define ACESS_ONCE(x) (* (volatile __typeof__(x) *) &(x))

using namespace std;

struct HazardPointer
{
    HazardPointer() :p_(nullptr) {}
    int *p_; // Hazard Pointer 封装了原始指针
};

int *p = new int(2); // 原始指针
HazardPointer *hp = new HazardPointer(); // 只有一个线程可以写

class Writer
{
public:
    void retire (int *tmp)
    {
        retire_list = tmp;
    }

    void gc()
    {
        if (retire_list == hp->p_)
        {
            // 说明有读者正在使用，不能释放
        } 
        else
        {
            // 可以安全释放
            delete retire_list;
        }
    }

    void write() 
    {
        int *tmp = ACESS_ONCE(p);
        p = nullptr;
        retire(tmp);
        gc();
    }

private:
    int *retire_list;
};

class Reader
{
public:
    void acquire(int *tmp)
    {
        hp->p_ = tmp;
    }

    void release() 
    {
        hp->p_ = nullptr;
    }

    void read()
    {
        int *tmp = nullptr;
        do
        {
            tmp = ACESS_ONCE(p);
            Memory_Barrier;
            acquire(tmp); // 封装，这是告诉writer：我要读，别释放
        } while(tmp != ACESS_ONCE(p)); // 仔细想想，为什么这里还要判断？

        if (nullptr !=  tmp) // 这里为什么也要判断
        {
            // it is safe to access *tmp from now on
            cout << *tmp << endl; // 
        }

        // when you finish reading it, just release it
        release(); // 这是告诉writer，用完了，可以释放了
    }
};


int main()
{
    thread t1(&Reader::read, Reader());
    thread t2(&Writer::write, Writer());
    t1.join();
    t2.join();
    return 0;
}

