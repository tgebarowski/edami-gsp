/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_class.h
 *
 * @brief Sequence datatype class declaration
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:45:45 2012
 */

#ifndef __GSP_SEQUENCE_H__
#define __GSP_SEQUENCE_H__

#include <list>
#include <string>
#include <set>
#include <map>

#include "gsp_itemset_class.h"

/**
 * @class GspSequence
 *
 * @brief Class defining a sequence of itemsets
 */
class GspSequence
{
  public:
    typedef std::list<GspItemset *>::iterator IterType;
    /**
     * @brief Constructs Sequence object
     */
    GspSequence(const std::string &id_);

    /**
     * @brief Copy constructor
     *
     * @param[in] src source object
     */
    GspSequence(const GspSequence &src);

    /**
     * @brief Destroys Sequence object
     */
    ~GspSequence();

    /**
     * @brief Get N-th item from the sequence
     *
     * @param[in] n Index representing N-th item from sequence
     *
     * @return string representing N-th item content
     */
    std::string GetItemByIndex(int n);

    /**
     * @brief Compare this GspSequence (s1) with another provided as argument (s2)
     *        If a subsequence s1' obtained from s1 by droping first item
     *        is equal to a subsequence s2' obtained from s2 by droping last item
     *        then the method return TRUE.
     *        Otherwise it returns always false
     *
     * @param[in] s2 Second subsequence used for comparision
     *
     * @return True if above condition is satisfied, otherwise false
     */
    bool CompareWithSubsequence(GspSequence &s2);

    /**
     * @brief Join GspSequence represented by this object (s1)
     *        with an GspItemset provided in the argument
     *        If GspItemset is a separate element then this element
     *        will constitute to the last itemset of s1.
     *        Otherwise it will be appended as a separate itemset to s1
     *
     * @param[in] itemset GspItemset to be appended, owned by GspSequence
     */
    void AppendItemset(GspItemset *itemset);

    /**
     * @brief Get string representation of sequence
     */
    std::string ToString() const;

    /* Operators */

    /**
     * @brief Equal operator
     */
    bool operator==(const GspSequence &other) const;

    /* Inline members */

    /**
     * @brief Add itemset to the list of itemsets_
     *
     * @param[in] itemset Added itemset, to which the method
     *            owns the pointer
     */
    inline void add_itemset(GspItemset *itemset)
    {
      itemsets_.push_back(itemset);
    }

    /**
     * @brief Get last Itemset from GspSequence
     */
    inline GspItemset * get_first_itemset()
    {
      return itemsets_.front();
    }

    /**
     * @brief Get last Itemset from GspSequence
     */
    inline GspItemset * get_last_itemset()
    {
      return itemsets_.back();
    }

    void rewind()
    {
      iter_ = itemsets_.begin();
    }

    void rewind_all()
    {
      rewind();
      for(std::list<GspItemset *>::const_iterator it = itemsets_.begin(); it != itemsets_.end(); ++it)
        (*it)->rewind();
    }

    bool next_itemset()
    {
      ++iter_;

      if (iter_ != itemsets_.end())
      {
        (*iter_)->rewind();
        return true;
      }

      return false;
    }

    GspItemset *current_itemset()
    {
      if (iter_ == itemsets_.end())
        return NULL;

      return *iter_;
    }

    IterType begin()
    {
      return itemsets_.begin();
    }

    IterType end()
    {
      return itemsets_.end();
    }

    void increase_support()
    {
      ++support_;
    }

    unsigned get_support()
    {
      return support_;
    }

    std::string get_id()
    {
      return id_;
    }

    void set_support(unsigned support)
    {
      support_ = support;
    }

    GspSequence *JoinSequences(GspSequence *right);

    void DropFirstItem();

    void DropLastItem();

    void AppendSequence(GspSequence *right);

    void CheckCandidates(int windowSize, int minGap, int maxGap);

    inline void add_candidate_sequence(GspSequence *toAdd)
    {
      candidates_.insert(toAdd);
    }

