/*
 * gsp_join_tree.h
 *
 *  Created on: Jun 14, 2012
 *      Author: adas
 */

#ifndef GSP_JOIN_TREE_H_
#define GSP_JOIN_TREE_H_

#include <map>
#include <list>
#include <string>

#include "gsp_sequence_class.h"

class GspJoinTree
{
  public:
    GspJoinTree();
    ~GspJoinTree();

    void AddSequence(GspSequence *seq);
    void FindJoinable(GspSequence *seq, std::list<GspSequence *> *&result);

  private:
    class Node
    {
      private:
        std::map<std::string, Node *> *nodes_;
        std::list<GspSequence *> *sequences_;

      public:
        Node() : nodes_(NULL), sequences_(NULL)
        {
        }

        ~Node()
        {
          delete sequences_;
          if (nodes_)
            for(std::map<std::string, Node *>::iterator it = nodes_->begin(); it != nodes_->end(); ++it)
              delete it->second;
          delete nodes_;
        }

        void AddSequence(GspSequence *seq, GspSequence::IterType iter);
        void FindJoinable(GspSequence::IterType &current, GspSequence::IterType &final, std::list<GspSequence *> *&result);
    };

    Node *root_;
};

#endif /* GSP_JOIN_TREE_H_ */
