#pragma once

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "hwt.hpp"

namespace test_funcs {

using KeyT = int;

auto range_query(Trees::SearchTree<KeyT, KeyT> &tree, const KeyT &fst,
                 const KeyT &snd) 
{
  return tree.my_distance(fst, snd);
}

const int error_id = -1;

void get_result(const std::string &filename, std::vector<int> &res) {
  std::ifstream file(filename);
  if (!file) {
    std::cout << "error\n";
    exit(1);
  }

  Trees::SearchTree<KeyT, KeyT> tree = {};

  char type = 0;
  while (file >> type) {
    if (type == 'k') {
      KeyT key = 0;
      file >> key;
      if (!file.good()) {
        throw "Error : invalid type of key\n";
      }

      tree.insert(key);
    } else if (type == 'q') {
      KeyT key1 = 0;
      KeyT key2 = 0;

      file >> key1;
      if (!std::cin.good()) {
        throw "Error : invalid type of key1\n";
      }

      file >> key2;
      if (!std::cin.good()) {
        throw "Error : invalid type of key2\n";
      }

      if (key1 >= key2) {
        res.push_back(0);
        continue;
      }

      res.push_back(range_query(tree, key1, key2));
    } else if (type) {
      throw "Error: invalit type\n";
    }
  }
}

void get_answer(const std::string &filename, std::vector<int> &ans) {
  std::ifstream file(filename);
  if (!file) {
    std::cout << "error\n";
    exit(1);
  }

  using KeyT = int;
  std::set<KeyT> tree = {};

  char type = 0;
  while (file >> type) {
    if (type == 'k') {
      KeyT key = 0;
      file >> key;
      if (!file.good()) {
        throw "Error : invalid type of key\n";
      }

      tree.insert(key);
    } else if (type == 'q') {
      KeyT key1 = 0;
      KeyT key2 = 0;

      file >> key1;
      if (!std::cin.good()) {
        throw "Error : invalid type of key1\n";
      }

      file >> key2;
      if (!std::cin.good()) {
        throw "Error : invalid type of key2\n";
      }

      if (key1 >= key2) {
        ans.push_back(0);
        continue;
      }

      ans.push_back(
          std::distance(tree.lower_bound(key1), tree.upper_bound(key2)));
    } else if (type) {
      throw "Error: invalit type\n";
    }
  }
}

void run_test(const std::string &test_name) {
  std::string test_directory = "/tests";

  std::string test_path =
      std::string(TEST_DATA_DIR) + test_directory + test_name;

  std::vector<int> res;
  get_result(test_path, res);

  std::vector<int> ans;
  get_answer(test_path, ans);

  EXPECT_TRUE(res.size() == ans.size());
  for (int i = 0; i < ans.size(); i++) {
    EXPECT_EQ(res[i], ans[i]);
  }
}
} // namespace test_funcs