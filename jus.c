#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct Node {
  struct Node *next;
  char         word[81];
};

struct {
  struct Node *first;
  struct Node *last;
} fifo = { NULL, NULL };

void fifo_add_word(const char *word)
{
  struct Node *prev;

  prev = fifo.last;
  fifo.last = malloc(sizeof(struct Node));

  if (!fifo.last) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  if (prev)
    prev->next = fifo.last;
  
  if (!fifo.first)
    fifo.first = fifo.last;

  fifo.last->next = NULL;
  strcpy(fifo.last->word, word);
}

int fifo_print_word(void)
{
  struct Node *node;

  node = fifo.first;

  if (!node)
    return 0;

  fifo.first = node->next;

  if (!fifo.first)
    fifo.last = NULL;

  printf("%s", node->word);
  free(node);

  return 1;
}

inline int fifo_is_empty(void)
{
  return fifo.first == NULL;
}

inline void display_spaces(size_t spaces)
{
  size_t i;

  for (i = 0; i < spaces; ++i)
    putchar(' ');
}

inline float get_max_tab(size_t graphs, size_t words)
{
  --words;

  if (words == 0)
    return 0;

  return (80.0 - graphs) / words;
}

size_t get_tab(float max_tab, size_t i, size_t words)
{
  static size_t spaces = 0;  /* number of spaces in line */
  size_t        tab;

  tab     = floor(i * max_tab) - floor((i - 1) * max_tab);
  spaces += tab;

  if (i == words - 1) {
    if (round(i * max_tab) != spaces) 
      tab += round(i * max_tab) - spaces;

    spaces = 0;
  }

  return tab;
}

int main()
{
  char   word[81]  = { 0 };  /* actual parsed word */
  size_t words     = 0,      /* words in line */
         wordlen,            /* strlen(word) */
         graphs    = 0,      /* number of graphical characters in line */
         i;                  /* word index */
  double max_tab;            /* max length of tab measured in spaces */

  while (scanf("%80s", word) > 0) {
    wordlen = strlen(word);

    /* if last readed word overflows the line */
    if (get_max_tab(graphs + wordlen, words + 1) < 1.0) {
      max_tab = get_max_tab(graphs, words);
      fifo_print_word();

      for (i = 1; !fifo_is_empty(); ++i) {
        display_spaces(get_tab(max_tab, i, words));
        fifo_print_word();
      }

      words = 1;
      graphs = wordlen;
    } else {
      graphs += wordlen;
      ++words;
    }

    fifo_add_word(word);
  }

  while (!fifo_is_empty()) {
    fifo_print_word();
    putchar(' ');
  }

  putchar('\n');
  return 0;
}
