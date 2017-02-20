#ifndef _TRIE_CPP_
#define _TRIE_CPP_

/* -----------trie----------- */

#include "../include/trie.hpp"


template<typename T, long key1, long key2> 
trie<T, key1, key2>::trie()
{
  root = trie_node();
  _end.it = NULL;
  _begin.it = &root;
}


template<typename T, long key1, long key2> 
int trie<T, key1, key2>::insert(char *str,T &info)
{
  return insert(&root, str, info);
}

template<typename T, long key1, long key2> 
int trie<T, key1, key2>::insert(trie_node* node, char *str, T &info)
{
  char val = *str;    

  if(val == 0)
  {
    node->infos.push_back(T{});
    for(int i=0;i<info.name.size();i++)
      node->infos.back().name.push_back(info.name[i]);
    for(int i=0;i<info.image.size();i++)
      node->infos.back().image.push_back(info.image[i]);
    for(int i=0;i<info._id.size();i++)
      node->infos.back()._id.push_back(info._id[i]);
    return 1;
  }

  auto ret = node->children.find(val);
  if(ret == node->children.end())
  {
    node->children.insert(std::make_pair(val,trie_node(val)));
    ret = node->children.find(val);
  }

  return insert(&ret->second, str+1, info);
}

template<typename T, long key1, long key2> 
typename trie<T, key1, key2>::iterator trie<T, key1, key2>::end()
{
  return _end;
}

template<typename T, long key1, long key2>
typename trie<T, key1, key2>::iterator trie<T, key1, key2>::begin()
{
  _begin.it = &root;
  return _begin;
}

template<typename T, long key1, long key2>
typename trie<T, key1, key2>::iterator trie<T, key1, key2>::find(char *str)
{
  trie<T, key1, key2>::iterator it;
  it.caption = std::string(str);
  find(&root, str, it); 
  
  return it;
}

template<typename T, long key1, long key2>
void trie<T, key1, key2>::findAll(char *str, std::vector<T*> &v)
{
  this->find_all_flag = 0;
  visit.clear();
  findAll(&root, str, v);
}

template<typename T, long key1, long key2>
void trie<T, key1, key2>::find(trie<T, key1, key2>::trie_node *node, char *str, trie<T, key1, key2>::iterator &it)
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
  node->children[val].parent = node;
  find(&node->children[val], str+1, it);
}

template<typename T, long key1, long key2>
void trie<T, key1, key2>::findAll(trie<T, key1, key2>::trie_node *node, char *str,std::vector<T*> &v)
{
  if(visit[std::make_pair(node,str)]) return;
  visit[std::make_pair(node,str)] = true;
  if(this->find_all_flag) return;

  if(*str == 0) 
  {
    int sz = node->infos.size();
    for(int i=0;i<sz;i++) {
      if(std::find(v.begin(),v.end(),&node->infos[i]) != v.end()) continue;
      v.push_back(&node->infos[i]);
    }

    if(v.size()>=5) find_all_flag = true;

    sz = node->children.size();
    auto it = node->children.begin();
    for(int i=0;i<sz;i++)
    {
      if(this->find_all_flag) return;
      findAll(&(it->second),str,v);
      if(i!=sz-1) it++;
    }
    return;
  }

  if(node->children.find(*str) != node->children.end() && !find_all_flag)
    findAll(&node->children[*str], str+1, v); 
  

  int sz = node->children.size();
  auto it = node->children.begin();

  for(int i=0;i<sz;i++)
  {
    findAll(&(it->second),str, v);
    if(this->find_all_flag) return;
    if(i!=sz-1) it++;
  }
}
/* -----------trie::iterator----------- */

template<typename T, long key1, long key2> 
trie<T, key1, key2>::iterator::iterator()
{
  it = NULL;
}

template<typename T, long key1, long key2>
const std::string& trie<T, key1, key2>::iterator::get_caption()
{
  return this->caption;
} 

