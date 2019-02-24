#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
     QMainWindow(parent),
     ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     ui->pushButtonCompress->setEnabled(false);
     ui->pushButtonDecompress->setEnabled(false);
     currentProgressValue = 0;
     ui->labelMessageProcess->setText("");
     ui->progressBarProcess->setValue(0);
     sourceFile = "";
     savePath = "";
     sourceDecompressFile = "";
     saveDecompressedPath = "";
}

MainWindow::~MainWindow()
{
     delete ui;
}

void MainWindow::on_pushButtonSave_clicked()
{
     QString sp = "";
     if (sourceFile != "")
     {
          sp = QFileDialog::getSaveFileName(this, "Save", sourceFile+".hff", "All files (*.*)");
     }
     else
     {
          sp = QFileDialog::getSaveFileName(this, "Save", "*.hff", "All files (*.*)");
     }
     if (sp != "")
     {
          savePath = sp;
          ui->lineEditPathToSave->setText(savePath);
     }
     ui->progressBarProcess->setValue(0);
}

void MainWindow::on_pushButtonCompress_clicked()
{
     ui->tabWidget->setEnabled(false);
     Huffman huffman;
     connect(&huffman, SIGNAL(changeProgress(int)), this, SLOT(changeProgress(int)));
     int nFR = sourceFile.size();
     wchar_t pathFR[nFR+1];
     sourceFile.toWCharArray(pathFR);
     pathFR[nFR] = '\0';
     int nFW =savePath.size();
     wchar_t pathFW[nFW+1];
     savePath.toWCharArray(pathFW);
     pathFW[nFW] = '\0';
     ui->progressBarProcess->setValue(0);
     ui->labelMessageProcess->setText("Compressing... Please, wait!");
     if (!huffman.compress(pathFR, pathFW))
     {
          ui->labelMessage->setText("Perfect result!");
          ui->labelOriginal->setText("0");
          ui->labelCompressed->setText("0");
          ui->labelCompressionRatio->setText("1");
          QMessageBox::warning(this, "Warning", "Zero file can not be compressed better!");
     }
     else
     {
          ui->labelOriginal->setText(QString::number(huffman.getSizeOfSourceFile()));
          ui->labelCompressed->setText(QString::number(huffman.getSizeOfCompressedFile()));
          double ratio = huffman.calculateCompressionRatio();
          ui->labelCompressionRatio->setText(QString::number(ratio));
          if (ratio > 1)
          {
               ui->labelMessage->setText("Good result!");
          }
          else
          {
               ui->labelMessage->setText("Compression was not effective!");
          }
     }
     ui->progressBarProcess->setValue(100);
     ui->labelMessageProcess->setText("");
     ui->tabWidget->setEnabled(true);

}

void MainWindow::on_pushButtonChooseFile_clicked()
{   
     QString sf = "";
     sf = QFileDialog::getOpenFileName(0, "Open File", "", "*.*");
     if (sf != "")
     {
          sourceFile = sf;
          ui->lineEditPathToCompress->setText(sourceFile);
          savePath = sourceFile+".hff";
          ui->lineEditPathToSave->setText(savePath);
          ui->pushButtonCompress->setEnabled(true);
     }
     ui->progressBarProcess->setValue(0);
}

void MainWindow::on_pushButtonChooseFileDecompress_clicked()
{
     QString sf = "";
     sf = QFileDialog::getOpenFileName(0, "Open File", "", "*.hff");
     if (sf != "")
     {
          sourceDecompressFile = sf;
          ui->lineEditPathToDecompress->setText(sourceDecompressFile);
          saveDecompressedPath = sourceDecompressFile;
          saveDecompressedPath.chop(4);
          ui->lineEditPathToSaveDecompress->setText(saveDecompressedPath);
          ui->pushButtonDecompress->setEnabled(true);
     }
     ui->progressBarProcess->setValue(0);
}

void MainWindow::on_pushButtonSaveDecompress_clicked()
{
     QString sp = "";
     sp = QFileDialog::getSaveFileName(this, "Save", saveDecompressedPath, "All files (*.*)");
     if (sp != "")
     {
          saveDecompressedPath = sp;
          ui->lineEditPathToSaveDecompress->setText(saveDecompressedPath);
     }
     ui->progressBarProcess->setValue(0);
}

void MainWindow::on_pushButtonDecompress_clicked()
{
     ui->tabWidget->setEnabled(false);
     Huffman huffman;
     connect(&huffman, SIGNAL(changeProgress(int)), this, SLOT(changeProgress(int)));

     int nFR = sourceDecompressFile.size();
     wchar_t pathFR[nFR+1];
     sourceDecompressFile.toWCharArray(pathFR);
     pathFR[nFR] = '\0';

     int nFW =saveDecompressedPath.size();
     wchar_t pathFW[nFW+1];
     saveDecompressedPath.toWCharArray(pathFW);
     pathFW[nFW] = '\0';
     ui->progressBarProcess->setValue(0);
     ui->labelMessageProcess->setText("Decompressing... Please, wait!");
     if (!huffman.decompress(pathFR, pathFW))
     {
          QMessageBox::warning(this, "Warning", "This file can not be decompressed!");
     }
     ui->progressBarProcess->setValue(100);
     ui->labelMessageProcess->setText("");
     ui->tabWidget->setEnabled(true);


}

void MainWindow::changeProgress(int value)
{
     ui->progressBarProcess->setValue(value);
}


void MainWindow::on_lineEditPathToCompress_textEdited(const QString& fileName)
{
     ui->progressBarProcess->setValue(0);
     sourceFile = fileName;
     if (sourceFile != "")
     {
          if (savePath == "")
          {
               savePath = sourceFile+".hff";
               ui->lineEditPathToSave->setText(savePath);
          }
          ui->pushButtonCompress->setEnabled(true);
     }
     else
     {
          ui->pushButtonCompress->setEnabled(false);
     }
}

void MainWindow::on_lineEditPathToSave_textEdited(const QString& fileName)
{  
     ui->progressBarProcess->setValue(0);
     savePath = fileName;
     if (savePath != "")
     {
          ui->pushButtonCompress->setEnabled(true);
     }
     else
    {
         ui->pushButtonCompress->setEnabled(false);
    }
}

void MainWindow::on_lineEditPathToDecompress_textEdited(const QString &fileName)
{
     ui->progressBarProcess->setValue(0);
     sourceFile = fileName;
     if (sourceFile != "")
     {
          if (savePath == "")
          {
               savePath = sourceFile+".hff";
               ui->lineEditPathToSave->setText(savePath);
          }
          ui->pushButtonCompress->setEnabled(true);
     }
     else
     {
          ui->pushButtonCompress->setEnabled(false);
     }


     sourceDecompressFile = fileName;
     if (sourceDecompressFile != "")
       {
          if (saveDecompressedPath == "")
          {
               saveDecompressedPath = sourceDecompressFile;
               saveDecompressedPath.chop(4);
               ui->lineEditPathToSaveDecompress->setText(saveDecompressedPath);
           }
          ui->pushButtonDecompress->setEnabled(true);
       }
     else
     {
          ui->pushButtonDecompress->setEnabled(false);
     }
}

void MainWindow::on_lineEditPathToSaveDecompress_textEdited(const QString &fileName)
{
     ui->progressBarProcess->setValue(0);
     saveDecompressedPath = fileName;
     if (saveDecompressedPath != "")
       {
          ui->pushButtonDecompress->setEnabled(true);
       }
     else
     {
          ui->pushButtonDecompress->setEnabled(false);
     }
}

