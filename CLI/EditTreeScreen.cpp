#pragma once

#include "CLI.h"
#include "CLIHelper.h"

#include "../OBST.h"  // OBST class
#include "../Utils.h" // Utility functions
#include <iomanip>
#include <algorithm>

void CLI::editTree()
{
  while (true)
  {
    Utils::clearTerminal();
    std::cout << "\n===== Edit Tree =====\n";
    std::cout << "1. Create a Tree from Scratch\n";
    std::cout << "2. Add a New Node\n";
    std::cout << "3. Delete a Node\n";
    std::cout << "0. Back to Main Menu\n";

    int choice = CLIHELPER::getChoice(3, "USE_DEFAULT");

    switch (choice)
    {
    case 1:
      createTreeFromScratch();
      break;
    case 2:
      addNode();
      break;
    case 3:
      deleteNode();
      break;
    case 0:
      return; // Go back to the main menu
    default:
      std::cout << "Invalid choice. (from default of while)\n";
    }
  }
}

void CLI::createTreeFromScratch()
{
  Utils::clearTerminal();
  std::cout << "\n===== Create Tree from Scratch =====\n";

  bool doesHaveATree = !tree.isEmpty();
  {
    labels.resize(0);
    p.resize(1);
    q.resize(1);
  }

  useQ = Utils::getDataFromUser(labels, n, p, q);
  tree.assign(OBST::generateTheOBST(p, q, labels, false));
}

void CLI::addNode()
{
  Utils::clearTerminal();
  std::cout << "\n===== Add New Node =====\n";

  if (useQ)
  {
    CLIHELPER::popAlert("You cannot edit a tree with un-successful search probabilities (q).");
    return;
  }

  std::string newNodeLabel = Utils::readLabel(labels, "Enter the label for the new node: ");
  float newNodeP = Utils::readFloatInput("Enter the probability of successful search (p): ", false);

  if (tree.isEmpty())
  {
    labels.resize(1);
    labels[0] = newNodeLabel;

    p.resize(2);
    p[0] = 0;
    p[1] = newNodeP;

    q.resize(2);
    q[0] = 0;
    q[1] = 0;
  }
  else
  {
    int size = labels.size();
    labels.resize(size + 1);
    labels[size] = newNodeLabel;

    p.resize(size + 2);
    p[size + 1] = newNodeP;

    q.resize(size + 2);
    q[size + 1] = 0;
  }

  Utils::sortInputs(labels, p);

  tree.assign(OBST::generateTheOBST(p, q, labels, false));

  CLIHELPER::popAlert("Node added successfully!");
}

void CLI::deleteNode()
{
  Utils::clearTerminal();
  std::cout << "\n===== Delete Node =====\n";

  if (useQ)
  {
    CLIHELPER::popAlert("You cannot edit a tree with un-successful search probabilities (q).");
    return;
  }

  if (tree.isEmpty())
  {
    CLIHELPER::popAlert("The tree is empty! Please create a tree first.");
    return;
  }

  std::cout << "\n===== Entered Data =====\n\n";

  // Display header
  std::cout << std::left << std::setw(15) << "Label"
            << std::setw(15) << "P"
            << std::setw(15) << "Q" << "\n";
  std::cout << std::string(45, '-') << "\n"; // Table divider

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

  std::string nodeToDelete = Utils::readLabel(labels, "\nEnter the label of the node to delete: ", true);

  int index = labels.findOne(nodeToDelete);
  if (index == -1)
  {
    CLIHELPER::popAlert("The node does not exist in the tree!");
    return;
  }

  labels.removeByIndex(index);
  p.removeByIndex(index + 1);
  q.removeByIndex(index + 1);

  tree.assign(OBST::generateTheOBST(p, q, labels, false));

  CLIHELPER::popAlert("Node deleted successfully!");
}
