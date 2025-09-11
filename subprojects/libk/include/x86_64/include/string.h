#ifndef _STRING_H
#define _STRING_H

#define NULL ((void*)0)

#include <stddef.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, int c, size_t n);
int memcmp (const void *, const void *, size_t);
void *memchr (const void *, int, size_t);

char *strcpy(char *restrict dest, const char *restrict src);
char *strncpy(char *restrict d, const char *restrict s, size_t n);

char *strcat(char *restrict dest, const char *restrict src);
char *strncat(char *restrict d, const char *restrict s, size_t n);

int strcmp(const char *l, const char *r);
int strncmp(const char *_l, const char *_r, size_t n);

int strcoll(const char *l, const char *r);
size_t strxfrm (char *restrict dest, const char *restrict src, size_t n);

char *strchr (const char *, int);
char *strrchr (const char *, int);

size_t strcspn (const char *, const char *);
size_t strspn (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok(char *restrict s, const char *restrict sep);

size_t strlen (const char *);

char *stpcpy(char *restrict d, const char *restrict s);
char *stpncpy(char *restrict d, const char *restrict s, size_t n);
char *strdup (const char *);

#endif