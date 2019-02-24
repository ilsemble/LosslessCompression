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
     bool compress(wchar_t* , wchar_t*); // сжатие файла
     bool decompress(wchar_t* , wchar_t*); // разжатие файла
     unsigned long int getSizeOfSourceFile();
     unsigned long int getSizeOfCompressedFile();
     double calculateCompressionRatio();
signals:
     void changeProgress(int);

private:
     WorkWithFile file;
     std::map <unsigned char, int> alphabetInit(FILE* f); // создает словарь, ключ - символ, значение - частота символа
     std::vector<WorkWithTree::node*> elements; // очередь для формирования дерева
     std::map <unsigned char, std::string > encodeTable; // таблица с кодами Хаффмана
     void treeCode(WorkWithTree::node*, std::string, std::string); // создание кодов для символов
     void encode(FILE*, FILE*); // закодировать (FILE* fileInput, FILE* fileOutput)
     void decodeChar(FILE*, FILE*, WorkWithTree::node*); // восстановить один символ (FILE* fileInput, FILE* fileOutput)
     unsigned long int sizeOfSourceFile;
     unsigned long int sizeOfCompressedFile;
};

