#ifndef SPINLOCK_H
#define SPINLOCK_H

typedef struct {
    volatile int lock;
} spinlock_t;

#define SPINLOCK_INIT { 0 }

static inline void spinlockAcquire(spinlock_t *lock) {
    while (__atomic_test_and_set(&lock->lock, __ATOMIC_ACQUIRE)) {
        // Busy-wait (spin) until the lock is acquired
    }
}

static inline void spinlockRelease(spinlock_t *lock) {
    __atomic_clear(&lock->lock, __ATOMIC_RELEASE);
}

#endif