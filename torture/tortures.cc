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
 * @brief Test Case fo DataStore->InsertData
 */
BOOST_AUTO_TEST_CASE(tc_hashtree_add_sequence)
{
  GspHashTree ht(3);
  GspSequence seq;

  //  BOOST_CHECK_EQUAL(GSP_OK, ht.AddSequence(seq));
}

/**
 * @brief Test Case fo DataStore->InsertData
 */
BOOST_AUTO_TEST_CASE(tc_hashtree_find_sequence)
{
  GspHashTree ht(3);
  GspSequence seq;  
}


