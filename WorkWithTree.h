#pragma once
#include <stdlib.h>
#include "WorkWithFile.h"

class WorkWithTree
{
public:
     WorkWithTree();
     ~WorkWithTree();
     struct node // ���� ������
     {
          unsigned char simbol; // �������� �����
          int probability; // ����� ���������
          struct node *left; // ����� �������
          struct node *right; // ������ �������
     };
     node* createLeaf(unsigned char, int); // ������� ���� ������
     node* uniteNodes(node*, node*); // ���������� 2 ���� � ����
     node* treeFromFile(FILE*, WorkWithFile*); // ��������������� ������ �� ����� 
     void treeToFile(FILE*, node*, WorkWithFile*); // ������ ������ � ���� 
     void freemem(node*); // �������� ���������

};











