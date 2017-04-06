#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>

extern uint32_t end;

caddr_t _sbrk ( int incr )
{
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = (unsigned char *)&end;
  }
  prev_heap = heap;

  heap += incr;

  return (caddr_t) prev_heap;
}

int link(char *old, char *new)
{
	return -1;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

void abort(void)
{
	/* Abort called */
	while(1);
}

int _write (int fd, char *ptr, int len)
{
/*	for(int i = 0; i < len; i++)
	{
		ITM_SendChar(*ptr++);
	}
*/
	return len;
}
