#ifndef STDDEF_H_
#define STDDEF_H_

#define NULL ((void*)0)

typedef unsigned long size_t;

typedef long ssize_t;

#define	offsetof(type, member)	__builtin_offsetof(type, member)

#endif