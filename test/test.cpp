#include "../include/trie.hpp"

#include <cstdio>

int main( )
{
  trie<int> test;
  char a[5] = "aABC", b[5] = "aDEF", c[3] = "aA";
  int i = 3, j = 5;

  test.insert(a,i);
  test.insert(b,j);

  printf("bfs..!\n");
  for(trie<int>::bfs_iterator it = test.find(c); it != test.end(); ++it) 
  {
    printf("%s\n",it.get_caption().c_str());
  }

  printf("dfs..!\n");
  for(trie<int>::dfs_iterator it = test.find(c); it != test.end(); ++it) 
  {
    printf("%s\n",it.get_caption().c_str());
  }

  return 0;
}
