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

  return header;
}

void skip_frame(FILE* stream, id3v2_frame_header frame) {
  fseek(stream, frame.size, SEEK_CUR);

}
