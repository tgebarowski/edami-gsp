/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_class.cc
 *
 * @brief Sequence datatype class definition
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:46:11 2012
 */

#include "gsp_sequence_class.h"
#include "gsp_itemset_class.h"
#include <cmath>
#include <sstream>

/* Documented in header */
GspSequence::GspSequence(const std::string &id) : id_(id), support_(0)
{
}

/* Documented in header */
GspSequence::GspSequence(const GspSequence &src)
{
  for (std::list<GspItemset *>::const_iterator it = src.itemsets_.begin();
       it != src.itemsets_.end();
       ++it)
  {
    itemsets_.push_back(new GspItemset(**it));
  }
  iter_ = itemsets_.begin();

  support_ = 0;
}


/* Documented in header */
GspSequence::~GspSequence()
{
  /* Remove all itemsets added to this sequence */
  for (std::list<GspItemset *>::iterator it = itemsets_.begin();
       it != itemsets_.end();
       ++it)
  {
    delete *it;
  }
}

/* Documented in header */
/*
 * TODO needed?
 */
//string GspSequence::GetItemByIndex(int n)
//{
//  int max_current_count = 0; /**< Maximum count achievable while
//                                  processing current itemset */
//  int max_next_count = 0; /**< Maximum count achievable while
//                               processing next itemset */
//  for (list<GspItemset *>::const_iterator it = itemsets_.begin();
//       it != itemsets_.end();
//       ++it)
//  {
//    max_next_count = max_current_count + (*it)->item_count();
//    if (n >= max_current_count && n < max_next_count)
//    {
//      return (*it)->item_by_index(abs(max_current_count - n));
//    }
//    max_current_count = max_next_count;
//  }
//  return EMPTY_SET;
//}


/* Documented in header */
/*
 * TODO needed?
 */
//bool GspSequence::CompareWithSubsequence(GspSequence &s)
//{
//  GspSequence s1(*this);
//  GspSequence s2(s);
//
//  GspItemset *s1_itemset_first = s1.get_first_itemset();
//  GspItemset *s2_itemset_last = s2.get_last_itemset();
//
//  /* Remove first item from first sequence */
//  if (s1_itemset_first != NULL)
//  {
//    s1_itemset_first->remove_first_item();
//  }
//  /* Remove last item from last sequence */
//  if (s2_itemset_last != NULL)
//  {
//    s2_itemset_last->remove_last_item();
//  }
//  return (s1 == s2);
//}

/* Documented in header */
/*
 * TODO needed?
 */
//void GspSequence::AppendItemset(GspItemset *itemset)
//{
//  if (itemset != NULL)
//  {
//    /** Append to existing last itemset */
//    if (itemset->item_count() == 1)
//    {
//      GspItemset *last_itemset = get_last_itemset();
//      if (last_itemset != NULL)
//      {
//        last_itemset->add_item(itemset->item_by_index(0));
//      }
//    }
//    /** Append as separate itemset */
//    else if (itemset->item_count() > 1)
//    {
//      itemsets_.push_back(itemset);
//    }
//  }
//}

GspSequence *GspSequence::JoinSequences(GspSequence *right)
{

  GspSequence *leftCopy = new GspSequence(*this);
  GspSequence *rightCopy = new GspSequence(*right);
  GspSequence *ret = NULL;

  std::cout<<"\nJoining "<<leftCopy->ToString()<<" with "<<rightCopy->ToString()<<std::endl;
  leftCopy->DropFirstItem();
  rightCopy->DropLastItem();
  std::cout<<"Left drop: "<<leftCopy->ToString()<<std::endl;
  std::cout<<"Right drop: "<<rightCopy->ToString()<<std::endl;

  if (*leftCopy == *rightCopy)
  {
    ret = new GspSequence(*this);
    ret->AppendSequence(right);
    std::cout<<"Success: "<<ret->ToString()<<std::endl;
  }

  std::cout<<std::endl;

  delete leftCopy;
  delete rightCopy;

  return ret;
}

void GspSequence::DropFirstItem()
{
  if (itemsets_.size() > 0)
  {
    GspItemset *firstItemSet = itemsets_.front();
    if (firstItemSet->item_count() == 1)
    {
      delete firstItemSet;
      itemsets_.pop_front();
    }
    else
      firstItemSet->remove_first_item();
  }
}

void GspSequence::DropLastItem()
{
  if (itemsets_.size() > 0)
  {
    GspItemset *lastItemSet = itemsets_.back();
    if (lastItemSet->item_count() == 1)
    {
      delete lastItemSet;
      itemsets_.pop_back();
    }
    else
      lastItemSet->remove_last_item();
  }
}

