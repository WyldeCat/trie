#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <vector>
#include <map>
#include <queue>
#include <stack>

template<typename T>
class trie
{
  class iterator;
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
  protected:
    trie_node* it;
  private:
    iterator& operator++();    // Overloading prefix increment operator
    iterator  operator++(int); // Overloading postfix increment operator
 
    friend class trie<T>;
  };

  class bfs_iterator: public iterator
  {
  public:
    typedef iterator super;
    bfs_iterator();

    bfs_iterator(const iterator &_it);
    bfs_iterator& operator=(const iterator &_it);

    bfs_iterator& operator++();
    bfs_iterator  operator++(int);
  private:
    std::queue<trie_node*> queue;
  };

  class dfs_iterator: public iterator
  {
  public:
    typedef iterator super;
    dfs_iterator();

    dfs_iterator(const iterator &it); 
    dfs_iterator& operator=(const iterator &it);

    dfs_iterator& operator++();
    dfs_iterator  operator++(int);
  private:
    std::stack<trie_node*> stack;
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
