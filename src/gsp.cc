/* -*- mode: cc-mode; tab-width: 2; -*- */

/**
 * @file  gsp.cc
 *
 * @brief Generalized Sequential Patterns (GSP) application
 *
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Fri May 18 15:30:39 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "gsp_array_reader.h"
#include "gsp_file_reader_class.h"
#include "gsp_sequence_class.h"
#include "gsp_algorithm.h"

/* Predefinitiosn */
GspArrayReader *CreateSampleArrayReader();


/**
 * @brief Parse command line arguments
 */
bool ParseCommandLineArguments(int argc, char *argv[],
                               std::string &p_data_file_path,
                               unsigned int &p_min_support,
                               unsigned int &p_window_size,
                               unsigned int &p_min_gap,
                               unsigned int &p_max_gap)
{
  bool ret = false;

  if (argc == 6)
  {
    p_data_file_path = std::string(argv[1]);
    p_min_support = atoi(argv[2]);
    p_window_size = atoi(argv[3]);
    p_min_gap = atoi(argv[4]);
    p_max_gap = atoi(argv[5]);
    ret = true;
  }
  else
  {
    std::cout << "Invalid input arguments" << std::endl;
    std::cout << "Usage: " << std::string(argv[0]) << " "                     \
              << "[data-file] [min_support] [window_size] [min_gap] [max_gap]"\
              << std::endl << std::endl;
    std::cout << "Example: " << std::string(argv[0]) << " "                     \
              << "datasets/dataset-1k.dat 2 2 1 5"\
              << std::endl;
  }
  return ret;
}


int main(int argc, char *argv[])
{
  std::string data_file_path;
  unsigned int min_support;
  unsigned int window_size;
  unsigned int min_gap;
  unsigned int max_gap;

  if (ParseCommandLineArguments(argc, argv,
                                data_file_path,
                                min_support,
                                window_size,
                                min_gap,
                                max_gap))
  {
    GspSequenceReader *reader = new GspFileReader(data_file_path);

    if (reader != NULL &&
        reader->IsValid())
    {
      GspAlgorithm gsp(reader, min_support, window_size,
                       min_gap, max_gap);      

      gsp.printFrequentSeqs();
      std::cout << "\n\n";
      gsp.printCandidateSeqs();
      std::cout << "\n\n";
      gsp.runPass();
    }
    else
    {
      std::cout << "Could not open dataset file: " << data_file_path << std::endl;
    }
    delete reader;
  }
  return 0;
}


GspArrayReader *CreateSampleArrayReader()
{
  std::vector<GspSequence *> sequences;

  GspSequence *seq = new GspSequence("C1");
  GspItemset *itSet = new GspItemset(1);
  itSet->add_item("R");
  seq->add_itemset(itSet);
  itSet = new GspItemset(2);
  itSet->add_item("F");
  seq->add_itemset(itSet);
  itSet = new GspItemset(15);
  itSet->add_item("S");
  itSet->add_item("T");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  seq = new GspSequence("C2");
  itSet = new GspItemset(1);
  itSet->add_item("F");
  itSet->add_item("R");
  seq->add_itemset(itSet);
  itSet = new GspItemset(20);
  itSet->add_item("G");
  seq->add_itemset(itSet);
  itSet = new GspItemset(50);
  itSet->add_item("S");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  return new GspArrayReader(sequences);
}

