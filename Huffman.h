#pragma once
#include <QObject>
#include <QApplication>

#include <map>
#include <vector>
#include <algorithm>

#include "WorkWithTree.h"
#include "WorkWithFile.h"



class Huffman : public QObject
{
     Q_OBJECT
public:
     Huffman();
     ~Huffman();
     bool compress(wchar_t* , wchar_t*); // ������ �����
     bool decompress(wchar_t* , wchar_t*); // �������� �����
     unsigned long int getSizeOfSourceFile();
     unsigned long int getSizeOfCompressedFile();
     double calculateCompressionRatio();
signals:
     void changeProgress(int);

private:
     WorkWithFile file;
     std::map <unsigned char, int> alphabetInit(FILE* f); // ������� �������, ���� - ������, �������� - ������� �������
     std::vector<WorkWithTree::node*> elements; // ������� ��� ������������ ������
     std::map <unsigned char, std::string > encodeTable; // ������� � ������ ��������
     void treeCode(WorkWithTree::node*, std::string, std::string); // �������� ����� ��� ��������
     void encode(FILE*, FILE*); // ������������ (FILE* fileInput, FILE* fileOutput)
     void decodeChar(FILE*, FILE*, WorkWithTree::node*); // ������������ ���� ������ (FILE* fileInput, FILE* fileOutput)
     unsigned long int sizeOfSourceFile;
     unsigned long int sizeOfCompressedFile;
};

