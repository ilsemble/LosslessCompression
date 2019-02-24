#pragma once
#include <stdlib.h>
#include "WorkWithFile.h"

class WorkWithTree
{
public:
     WorkWithTree();
     ~WorkWithTree();
     struct node // узел дерева
     {
          unsigned char simbol; // значение байта
          int probability; // число вхождений
          struct node *left; // левый потомок
          struct node *right; // правый потомок
     };
     node* createLeaf(unsigned char, int); // создает лист дерева
     node* uniteNodes(node*, node*); // объединяет 2 узла в один
     node* treeFromFile(FILE*, WorkWithFile*); // восстанавливает дерево из файла 
     void treeToFile(FILE*, node*, WorkWithFile*); // запись дерева в файл 
     void freemem(node*); // удаление поддерева

};











