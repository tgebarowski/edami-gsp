/**
 * @file  tortures.cc
 *
 * @brief Unit tests
 * 
 * @author: Tomasz Gebarowski <gebarowski@gmail.com>
 * @date: Sat May 19 18:19:22 2012
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN


#include <boost/test/unit_test.hpp>
#include <stdio.h>

using boost::unit_test::test_suite;

#include "gsp_hashtree_class.h"
#include "gsp_sequence_class.h"
#include "gsp_itemset_class.h"

/**
 * @brief Test Case for GspSequence primitives
 */
BOOST_AUTO_TEST_CASE(tc_sequence_primitives)
{
  GspSequence *seq = new GspSequence();
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  GspItemset *is3 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  is2->add_item("d");
  is2->add_item("e");
  is2->add_item("f");
  is3->add_item("g");
  
  seq->add_itemset(is1);
  seq->add_itemset(is2);
  seq->add_itemset(is3);

  cout << "Input sequence: " + seq->ToString() << endl;

  const string sequence_str = "abcdefgh";
  
  for (size_t pos = 0; pos < sequence_str.length() - 1; pos++)
  {
    string found_item = seq->GetItemByIndex(pos);
    string correct_item = ToString(sequence_str[pos]);
    cout << "Find element at index: "\
         << pos << "=>" << found_item  << endl;
    BOOST_CHECK(correct_item == found_item);
  }
}


/**
 * @brief Test Case for HashTree->AddSequence
 */
BOOST_AUTO_TEST_CASE(tc_hashtree_add_sequence)
{
  GspHashTree ht(3);
  GspSequence *seq = new GspSequence();
  GspItemset *is1 = new GspItemset();
  GspItemset *is2 = new GspItemset();
  is1->add_item("a");
  is1->add_item("b");
  is2->add_item("c");
  
  seq->add_itemset(is1);
  seq->add_itemset(is2);

  cout << "Input sequence: " + seq->ToString() << endl;

  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq));
}

/**
 * @brief Test Case for HashTree->FindSequence
 */
BOOST_AUTO_TEST_CASE(tc_hashtree_find_sequence)
{
  GspHashTree ht(3);
  GspSequence seq;  
}


