#include "../src/trie.hpp"
#include <cstdio>

int main( )
{
  trie<int> test;
  char a[5] = "aABC", b[5] = "aDEF", c[2] = "a";
  int i = 3, j = 5;

  test.insert(a,i);
  test.insert(b,j);

  printf("bfs..! ");
  for(trie<int>::bfs_iterator it = test.find(c); it != test.end(); ++it) 
    printf("%c ",it->get_val());

  printf("\ndfs..! ");
  for(trie<int>::dfs_iterator it = test.find(c); it != test.end(); ++it) 
    printf("%c ",it->get_val());

  printf("\n");

  return 0;
}
