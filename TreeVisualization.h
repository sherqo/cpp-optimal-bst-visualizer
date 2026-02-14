#pragma once

// impelement
#include <iostream>
#include <fstream>
#include <cstdlib> // For system()
#include <string>
#include "TreeNode.h"
#include "Tree.h"
#include "Settings.h"

class TreeVisualization
{
private:
  void static generateDotHelper(TreeNode *node, std::ofstream &dotFile, int &nullCount)
  {
    if (!node)
      return;

    // If the node has a left child, create an edge
    if (node->left)
    {
      dotFile << "  \"" << node->key << "\" -> \"" << node->left->key << "\";\n";
      generateDotHelper(node->left, dotFile, nullCount);
    }
    else
    {
      // Represent null node for left child
      dotFile << "  null" << nullCount << " [shape=point];\n";
      dotFile << "  \"" << node->key << "\" -> null" << nullCount << ";\n";
      nullCount++;
    }

    // If the node has a right child, create an edge
    if (node->right)
    {
      dotFile << "  \"" << node->key << "\" -> \"" << node->right->key << "\";\n";
      generateDotHelper(node->right, dotFile, nullCount);
    }
    else
    {
      // Represent null node for right child
      dotFile << "  null" << nullCount << " [shape=point];\n";
      dotFile << "  \"" << node->key << "\" -> null" << nullCount << ";\n";
      nullCount++;
    }
  }

  void static generateDotFile(const std::string &filename, TreeNode *root)
  {
    std::ofstream dotFile(filename); // Open file for writing
    if (!dotFile.is_open())
    {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return;
    }

    dotFile << "digraph OBST {\n"; // Start of DOT syntax

    // Label for the graph
    dotFile << "  label=\"" << Settings::getGraphLabel() << "\";\n";
    dotFile << "  labelloc=\"" << "t" << "\";\n";
    dotFile << "  fontsize=" << Settings::getGraphFontSize() << ";\n";

    // Node properties
    dotFile << "node [";
    dotFile << "shape=" << Settings::getNodeShape() << ", ";
    dotFile << "style=" << Settings::getNodeStyle() << ", ";
    dotFile << "color=" << Settings::getNodeColor() << ", ";
    dotFile << "fontcolor=" << Settings::getNodeFontColor() << ", ";
    dotFile << "fontsize=" << Settings::getNodeFontSize() << ";\n";
    dotFile << "];\n";

    // Edge properties
    std::string edgeColor = Settings::getEdgeColor();

    dotFile << "edge [color=" << edgeColor << "];\n";

    int nullCount = 0;
    if (root)
    {
      generateDotHelper(root, dotFile, nullCount); // Recursive helper to add nodes/edges
    }

    dotFile << "}\n"; // Close the DOT graph definition
    dotFile.close();

    std::cout << "DOT file generated successfully: " << filename << std::endl;
  }

  void static renderDotFile(const std::string &dotFile, const std::string &outputImage)
  {
    // Construct the command to execute Graphviz
    std::string command = "dot -Tpng " + dotFile + " -o " + outputImage;
    int result = system(command.c_str());
    if (result == 0)
    {
      std::cout << "Graph rendered to " << outputImage << std::endl;
    }
    else
    {
      std::cerr << "Failed to render graph. Ensure Graphviz is installed and accessible.\n";
    }
  }
  void static openFileWithSystem(const std::string &filename)
  {
    // Construct the command to open the file with the default application
    std::string command;
#ifdef _WIN32
    command = "start " + filename;
#elif __APPLE__
    command = "open " + filename;
#else
    command = "xdg-open " + filename;
#endif

    // Execute the command
    int result = system(command.c_str());

    // Check for errors
    if (result != 0)
    {
      std::cerr << "Failed to open the file: " << filename << std::endl;
    }
    else
    {
      std::cout << "File opened successfully: " << filename << std::endl;
    }
  }

public:
  void static visualizeTree(const Tree &treeToVisualize, const std::string &dotFile = "tree.dot", const std::string &outputImage = "tree.png", bool openAfterRendering = false)
  {
    generateDotFile(dotFile, treeToVisualize.getRoot());
    renderDotFile(dotFile, outputImage);

    if (openAfterRendering)
    {
      openFileWithSystem(outputImage);
    }
  }
};