/* Documented in header */
std::string GspSequence::ToString() const
{
  std::stringstream strStream;
  unsigned size = itemsets_.size();

  for(std::list<GspItemset *>::const_iterator it = itemsets_.begin(); it != itemsets_.end(); ++it)
  {
    strStream<<(*it)->get_timestamp()<<":(";
    strStream<<(*it)->ToString();
    strStream<<")";
    if(size-- > 1)
      strStream<<",";
  }

  return strStream.str();
}

/* Operators */

/* Documented in header */
bool GspSequence::operator==(const GspSequence &other) const
{
  if (this->itemsets_.size() != other.itemsets_.size())
    return false;

  std::list<GspItemset *>::const_iterator
    leftIt = this->itemsets_.begin(),
    rightIt = other.itemsets_.begin();

  while (leftIt != this->itemsets_.end())
  {
    if (!(**leftIt == **rightIt))
      return false;
    ++leftIt;
    ++rightIt;
  }

  return true;
}

void GspSequence::AppendSequence(GspSequence *right)
{
  GspItemset *lastRight = *(--right->itemsets_.end());

  if(lastRight->item_count() == 1)
  {
    this->add_itemset(new GspItemset(*lastRight));
  }
  else
  {
    GspItemset *lastLeft = *(--this->itemsets_.end());
    lastRight->rewind();
    std::string insertItem = lastRight->current_item();
    lastLeft->add_item(insertItem);
  }
}

/*
class GspSequenceLL
  {
    private:


      std::map<std::string, std::set<GspItemset *, GspItemsetComparer> > itemMap_;
      void InsertItemset(GspItemset *itemSet);
    public:
      GspSequenceLL(GspSequence *sequence);
  };
  */

void GspSequence::CheckCandidates(int windowSize, int minGap, int maxGap)
{
  OrderedItemMap itemListMap;
  for(GspSequence::IterType it = begin(); it != end(); ++it)
  {
    for (GspItemset::IterType itemIter = (*it)->begin(); itemIter != (*it)->end(); ++itemIter)
    {
      std::string item = *itemIter;
      if (itemListMap.find(item) != itemListMap.end())
      {
        itemListMap[item].insert(*it);
      }
      else
      {
        std::set<GspItemset *, GspItemsetComparer> timestampList;
        timestampList.insert(*it);
        itemListMap[item] = timestampList;
      }
    }
  }

  std::cout<<"\nMapa:\n";
  for(OrderedItemMap::iterator mapYt = itemListMap.begin(); mapYt != itemListMap.end(); ++mapYt)
  {
    std::cout<<mapYt->first<<": ";
    for(OrderedItemsetSet::iterator setYt = mapYt->second.begin(); setYt != mapYt->second.end(); ++setYt)
      std::cout<<(*setYt)->get_timestamp()<<" ";
    std::cout<<std::endl;
  }
  std::cout<<std::endl<<std::flush;

  for(std::set<GspSequence *>::const_iterator candIter = candidates_.begin(); candIter != candidates_.end(); ++candIter)
  {
    GspSequence *candSeq = *candIter;
    std::vector<ItemSetIterators *> candItemSets;
    candItemSets.clear();
    bool created = true;
    for(GspSequence::IterType itemsetIter = candSeq->begin(); itemsetIter != candSeq->end(); ++itemsetIter)
    {
      ItemSetIterators *candStruct = new ItemSetIterators(&itemListMap, *itemsetIter, windowSize);
      if(!candStruct->init())
      {
        delete candStruct;
        created = false;
        break;
      }
      else
        candItemSets.push_back(candStruct);
    }
    int maxEl = candItemSets.size();
    if (created)
    {
      int current = 0;
      int currentMin = -1;
      int currentTimeEnd = candItemSets[current]->getMaxTime();

      bool OK = true;
      while(current != maxEl)
      {
        if(candItemSets[current]->move(currentMin))
        {
          if(candItemSets[current]->find())
          {
            if(current == 0
                || (maxGap <= 0) || (currentTimeEnd - candItemSets[current - 1]->getMinTime() <= maxGap))
            {
              currentMin = currentTimeEnd + minGap;
              ++current;
            }
            else
            {
              currentMin = candItemSets[current]->getMinTime() - maxGap;
              --current;
            }
          }
          else
          {
            OK = false;
          }
        }
        else
        {
          OK = false;
        }

        if(!OK)
          break;
      }

      if(current == maxEl)
        candSeq->increase_support();
    }
    for(int wt = 0; wt < maxEl; ++wt)
      delete candItemSets[wt];
  }
}


