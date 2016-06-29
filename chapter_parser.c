#include <stdio.h>
#include "parser.h"
#include <string.h>

int main() {
  FILE* f = fopen("part0.mp3","rb");
  chapters chaps = get_chapters(f);

  for(int i = 0; i < chaps.size; ++i) {
    printf("%s::%i\n", chaps.chapters[i].title, chaps.chapters[i].start_time);
  }
}
