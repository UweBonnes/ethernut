#ifndef _IO_H_
#define	_IO_H_

#include <compiler.h>

extern int _close(int fd);
extern int _open(CONST char *name, int mode);
extern int _read(int fd, void *buffer, unsigned int count);
extern int _write(int fd, CONST void *buffer, unsigned int count);
extern int _write_P(int fd, PGM_P buffer, unsigned int count);
extern int _ioctl(int fd, int cmd, void *buffer);
extern long _filelength(int fd);

#endif
