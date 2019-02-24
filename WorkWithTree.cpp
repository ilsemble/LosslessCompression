#include "WorkWithTree.h"


WorkWithTree::WorkWithTree()
{
}


WorkWithTree::~WorkWithTree()
{
}



WorkWithTree::node* WorkWithTree::createLeaf(unsigned char simbol, int count)
{
     node* leaf = (node *)malloc(sizeof(node));
     leaf->simbol = simbol;
     leaf->probability = count;
     leaf->left = leaf->right = NULL;
     return leaf;
}

WorkWithTree::node* WorkWithTree::uniteNodes(node* leftNode, node* rightNode)
{
     node* center = (node *)malloc(sizeof(node));
     center->simbol = leftNode->simbol;
     center->probability = leftNode->probability + rightNode->probability;
     center->left = leftNode;
     center->right = rightNode;
     return center;
}

// Функция удаления поддерева
void WorkWithTree::freemem(node *tree)
{
     if (tree != NULL) {
          freemem(tree->left);
          freemem(tree->right);
          free(tree);
     }
}

WorkWithTree::node* WorkWithTree::treeFromFile(FILE* fileInput, WorkWithFile* file)
{
     node* tree;
     if (file->oneBit(fileInput))
     {
          tree = createLeaf(file->charFromBits(fileInput), 0);
     }
     else
     {
          tree = createLeaf('\0', 0);
          tree->left = treeFromFile(fileInput, file);
          tree->right = treeFromFile(fileInput, file);
     }
     return tree;

}

void WorkWithTree::treeToFile(FILE* fileOutput, node *tree, WorkWithFile* file)
{
     if ((tree->left == NULL) && (tree->right == NULL))
     {
          file->writeBitsToFile(fileOutput, true);
          file->charToBits(fileOutput, tree->simbol);
     }
     else
     {
          file->writeBitsToFile(fileOutput, false);
          treeToFile(fileOutput, tree->left, file);
          treeToFile(fileOutput, tree->right, file);
     }
}

