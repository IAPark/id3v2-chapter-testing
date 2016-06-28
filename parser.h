#ifndef parser_h
#define parser_h

#include <stdio.h>

typedef struct {
  unsigned char major;
  unsigned char minor;
} id3v2_version;

typedef struct {
  char unsynchronisation;
  char extended_header;
  char experimental;
} id3v2_flags;

typedef struct {
  id3v2_version version;
  id3v2_flags flags;
  int size;
  char valid;
} id3v2_header;

typedef struct {
  char id[5];
  int size;
} id3v2_frame_header;

id3v2_header parse_header(FILE* stream);
id3v2_frame_header parse_frame_header(FILE* stream);
void skip_frame(FILE* stream, id3v2_frame_header frame);

#endif