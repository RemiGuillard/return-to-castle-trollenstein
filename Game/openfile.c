#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "Trollenstein.h"

mapData *loadMap(char* fileName)
{
  int fd;
  int rsize = 1;
  struct stat st;
  char *buf;
  char **map;
  fd = open(fileName, O_RDONLY);
  if (fd == -1)
    return 0;
  stat(fileName, &st);
  buf = malloc((int)st.st_size + 1);
  int tsize = 0;
  while (rsize > 0)
    {
      rsize = read(fd, buf, (int)st.st_size);
    }
  int bsize = strlen(buf);
  int i = 0;
  int n = 0;
  int c = 0;
  while (i < bsize)
    {
      if (n == 0)
        c++;
      if (buf[i] == '\n')
        n++;
      i++;
    }
  printf("nb de colonne = %d    || nb de ligne = %d\n", c, n);
  map = malloc(sizeof(char *) * (n + 1));
  map[n] = NULL;
  i = 0;
  int border = n;
  int k = 0;
  while (i < border)
    {
      map[i] = malloc(c + 1);
      strncpy(map[i], &buf[k], c - 1);
      map[i][c] = '\0';
      k += c;
      i++;
    }
  i = 0;
  while (map[i] != NULL)
    {
      printf("%s\n", map[i]);
      i++;
    }

  free(buf);
  mapData *res = malloc(sizeof(res));
  res->map = map;
  res->height = n;
  res->width = c;
  return res;
}


