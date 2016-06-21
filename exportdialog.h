#ifndef EXPORTDIALOG
#define EXPORTDIALOG

#include <QtWidgets>

class ExportDialog : public QDialog{
    Q_OBJECT
public:
    ExportDialog(int chartCount, QWidget *parent = 0);
    ~ExportDialog();

signals:
    void exportFiles(QHash< int, QList<QString> >);

private:
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void browseClicked();
    void executeClicked();

private:
    QLineEdit *dataDefineEdit;
    QPushButton *dataDefineButton;
    QList<QLineEdit *> chartEditList;
    QList<QPushButton *> chartButtonList;
    QPushButton *executeButton;
    QHash< int, QList<QString> > fileHash;
    int m_chartCount;
};


#endif // EXPORTDIALOG

