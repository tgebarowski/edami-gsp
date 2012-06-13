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
                               int &p_min_support,
                               int &p_window_size,
                               int &p_min_gap,
                               int &p_max_gap)
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
//  std::string data_file_path;
//  int min_support;
//  int window_size;
//  int min_gap;
//  int max_gap;

  GspAlgorithm gsp(CreateSampleArrayReader(), 1, 7, 0, 30);
  std::cout<<std::endl;
  gsp.PrintFrequentSequences();
  std::cout<<std::endl;

  while(!gsp.is_finished())
    gsp.RunPass();

  std::cout<<std::endl<<std::endl<<"Result:"<<std::endl;
  gsp.PrintResult(std::cout);

  std::cout<<std::endl;



  /*
  if (ParseCommandLineArguments(argc, argv,
                                data_file_path,
                                min_support,
                                window_size,
                                min_gap,
                                max_gap))
  {
    // reader will be released by auto_ptr in gsp
    GspSequenceReader *reader = new GspFileReader(data_file_path);

    if (reader != NULL &&
        reader->IsValid())
    {
      GspAlgorithm gsp(reader, min_support, window_size,
                       min_gap, max_gap);
//

//      gsp.PrintFrequentSequences();
      std::cout << "\n\n";
//      gsp.PrintCandidateSequences();
      std::cout << "\n\n";
      gsp.RunPass();
    }
    else
    {
      std::cout << "Could not open dataset file: " << data_file_path << std::endl;
    }
  }
  */

  std::cout<<"Exiting!!";
  return 0;
}


GspArrayReader *CreateSampleArrayReader()
{
  std::vector<GspSequence *> sequences;

  GspSequence *seq = new GspSequence("C1");
  GspItemset *itSet = new GspItemset(1);
  itSet->add_item("Ringworld");
  seq->add_itemset(itSet);
  itSet = new GspItemset(2);
  itSet->add_item("Foundation");
  seq->add_itemset(itSet);
  itSet = new GspItemset(15);
  itSet->add_item("Ringworld Engineers");
  itSet->add_item("Second Foundation");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  seq = new GspSequence("C2");
  itSet = new GspItemset(1);
  itSet->add_item("Foundation");
  itSet->add_item("Ringworld");
  seq->add_itemset(itSet);
  itSet = new GspItemset(20);
  itSet->add_item("Foundation and Empire");
  seq->add_itemset(itSet);
  itSet = new GspItemset(50);
  itSet->add_item("Ringworld Engineers");
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  return new GspArrayReader(sequences);
}

