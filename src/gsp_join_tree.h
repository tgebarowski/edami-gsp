
#ifndef GSP_JOIN_TREE_H_
#define GSP_JOIN_TREE_H_

#include <map>
#include <list>
#include <string>

#include "gsp_sequence_class.h"


/**
 * @class GspJoinTree
 *
 * @brief Class providing a suffix tree representation of the sequence set.
 */
class GspJoinTree
{
  public:
    /**
     * @brief Constructs an empty tree
     */
    GspJoinTree();
    /**
     * @brief Destroys a tree
     */
    ~GspJoinTree();

    /**
     * @brief insert a sequence into the tree
     */
    void AddSequence(GspSequence *seq);

    /**
     * @brief finds all joinable sequences of a passed sequence
     */
    void FindJoinable(GspSequence *seq, std::list<GspSequence *> &result);

    /**
     * @brief Checks if a sequence is in the tree
     */
    bool FindSequence(GspSequence *seq);

  private:
    /**
     * @Class Internal node of a tree
     */
    class Node
    {
      private:
        class GspItemSetrPtrComparer
        {
          public:
            bool operator()(const GspItemset *left, const GspItemset *right)
            {
              return (*left) < (*right);
            }
        };

        std::map<GspItemset *, Node *, GspItemSetrPtrComparer> *nodes_;
        GspSequence *seq_;

      public:
        /**
         * @brief Construct a node
         */
        Node() : nodes_(NULL), seq_(NULL)
        {
        }
        /**
         * @brief Destroy a node and all its subnodes
         */
        ~Node()
        {
          if (nodes_)
            for(std::map<GspItemset *, Node *, GspItemSetrPtrComparer>::iterator it = nodes_->begin(); it != nodes_->end(); ++it)
              delete it->second;
          delete nodes_;
        }

        /**
         * @brief Recursively traverse tree and insert a sequence where it belongs
         */
        void AddSequence(GspSequence *seq, GspSequence::IterType &iter);

        /**
         * @brief Recursively traverse a tree and find all the sequences that can be joined with the passed sequence
         */
        void FindJoinable(GspSequence::IterType current, GspSequence::IterType &final, std::list<GspSequence *> &result);

        /**
         * @brief Checks if the sequence is in the tree
         */
        bool FindSequence(GspSequence *seq, GspSequence::IterType &iter);

        /**
         * @brief For joining phase - adds all sequences ending with a single-item element
         */
        void Add1AtEnd(std::list<GspSequence *> &result);
    };

    Node *root_;
};

#endif /* GSP_JOIN_TREE_H_ */
