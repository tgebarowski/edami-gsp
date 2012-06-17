#include "gsp_join_tree.h"
#include <cstdlib>

GspJoinTree::GspJoinTree() : root_(new Node())
{

}

GspJoinTree::~GspJoinTree()
{
  delete root_;
}

void GspJoinTree::AddSequence(GspSequence * seq)
{
  GspSequence::IterType iter = seq->begin();
  root_->AddSequence(seq, iter);
}

void GspJoinTree::FindJoinable(GspSequence *seq, std::list<GspSequence *> &result)
{
  GspSequence::IterType current = seq->begin();
  GspSequence::IterType final = seq->end();
//  std::cout<<"Finding joinables! "<<seq->ToString()<<std::endl;
  root_->FindJoinable(current, final, result);
}

bool GspJoinTree::FindSequence(GspSequence *seq)
{
  GspSequence::IterType iter = seq->begin();
  return root_->FindSequence(seq, iter);
}

void GspJoinTree::Node::FindJoinable(GspSequence::IterType current, GspSequence::IterType &final, std::list<GspSequence *> &result)
{
  if(current == final)
  {
    if(seq_)
    {
//      std::cout<<"Found! "<<seq_->ToString()<<std::endl;
      result.push_back(seq_);
    }
    return;
  }

  GspSequence::IterType tempIter = current;
  ++tempIter;

  //last element of the sequence being checked, now find joinables
  if(tempIter == final)
  {
    if(nodes_)
    {
      //find exact match to add sequences with single-item transactions at the end
      std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator it = nodes_->find(*current);
      if(it != nodes_->end())
      {
        it->second->Add1AtEnd(result);
      }

      for(std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator it = nodes_->begin(); it != nodes_->end(); ++it)
      {
        if((*current)->CompareNoLast(*(it->first)))
          it->second->FindJoinable(tempIter, final, result);
      }
    }
  }
  else
  {
    if(nodes_)
    {
      std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator it = nodes_->find(*current);
      if(it != nodes_->end())
      {
        it->second->FindJoinable(tempIter, final, result);
      }
    }
  }
}

void GspJoinTree::Node::Add1AtEnd(std::list<GspSequence *> &result)
{
  if(nodes_)
  {
    for(std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator it = nodes_->begin(); it != nodes_->end(); ++it)
    {
      if(it->first->item_count() == 1)
      {
        if(it->second->seq_)
          result.push_back(it->second->seq_);
      }
    }
  }
}

bool GspJoinTree::Node::FindSequence(GspSequence *seq, GspSequence::IterType &iter)
{
  if(iter == seq->end())
  {
    if (seq_)
      return true;
    return false;
  }

  std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator mapIter = nodes_->find(*iter);
  if(mapIter == nodes_->end())
    return false;
  Node *nextNode = mapIter->second;
  return nextNode->FindSequence(seq, ++iter);
}

void  GspJoinTree::Node::AddSequence(GspSequence *seq, GspSequence::IterType &iter)
{
  //if previous last
  if(iter == seq->end())
  {
    seq_ = seq;
    return;
  }


  GspItemset *itemset = *iter;
  ++iter;

  if(!nodes_)
  {
    nodes_ = new std::map<GspItemset *, Node *, GspItemSetrPtrComparer>();
  }
  std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator itt = nodes_->find(itemset);
  if(itt == nodes_->end())
  {
    (*nodes_)[itemset] = new Node();
  }

  (*nodes_)[itemset]->AddSequence(seq, iter);
}

