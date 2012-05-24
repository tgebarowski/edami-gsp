/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_hashtree_class.h
 *
 * @brief Hash Tree datatype class declaration
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:14:38 2012
 */

#ifndef __GSP_HASHTREE_H__
#define __GSP_HASHTREE_H__

#include "gsp_common.h"

#include <list>
#include <vector>

using namespace std;

/* Class predefinitions */
class GspSequence;

/**
 * @class GspHashTree
 *
 * @brief HashTree used to efficiently store and manage sequential patterns
 *        for the purpose of GSP algorithm
 */
class GspHashTree
{
  public:

    /**
     * @brief Constructs HashTree object
     */
    GspHashTree(int level);

    /**
     * @brief Destroys HashTree object
     */
    ~GspHashTree();

    /**
     * @brief Add GspSequence into HashTree
     *
     * @param[in] sequence Added sequence, pointer ownership belongs to this object
     *            
     * @return Status of method invocation
     *         (GSP_OK) - OK
     *         (GSP_ERROR) - Could not add 
     */
    gsp_status_t AddSequence(GspSequence *sequence);

    /**
     *
     * @brief Find complete GspSequence within Hash Tree
     *
     * @param[in] Searched GspSequence object
     *
     * @return Element stored in hash tree or NULL if not found
     */
    const GspSequence * FindSequence(GspSequence &sequence);


  private:
    
    /**
     * @brief Representation of generic item in HashTree
     */
    class Node
    {
      typedef enum 
      {
        NODE_INTERIOR = 0,
        NODE_LEAF
      } node_t;

      public:

        /** 
         * @brief Create root node
         */
        Node()
          : level_(0),
            type_(NODE_LEAF) {}

        /**
         * @brief Node constructor
         *
         * @param[in] level Level of node
         * @param[in] type Type of created node
         *                 NODE_LEAF, NODE_INTERIOR
         */
        Node(int level, node_t type)
          : level_(level) {};

        /**
         * @brief Set GspSequence stored in Node
         *
         * @param[in] sequence Pointer to GspSequence *
         */
        void SetSequence(GspSequence *sequence)
        {
          if (type_ == NODE_LEAF)
          {
            if (nodes_.size() < max_sequences_)
              sequences_.push_back(sequence);
            else
              TransformToInterior(sequence);
          }
        }

        /**
         * @brief Add new interior leaf
         */
        void AddLeaf(GspSequence *sequence)
        {
          int hash = 0; // hash(sequence, level_)
          if (nodes_[hash] == NULL)
            nodes_[hash] = new Node(++level_, NODE_LEAF);
          
          nodes_[hash]->SetSequence(sequence);          
        }

        /**
         * @brief Transform node from Leaf to Interior
         */
        void TransformToInterior(GspSequence *sequence)
        {
          if (type_ == NODE_LEAF)
          {
            type_ =  NODE_INTERIOR;

            for (list<GspSequence *>::iterator it = sequences_.begin();
                 it != sequences_.end();
                 ++it)
            {
              AddLeaf(*it);
            }
            AddLeaf(sequence);
          }
        }
        

      private:
        int level_; /**< Level of node */
        node_t type_;

        /* Leaf node only */
        list<GspSequence *> sequences_; /**< Sequences stored in a leaf */
        unsigned int max_sequences_; /**< Maximum number of sequences stored in a leaf */

        /* Interior node only */
        vector<Node *> nodes_; /**< Nodes belonging to Node */
        /* ^^^ Should we replace it with map ? */
    };

    unsigned int max_level_; /**< HashTree maximum level */
    Node * root_; /**< HashTree root */
};


#endif /* __GSP_HASHTREE_H__ */
