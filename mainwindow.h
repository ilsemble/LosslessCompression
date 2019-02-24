#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include "Huffman.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonCompress_clicked();

    void on_pushButtonChooseFile_clicked();

    void on_pushButtonChooseFileDecompress_clicked();

    void on_pushButtonSaveDecompress_clicked();

    void on_pushButtonDecompress_clicked();

    void changeProgress(int);


    void on_lineEditPathToCompress_textEdited(const QString &arg1);

    void on_lineEditPathToSave_textEdited(const QString &arg1);

    void on_lineEditPathToDecompress_textEdited(const QString &arg1);

    void on_lineEditPathToSaveDecompress_textEdited(const QString &arg1);

private:
    Ui::MainWindow* ui;
    QString sourceFile;
    QString savePath;
    QString sourceDecompressFile;
    QString saveDecompressedPath;
    int currentProgressValue;

};

#endif // MAINWINDOW_H
