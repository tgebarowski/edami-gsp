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

class GspJoinTree;

/**
 * @class GspSequence
 *
 * @brief Class defining a sequence of itemsets. It provides an internal
 * iterator over itemsets. It also provides methods for gettin the iterators
 * to the first and last itemsets. It provides representation for both the
 * candidate sequence and a client sequence (internal set of candidate sequences)
 */
class GspSequence
{
  public:
    /**
     * @brief Iterator type for the GspSequence class
     */
    typedef std::list<GspItemset *>::iterator IterType;
    /**
     * @brief Constructs Sequence object
     */
    GspSequence(int id_);

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
     * @brief Get string representation of sequence
     */
    std::string ToString() const;

    /**
     * @brief Equal operator
     */
    bool operator==(const GspSequence &other) const;

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
     * @brief Get first Itemset from GspSequence
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

    /**
     * @brief Set the internal iterator to the first itemset of a sequence
     */
    void rewind()
    {
      iter_ = itemsets_.begin();
    }

    /**
     * @brief Set the internal iterator to the first itemset of a sequence. Also
     * rewind all the itemsets in the sequence
     */
    void rewind_all()
    {
      rewind();
      for(std::list<GspItemset *>::const_iterator it = itemsets_.begin(); it != itemsets_.end(); ++it)
        (*it)->rewind();
    }

    /**
     * @brief Tries to move the internal iterator to the next itemset. Returns
     * a value indicating if the operation was correct
     */
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

    /**
     * @brief Returns the itemset class pointed by the internal iterator
     */
    GspItemset *current_itemset()
    {
      if (iter_ == itemsets_.end())
        return NULL;

      return *iter_;
    }

    /**
     * @brief Returns the iterator to the first itemset
     */
    IterType begin()
    {
      return itemsets_.begin();
    }

    /**
     * @brief Returns the iterator to the last itemset
     */
    IterType end()
    {
      return itemsets_.end();
    }

    void increase_support()
    {
      ++support_;
    }

    /**
     * @brief Returns the support value for a sequence
     */
    unsigned get_support()
    {
      return support_;
    }

    /**
     * @brief Returns the sequence identifier
     */
    int get_id()
    {
      return id_;
    }

    /**
     * @brief Sets the sequence support to a given value
     */
    void set_support(unsigned support)
    {
      support_ = support;
    }

    /**
     * @brief Attempt to join the two sequences according to the GSP algorithm
     * procedure
     */
    GspSequence *JoinSequences(GspSequence *right);

    /**
     * @brief Removes the first item in the sequence
     */
    void DropFirstItem();

    /**
     * @brief Removes the first item in the sequence
     */
    void DropLastItem();

    /**
     * @brief Appends the original sequence with the last item in the sequence
     * right according to the GSP algorithm procedure
     */
    void AppendSequence(GspSequence *right);

    /**
     * @brief Checks if the candidate sequences stored in candidates_ are
     * supported by the sequence
     */
    void CheckCandidates(int windowSize, int minGap, int maxGap);

    /**
     * @brief Adds a candidate sequence
     */
    inline void add_candidate_sequence(GspSequence *toAdd)
    {
      candidates_.insert(toAdd);
    }

    /**
     * @brief Prints the candidate sequences for this sequence.
     */
    inline void print_candidates()
    {
      for (std::set<GspSequence *>::iterator it = candidates_.begin(); it != candidates_.end(); ++it)
      {
        std::cout<<(*it)->ToString()<<std::endl;
      }
    }

    bool ContiguousCheck(GspJoinTree *joinTree);

    int get_itemset_count()
    {
      return itemsets_.size();
    }

  private:
    std::list<GspItemset *> itemsets_; /**< List of itemsets */
    std::list<GspItemset *>::const_iterator iter_;
    int id_;
    unsigned support_;
    std::set<GspSequence *> candidates_;

    /**
     * @brief Comparator class for two itemsets. Orders them by their timestamp
     */
    class GspItemsetTimeComparer
    {
      public:
        bool operator()(GspItemset *a, GspItemset *b)
        {
          return a->get_timestamp() < b->get_timestamp();
        }
    };

    /**
     * @brief Type representing a sorted list of itemsets ordered by their
     * timestamps. It is used to determine appearances of items in a sequence
     */
    typedef std::set<GspItemset *, GspItemsetTimeComparer> OrderedItemsetSet;

    /**
     * @brief Type used to build a representation of a sequence containing items
     * as keys and an orderel list of timestamps at which those items appear
     * in the sequence
     */
    typedef std::map<int, OrderedItemsetSet> OrderedItemMap;

    /**
     * @brief Class representing an item in a client sequence during the process
     * of subsequence matching. It points to an itemset in a client sequence
     * in which this item is contained
     */
    class GspItemIterator
    {
      private:
        OrderedItemsetSet *itemList_;
        OrderedItemsetSet::const_iterator iter_;
        int item_;
        int currentTime_;
      public:
        /**
         * @brief Class used to compare the GspItemIterator objects in a set
         * to order them by the time of their appearances in a client sequence
         */
        class Comparer
        {
          public:
            bool operator()(GspItemIterator *a, GspItemIterator *b)
            {
              if (a->currentTime_ == b->currentTime_)
                return (a->item_ < b->item_);
              return (a->currentTime_ < b->currentTime_);
            }
        };

