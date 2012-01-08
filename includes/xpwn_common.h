#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#if _MSC_VER	//////////////////////////// MSVS //////////////////////////////
#include <direct.h>

#define __attribute__(x) 

#define inline __inline

#define getcwd _getcwd

#define PRId64 "I64d"
#define PRIx64 "I64x"

#define fseeko _fseeki64
#define ftello _ftelli64

#define snprintf _snprintf

#define exit(x) throw

#pragma pack (push)
#pragma pack(1)


#endif// ----------------------------------END MSVS
#if ! _MSC_VER	//////////////////////////// GCC //////////////////////////////

#include <unistd.h>
#include <inttypes.h>
//#define fseeko fseeko64
//#define ftello ftello64
//#define off_t off64_t
typedef unsigned char BOOL;


#endif //---------------------------------- END GCC -------------------------

#ifdef WIN32

#define mkdir(x, y) mkdir(x)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#define TRUE 1
#define FALSE 0

#define FLIPENDIAN(x) flipEndian((unsigned char *)(&(x)), sizeof(x))
#define FLIPENDIANLE(x) flipEndianLE((unsigned char *)(&(x)), sizeof(x))

#define IS_BIG_ENDIAN      0
#define IS_LITTLE_ENDIAN   1

#define TIME_OFFSET_FROM_UNIX 2082844800L
#define APPLE_TO_UNIX_TIME(x) ((x) - TIME_OFFSET_FROM_UNIX)
#define UNIX_TO_APPLE_TIME(x) ((x) + TIME_OFFSET_FROM_UNIX)

#define ASSERT(x, m) if(!(x)) { fflush(stdout); fprintf(stderr, "error: %s\n", m); perror("error"); fflush(stderr); exit(1); }

extern char xpwn_endianness;

static inline void flipEndian(unsigned char* x, int length) {
  int i;
  unsigned char tmp;

  if(xpwn_endianness == IS_BIG_ENDIAN) {
    return;
  } else {
    for(i = 0; i < (length / 2); i++) {
      tmp = x[i];
      x[i] = x[length - i - 1];
      x[length - i - 1] = tmp;
    }
  }
}

static inline void flipEndianLE(unsigned char* x, int length) {
  int i;
  unsigned char tmp;

  if(xpwn_endianness == IS_LITTLE_ENDIAN) {
    return;
  } else {
    for(i = 0; i < (length / 2); i++) {
      tmp = x[i];
      x[i] = x[length - i - 1];
      x[length - i - 1] = tmp;
    }
  }
}

static inline void hexToBytes(const char* hex, uint8_t** buffer, size_t* bytes) {
	size_t i;
	*bytes = strlen(hex) / 2;
	*buffer = (uint8_t*) malloc(*bytes);
	for(i = 0; i < *bytes; i++) {
		uint32_t byte;
		sscanf(hex, "%2x", &byte);
		(*buffer)[i] = byte;
		hex += 2;
	}
}

static inline void hexToInts(const char* hex, unsigned int** buffer, size_t* bytes) {
	size_t i;
	*bytes = strlen(hex) / 2;
	*buffer = (unsigned int*) malloc((*bytes) * sizeof(int));
	for(i = 0; i < *bytes; i++) {
		sscanf(hex, "%2x", &((*buffer)[i]));
		hex += 2;
	}
}

struct io_func_struct;

typedef int (*readFunc)(struct io_func_struct* io, off_t location, size_t size, void *buffer);
typedef int (*writeFunc)(struct io_func_struct* io, off_t location, size_t size, void *buffer);
typedef void (*closeFunc)(struct io_func_struct* io);

typedef struct io_func_struct {
  void* data;
  readFunc read;
  writeFunc write;
  closeFunc close;
} io_func;

#endif
