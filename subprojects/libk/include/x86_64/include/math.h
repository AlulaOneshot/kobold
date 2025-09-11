#ifndef  __MATH_H
#define  __MATH_H

#define DIV_ROUNDUP(A, B)    \
({                           \
    __typeof__(A) _a_ = A;       \
    __typeof__(B) _b_ = B;       \
    (_a_ + (_b_ - 1)) / _b_; \
})

#define ALIGN_UP(A, B)              \
({                                  \
    __typeof__(A) _a__ = A;             \
    __typeof__(B) _b__ = B;             \
    DIV_ROUNDUP(_a__, _b__) * _b__; \
})

#define ALIGN_DOWN(A, B)    \
({                          \
    __typeof__(A) _a_ = A;      \
    __typeof__(B) _b_ = B;      \
    (_a_ / _b_) * _b_;      \
})

#define MAX(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define MIN(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

#endif  /* __MATH_H */