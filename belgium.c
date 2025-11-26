#include <assert.h>
#include <errno.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

char *word = "belgium";
char *colors[3] = {"\x1B[30m", "\x1B[33m", "\x1B[31m"};
int word_len = 0;

static inline void clear_screen() {
  putchar(0x1B);
  putchar('c');
}

static void draw() {
  struct winsize winsz;
  ioctl(0, TIOCGWINSZ, &winsz);

  for (int i = 0; i < winsz.ws_row; ++i) {
    int to_print = 0;
    for (int j = 0; j < 3; ++j) {
      to_print = (winsz.ws_col / 3);
      while (to_print != 0) {
        if (to_print < word_len) {
          printf("%s%.*s", colors[j], to_print, word);
          to_print = 0;
        } else {
          printf("%s%s", colors[j], word);
          to_print = to_print - word_len;
        }
      }
    }
    putchar('\n');
  }
}

static void sig_handler(int signum)
{
    if(signum == SIGWINCH){
        clear_screen();
        draw();
    }
}

int main(void) {
  word_len = strlen(word);
  signal(SIGWINCH, sig_handler);

  clear_screen();
  draw();

  while(1)
  {
      pause();
  }
  return 0;
}
