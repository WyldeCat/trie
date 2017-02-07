#include <vector>

template<typename T>
class trie
{
  class trie_node
  {
  public:
    char get_val();
    std::vector<T*>* get_infos();

  private:
    char val;
    std::vector<trie_node*> child;
    std::vector<T*> infos;
  };

  class iterator
  {
  public:
    trie_node* operator->()
    {
      return it;
    }
  private:
    iterator();
    trie_node* it;
  };

  class bfs_iterator: public iterator
  {
  public:
    virtual iterator& operator++();
    virtual iterator  operator++(int);
  };

  class dfs_iterator: public iterator
  {
  public:
    virtual iterator& operator++();
    virtual iterator  operator++(int);
  };

public:
  int insert_node(char *arr); 
  iterator* end();
  iterator* find(char *arr);

private:
  trie_node* root;

};
