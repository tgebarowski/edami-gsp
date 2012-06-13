/**
 * @file  gsp_algorithm.h
 *
 * @brief Generalized Sequential Patterns (GSP) algorithm implementation
 *
 * @author: Adam Szczepankiewicz <adam.szczepankiewicz@cern.ch>
 * @date: Sat Jun 9 15:30:39 2012
 */


#ifndef GSP_ALGORITHM_H_
#define GSP_ALGORITHM_H_

#include <memory>
#include <fstream>

#include "gsp_sequence_reader.h"
#include "gsp_sequence_pool.h"

/**
 * @brief Class Implementing GSP Algorithm interface
 */
class GspAlgorithm
{
  public:
    /**
     * @brief Construct the class with algorithm parameters
     */
    GspAlgorithm(GspSequenceReader *in_reader,
                 unsigned int min_support,
                 int window_size,
                 int min_gap,
                 int max_gap);

    /**
     * @brief Destroy the object
     */
    ~GspAlgorithm();

    /**
     * @brief Print the current frequent set into the specified stream
     */
    void PrintResult(std::ostream &str);

    /**
     * @brief Get number of Frequent Sequences
     *
     * @return integer representing frequent sequences count
     */
    unsigned int get_frequent_count()
    {
      if (frequent_)
        return frequent_->GetSequenceCount();
      return 0;
    }

    /**
     * @brief Get number of Candidate Sequences
     *
     * @return integer representing candidate sequences count
     */
    unsigned int get_candidate_count()
    {
      if (candidates_)
        return candidates_->GetSequenceCount();
      return 0;
    }

    /**
     * @brief Print all Frequent Sequences found 
     */
    void PrintFrequentSequences();

    /**
     * @brief Print all Candidate Sequences found 
     */
    void PrintCandidateSequences();

    /**
     * @brief Invoke one pass of GSP algorithm logic
     */
    void RunPass();

    /**
     * @brief Has algorithm finished_
     */
    bool is_finished()
    {
      return finished_;
    }

    /* Getters */

    inline unsigned int min_support()
    {
      return min_support_;
    }

    inline int window_size()
    {
      return window_size_;
    }

    inline int min_gap()
    {
      return min_gap_;
    }

    inline int max_gap()
    {
      return max_gap_;
    }

  private:

    std::auto_ptr<GspSequenceReader> reader_;
    unsigned int min_support_; /**< Min support of sequence to consider as frequent */
    int window_size_; /**< Window size - see GSP algorithm paper */
    int min_gap_; /**< Min gap - see GSP algorithm paper */
    int max_gap_; /** <Max gap - see GSP algorithm paper */

    GspSequencePool *frequent_; /**< Pool with frequent sequences */
    GspSequencePool *candidates_; /**< Pool with candidate sequences */
 
    bool finished_; /**< Has algorith finished */
};

#endif /* GSP_ALGORITHM_H_ */
