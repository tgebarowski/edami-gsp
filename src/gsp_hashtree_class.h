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
    };

    /**
     * @brief Representation of a leaf in HashTree
     */
    class Leaf : Node
    {
      public:

      private:
        list<GspSequence *> sequences_; /**< Sequences stored in a leaf */
        int max_sequences_; /**< Maximum number of sequences stored in a leaf */
    };

    /**
     * @brief Representation of a interior node in HashTree
     */
    class Interior : Node
    {
      vector<Node> nodes_; /**< Nodes belonging to InteriorNode */
                           /* ? Should we replace it with map ? */
    };

    int level_; /**< HashTree maximum level */
};


#endif /* __GSP_HASHTREE_H__ */
