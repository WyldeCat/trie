#ifndef _TRIE_CPP_
#define _TRIE_CPP_

/* -----------trie----------- */

#include "../include/trie.hpp"


template<typename T, long key> 
trie<T, key>::trie()
{
  root = trie_node();
  _end.it = NULL;
  _begin.it = &root;
}


template<typename T, long key> 
int trie<T, key>::insert(char *str,T info)
{
  return insert(&root, str, info);
}

template<typename T, long key> 
int trie<T, key>::insert(trie_node* node, char *str, T info)
{
  char val = *str;    

  if(val == 0)
  {
    node->infos.push_back(info);
    return 1;
  }

  auto ret = node->children.find(val);
  if(ret == node->children.end())
  {
    node->children.insert(std::make_pair(val, trie_internal_node(node)));
    ret = node->children.find(val);
    ret->second.set_val(val);
  }

  return insert(&ret->second, str+1, info);
}

template<typename T, long key> 
typename trie<T, key>::iterator trie<T, key>::end()
{
  return _end;
}

template<typename T, long key>
typename trie<T, key>::iterator trie<T, key>::begin()
{
  _begin.it = &root;
  return _begin;
}

template<typename T, long key>
typename trie<T, key>::iterator trie<T, key>::find(char *str)
{
  trie<T, key>::iterator it;
  it.caption = std::string(str);
  find(&root, str, it); 
  
  return it;
}

template<typename T, long key>
void trie<T, key>::find(trie<T, key>::trie_node *node, char *str, trie<T, key>::iterator &it)
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
  find(&node->children[val], str+1, it);
}

/* -----------trie::iterator----------- */

template<typename T, long key> trie<T, key>::iterator::iterator()
{
  it = NULL;
}

template<typename T, long key>
const std::string& trie<T, key>::iterator::get_caption()
{
  return this->caption;
} /* -----------trie::bfs_iterator----------- */

template<typename T, long key>
const std::string& trie<T, key>::bfs_iterator::get_caption()
{
  this->caption = std::string();
  trie_node* node = this->it;
  
  while(1)
  {
    if(!node->is_internal) break;
    this->caption.push_back(node->get_val());
    trie_internal_node *internal_node = (trie_internal_node *)node;
    node = &internal_node->parent;
  }
  if(!this->caption.empty())
    std::reverse(this->caption.begin(),this->caption.end());

  return this->caption;
}

template<typename T, long key> trie<T, key>::bfs_iterator::bfs_iterator():iterator(){}
template<typename T, long key> trie<T, key>::bfs_iterator::bfs_iterator(const trie<T, key>::iterator &_it)
{
  this->it = _it.it;
  queue.push(this->it);
}

template<typename T, long key> 
typename trie<T, key>::bfs_iterator& trie<T, key>::bfs_iterator::operator=(const trie<T, key>::iterator &_it)
{ 
  this->it = _it.it;
  while(!queue.empty()) queue.pop();
  queue.push(this->it);
  return *this;
}

// prefix
template<typename T, long key>
typename trie<T, key>::bfs_iterator& trie<T, key>::bfs_iterator::operator++()
{
  if(queue.empty())
  {
    this->it = NULL;
    return *this;
  }

  trie_node *front = queue.front();
  queue.pop();
  for(auto &pair: front->children) 
    queue.push(&pair.second);
  this->it = queue.empty() ? NULL : queue.front();

  return *this;
}

// postfix
template<typename T, long key>
typename trie<T, key>::bfs_iterator trie<T, key>::bfs_iterator::operator++(int)
{
  bfs_iterator temp = *this;
  ++*this;
  return temp;
}

/* -----------trie::dfs_iterator----------- */

template<typename T, long key> trie<T, key>::dfs_iterator::dfs_iterator():iterator(){}
template<typename T, long key> trie<T, key>::dfs_iterator::dfs_iterator(const trie<T, key>::iterator &_it)
{
  this->it = _it.it;
  this->caption = _it.caption;
  stack.push(std::make_pair(this->it,0));
}

template<typename T, long key> 
typename trie<T, key>::dfs_iterator& trie<T, key>::dfs_iterator::operator=(const trie<T, key>::iterator &_it)
{
  this->it = _it.it;
  this->caption = _it.caption;
  while(!stack.empty()) stack.pop();
  stack.push(std::make_pair(this->it,0));

  return *this;
}

// prefix
  template<typename T, long key>
typename trie<T, key>::dfs_iterator& trie<T, key>::dfs_iterator::operator++()
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
    stack.push(std::make_pair(&(it->second),level+1));

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
  template<typename T, long key>
typename trie<T, key>::dfs_iterator trie<T, key>::dfs_iterator::operator++(int)
{
  dfs_iterator temp = *this;
  ++*this;
  return temp;
}

/* -----------trie::trie_node----------- */

template<typename T, long key>
const std::vector<T>& trie<T, key>::trie_node::get_infos()
{
  return infos;
}

template<typename T, long key>
char trie<T, key>::trie_node::get_val()
{
  fprintf(stderr,"asdfasdlfkjasdlfkjasldkfj\n");
  return val;
}

template<typename T, long key>
void trie<T, key>::trie_node::set_val(char _val)
{
  val = _val;
}

#endif
