#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUdpSocket>

class Mainwindow : public QObject
{
    Q_OBJECT
public:
    explicit Mainwindow(QObject *parent = nullptr);

private:
    QUdpSocket * sock_ipuDataReceive;
    QString ieuIp;
    int ieuRecvPort;
    int sendBackPort;



public:
    void ieuDataReceived();
    void dataValidation(QByteArray ipuReceivedData);
    quint8 slotComputeChksum(const quint8 *data, size_t length);
    void storeData(QString ssid, quint8 identifier, QByteArray ipuReceivedData);
};

#endif // MAINWINDOW_H
