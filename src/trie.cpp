#include "trie.hpp"
#include <stdio.h>
/* -----------trie----------- */


template<typename T> 
trie<T>::trie()
{
  root = new trie_node();
}

template<typename T> 
int trie<T>::insert(char *str,T info)
{
  return insert(root, str, info);
}

template<typename T> 
int trie<T>::insert(trie_node* node, char *str, T info)
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

  return insert(node->children[val], str+1, info);
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

template<typename T> trie<T>::iterator::iterator()
{
  it = NULL;
}
template<typename T> trie<T>::iterator::iterator(trie<T>::trie_node *node)
{
  it = node;
}

/* -----------trie::bfs_iterator----------- */

template<typename T> trie<T>::bfs_iterator::bfs_iterator():iterator(){}
template<typename T> trie<T>::bfs_iterator::bfs_iterator(const trie<T>::iterator &_it)
{
  this->it = _it.it;
  queue.push(this->it);
}

template<typename T> 
typename trie<T>::bfs_iterator& trie<T>::bfs_iterator::operator=(const trie<T>::iterator &_it)
{
  this->it = _it.it;
  while(!queue.empty()) queue.pop();
  queue.push(this->it);

  return *this;
}

// prefix
template<typename T>
typename trie<T>::bfs_iterator& trie<T>::bfs_iterator::operator++()
{
  if(queue.empty())
  {
    this->it = NULL;
    return *this;
  }

  trie_node *front = queue.front();
  queue.pop();

  for(std::pair<char,trie_node*> pair: front->children) queue.push(pair.second);
  this->it = queue.empty() ? NULL : queue.front();

  return *this;
}

// postfix
template<typename T>
typename trie<T>::bfs_iterator trie<T>::bfs_iterator::operator++(int)
{
  bfs_iterator temp = *this;
  ++*this;
  return temp;
}

/* -----------trie::dfs_iterator----------- */

template<typename T> trie<T>::dfs_iterator::dfs_iterator():iterator(){}
template<typename T> trie<T>::dfs_iterator::dfs_iterator(const trie<T>::iterator &_it)
{
  this->it = _it.it;
  stack.push(this->it);
}

template<typename T> 
typename trie<T>::dfs_iterator& trie<T>::dfs_iterator::operator=(const trie<T>::iterator &_it)
{
  this->it = _it.it;
  while(!stack.empty()) stack.pop();
  stack.push(this->it);

  return *this;
}

// prefix
template<typename T>
typename trie<T>::dfs_iterator& trie<T>::dfs_iterator::operator++()
{
  if(stack.empty())
  {
    this->it = NULL;
    return *this;
  }

  trie_node *top = stack.top();
  stack.pop();

  for(auto it = top->children.rbegin(); it != top->children.rend(); ++it) stack.push(it->second);
  this->it = stack.empty() ? NULL : stack.top();

  return *this;
}

// postfix
template<typename T>
typename trie<T>::dfs_iterator trie<T>::dfs_iterator::operator++(int)
{
  dfs_iterator temp = *this;
  ++*this;
  return temp;
}

/* -----------trie::trie_node----------- */

template<typename T>
const std::vector<T>& trie<T>::trie_node::get_infos()
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

template class trie<int>;
