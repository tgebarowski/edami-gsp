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

#include <list>
#include <vector>

#define MAX_NODES 1009 /* Maximum number of Nodes pointed by Internal node */

#include "gsp_common.h"
#include "gsp_sequence_class.h"


/* Class predefinitions */
class GspAlgorithm;

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
     *
     * @param[in] level Maximum number of levels
     * @param[in] max_leaf Maximum number of elements in a Leaf
     */
    GspHashTree(int level,
                int max_leaf);

    /**
     * @brief Destroys HashTree object
     */
    ~GspHashTree();

    /**
     * @brief Add GspSequence into HashTree
     *
     * @param[in] sequence Added sequence, pointer ownership belongs to this object
     *            gsp_status_h_
     * @return Status of method invocation
     *         (GSP_OK) - OK
     *         (GSP_ERROR) - Could not add 
     */
    enum GSP_STATUS AddSequence(GspSequence *sequence);
    
    /**
     * @brief Hash function converting 
     *
     * @param[in] item the item to calculate the hash by summing all its positions
     * @param[in[ upper_bound Upper boundary for hash value
     */
    static int Hash(const std::string &item, int upper_bound);

    /**
     * @brief Prints the representation of the tree to the standard output
     *
     */
    void PrintTree()
    {
      root_->PrintNode();
    }

    /**
     * @brief Finds the candidate sequences for the client sequence
     *
     * @param[in] seq the client sequence
     * @param[in[ parent the class containing the parameters of the GSP algorithm
     */
    void CheckClientSequence(GspSequence *seq,
                             GspAlgorithm *parent);

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
         Node(unsigned int max_leaf,
              unsigned int max_level)
          : level_(0),
            type_(NODE_LEAF),
            max_sequences_(max_leaf),
            max_level_(max_level)
         {
           for(int it = 0; it < MAX_NODES; ++it)
             nodes_[it] = NULL;
         }

         /**
          * @brief Print node information
          */
         void PrintNode()
         {
           if (type_ == NODE_LEAF)
           {
             std::cout<<"Leaf node, level: "<<level_<<":"<<std::endl;
             for (std::list<GspSequence *>::iterator it = sequences_.begin();
                  it != sequences_.end(); ++it)
             {
               std::cout << (*it)->ToString() << " " << (*it)->get_support()<<std::endl;
             }
             std::cout<<std::endl;
           }
           else
           {
             std::cout<<"Interior node, level: "<<level_<<":"<<std::endl;
             for(int it = 0; it < MAX_NODES; ++it)
             {
               std::cout<<"Child "<<it<<":"<<std::endl;
               nodes_[it]->PrintNode();
             }
             std::cout << std::endl<<"END Interior node, level: "
                       << level_<<std::endl<<std::endl;
           }
         }

        /**
         * @brief Node constructor
         *
         * @param[in] level Level of node
         * @param[in] type Type of created node
         *                 NODE_LEAF, NODE_INTERIOR
         *
         * @param[in] max_leaf Maximum number of sequences 
         *            stored in a Leaf before transforming to Interior node
         *
         * @param[in] max_level Maximum level of Node, when exceeded
         *            no more transforming is realized, instead element list
         *            may exceed max_sequences_ size
         */
         Node(int level, node_t type, int max_leaf, int max_level)
           : level_(level),
             type_(type),
             max_sequences_(max_leaf),
             max_level_(max_level)
         {
           for(int it = 0; it < MAX_NODES; ++it)
             nodes_[it] = NULL;
         }

         /**
          * @brief Node destructor
          */
         ~Node()
         {
           for(int it = 0; it < MAX_NODES; ++it)
             delete nodes_[it];
         }

         /**
          * @brief A recursive procedure that traverses the tree depth first to
          * find the candidate sequences
          *
          * @param[in] seq Pointer to client sequence
          * @param[in] itemSet the currently processed itemset
          * @param[in] itemSetIter the iterator to the current itemset in the sequence
          * @param[in] itemIter the iterator to current item in itemset
          * @param[in] parent algorithm parameters (sliding window, max gap, min gap)
          */
         void CheckClientSequence(GspSequence *seq,
                                  GspItemset *itemSet,
                                  GspSequence::IterType itemSetIter,
                                  GspItemset::IterType itemIter,
                                  GspAlgorithm *parent);

        /**
         * @brief Set GspSequence stored in Node
         *
         * @param[in] sequence Pointer to GspSequence *
         */
        void SetSequence(GspSequence *sequence)
        {
          //if leaf
          if (type_ == NODE_LEAF)
          {
            //std::cout << "Leaf - Level: " << level_
   //                   << " Sequence: "<<sequence->ToString()<<std::endl;
            //add sequence
            sequences_.push_back(sequence);
            if (sequences_.size() > max_sequences_ &&
                level_ < max_level_)
            {
              //if the number of sequences is bigger then specified, transform the node to interior
              TransformToInterior();
            }
          }
          else
          {
            //if interior node, check whick child node to follow

            //std::cout << "Interior - Level: " << level_
    //                  << " Sequence: "<<sequence->ToString();
            GspItemset *curItemSet = sequence->current_itemset();
            std::string curItem = curItemSet->current_item();
            if(!curItemSet->next_item())
              sequence->next_itemset();

            int hash = Hash(curItem, MAX_NODES);
            //std::cout << " Itemset: "<<curItemSet->ToString()
        //              << " Item: " <<curItem<< " Hash: "<<hash<<std::endl;
            nodes_[hash]->SetSequence(sequence);
          }
        }

        /**
         * @brief Transform node from Leaf to Interior
         */
        void TransformToInterior()
        {
          //std::cout << std::endl
    //                <<  "Transforming from leaf to interior at level: "
     //               << level_ << std::endl;
          if (type_ == NODE_LEAF)
          {
            type_ =  NODE_INTERIOR;

            for(int it = 0; it < MAX_NODES; ++it)
            {
              nodes_[it] = new Node(level_ + 1, NODE_LEAF, max_sequences_, max_level_);
            }

            for (std::list<GspSequence *>::iterator it = sequences_.begin();
                 it != sequences_.end();
                 ++it)
            {
              GspItemset *curItemSet = (*it)->current_itemset();
              //std::cout<<(*it)->ToString();
              //std::cout<<" A"<<std::flush;
              std::string curItem = curItemSet->current_item();
              //std::cout<<" B"<<std::flush;
              if(!curItemSet->next_item())
              {
                //std::cout<<" C"<<std::flush;
                (*it)->next_itemset();
              }
              //std::cout<<" D"<<std::endl;

              int hash = Hash(curItem, MAX_NODES);
              //std::cout << "Interior - Level: " << level_
          //              <<" Sequence: "<<(*it)->ToString()
           //             <<" Itemset: "<<curItemSet->ToString()
            //            <<" Item: "<<curItem<< " Hash: "<<hash<<std::endl;
              nodes_[hash]->SetSequence((*it));
            }
            sequences_.clear();
          }
          //std::cout << "Transforming done for level "<< std::endl<<std::endl;
        }

        /**
         * @brief Clear the history of visits before the new sequence
         */
        void clear_history()
        {
          if (type_ ==  NODE_INTERIOR)
          {
            for(int it = 0; it < MAX_NODES; ++it)
              nodes_[it]->clear_history();
          }
          visitedSet.clear();
        }

      private:
        unsigned int level_; /**< Level of node */
        node_t type_;

        /* Leaf node only */
        std::list<GspSequence *> sequences_; /**< Sequences stored in a leaf */
        unsigned int max_sequences_; /**< Maximum number of sequences stored in leaf */
        unsigned int max_level_; /**< Maximum level of node */

        /* Interior node only */
        Node *nodes_[MAX_NODES]; /**< Nodes belonging to Node */
        std::set<std::pair<std::string, int> > visitedSet; //history of items that visited the node
    };

    Node * root_; /**< HashTree root */
    unsigned int max_level_; /**< HashTree maximum level */
    unsigned int max_leaf_; /**< Max elements in a leaf */
};


#endif /* __GSP_HASHTREE_H__ */
