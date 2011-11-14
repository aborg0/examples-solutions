#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

#define C(a) ((int *)a)

char inputBuffer[4096*16+1];
int content[4096];
int nextState[] = {1, 2, 3, 4, 3};

int compare(const void *a, const void *b) {
  float left = *C(a) / (float)*(C(a) + 1), right = *C(b) / (float)*(C(b) + 1);
  return left < right ? -1 : left > right ? 1 : 0;
}

int main(const int argc, const char ** const argv) {
  char *inputBufferPtr, *inputBufferEnd;
  int *contentPtr, *i;
  char ch;
  int unload, n, sumCost;
  int r = 0;
  int state = 0;

  inputBufferPtr = inputBufferEnd = inputBuffer + sizeof(inputBuffer);
  contentPtr = content;
  int fh = open(argv[1], O_RDONLY);
  while (1) {
    if (inputBufferPtr >= inputBufferEnd) {
      inputBufferEnd = inputBuffer + read(fh, inputBuffer, sizeof(inputBuffer));
      if (inputBufferEnd == inputBuffer)
        break;
      inputBufferPtr = inputBuffer;
    }
    ch = *inputBufferPtr++;
    switch (state) {
      case 0://garbage
      case 2:
        unload = r;
        if (ch >= '0' && ch <= '9') {//if this
          state = nextState[state];
          r = ch - '0';
        }
        break;//Till this is not added, the O3 compilation gives different results. :( gcc bug
      case 4:
        *contentPtr = r;
        if (ch >= '0' && ch <= '9') {
          ++contentPtr;
          state = nextState[state];
          r = ch - '0';
        }
        break;
      case 1://reading number
      case 3:
        if (ch >= '0' && ch <= '9') {
          r *= 10;
          r += ch - '0';
        }
        else {
          *contentPtr = r;
          state = nextState[state];
        }
        break;
    }
  }
  close(fh);
  i = content;
  qsort(i, (contentPtr + 1 - i) / 2, 2 * sizeof(int), compare);
  int min = INT_MAX;
  for (; i <= contentPtr; i += 2) {
    //printf("%d %d\n", *i, *(i + 1));
    fflush(stdout);
    n = unload / *(i + 1);
    sumCost += n * *i;
    r = sumCost + *i;
    min = min > r ? r : min;
    unload %= *(i + 1);
    if (unload == 0) {
      min = min > (r -= *i) ? r : min;
    }
  }
  printf("%d\n", min);
  return 0;
}
