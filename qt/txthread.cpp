#include "txthread.h"
#include <QDebug>
#include <bitmailcore/bitmail.h>

static
int TxProgressHandler(RTxState st, const char * info, void * userp);

TxThread::TxThread(BitMail * bm)
    : m_bitmail(bm)
    , m_txq(1000)
    , m_fStopFlag(false)
{

}

TxThread::~TxThread()
{

}

void TxThread::onSendMessage(const QString & from
                             , const QStringList & recip
                             , const QString & content)
{
    RTXMessage txMsg;
    txMsg.rtx(true);
    txMsg.from(from);
    txMsg.recip(recip);
    txMsg.content(content);
    txMsg.cert("");
    txMsg.certid("");
    if (m_txq.writable(25/*milliseconds*/)){
        m_txq.push(txMsg);
    }else{
        qDebug() << "Tx Queue Overflow, missing message.";
    }
}

void TxThread::run()
{    
    while(!m_fStopFlag){
        if (m_txq.readable(6*1000)){
            RTXMessage msg = m_txq.pop();
            QStringList qslTo = msg.recip();
            std::vector<std::string> vecTo;
            for (QStringList::iterator it = qslTo.begin(); it != qslTo.end(); it++){
                vecTo.push_back(it->toStdString());
            }
            QString qsContent = msg.content();
            if (m_bitmail){
                m_bitmail->SendMsg(vecTo
                                   , qsContent.toStdString()
                                   , TxProgressHandler
                                   , this);
            }

        }
    }

    emit done();
}

void TxThread::stop()
{
    m_fStopFlag = true;
}

void TxThread::NotifyTxProgress(const QString &info)
{
    emit txProgress(info);
}

int TxProgressHandler(RTxState st, const char * info, void * userp)
{
    (void)st;
    TxThread * self = (TxThread *)userp;
    if (self == NULL){
        return bmInvalidParam;
    }
    self->NotifyTxProgress(QString::fromLatin1(info));
    return bmOk;
}
