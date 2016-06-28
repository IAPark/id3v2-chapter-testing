#include <stdio.h>
#include "parser.h"
#include <string.h>

int main() {
  FILE* f = fopen("part0.mp3","rb");
  id3v2_header header = parse_header(f);
  id3v2_frame_header frame_header;
  strcpy(frame_header.id,"");
  frame_header.size = 0;
  do {
    frame_header = parse_frame_header(f);
    if (strcmp(frame_header.id,"CHAP") != 0) {
      skip_frame(f, frame_header);
    }
    printf("At %li\n", ftell(f));
  } while(strcmp(frame_header.id,"CHAP") != 0 && ftell(f) < header.size);
  parse_chapter(f);
}
