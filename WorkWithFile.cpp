#include "WorkWithFile.h"


WorkWithFile::WorkWithFile()
{
     bits = 0;
     bitsCount = 0;
     bitsSimbolCount = numberBitsInByte;
}


WorkWithFile::~WorkWithFile()
{
}

long int  WorkWithFile::fileSize(FILE* f)
{
     rewind(f); // устанавливаем позицию в начало
     fseek(f, 0, SEEK_END); // устанавливаем позицию в конец файла
     long size = ftell(f); // получаем размер в байтах
     rewind(f); // устанавливаем позицию в начало
     return size;
}

void  WorkWithFile::writeBitsToFile(FILE* fileOutput, bool bit)
{
     if (bit)
     {
          bits = (bits << 1) + 1;
     }
     else
     {
          bits = (bits << 1);
     }
     ++bitsCount;
     if (bitsCount > 7)
     {
          unsigned char byte[1];
          byte[0] = bits;
          fwrite(byte, 1, 1, fileOutput);
          bits = 0;
          bitsCount = 0;
     }
}

void WorkWithFile::stringToBits(FILE* fileOutput, std::string str)
{
     int n = str.size();
     for (int i = 0; i < n; ++i)
     {
          if (str[i] == '0')
          {
               writeBitsToFile(fileOutput, false);
          }
          else
          {
               writeBitsToFile(fileOutput, true);

          }
     }
}

void WorkWithFile::charToBits(FILE* fileOutput, char simbol)
{
     for (int i = 0; i < numberBitsInByte; ++i)
     {
          writeBitsToFile(fileOutput, (simbol & 128) != 0);
          simbol = simbol << 1;
     }
}

void WorkWithFile::closeWriteBitsToFile(FILE* fileOutput)
{
     if (bitsCount != 0)
     {
          int n = bitsCount;
          for (int i = 0; i < numberBitsInByte - n; ++i)
          {
               writeBitsToFile(fileOutput, false);
          }

     }
}

void WorkWithFile::readBitsFromFile(FILE* fileInput)
{
     char byte[1];
     try
     {
          fread(byte, 1, 1, fileInput);
     }
     catch (const std::exception&)
     {
          error = "The file is corrupted!";
     }
     bitsSimbolCount = 0;
     for (int i = 0; i < numberBitsInByte; ++i)
     {
          bitsSimbol[i] = (byte[0] & 128) != 0;
          byte[0] = byte[0] << 1;
     }
}

bool WorkWithFile::oneBit(FILE* fileInput)
{
     if (bitsSimbolCount >= numberBitsInByte)
     {
          readBitsFromFile(fileInput);
     }
     bool bit = bitsSimbol[bitsSimbolCount];
     ++bitsSimbolCount;
     return bit;
}

char WorkWithFile::charFromBits(FILE* fileInput)
{
     char simbol = 0;
     for (int i = 0; i < numberBitsInByte; ++i)
     {
          if (oneBit(fileInput))
          {
               simbol = (simbol << 1) + 1;
          }
          else
          {
               simbol = (simbol << 1);
          }

     }
     return simbol;
}

std::string WorkWithFile::getErrorMessage()
{
     return error;
}
