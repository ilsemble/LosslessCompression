#include "Huffman.h"
#include <windows.h>

Huffman::Huffman()
{

}

Huffman::~Huffman()
{
     
}


std::map <unsigned char, int> Huffman::alphabetInit(FILE* f)
{
     std::map <unsigned char, int> alphabet;
     char byte[1];
     int time = 0;
     while (true)
     {
          fread(byte, 1, 1, f);
          if (feof(f)) break;
          ++time;
          if (time > 500)
          {
               QApplication::processEvents();
               time = 0;
          }
          std::map <unsigned char, int>::iterator it = alphabet.find(byte[0]);
          if (it != alphabet.end()) //найдено
          {
               it->second++;
          }
          else
          {
               alphabet.insert(std::pair<unsigned char, int>(byte[0], 1));
          }
     }
     rewind(f);
     return alphabet;
}

void Huffman::treeCode(WorkWithTree::node *tree, std::string code, std::string simbol)
{
     if (!simbol.empty())
     {
          code = code + simbol;
     }
     if ((tree->left == NULL) && (tree->right == NULL))
     {
          encodeTable.insert(std::pair<unsigned char, std::string>(tree->simbol, code));
     }
     else
     {
          treeCode(tree->left, code, "0");
          treeCode(tree->right, code, "1");
     }
}

void Huffman::encode(FILE* fileInput, FILE* fileOutput)
{
     char byte[1];
     double progress = 0.0;
     int time = 0;
     while (true)
     {
          fread(byte, 1, 1, fileInput);
          if (feof(fileInput)) break;
          ++time;
          if (time > 500)
          {
               QApplication::processEvents();
               time = 0;
          }
          std::string code = encodeTable.at(byte[0]);
          file.stringToBits(fileOutput, code);
          progress = progress + 100.0/sizeOfSourceFile;
          emit changeProgress(progress);
     }
}

bool compare(WorkWithTree::node* a, WorkWithTree::node* b) // сравнение узлов дерева, возврат a->probability>b->probability
{
     return (a->probability>b->probability);
}

bool Huffman::compress(wchar_t* pathToInputFile, wchar_t* pathToOutputFile)
{
     WorkWithTree tree;
     FILE* fileSource;
     errno_t err = _wfopen_s(&fileSource, pathToInputFile, L"rb");
     if (err != 0)
     {
          return false;
     }
     else
     {
          FILE* fileOutput;
          errno_t err = _wfopen_s(&fileOutput, pathToOutputFile, L"wb");

          if (err != 0)
          {
               fclose(fileSource);
               return false;
          }
          else
          {
               unsigned long int countOfSimbolsInFile = file.fileSize(fileSource);
               if (0 == countOfSimbolsInFile)
               {
                    fclose(fileSource);
                    fclose(fileOutput);
                    return false;
               }
               else
               {
                    sizeOfSourceFile = countOfSimbolsInFile;
                    char count = 0;
                    do
                    {
                         unsigned char byte[1];
                         unsigned char p = countOfSimbolsInFile % 256;
                         byte[0] = p;
                         fwrite(byte, 1, 1, fileOutput);
                         countOfSimbolsInFile = countOfSimbolsInFile >> 8;
                         ++count;
                    } while (countOfSimbolsInFile > 0);
                    if (count < 4)
                    {
                         for (int i = 0; i < 4 - count; ++i)
                         {
                              unsigned char byte[1];
                              byte[0] = 0;
                              fwrite(byte, 1, 1, fileOutput);
                         }
                    }

                    std::map <unsigned char, int> alphabet = alphabetInit(fileSource);
                    std::map <unsigned char, int>::iterator it;
                    for (it = alphabet.begin(); it != alphabet.end(); ++it)
                    {
                         WorkWithTree::node* leaf = tree.createLeaf(it->first, it->second);
                         elements.push_back(leaf);
                    }
                    std::sort(elements.begin(), elements.end(), compare);
                    WorkWithTree::node* center;
                    if (1 == elements.size())
                    {
                         center = elements.back();
                         elements.pop_back();
                    }
                    else
                    {
                         center = tree.createLeaf('0', 0);
                    }
                    while (elements.size() > 1)
                    {
                         WorkWithTree::node* nodeLeft = elements.back();
                         elements.pop_back();
                         WorkWithTree::node* nodeRight = elements.back();
                         elements.pop_back();
                         center = tree.uniteNodes(nodeLeft, nodeRight);
                         elements.push_back(center);
                         std::sort(elements.begin(), elements.end(), compare);
                    }
                    tree.treeToFile(fileOutput, center, &file);
                    treeCode(center, "", "");
                    encode(fileSource, fileOutput);
                    tree.freemem(center);
               }
               file.closeWriteBitsToFile(fileOutput);
               sizeOfCompressedFile = file.fileSize(fileOutput);
               fclose(fileOutput);
          }

          fclose(fileSource);
     }
     return true;

}


void Huffman::decodeChar(FILE* fileInput, FILE* fileOutput, WorkWithTree::node* tree)
{
     if ((tree->left == NULL) && (tree->right == NULL))
     {
          unsigned char byteToWrite[1];
          byteToWrite[0] = tree->simbol;
          fwrite(byteToWrite, 1, 1, fileOutput);
     }
     else
     {
          if (file.oneBit(fileInput))
          {
               decodeChar(fileInput, fileOutput, tree->right);
          }
          else
          {
               decodeChar(fileInput, fileOutput, tree->left);
          }
     }

}

bool Huffman::decompress(wchar_t* pathToInputFile, wchar_t* pathToOutputFile)
{
     WorkWithTree tree;
     FILE* fileSource;
     errno_t err = _wfopen_s(&fileSource, pathToInputFile, L"rb");
     if (err != 0)
     {
          return false;
     }
     else
     {
          FILE* fileOutput;
          errno_t err = _wfopen_s(&fileOutput, pathToOutputFile, L"wb");
          if (err != 0)
          {
               fclose(fileSource);
               return false;
          }
          else
          {
               unsigned long int countOfSimbolsInFile = 0;
               for (int i = 0; i < 4; ++i)
               {
                    unsigned char byte[1];
                    fread(byte, 1, 1, fileSource);
                    countOfSimbolsInFile = countOfSimbolsInFile + (byte[0] << 8 * i);
                    
               }
               WorkWithTree::node* decodeTree = tree.treeFromFile(fileSource, &file);
               double progress = 0.0;
               int time = 0;
               for (unsigned int i = 0; i < countOfSimbolsInFile; ++i)
               {
                    ++time;
                    if (time > 500)
                    {
                         QApplication::processEvents();
                         time = 0;
                    }

                    decodeChar(fileSource, fileOutput, decodeTree);
                    progress = progress + 100.0/countOfSimbolsInFile;
                    emit changeProgress(progress);
               }
               tree.freemem(decodeTree);
               fclose(fileOutput);
          }
          fclose(fileSource);
     }
     return true;
}


unsigned long int Huffman::getSizeOfSourceFile()
{
  return sizeOfSourceFile;
}
unsigned long int Huffman::getSizeOfCompressedFile()
{
  return sizeOfCompressedFile;
}

double Huffman::calculateCompressionRatio()
{
  double ratio = ((double)sizeOfSourceFile/sizeOfCompressedFile);
  return ratio;
}

