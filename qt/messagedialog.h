#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

class QLineEdit;
class QPlainTextEdit;
class QPushButton;

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = 0);
    ~MessageDialog();

    void SetFrom(const QString & qsFrom);
    QString GetFrom() const;

    void SetCertID(const QString & qsCertID);
    QString GetCertID() const;

    void SetMessage(const QString & qsMessage);
    QString GetMessage() const;

    void SetCert(const QString & qsCert);
    QString GetCert() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::MessageDialog *ui;

    QLineEdit * m_leFrom;
    QLineEdit * m_leCertID;
    QPlainTextEdit * m_ptxtMessage;
    QPlainTextEdit * m_ptxtCert;
};
#endif // MESSAGEDIALOG_H
