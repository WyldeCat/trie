#ifndef _TRIE_CPP_
#define _TRIE_CPP_

/* -----------trie----------- */

#include "../include/trie.hpp"

template<typename T> 
trie<T>::trie()
{
  root = new trie_node();
  _end.it = NULL;
  _begin.it = root;
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
    node->children[val]->parent = node;
  }

  return insert(node->children[val], str+1, info);
}

template<typename T> 
typename trie<T>::iterator trie<T>::end()
{
  return _end;
}

template<typename T>
typename trie<T>::iterator trie<T>::start()
{
  return _begin;
}

template<typename T>
typename trie<T>::iterator trie<T>::find(char *str)
{
  trie<T>::iterator it;
  it.caption = std::string(str);
  find(root, str, it); 
  
  return it;
}

template<typename T>
void trie<T>::find(trie<T>::trie_node *node, char *str, trie<T>::iterator &it)
{
  char val = *str;
  if(val == 0) 
  {
    it.it = node;
    return;
  }
  if(node->children.find(val) == node->children.end()) {
    it.it = NULL;
    it.caption = std::string();
  }
  find(node->children[val], str+1, it);
}

/* -----------trie::iterator----------- */

template<typename T> trie<T>::iterator::iterator()
{
  it = NULL;
}

template<typename T>
const std::string& trie<T>::iterator::get_caption()
{
  return this->caption;
}
/* -----------trie::bfs_iterator----------- */

template<typename T>
const std::string& trie<T>::bfs_iterator::get_caption()
{
  this->caption = std::string();
  trie_node* it = this->it;
  while(1)
  {
    this->caption.push_back(it->get_val());
    it = it->parent;
    if(it == NULL) break;
  }
  this->caption.pop_back();
  std::reverse(this->caption.begin(),this->caption.end());

  return this->caption;
}

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
  this->caption = _it.caption;
  stack.push(std::make_pair(this->it,0));
}

template<typename T> 
typename trie<T>::dfs_iterator& trie<T>::dfs_iterator::operator=(const trie<T>::iterator &_it)
{
  this->it = _it.it;
  this->caption = _it.caption;
  while(!stack.empty()) stack.pop();
  stack.push(std::make_pair(this->it,0));

  return *this;
}

// prefix
template<typename T>
typename trie<T>::dfs_iterator& trie<T>::dfs_iterator::operator++()
{
  if(stack.empty())
  {
    this->it = NULL;
    this->caption = std::string();
    return *this;
  }

  trie_node *top = stack.top().first;
  int level = stack.top().second;

  stack.pop();

  for(auto it = top->children.rbegin(); it != top->children.rend(); ++it) 
    stack.push(std::make_pair(it->second,level+1));
  
  if(top->children.empty() && !stack.empty()) 
  {
    int loop_cnt = level - stack.top().second + 1;
    for(int i=0;i<loop_cnt;i++) this->caption.pop_back();
  }

  if(!stack.empty()) 
  {
    this->it = stack.top().first;
    this->caption.push_back(stack.top().first->get_val());
  }
  else 
  {
    this->it = NULL;
    this->caption = std::string();
  }

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

#endif
