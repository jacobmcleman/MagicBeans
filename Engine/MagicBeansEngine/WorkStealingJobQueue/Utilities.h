#pragma once

#define COMPILER_BARRIER _ReadWriteBarrier()

#define MEMORY_BARRIER _ReadWriteBarrier()

struct ScopedLockStruct
{
    ScopedLockStruct(std::mutex& mutex) : m(mutex)
    {
        m.lock();
    }

    ~ScopedLockStruct()
    {
        m.unlock();
    }

private:
    std::mutex& m;
};

#define SCOPED_LOCK(mutex) ScopedLockStruct MACRODEFINED_scopelock = ScopedLockStruct( mutex )