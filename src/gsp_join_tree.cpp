#include "gsp_join_tree.h"

GspJoinTree::GspJoinTree() : root_(new Node())
{

}

GspJoinTree::~GspJoinTree()
{
  delete root_;
}

void GspJoinTree::AddSequence(GspSequence * seq)
{
  root_->AddSequence(seq, seq->begin());
}

void GspJoinTree::FindJoinable(GspSequence *seq, std::list<GspSequence *> *&result)
{
  GspSequence::IterType current = seq->begin();
  GspSequence::IterType final = seq->end();
  root_->FindJoinable(current, final, result);
}

void GspJoinTree::Node::FindJoinable(GspSequence::IterType &current, GspSequence::IterType &final, std::list<GspSequence *> *&result)
{
  if(current == final)
  {
    if(sequences_)
    {
      result = sequences_;
    }
    return;
  }

  std::string hash = (*current)->ToStringNoBrac();
  if(nodes_)
  {
    if(nodes_->find(hash) != nodes_->end())
      (*nodes_)[hash]->FindJoinable(++current, final, result);
  }
}

void  GspJoinTree::Node::AddSequence(GspSequence *seq, GspSequence::IterType iter)
{
  //if previous last
  if(iter == seq->end())
  {
    if(!sequences_)
    {
      sequences_ = new std::list<GspSequence *>();
    }
    sequences_->push_back(seq);
    return;
  }

  GspItemset *itemset = *iter;
  ++iter;
  //if current last
  if(iter == seq->end())
  {
    //single item in last itemset, need to drop it, so we insert here
    if(itemset->item_count() == 1)
    {
      if(!sequences_)
      {
        sequences_ = new std::list<GspSequence *>();
      }
      sequences_->push_back(seq);
    }
    else
    {
      //need to insert in next, but hashing with dropping the last item
      std::string hash = itemset->ToStringNoBracNoLast();
      if(!nodes_)
      {
        nodes_ = new std::map<std::string, Node *>();
      }
      if(nodes_->find(hash) == nodes_->end())
      {
        (*nodes_)[hash] = new Node();
      }
      (*nodes_)[hash]->AddSequence(seq, iter);
    }
    return;
  }

  std::string hash = itemset->ToStringNoBrac();
  if(!nodes_)
  {
    nodes_ = new std::map<std::string, Node *>();
  }
  if(nodes_->find(hash) == nodes_->end())
  {
    (*nodes_)[hash] = new Node();
  }
  (*nodes_)[hash]->AddSequence(seq, iter);
}

