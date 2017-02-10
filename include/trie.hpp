#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <vector>
#include <map>
#include <queue>
#include <stack>

template<typename T>
class trie
{

public: 
  class iterator;
  class trie_node
  { 
  public:
    char get_val();
    void set_val(char _val);
    const std::vector<T>& get_infos();

  private:
    char val;
    std::map<char,trie_node*> children;
    std::vector<T> infos;

    friend class trie<T>;
    friend class trie<T>::iterator;
  };

public:
  class bfs_iterator;
  class dfs_iterator;
  class iterator
  {
  public:
    trie_node* operator->()
    {
      return it;
    }
    iterator(trie_node *node);
    iterator();

    bool operator==(const bfs_iterator& r){ return (this->it == r.it); }
    bool operator==(const dfs_iterator& r){ return (this->it == r.it); }
    bool operator!=(const bfs_iterator& r){ return !(*this == r); }
    bool operator!=(const dfs_iterator& r){ return !(*this == r); }

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

    bool operator==(const iterator& r){ return (this->it == r.it); }
    bool operator==(const dfs_iterator& r){ return (this->it == r.it); }
    bool operator!=(const iterator& r){ return !(*this == r); }
    bool operator!=(const dfs_iterator& r){ return !(*this == r); }


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
    std::stack<trie_node*> stack;
  };

  trie();
  int insert(char *str, T info);
  iterator find(char *str);
  trie<T>::iterator end();

private:
  iterator _end;
  int insert(trie_node *node, char *str, T info);
  iterator find(trie_node *node, char *str);
  trie_node* root;
};

#include "trie.cpp"

#endif
