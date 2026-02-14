#pragma once

#include "CLI.h"
#include "CLIHelper.h"

#include "../Utils.h" // Utility functions
#include "../TreeVisualization.h"
#include "../OBST.h" // OBST class
#include <iomanip>
#include <algorithm>

void CLI::displayTree() const
{
  if (tree.isEmpty())
  {
    CLIHELPER::popAlert("The tree is empty! Please create a tree first.");
    return;
  }

  while (true)
  {
    Utils::clearTerminal();
    std::cout << "\n===== Displaying Tree =====\n";
    tree.displayTree();

    int choice = CLIHELPER::getChoice(0, "\nPress [0] to back to main menu\n");

    switch (choice)
    {
    case 0:
      return; // Go back to the main menu
      break;

    default:
      std::cout << "Invalid choice. (from default of while)\n";
      break;
    }
  }
}

void CLI::visualizeTree()
{
  Utils::clearTerminal();

  if (tree.isEmpty())
  {
    CLIHELPER::popAlert("The tree is empty! Please create a tree first.");
    return;
  }

  std::cout << "Visualizing the tree...\n";
  TreeVisualization::visualizeTree(tree, DOT_FILE, OUTPUT_IMAGE, true);
}

void CLI::analyzeTree()
{
  if (tree.isEmpty())
  {
    CLIHELPER::popAlert("The tree is empty! Please create a tree first.");
    return;
  }

  while (true)
  {
    Utils::clearTerminal();
    std::cout << "\n===== Tree Analysis =====\n";

    tree.analyzeTree();

    int choice = CLIHELPER::getChoice(0, "\nPress [0] to back to main menu\n");

    switch (choice)
    {
    case 0:
      return; // Go back to the main menu
    default:
      std::cout << "Invalid choice. (from default of while)\n";
    }
  }
}

void CLI::DisplayDerivedTables()
{
  if (tree.isEmpty())
  {
    CLIHELPER::popAlert("The have not enter data yet! Please create data first.");
    return;
  }

  while (true)
  {
    Utils::clearTerminal();
    std::cout << "\n===== Display Derived Tables =====\n";

    OBST::generateTheOBST(p, q, labels, true);

    int choice = CLIHELPER::getChoice(0, "\nPress [0] to back to main menu\n");

    switch (choice)
    {
    case 0:
      return; // Go back to the main menu
    default:
      std::cout << "Invalid choice. (from default of while)\n";
    }
  }
}

void CLI::DisplayEnteredData()
{
  if (tree.isEmpty())
  {
    CLIHELPER::popAlert("You have not entered data yet! Please create data first.");
    return;
  }

  while (true)
  {
    Utils::clearTerminal();
    std::cout << "\n===== Display Entered Data =====\n\n";

    // Display header
    std::cout << std::left << std::setw(15) << "Label"
              << std::setw(15) << "P"
              << std::setw(15) << "Q" << "\n";
    std::cout << std::string(45, '-') << "\n"; // Table sep

    for (size_t i = 0; i < std::max({labels.size(), p.size(), q.size()}); ++i)
    {
      std::string label = (i < labels.size()) ? labels[i] : "";
      std::string probP = (i + 1 < p.size()) ? std::to_string(p[i + 1]) : "";
      std::string probQ = (i < q.size()) ? std::to_string(q[i]) : "";

      // Print each row
      std::cout << std::left << std::setw(15) << label
                << std::setw(15) << probP
                << std::setw(15) << probQ << "\n";
    }

    // Add a footer for user choice
    int choice = CLIHELPER::getChoice(0, "\nPress [0] to return to the main menu\n");

    switch (choice)
    {
    case 0:
      return; // Go back to the main menu
    default:
      std::cout << "Invalid choice.\n"; // Default invalid choice handler
    }
  }
}
