#ifndef __MINI_LIB_H__
#define __MINI_LIB_H__

// XXX : why ?
// Guess : the frequency of the mcycle is
// 2GHz ( the frequency of CPU )
#ifndef HZ
#define HZ 2000000000UL
#endif

int strcmp(const char *str1, const char *str2);
char *strcpy(char *dst, const char *src);
int times(struct tms *buf);

#endif
