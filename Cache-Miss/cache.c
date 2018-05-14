#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char**argv) 
{
  size_t n = (argc == 2 ) ? atoi(argv[1]) : 100;
  double* v = malloc(sizeof(double) * n);
  for(size_t i = 0; i < n ; i++)
    v[i] = i;

  double s = 0;
  for(size_t i = 0; i < n ; ++i)
    s += v[i] * v[n - 1 - i];
  printf("%ld\n", s);
  free(v);
  return 0;
}
