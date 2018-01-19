/*=====================================================
# Author:        Zeegine Team
# Copyright:     www.thu3d.com        
# Last modified: 2015-01-26 12:31
# Filename:      strbuf.h
# Description:   
=====================================================*/

#ifndef __libconfig_strbuf_h
#define __libconfig_strbuf_h

#include <string.h>
#include <sys/types.h>

typedef struct
{
  char *string;
  size_t length;
  size_t capacity;
} strbuf_t;

char *strbuf_release(strbuf_t *buf);

void strbuf_append(strbuf_t *buf, const char *text);

#endif /* __libconfig_strbuf_h */
