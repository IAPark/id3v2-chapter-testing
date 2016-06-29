#include "parser.h"
#include <string.h>
#include <stdlib.h>

chapters get_chapters(FILE* stream) {
  chapters chaps;
  chaps.size = 0;
  id3v2_header header = parse_header(stream);

  while (ftell(stream) < header.size) {
    id3v2_frame_header frame_header = parse_frame_header(stream);
    skip_frame(stream, frame_header);
    if (memcmp(frame_header.id, "CHAP", 4) == 0) {
      chaps.size += 1;
    }
  }
  fseek(stream, 0, SEEK_SET);
  header = parse_header(stream);
  chaps.chapters = malloc (sizeof(chapter)*chaps.size);

  int i = 0;
  while (ftell(stream) < header.size) {
    id3v2_frame_header frame_header = parse_frame_header(stream);
    if (memcmp(frame_header.id, "CHAP", 4) == 0) {
      chaps.chapters[i] = get_chapter(stream, frame_header);
      i++;
    }
    skip_frame(stream, frame_header);
  }
  return chaps;
}

id3v2_header parse_header(FILE *stream) {
  id3v2_header header;
  char identifier[4];
  identifier[3] = 0;
  fread(identifier, 3, 1, stream);

  if (strcmp(identifier,"ID3") != 0) {
    header.valid = 0; //false
    return header;
  }
  unsigned char version[2];
  fread(version, 2, 1, stream);
  header.version.major = version[0];
  header.version.minor = version[1];


  unsigned char flags;
  fread(&flags, 1, 1, stream);


  unsigned char size[4];
  fread(size, 4, 1, stream);

  header.size = size[0]*128*128*128 + size[1]*128*128 + size[2]*128 + size[3];
  return header;
}


id3v2_frame_header parse_frame_header(FILE* stream) {
  id3v2_frame_header header;
  header.start = ftell(stream);

  char identifier[5];
  identifier[4] = 0;

  fread(identifier, 4, 1, stream);
  strcpy(header.id, identifier);

  char size[4];
  fread(size, 4, 1, stream);
  header.size = size[3] + size[2]*128 + size[1]*128*128 + size[0]*128*128;

  unsigned char flags[2];
  fread(flags, 2, 1, stream);

  return header;
}

void skip_frame(FILE* stream, id3v2_frame_header frame) {
  fseek(stream, frame.size + frame.start + 10, SEEK_SET);
}

void swap(char *a, char *b) {
  char temp = *a;
  *a = *b;
  *b = temp;
}

chapter get_chapter(FILE* stream, id3v2_frame_header frame) {
  chapter chap;
  chapter_frame chap_frame = parse_chapter(stream);
  chap.start_time = chap_frame.start_time;
  if (ftell(stream) <= frame.start + frame.size + 10) {
    id3v2_frame_header frame = parse_frame_header(stream);
    char *title = malloc(frame.size - 1);
    char type;
    fread(&type, 1, 1, stream);
    fread(title, frame.size - 1, 1, stream);
    chap.title = title;
  } else {
    chap.title = 0;
  }
  return chap;
}


chapter_frame parse_chapter(FILE* stream) {
  chapter_frame frame;
  for(int i=0; i<20; ++i) {
    fread((frame.id + i), 1, 1, stream);
    if (frame.id[i] == 0) {
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
  return frame;
}
