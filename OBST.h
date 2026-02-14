/**
 * @file OBST.h
 * @brief Simple and clear implementation of the Optimal Binary Search Tree (OBST) algorithm.
 */

#pragma once

#include <iostream>
#include <cstdint>
#include "Vector.h"
#include "TreeNode.h"
#include "Tree.h"
#include "Utils.h"

/**
 * @class OBST
 * @brief Handles the construction of the Optimal Binary Search Tree (OBST).
 */
class OBST
{
private:
  /**
   * @brief Initializes the base cases for the dynamic programming tables.
   *
   * This function sets up the initial values for the `E`, `W`, and `Root` tables.
   * These base cases represent the costs, weights, and roots for empty subtrees
   * and subtrees with a single key.
   */
  void static initializeLoop(Vector<Vector<float>> &E, Vector<Vector<float>> &W, Vector<Vector<float>> &Root,
                             const int &N, const Vector<float> &P, const Vector<float> &Q)
  {
    for (int a = 1; a <= N; a++)
    {
      // Base case for subtrees with no keys
      W[a][a - 1] = E[a][a - 1] = Q[a - 1];

      // Base case for subtrees with one key: root is the key itself
      Root[a][a] = a;                   // The single key is the root
      W[a][a] = Q[a - 1] + P[a] + Q[a]; // Weight includes key and adjacent dummy keys
      E[a][a] = W[a][a];                // Cost is equal to the weight for single keys
    }

    // Handle the edge case for the last dummy key
    W[N + 1][N] = E[N + 1][N] = Q[N];
  }

  /**
   * @brief Runs the main dynamic programming algorithm to compute the OBST tables.
   *
   * This function calculates the cost, weight, and root for subtrees of increasing lengths.
   * It tries every possible root for each subtree and picks the one that minimizes the cost.
   */
  void static computeOBST(Vector<Vector<float>> &E, Vector<Vector<float>> &W, Vector<Vector<float>> &Root,
                          const int &N, const Vector<float> &P, const Vector<float> &Q)
  {
    for (int l = 2; l <= N; l++) // l is the length of the subtree
    {
      for (int i = 1; i <= N - l + 1; i++) // i is the start of the subtree
      {
        int j = i + l - 1;   // j is the end of the subtree
        E[i][j] = INT32_MAX; // Initialize the cost to a large value (infinity)

        // Update the weight of the subtree [i, j]
        W[i][j] = W[i][j - 1] + P[j] + Q[j];

        // Test each possible root from `root[i][j-1]` to `root[i+1][j]`
        for (int r = Root[i][j - 1]; r <= Root[i + 1][j]; r++)
        {
          // Calculate the cost if `r` is chosen as the root
          float currCost = E[i][r - 1] + E[r + 1][j] + W[i][j];

          // If this cost is the smallest, update the root and cost
          if (currCost < E[i][j])
          {
            E[i][j] = currCost; // Update minimum cost
            Root[i][j] = r;     // Save the optimal root
          }
        }
      }
    }
  }

  /**
   * @brief Builds a binary tree from the root table.
   *
   * The function uses recursive calls to construct the tree by looking up
   * the root table. It breaks the tree into left and right subtrees based
   * on the optimal root for each range.
   */
  TreeNode static *buildTreeFromRoot(const Vector<Vector<float>> &root, const Vector<std::string> &labels, int i, int j)
  {
    // Base case: If the range is invalid, return null
    if (i > j || root[i][j] == 0)
      return nullptr;

    // Get the root index for the range [i, j]
    int r = int(root[i][j]);

    // Create a new tree node for this root
    TreeNode *node = new TreeNode(labels[r - 1]); // Labels are 0-indexed

    // Recursively build the left and right subtrees
    node->left = buildTreeFromRoot(root, labels, i, r - 1);  // Left subtree is [i, r-1]
    node->right = buildTreeFromRoot(root, labels, r + 1, j); // Right subtree is [r+1, j]

    return node; // Return the constructed node
  }

  /**
   * @brief Converts the root table into a complete binary tree.
   *
   * This is a helper function to create the `Tree` object using
   * the root table and the provided labels.
   */
  Tree static convertToTree(const Vector<Vector<float>> &root, const Vector<std::string> &labels, int n)
  {
    Tree tree;
    tree.setRoot(buildTreeFromRoot(root, labels, 1, n)); // Build the full tree
    return tree;                                         // Return the constructed tree
  }

public:
  void static displayTables(const Vector<Vector<float>> &E, const Vector<Vector<float>> &W, const Vector<Vector<float>> &Root)
  {
    int n = E.size() - 1;

    std::cout << "Cost Table (E):\n";
    Utils::displayTwoDVec(E);

    std::cout << "\nWeight Table (W):\n";
    Utils::displayTwoDVec(W);

    std::cout << "\nRoot Table:\n";
    Utils::displayTwoDVec(Root);
  }

  /**
   * @brief Generates an Optimal Binary Search Tree.
   *
   * This function is the main entry point for constructing an OBST. It takes
   * the probabilities of keys and dummy keys, calculates the dynamic programming
   * tables, and builds the final binary tree.
   *
   * @param p Probabilities of successfully searching for each key.
   * @param q Probabilities of searching for dummy keys.
   * @param labels Names of the keys (used as labels in the tree).
   * @param displayTables Whether to display the intermediate tables (default: false).
   * @return Tree The constructed Optimal Binary Search Tree.
   */
  Tree static generateTheOBST(const Vector<float> &p, const Vector<float> &q, const Vector<std::string> &labels, bool _displayTables = false)
  {
    int n = p.size() - 1; // Number of keys (p[0] is unused)

    // Create 2D Vectors for cost, weight, and root
    Vector<Vector<float>> e = Utils::create2D<float>(n + 2, n + 2);
    Vector<Vector<float>> w = Utils::create2D<float>(n + 2, n + 2);
    Vector<Vector<float>> root = Utils::create2D<float>(n + 2, n + 2);

    // Initialize base cases
    initializeLoop(e, w, root, n, p, q);

    // Compute the tables for all subtrees
    computeOBST(e, w, root, n, p, q);

    // Display the tables if u want
    if (_displayTables)
    {
      displayTables(e, w, root);
    }

    // Build and return the OBST as a Tree object
    return convertToTree(root, labels, n);
  }

  void static addNode(std::string nodeLabel, float p, float q, Vector<std::string> &labels, Vector<float> &P, Vector<float> &Q)
  {

    labels.push_back(nodeLabel);
    P.push_back(p);
    Q.push_back(q);

    Utils::sortInputs(labels, P);
  }
};