        /**
         * @brief Construct the OrderedItemsetSet pointing to the first appearance
         * of a corresponding item in the client sequence
         *
         * @param[in] itemList A list of sorted timestamps of appearances of the item
         * in the client sequence
         */
        GspItemIterator(OrderedItemsetSet *itemList, int item)
          : itemList_(itemList), iter_(itemList_->begin()), item_(item), currentTime_((*iter_)->get_timestamp())
        {
        }

        int get_item()
        {
          return item_;
        }

        /**
         * @brief Method tells if the object points to a valid item
         */
        bool is_valid()
        {
          return iter_ != itemList_->end();
        }

        /**
         * @brief Tries to move the pointer to the next appearance of the item
         * in the client sequence.
         */
        bool next()
        {
          bool ret;
          ++iter_;
          ret = is_valid();
          if(ret)
            currentTime_ = (*iter_)->get_timestamp();

          return ret;
        }

        int get_time()
        {
          return currentTime_;
        }

        /**
         * @brief Tries to move the pointer to the first appearance of item after
         * the specified timestemp
         */
        bool move(int time = -1)
        {
          while(iter_ != itemList_ -> end())
          {
            currentTime_ = (*iter_)->get_timestamp();
            if (currentTime_ > time)
              return true;
            ++iter_;
          }
          return false;
        }
    };

    /**
     * @brief Class representing the itemset during the process of subsequence
     * mathing in the GSP algorithm. It stores the GspItemIterator pointers for
     * all items in the itemset. It provides the mechanism to move all the items
     * in the itemset past the specified minimal time and to find the next mathing
     * items in the specified sliding window
     */
    class ItemSetIterators
    {
      private:
        typedef std::set<GspItemIterator *, GspItemIterator::Comparer>::iterator IterType;
        std::set<GspItemIterator *, GspItemIterator::Comparer> items_;
        OrderedItemMap *itemMap_;
        GspItemset *itemSet_;
        int windowSize_;

      public:
        /**
         * @brief Constructs the object for the specified itemset and a client
         * sequence list representation
         */
        ItemSetIterators(OrderedItemMap *itemMap, GspItemset *itemSet, int windowSize)
          : itemMap_(itemMap), itemSet_(itemSet), windowSize_(windowSize)
        {
        }

        /**
         * @brief Destroys the object
         */
        ~ItemSetIterators()
        {
          for(IterType it = items_.begin(); it != items_.end();)
          {
            delete *it++;
          }
        }

        void print_content()
        {
          for(IterType it = items_.begin(); it != items_.end(); ++it)
            std::cout<<"("<<(*it)->get_item()<<"|"<<(*it)->get_time()<<")";
        }

        /**
         * @brief Tries to initialize the item pointer values to their first
         * appearances in a client sequence. It fails if at least one of the items is
         * not at all representet
         */
        bool init()
        {
          for(GspItemset::IterType it = itemSet_->begin(); it != itemSet_->end(); ++it)
          {
            int item = *it;
            OrderedItemMap::iterator mapIt = itemMap_->find(item);
            if(mapIt == itemMap_->end())
            {
              return false;
            }

            items_.insert(new GspItemIterator(&(mapIt->second), item));
          }

//          std::cout<<std::endl;

          return true;
        }

        /**
         * @brief Tries to move all the items in an itemset past the specified
         * timestamp. It fails if at least one of the items does not appear in
         * the client sequence after the specified time
         */
        bool move(int val)
        {
          while(1)
          {
            IterType first = items_.begin();
            GspItemIterator *firstItem = *first;
            int tStamp = firstItem->get_time();
//            std::string item = firstItem->get_item();
//            print_content();
//            std::cout<<" curr:  "<<item<<" "<<tStamp<<" "<<val<<std::endl;
            if(tStamp > val)
              break;
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

        /**
         * @brief Tries to find a sequence in a specified sliding window in a
         * client sequence
         */
        bool find()
        {
          while(1)
          {
            int maxTime = (*(--items_.end()))->get_time();
            IterType first = items_.begin();
            GspItemIterator *firstItem = *(first);
            int tStamp = firstItem->get_time();
//            std::string item = firstItem->get_item();
//            print_content();
//            std::cout<<" curr:  "<<item<<" "<<tStamp<<" "<<std::endl;
            if (maxTime - tStamp <= windowSize_)
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

        /**
         * @brief Gets the start time of a matched itemset
         */
        int getMinTime()
        {
          return (*items_.begin())->get_time();
        }

        /**
         * @brief Gets the end time of a matched itemset
         */
        int getMaxTime()
        {
          return (*(--items_.end()))->get_time();
        }
    };
};

#endif /* __GSP_SEQUENCE_H__ */
