#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <vector>
#include <map>

template<typename T>
class trie
{
  class trie_node
  {
  public:
    char get_val();
    void set_val(char _val);
    const std::vector<T*>& get_infos();

  private:
    char val;
    std::map<char,trie_node*> children;
    std::vector<T*> infos;

    friend class trie<T>;
    friend class trie<T>::iterator;
  };

  class iterator
  {
  public:
    trie_node* operator->()
    {
      return it;
    }
    iterator(trie_node *node);
    iterator();
  private:
    trie_node* it;
    iterator& operator++();
    iterator  operator++(int);
 
    friend class trie<T>;
  };

  class bfs_iterator: public iterator
  {
  public:
    bfs_iterator& operator++();
    bfs_iterator  operator++(int);
  };

  class dfs_iterator: public iterator
  {
  public:
    dfs_iterator& operator++();
    dfs_iterator  operator++(int);
  };

public:
  trie();
  int insert_node(char *str, T *info);
  iterator find(char *str);
  trie<T>::iterator end();

private:
  iterator _end;
  int insert_node(trie_node *node, char *str, T *info);
  iterator find(trie_node *node, char *str);
  trie_node* root;
};

template class trie<int>;

#endif
