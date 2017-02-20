#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <functional>

#include <sys/mman.h>
#include <sys/shm.h>

#include "shared_stl_allocator.hpp"

template<typename T, long key1, long key2>
class trie
{

public: 
  static void attach()
  {
    shared_stl_allocator<std::pair<char, trie_node>,key1>::attach();
  }
  class iterator;
  class bfs_iterator;
  class dfs_iterator;

  class trie_node
  { 
  public:
    char get_val();
    void set_val(char _val);
    const std::vector<T, shared_stl_allocator<T, key2> >& get_infos();

    trie_node *parent;
    std::map<char,trie_node, std::less<char>, shared_stl_allocator<std::pair<char,trie_node>,key1> > children;
    char val = 0;

    trie_node() { }
    trie_node(char _val):val(_val) { }

    std::vector<T, shared_stl_allocator<T, key2> > infos;

    friend class trie<T, key1, key2>;
    friend class trie<T, key1, key2>::iterator;
    friend class trie<T, key1, key2>::bfs_iterator;
    friend class trie<T, key1, key2>::dfs_iterator;
  };

public:
  class iterator
  {
  public:
    trie_node* operator->()
    { 
      return it;
    }
    iterator();

    bool operator==(const bfs_iterator& r){ return (this->it == r.it); }
    bool operator==(const dfs_iterator& r){ return (this->it == r.it); }
    bool operator!=(const bfs_iterator& r){ return !(*this == r); }
    bool operator!=(const dfs_iterator& r){ return !(*this == r); }
    const std::string& get_caption();

  protected:
    trie_node* it;
    std::string caption;
  private:
    iterator& operator++();    // Overloading prefix increment operator
    iterator  operator++(int); // Overloading postfix increment operator

    friend class trie<T, key1, key2>;
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

    bool operator==(const iterator& r){ return (this->it == r.it); }
    bool operator==(const dfs_iterator& r){ return (this->it == r.it); }
    bool operator!=(const iterator& r){ return !(*this == r); }
    bool operator!=(const dfs_iterator& r){ return !(*this == r); }
    const std::string& get_caption();


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

    bool operator==(const iterator& r){ return (this->it == r.it); }
    bool operator==(const bfs_iterator& r){ return (this->it == r.it); }
    bool operator!=(const iterator& r){ return !(*this == r); }
    bool operator!=(const bfs_iterator& r){ return !(*this == r); }

  private:
    std::stack<std::pair<trie_node*,int>> stack;
  };


  trie();
  int insert(char *str, T &info);
  iterator find(char *str);
  void findAll(char *str, std::vector<T*> &v);
  trie<T, key1, key2>::iterator begin();
  trie<T, key1, key2>::iterator end();

public:
  std::map<std::pair<trie_node*,char*>, bool> visit;
  iterator _begin;
  iterator _end;
  bool find_all_flag;
  int insert(trie_node *node, char *str, T &info);
  void find(trie_node *node, char *str, iterator &it);
  void findAll(trie_node *node, char *str, std::vector<T*> &v);
  trie_node root;
};

#include "../src/trie.cpp"

#endif
