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
#include <ctime>

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
                               std::string &p_output_file_path,
                               int &p_min_support,
                               int &p_window_size,
                               int &p_min_gap,
                               int &p_max_gap)
{
  bool ret = false;

  if (argc == 7)
  {
    p_data_file_path = std::string(argv[1]);
    p_output_file_path = std::string(argv[2]);
    p_min_support = atoi(argv[3]);
    p_window_size = atoi(argv[4]);
    p_min_gap = atoi(argv[5]);
    p_max_gap = atoi(argv[6]);
    ret = true;
  }
  else
  {
    std::cout << "Invalid input arguments" << std::endl;
    std::cout << "Usage: " << std::string(argv[0]) << " "                     \
              << "[data-file] [output-file] [min_support] [window_size] [min_gap] [max_gap]"\
              << std::endl << std::endl;
    std::cout << "Example: " << std::string(argv[0]) << " "                     \
              << "datasets/dataset-1k.dat output.dat 2 2 1 5"\
              << std::endl;
  }
  return ret;
}


int main(int argc, char *argv[])
{
  std::string data_file_path;
  std::string out_file_path;
  int min_support;
  int window_size;
  int min_gap;
  int max_gap;

  int startClk = std::clock();

  if (ParseCommandLineArguments(argc, argv,
                                data_file_path,
                                out_file_path,
                                min_support,
                                window_size,
                                min_gap,
                                max_gap))
  {
    // reader will be released by auto_ptr in gsp
    ofstream myfile;
    myfile.open (out_file_path.c_str());

    if(!myfile.is_open())
    {
      std::cout<<"Cannot open output file... Exiting..."<<std::endl;

      return 1;
    }

    GspSequenceReader *reader = new GspFileReader(data_file_path);

    if (reader != NULL &&
        reader->IsValid())
    {
      GspAlgorithm gsp(reader, min_support, window_size,
                       min_gap, max_gap, &myfile);
      std::cout<<"Looking for 1-frequent sequences... "<<std::endl;
      std::cout<<gsp.get_frequent_count()<<" frequent 1-sequences found!"<<std::endl<<std::endl;
      gsp.PrintResult();

      if(!gsp.is_finished())
      {
        while(1)
        {
          gsp.RunPass();
          if(gsp.is_finished())
          {
            break;
          }
        }
      }

      std::cout<<std::endl<<"Writing output... ";
      myfile.flush();
      std::cout<<"Done! "<<std::endl;
    }
    else
    {
      std::cout << "Could not open dataset file: " << data_file_path << std::endl;
    }
  }

  int endClk = std::clock();
  float runTime = ((float)(endClk - startClk))/CLOCKS_PER_SEC;

  std::cout<<runTime<<" seconds elapsed"<<std::endl;
  std::cout<<"Exiting..."<<std::endl;

  return 0;
}

GspArrayReader *CreateSampleArrayReader()
{
  std::vector<GspSequence *> sequences;

  GspSequence *seq = new GspSequence(-1);
  GspItemset *itSet = new GspItemset(1);
  itSet->add_item(1);
  seq->add_itemset(itSet);
  itSet = new GspItemset(2);
  itSet->add_item(2);
  seq->add_itemset(itSet);
  itSet = new GspItemset(15);
  itSet->add_item(3);
  itSet->add_item(4);
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  seq = new GspSequence(-1);
  itSet = new GspItemset(1);
  itSet->add_item(2);
  itSet->add_item(1);
  seq->add_itemset(itSet);
  itSet = new GspItemset(20);
  itSet->add_item(5);
  seq->add_itemset(itSet);
  itSet = new GspItemset(50);
  itSet->add_item(3);
  seq->add_itemset(itSet);
  sequences.push_back(seq);

  return new GspArrayReader(sequences);
}

