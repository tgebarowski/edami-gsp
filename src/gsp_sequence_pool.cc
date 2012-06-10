/*
 * gsp_sequence_pool.cpp
 *
 *  Created on: Jun 9, 2012
 *      Author: adas
 */

#include <map>
#include <set>
#include <string>

#include "gsp_sequence_pool.h"
#include "gsp_sequence_reader.h"

GspSequencePool::GspSequencePool(unsigned minSupport_) : k(0), minSupport(minSupport_)
{

}

GspSequencePool::GspSequencePool(GspSequenceReader *reader_, unsigned minSupport_)
  : k(1), minSupport(minSupport_)
{
  GspSequence *seq;
  std::map<std::string, unsigned> cand1;
  std::set<std::string> cand1Sequence;

  while((seq = reader_->GetNextSequence()))
  {
    cand1Sequence.clear();
    seq->rewind();
    GspItemset *itemset;
    while((itemset = seq->nextItemset()))
    {
      cand1Sequence.insert(itemset->begin(), itemset->end());
    }

    for (std::set<std::string>::const_iterator seqIt = cand1Sequence.begin(); seqIt!= cand1Sequence.end(); ++seqIt)
    {
      std::map<std::string, unsigned>::iterator mapIter = cand1.find(*seqIt);
      if(mapIter != cand1.end())
        ++(mapIter->second);
      else
        cand1[*seqIt] = 1;
    }

    delete seq;
  }

  for(std::map<std::string, unsigned>::iterator mapIter = cand1.begin(); mapIter != cand1.end(); ++mapIter)
  {
    if (mapIter->second > minSupport)
    {
      seq = new GspSequence("");
      GspItemset *itemset = new GspItemset();
      itemset->add_item(mapIter->first);
      seq->add_itemset(itemset);
      seq->setSupport(mapIter->second);
      sequences.push_back(seq);
    }
  }
}

GspSequencePool *GspSequencePool::join()
{
  GspSequencePool *ret = new GspSequencePool(minSupport);
  GspSequence *newSeq;
  GspItemset *newItemset;
  GspSequence *tempSeqL, *tempSeqR;
  GspItemset *tempItemsetL, *tempItemsetR;
  ret->k = k+1;

  if (k == 1)
  {
    std::list<GspSequence *>::const_iterator it = sequences.begin();
    std::list<GspSequence *>::const_iterator jt = it++;
    for (; jt != sequences.end();)
    {
      //(L)(L)
      tempSeqL = *jt;
      tempSeqL->rewind();
      tempItemsetL = tempSeqL->nextItemset();
      tempItemsetL->rewind();
      std::string itemLeft = tempItemsetL->next();
      newSeq = new GspSequence("");
      newItemset = new GspItemset(0);
      newItemset->add_item(itemLeft);
      newSeq->add_itemset(newItemset);
      newItemset = new GspItemset(0);
      newItemset->add_item(itemLeft);
      newSeq->add_itemset(newItemset);
      ret->sequences.push_back(newSeq);

      for (; it != sequences.end(); ++it)
      {
        tempSeqR = *it;
        tempSeqR->rewind();
        tempItemsetR = tempSeqR->nextItemset();
        tempItemsetR->rewind();
        std::string itemRight = tempItemsetR->next();
        //(L)(R)
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        ret->sequences.push_back(newSeq);

        //(R)(L)
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newSeq->add_itemset(newItemset);
        newItemset = new GspItemset(0);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences.push_back(newSeq);

        //(LR) ordered
        newSeq = new GspSequence("");
        newItemset = new GspItemset(0);
        newItemset->add_item(itemRight);
        newItemset->add_item(itemLeft);
        newSeq->add_itemset(newItemset);
        ret->sequences.push_back(newSeq);
      }
      it = ++jt;
      jt = it++;
    }
  }
  else
  {

  }

  return ret;
}

void GspSequencePool::printSequences()
{
  for(std::list<GspSequence *>::const_iterator it = sequences.begin(); it != sequences.end(); ++it)
  {
    std::cout<<(*it)->ToString()<<" "<<(*it)->getSupport()<<"\n";
  }
}

GspSequencePool::~GspSequencePool()
{
  for(std::list<GspSequence *>::iterator it = sequences.begin(); it != sequences.end(); ++it)
  {
    delete *it;
  }
}

