/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp_algorithm.cc
 *
 * @brief Generalized Sequential Patterns (GSP) algorithm implementation
 *
 * @author: Adam Szczepankiewicz <adam.szczepankiewicz@cern.ch>
 * @date: Sat Jun 9 15:30:39 2012
 */

#include <cstdlib>

#include "gsp_algorithm.h"
#include "gsp_sequence_reader.h"
#include "gsp_sequence_pool.h"

/* Documented in header */
GspAlgorithm::GspAlgorithm(GspSequenceReader *in_reader,
                           unsigned int min_support,
                           int window_size,
                           int min_gap,
                           int max_gap,
                           std::ostream *str)
  : reader_(in_reader),
    min_support_(min_support),
    window_size_(window_size),
    min_gap_(min_gap),
    max_gap_(max_gap),
    frequent_(NULL),
    candidates_(NULL),
    finished_(false),
    str_(str)
{
  std::cout << "GSP (min-support: " << ToString(min_support_)\
            << " window-size: " << ToString(window_size_)\
            << " min-gap: " << ToString(min_gap_)\
            << " max-gap: " << ToString(max_gap_) <<")"<< std::endl;

  frequent_ = new GspSequencePool(reader_.get(),
                                  this);

  if (frequent_->GetSequenceCount() == 0)
  {
    finished_ = true;
  }
}

/* Documented in header */
GspAlgorithm::~GspAlgorithm()
{
  delete frequent_;
  delete candidates_;
}

void GspAlgorithm::PrintResult()
{
  frequent_->PrintResult(*str_);
}

/* Documented in header */
void GspAlgorithm::PrintFrequentSequences()
{
  if(frequent_)
    frequent_->PrintSequences();
}

/* Documented in header */
void GspAlgorithm::PrintCandidateSequences()
{
  if(candidates_)
    candidates_->PrintSequences();
}

/* Documented in header */
void GspAlgorithm::RunPass()
{
  if (finished_)
    return;
  std::cout<<"Looking for "<<frequent_->GetK() + 1<<"-frequent sequences..."<<std::endl;
  std::cout<<"Joining the "<<frequent_->GetK()<<"-frequent set... "<<std::flush;
  candidates_ = frequent_->Join();
//  candidates_->PrintSequences();
  std::cout<<candidates_->GetSequenceCount()<<" "<<candidates_->GetK()<<"-candidates found!"<<std::endl;

  if(candidates_->GetK() > 2)
  {
    std::cout<<"Removing candidates with nonfrequent contiguous subsequences... "<<std::flush;
    candidates_->DropNonContiguous(frequent_->get_join_tree());
//    candidates_->PrintSequences();
    std::cout<<candidates_->GetSequenceCount()<<" candidates left!"<<std::endl;
    if (candidates_->GetSequenceCount() == 0)
    {
      finished_ = true;
      delete candidates_;
      candidates_ = NULL;

      return;
    }
  }

  delete frequent_;
  frequent_ = NULL;
  std::cout<<"Counting support for candidates... "<<std::flush;
  candidates_->CountSupport(reader_.get());
//  candidates_->PrintSequences();
  std::cout<<"Done!"<<std::endl;
  std::cout<<"Dropping nonfrequent candidates... "<<std::flush;
  candidates_->DropSequences();
//  candidates_->PrintSequences();
  std::cout<<candidates_->GetSequenceCount()<<" frequent "<<candidates_->GetK()<<"-sequenced found!"<<std::endl<<std::endl;

  if (candidates_->GetSequenceCount() == 0)
  {
    finished_ = true;
    delete candidates_;
    candidates_ = NULL;

    return;
  }

//  delete frequent_;
  frequent_ = candidates_;
  candidates_ = NULL;
  frequent_->PrintResult(*str_);
}

