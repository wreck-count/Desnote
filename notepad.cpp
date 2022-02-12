#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Notepad),
      bModified(false),
      bExplicitModify(false)

{
    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);

    connect(ui->newFileButton, &QPushButton::clicked, this, &Notepad::newFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &Notepad::saveFile);
    connect(ui->saveNewButton, &QPushButton::clicked, this, &Notepad::saveAsFile);
    connect(ui->browseButton, &QPushButton::clicked, this, &Notepad::browse);
    connect(ui->undoButton, &QPushButton::clicked, this, &Notepad::undo);
    connect(ui->redoButton, &QPushButton::clicked, this, &Notepad::redo);
    connect(ui->pasteButton,&QPushButton::clicked, this, &Notepad::paste);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Notepad::setModified);
}


Notepad::~Notepad()
{
    delete ui;
}

void Notepad::newFile()
{
    if(isModified()){
        int usrChoice = promptSave();

        switch (usrChoice){
            case QMessageBox::Save :
                saveFile();
                break;
            case QMessageBox::Cancel :
                return;
                break;
            case QMessageBox::Discard :
                break;
        }
    }
    currentFile.clear();

    setExplicitModify(true);
    ui->textEdit->setText(QString());
    setExplicitModify(false);

}

void Notepad::saveFile()
{
    QString fileName;
    if(currentFile.isEmpty()){
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    }
    else{
        fileName = currentFile;
    }

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::saveAsFile()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "Save");

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::browse()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();

    setExplicitModify(true);
    ui->textEdit->setText(text);
    setExplicitModify(false);

    file.close();
}

void Notepad::undo()
{
    ui->textEdit->undo();
}

void Notepad::redo()
{
    ui->textEdit->redo();
}

void Notepad::paste()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->paste();
#endif
}

void Notepad::copy()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->copy();
#endif
}

void Notepad::cut()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->cut();
#endif
}

int Notepad::promptSave()
{
    QMessageBox savePrompt;

    savePrompt.setIcon(QMessageBox::Warning);
    savePrompt.setText("This file has been modified.");
    savePrompt.setInformativeText("Do you want to save your changes?");
    savePrompt.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    savePrompt.setDefaultButton(QMessageBox::Save);

    return savePrompt.exec();
}

bool Notepad::isModified()
{
    return bModified;
}

void Notepad::setModified()
{
    bModified = bExplicitModify^1;
}

void Notepad::setExplicitModify(bool bExplicitModify)
{
    this->bExplicitModify = bExplicitModify;
}

void Notepad::closeEvent(QCloseEvent *event)
{
    if(isModified()){
        int usrChoice = promptSave();

        switch (usrChoice){
            case QMessageBox::Save :
                saveFile();
                break;
            case QMessageBox::Cancel :
                return;
                break;
            case QMessageBox::Discard :
                break;
        }
    }
    QMainWindow::closeEvent(event);
}


