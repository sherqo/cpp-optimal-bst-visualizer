#pragma once

#include <iostream>
#include <iomanip>
#include "TreeNode.h"

/**
 * @class Tree
 * A class to represent a binary tree and provide utilities like displaying
 * the tree structure, cleaning up memory, and analyzing the tree.
 */
class Tree
{
private:
  TreeNode *root; // Pointer to the root of the tree

  // === Your Existing Helper Methods ===
  void displayTreeHelper(TreeNode *node, int depth = 0) const
  {
    if (!node)
      return;

    displayTreeHelper(node->right, depth + 1);
    for (int i = 0; i < 2 * depth; ++i)
      std::cout << " ";
    std::cout << node->key << std::endl;
    displayTreeHelper(node->left, depth + 1);
  }

  void deleteTree(TreeNode *node)
  {
    if (!node)
      return;

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }

  TreeNode *copySubtree(TreeNode *node) const
  {
    if (!node)
      return nullptr;

    TreeNode *newNode = new TreeNode(node->key);
    newNode->left = copySubtree(node->left);
    newNode->right = copySubtree(node->right);
    return newNode;
  }

  // === Analysis Helper Methods ===
  int computeHeight(TreeNode *node) const
  {
    if (!node)
      return 0;
    int leftHeight = computeHeight(node->left);
    int rightHeight = computeHeight(node->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }

  int countNodes(TreeNode *node) const
  {
    if (!node)
      return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
  }

  int countLeafNodes(TreeNode *node) const
  {
    if (!node)
      return 0;
    if (!node->left && !node->right)
      return 1;
    return countLeafNodes(node->left) + countLeafNodes(node->right);
  }

  int sumOfDepths(TreeNode *node, int depth) const
  {
    if (!node)
      return 0;
    return depth + sumOfDepths(node->left, depth + 1) + sumOfDepths(node->right, depth + 1);
  }

public:
  // === Your Existing Methods ===
  Tree() : root(nullptr) {}

  ~Tree()
  {
    deleteTree(root);
  }

  // Copy constructor
  Tree(const Tree &other) : root(copySubtree(other.root)) {}

  // Copy assignment operator
  Tree &operator=(const Tree &other)
  {
    if (this != &other)
    {
      deleteTree(root);
      root = copySubtree(other.root);
    }
    return *this;
  }

  // Move constructor
  Tree(Tree &&other) noexcept : root(other.root)
  {
    other.root = nullptr;
  }

  // Move assignment operator
  Tree &operator=(Tree &&other) noexcept
  {
    if (this != &other)
    {
      deleteTree(root);
      root = other.root;
      other.root = nullptr;
    }
    return *this;
  }

  void setRoot(TreeNode *node)
  {
    root = node;
  }

  TreeNode *getRoot() const
  {
    return root;
  }

  void displayTree() const
  {
    displayTreeHelper(root);
  }

  void assign(const Tree &otherTree)
  {
    *this = otherTree; // Use copy assignment operator
  }

  // === New Analysis Methods ===
  void analyzeTree() const
  {
    int height = computeHeight(root);
    int totalNodes = countNodes(root);
    int leafNodes = countLeafNodes(root);
    double averageDepth = (totalNodes == 0) ? 0.0 : static_cast<double>(sumOfDepths(root, 0)) / totalNodes;

    std::cout << "Height of the Tree: " << height << std::endl;
    std::cout << "Total Number of Nodes: " << totalNodes << std::endl;
    std::cout << "Number of Leaf Nodes: " << leafNodes << std::endl;
    std::cout << "Average Depth of Nodes: " << averageDepth << std::endl;
  }

  int getHeight() const
  {
    return computeHeight(root);
  }

  int getTotalNodes() const
  {
    return countNodes(root);
  }

  int getLeafNodes() const
  {
    return countLeafNodes(root);
  }

  double getAverageDepth() const
  {
    int totalNodes = countNodes(root);
    return (totalNodes == 0) ? 0.0 : static_cast<double>(sumOfDepths(root, 0)) / totalNodes;
  }

  bool isEmpty() const
  {
    return root == nullptr;
  }
};