#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>

template<typename T>
class trie
{

public: 
  class iterator;
  class bfs_iterator;
  class dfs_iterator;

  class trie_node
  { 
  public:
    char get_val();
    void set_val(char _val);
    const std::vector<T>& get_infos();

  private:
    char val;
    trie_node *parent;
    std::map<char,trie_node*> children;
    std::vector<T> infos;

    friend class trie<T>;
    friend class trie<T>::iterator;
    friend class trie<T>::bfs_iterator;
    friend class trie<T>::dfs_iterator;
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
  int insert(char *str, T info);
  iterator find(char *str);
  trie<T>::iterator start();
  trie<T>::iterator end();

private:
  iterator _end;
  iterator _start;
  int insert(trie_node *node, char *str, T info);
  void find(trie_node *node, char *str, iterator &it);
  trie_node* root;
};

#include "../src/trie.cpp"

#endif
