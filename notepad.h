#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

private slots:
    void newFile();

    void saveFile();

    void saveAsFile();

    void browse();

    void undo();

    void redo();

    void paste();

    void copy();

    void cut();

    void promptSave();

    bool isModified();

    void setModified();

    void exit();
private:
    Ui::Notepad *ui;
    QString currentFile;
    bool bModified;
};
#endif // NOTEPAD_H