/* -----------trie::bfs_iterator----------- */

template<typename T, long key1, long key2>
const std::string& trie<T, key1, key2>::bfs_iterator::get_caption()
{
  this->caption = std::string();
  trie_node* node = this->it;
  
  while(node->val)
  {
    this->caption.push_back(node->val);
    node = node->parent;
  }

  if(!this->caption.empty()) 
    std::reverse(this->caption.begin(),this->caption.end());

  return this->caption;
}

template<typename T, long key1, long key2> 
trie<T, key1, key2>::bfs_iterator::bfs_iterator():iterator(){}
template<typename T, long key1, long key2> 
trie<T, key1, key2>::bfs_iterator::bfs_iterator(const trie<T, key1, key2>::iterator &_it)
{
  this->it = _it.it;
  queue.push(this->it);
}

template<typename T, long key1, long key2> 
typename trie<T, key1, key2>::bfs_iterator& trie<T, key1, key2>::bfs_iterator::operator=(const trie<T, key1, key2>::iterator &_it)
{ 
  this->it = _it.it;
  while(!queue.empty()) queue.pop();
  queue.push(this->it);
  return *this;
}

// prefix
template<typename T, long key1, long key2>
typename trie<T, key1, key2>::bfs_iterator& trie<T, key1, key2>::bfs_iterator::operator++()
{
  if(queue.empty())
  {
    this->it = NULL;
    return *this;
  }

  trie_node *front = queue.front();
  queue.pop();

  int sz = front->children.size();

  if(sz)
  {
    for(auto it = front->children.begin();it!=front->children.end(); ++it) 
    {
      (it->second).parent = front;
      queue.push(&(it->second));
      sz--; if(sz==0) break;
    }
  }
  this->it = queue.empty() ? NULL : queue.front();

  return *this;
}

// postfix
  template<typename T, long key1, long key2>
typename trie<T, key1, key2>::bfs_iterator trie<T, key1, key2>::bfs_iterator::operator++(int)
{
  bfs_iterator temp = *this;
  ++*this;
  return temp;
}

/* -----------trie::dfs_iterator----------- */

template<typename T, long key1, long key2> 
trie<T, key1, key2>::dfs_iterator::dfs_iterator():iterator(){}
template<typename T, long key1, long key2> 
trie<T, key1, key2>::dfs_iterator::dfs_iterator(const trie<T, key1, key2>::iterator &_it)
{
  this->it = _it.it;
  this->caption = _it.caption;
  stack.push(std::make_pair(this->it,0));
}

template<typename T, long key1, long key2> 
typename trie<T, key1, key2>::dfs_iterator& trie<T, key1, key2>::dfs_iterator::operator=(const trie<T, key1, key2>::iterator &_it)
{
  this->it = _it.it;
  this->caption = _it.caption;
  while(!stack.empty()) stack.pop();
  stack.push(std::make_pair(this->it,0));

  return *this;
}

// prefix
template<typename T, long key1, long key2>
typename trie<T, key1, key2>::dfs_iterator& trie<T, key1, key2>::dfs_iterator::operator++()
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
  {
    (it->second).parent = top;
    stack.push(std::make_pair(&(it->second),level+1));
  }

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
template<typename T, long key1, long key2>
typename trie<T, key1, key2>::dfs_iterator trie<T, key1, key2>::dfs_iterator::operator++(int)
{
  dfs_iterator temp = *this;
  ++*this;
  return temp;
}

/* -----------trie::trie_node----------- */

template<typename T, long key1, long key2>
const std::vector<T, shared_stl_allocator<T, key2> >& trie<T, key1, key2>::trie_node::get_infos()
{
  return infos;
}

template<typename T, long key1, long key2>
char trie<T, key1, key2>::trie_node::get_val()
{
  return val;
}

template<typename T, long key1, long key2>
void trie<T, key1, key2>::trie_node::set_val(char _val)
{
  val = _val;
}

#endif
