#pragma once
#include <string>

class WorkWithFile
{
public:
     WorkWithFile();
     ~WorkWithFile();
     void writeBitsToFile(FILE*, bool);
     long int fileSize(FILE*); // возвращает размер файла
     void stringToBits(FILE*, std::string);
     void charToBits(FILE*, char);
     void closeWriteBitsToFile(FILE*);
     bool oneBit(FILE*);
     char charFromBits(FILE*);
     std::string getErrorMessage();


private:
     std::string error;
     const char numberBitsInByte = 8;
     char bits;
     char bitsCount;
     char bitsSimbolCount;
     bool bitsSimbol[8];
     void readBitsFromFile(FILE*);

};

