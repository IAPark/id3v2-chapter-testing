#include "parser.h"
#include <string.h>

id3v2_header parse_header(FILE *stream) {
  id3v2_header header;
  char identifier[4];
  identifier[3] = 0;
  fread(identifier, 3, 1, stream);
  printf("%s\n", identifier);

  if (strcmp(identifier,"ID3") != 0) {
    header.valid = 0; //false
    return header;
  }
  unsigned char version[2];
  fread(version, 2, 1, stream);
  header.version.major = version[0];
  header.version.minor = version[1];
  printf("%u.%u\n", (header.version.major),(header.version.minor));


  unsigned char flags;
  fread(&flags, 1, 1, stream);
  printf("%o\n", flags);


  unsigned char size[4];
  fread(size, 4, 1, stream);

  header.size = size[0]*128*128*128 + size[1]*128*128 + size[2]*128 + size[3];
  printf("%u\n", header.size);

}


id3v2_frame_header parse_frame_header(FILE* stream) {
  id3v2_frame_header header;
  char identifier[5];
  identifier[4] = 0;

  fread(identifier, 4, 1, stream);
  strcpy(header.id, identifier);
  printf("%s\n", header.id);

  char size[4];
  fread(size, 4, 1, stream);
  header.size = size[3] + size[2]*128 + size[1]*128*128 + size[0]*128*128;

  unsigned char flags[2];
  fread(flags, 2, 1, stream);
  printf("%u %u\n", flags[0], flags[1]);

  return header;
}

void skip_frame(FILE* stream, id3v2_frame_header frame) {
  fseek(stream, frame.size, SEEK_CUR);
}

void swap(char *a, char *b) {
  char temp = *a;
  *a = *b;
  *b = temp;
}

chapter_frame parse_chapter(FILE* stream) {
  chapter_frame frame;
  for(int i=0; i<20; ++i) {
    fread((frame.id + i), 1, 1, stream);
    if (frame.id[i] == 0) {
      printf("broke at %i\n", i);
      break;
    }
  }
  unsigned char time[4];
  fread(time, 4, 1, stream);
  frame.start_time = time[3] + time[2]*128 + time[1]*128*128 + time[0]*128*128;

  fread(time, 4, 1, stream);
  frame.end_time = time[3] + time[2]*128 + time[1]*128*128 + time[0]*128*128;


  fread(time, 4, 1, stream);
  frame.start_offset = time[3] + time[2]*128 + time[1]*128*128 + time[0]*128*128;

  fread(time, 4, 1, stream);
  frame.end_offset = time[3] + time[2]*128 + time[1]*128*128 + time[0]*128*128;

  printf("%li-%li\n", frame.start_time, frame.end_time);
  return frame;
}
