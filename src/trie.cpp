#include "trie.hpp"

/* -----------trie----------- */

template<typename T> 
trie<T>::trie()
{
  root = new trie_node();
}

template<typename T> 
int trie<T>::insert_node(char *str,T *info)
{
  insert_node(root, str, info);
}

template<typename T> 
int trie<T>::insert_node(trie_node* node, char *str, T *info)
{
  char val = *str;    

  if(val == 0)
  {
    node->infos.push_back(info);
    return 1;
  }

  if(node->children.find(val) == node->children.end())
  {
    node->children[val] = new trie_node();
    node->children[val]->set_val(val);
  }

  return insert_node(node->children[val], str+1, info);
}

template<typename T> 
typename trie<T>::iterator trie<T>::end()
{
  return _end;
}

template<typename T>
typename trie<T>::iterator trie<T>::find(char *str)
{
  return find(root, str); 
}

template<typename T>
typename trie<T>::iterator trie<T>::find(trie<T>::trie_node *node, char *str)
{
  char val = *str;
  if(val == 0) return iterator(node);
  if(node->children.find(val) == node->children.end()) return _end;
  return find(node->children[val], str+1);
}

/* -----------trie::iterator----------- */

template<typename T>
trie<T>::iterator::iterator()
{
  it = NULL;
}

template<typename T>
trie<T>::iterator::iterator(trie<T>::trie_node *node)
{
  it = node;
}


/* -----------trie::trie_node----------- */

template<typename T>
const std::vector<T*>& trie<T>::trie_node::get_infos()
{
  return infos;
}

template<typename T>
char trie<T>::trie_node::get_val()
{
  return val;
}

template<typename T>
void trie<T>::trie_node::set_val(char _val)
{
  val = _val;
}
