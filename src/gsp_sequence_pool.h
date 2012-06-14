/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_sequence_pool.h
 *
 * @brief GSP Sequence Pool
 *
 * @author: Adam Szczepankiewicz <adam.szczepankiewicz@cern.ch>
 * @date: Sat Jun 9 15:30:39 2012
 */

#ifndef GSP_SEQUENCE_POOL_H_
#define GSP_SEQUENCE_POOL_H_

#include <list>
#include <fstream>

#include "gsp_itemset_class.h"
#include "gsp_sequence_class.h"

/* Predefinitions */
class GspSequenceReader;
class GspAlgorithm;

/**
 * @class GspSequencePool
 *
 * @brief Class representing the set of k-sequences (k-item length). It performs
 * the steps of the GSP algorithm
 */
class GspSequencePool
{
  public:
    /**
     * @brief Constructs the empty pool with the parent class containing the algorithm parameters
     *
     */
    GspSequencePool(GspAlgorithm *parent);

    /**
     * @brief Constructs the 1-length frequent sequences basing on the data source provided
     *
     */
    GspSequencePool(GspSequenceReader *reader,
                    GspAlgorithm *parent);

    /**
     * @brief Destroys the pool
     *
     */
    ~GspSequencePool();

    /**
     * @brief Get the length of stored sequences
     *
     */
    unsigned int GetK()
    {
      return k_;
    }

    /**
     * @brief Get the number of sequences in the pool
     *
     */
    unsigned int GetSequenceCount()
    {
      return sequences_.size();
    }

    /**
     * @brief Join the pool with itself to create the (k+1)-length candidates
     *
     */
    GspSequencePool *Join();

    /**
     * @brief Count the support of the candidates using the specified data source
     *
     */
    void CountSupport(GspSequenceReader *reader);

    /**
     * @brief Drop the sequences with the support below minimum
     *
     */
    void DropSequences();

    /**
     * @brief Print the sequences to the standard output
     *
     */
    void PrintSequences();

    /**
     * @brief Print the sequences to the specified stream
     *
     */
    void PrintResult(std::ostream &str);

    std::set<std::string> *ToStringSet();

    void DropNonContiguous(std::set<std::string> *stringSet);

  private:

    GspAlgorithm *parent_;
    std::list<GspSequence *> sequences_;
    unsigned int k_;
};

#endif /* GSP_SEQUENCE_POOL_H_ */
