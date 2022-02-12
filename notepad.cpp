#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Notepad)
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
    connect(ui->exitButton, &QPushButton::clicked, this, &Notepad::exit);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Notepad::setModified);
}


Notepad::~Notepad()
{
    delete ui;
}

void Notepad::newFile()
{
    if(!isModified()){
        currentFile.clear();
        ui->textEdit->setText(QString());
    }
    else{
        //current file has undergone edits, prompt user for a save
    }
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
    ui->textEdit->setText(text);
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

void Notepad::promptSave()
{

}

bool Notepad::isModified()
{
    return bModified;
}

void Notepad::setModified()
{
    bModified = true;
}

void Notepad::exit()
{
    QCoreApplication::quit();
}