    inline void print_candidates()
    {
      for (std::set<GspSequence *>::iterator it = candidates_.begin(); it != candidates_.end(); ++it)
      {
        std::cout<<(*it)->ToString()<<std::endl;
      }
    }

  private:
    std::list<GspItemset *> itemsets_; /**< List of itemsets */
    std::list<GspItemset *>::const_iterator iter_;
    std::string id_;
    unsigned support_;
    std::set<GspSequence *> candidates_;

    class GspItemsetComparer
    {
      public:
        bool operator()(GspItemset *a, GspItemset *b)
        {
          return a->get_timestamp() < b->get_timestamp();
        }
    };

    typedef std::set<GspItemset *, GspItemsetComparer> OrderedItemsetSet;
    typedef std::map<std::string, OrderedItemsetSet> OrderedItemMap;

    class GspItemIterator
    {
      private:
        OrderedItemsetSet *itemList_;
        OrderedItemsetSet::const_iterator iter_;
      public:
        class Comparer
        {
          public:
            bool operator()(GspItemIterator *a, GspItemIterator *b)
            {
              return (*(a->iter_))->get_timestamp() < (*(b->iter_))->get_timestamp();
            }
        };

        GspItemIterator(OrderedItemsetSet *itemList)
          : itemList_(itemList), iter_(itemList_->begin())
        {
        }

        bool is_valid()
        {
          return iter_ != itemList_->end();
        }

        int get_timestamp()
        {
          return (*iter_)->get_timestamp();
        }

        bool next()
        {
          ++iter_;
          return is_valid();
        }

        bool move(int time = -1)
        {
          while(iter_ != itemList_ -> end())
          {
            if (get_timestamp() > time)
              return true;
            ++iter_;
          }
          return false;
        }
    };

    class ItemSetIterators
    {
      private:
        typedef std::set<GspItemIterator *, GspItemIterator::Comparer>::iterator IterType;
        std::set<GspItemIterator *, GspItemIterator::Comparer> items_;
        OrderedItemMap *itemMap_;
        GspItemset *itemSet_;
        int windowSize_;

      public:
        ItemSetIterators(OrderedItemMap *itemMap, GspItemset *itemSet, int windowSize)
          : itemMap_(itemMap), itemSet_(itemSet), windowSize_(windowSize)
        {
        }

        ~ItemSetIterators()
        {
          for(IterType it = items_.begin(); it != items_.end();)
          {
            delete *it++;
          }
        }

        bool init()
        {
          for(GspItemset::IterType it = itemSet_->begin(); it != itemSet_->end(); ++it)
          {
            std::string item = *it;
            OrderedItemMap::iterator mapIt = itemMap_->find(item);
            if(mapIt == itemMap_->end())
            {
              return false;
            }

            items_.insert(new GspItemIterator(&(mapIt->second)));

          }

          return true;
        }

        bool move(int val)
        {
          while(1)
          {
            IterType first = items_.begin();
            GspItemIterator *firstItem = *first;
            if(firstItem->get_timestamp() > val)
              break;;
            items_.erase(first);
            if(firstItem->move(val))
              items_.insert(firstItem);
            else
            {
              delete firstItem;
              return false;
            }
          }

          return true;
        }

        bool find()
        {
          while(1)
          {
            int maxTime = (*(--items_.end()))->get_timestamp();
            IterType first = items_.begin();
            GspItemIterator *firstItem = *(first);
            if (maxTime - firstItem->get_timestamp() <= windowSize_)
              break;
            items_.erase(first);
            if(firstItem->next())
              items_.insert(firstItem);
            else
            {
              delete firstItem;
              return false;
            }
          }

          return true;
        }

        int getMinTime()
        {
          return (*items_.begin())->get_timestamp();
        }

        int getMaxTime()
        {
          return (*(--items_.end()))->get_timestamp();
        }
    };
};

#endif /* __GSP_SEQUENCE_H__ */
