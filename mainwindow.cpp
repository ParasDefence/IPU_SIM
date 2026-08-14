#include "mainwindow.h"
#include <QSettings>
#include "charm.h"

PcktWinAcq        winAcq;
PcktPosWinAcq     posWinAcq;
PcktDetPolarity   detPolarity;
PcktDetPrority    detPriority;
PcktTrackLockData trackLockData;
PcktTrackID       trackID;
PcktTrackingData  trackingData;
PcktDataToCharm   dataToCharm;
PcktFilterWrap    filterWrap;
PcktResetWrap     resetWrap;
PcktAlgoMode      algoMode;

PcktDetStat stat;
PcktDetSymbology symb;

PcktVideoRes videoRes;

PcktImgStab        stab;
PcktSlewAlphaData   slewAlphaData;
PcktRelaxAlphaData  relaxAlphaData;
PcktSlewMagData     slewMagData;
PcktStabAlphaData   stabAlphaData;
PcktBorderXData     borderXData;
PcktBorderYData     borderYData;
PcktEstabZoomData   estabZoomData;

PcktContrast   contrast;
PcktOffsetData offsetData;
PcktBoostData  boostData;

PcktEdge  edge;

PcktCenterofRotX     centerOfRotX;
PcktCenterofRotY     centerOfRotY;
PcktPreRotX          preRotX;
PcktPreRotY          preRotY;
PcktRotX             rotX;
PcktRotY             rotY;
PcktImgFusionAlgo    imgFusionAlgo;
PcktImgFusionWeight  imgFusionWeight;
PcktRotationValue    rotationValue;
PcktTranslationXValue translationXValue;
PcktTranslationYValue translationYValue;



Mainwindow::Mainwindow(QObject *parent) : QObject(parent) {

    QSettings setting("/home/paras/Projects/S2/IPUSim/Settings.ini", QSettings::IniFormat);

    ieuRecvPort = setting.value("IP/IEUPORT").toInt();
    fusionRecvPort = setting.value("IP/FUSIONPORT").toInt();
    ieuIp = setting.value("IP/IEUIP").toString();
    sendBackPort = setting.value("IP/SENDPORT").toInt();

    qDebug()<<"ieuPort "<<ieuRecvPort<<"ieuIp "<<ieuIp;

    sock_ipuDataReceive = new QUdpSocket(this);
    if (!sock_ipuDataReceive->bind(QHostAddress::AnyIPv4, ieuRecvPort)) {
        printf("CHARM: Binding Problem to receiver Error socket\n");
        fflush(stdout);
    }
    connect(sock_ipuDataReceive, &QUdpSocket::readyRead, this, &Mainwindow::ieuDataReceived, Qt::DirectConnection);

    sock_fusionDataReceive = new QUdpSocket(this);
    if (!sock_fusionDataReceive->bind(QHostAddress::AnyIPv4, fusionRecvPort)) {
        printf("CHARM: Binding Problem to fusionPort Error socket\n");
        fflush(stdout);
    }
    connect(sock_fusionDataReceive, &QUdpSocket::readyRead, this, &Mainwindow::fusionDataReceived, Qt::DirectConnection);
}

void Mainwindow::ieuDataReceived() {
    while (sock_ipuDataReceive->hasPendingDatagrams()) {
        QByteArray ipuReceivedData;
        ipuReceivedData.resize(sock_ipuDataReceive->pendingDatagramSize());
        sock_ipuDataReceive->readDatagram(ipuReceivedData.data(), sock_ipuDataReceive->pendingDatagramSize());
        QString str = QString(ipuReceivedData.toHex('-'));
        qDebug() << "Data: " << str;
        Q_UNUSED(str);

        dataValidation(ipuReceivedData);

    }
}


void Mainwindow::fusionDataReceived() {
    while (sock_fusionDataReceive->hasPendingDatagrams()) {
        QByteArray fusionReceivedData;
        fusionReceivedData.resize(sock_fusionDataReceive->pendingDatagramSize());
        sock_fusionDataReceive->readDatagram(fusionReceivedData.data(), sock_fusionDataReceive->pendingDatagramSize());
        QString str = QString(fusionReceivedData.toHex('-'));
        qDebug() << "Fusion Data: " << str;
        Q_UNUSED(str);

        dataValidation(fusionReceivedData);

    }
}


void Mainwindow::dataValidation(QByteArray ipuReceivedData)
{
    RecvHeader header;
    memcpy(&header, ipuReceivedData.constData(), sizeof(RecvHeader));

//    qDebug() << "SSID" << QString::number(header.ssid, 16);
    PcktStatus statusPckt;
    statusPckt.header     = header.header;
    statusPckt.ssid       = header.ssid;
    statusPckt.status     = 0x01;  //Validating
    statusPckt.datalength = header.datalength;

    const quint8 *rawBytes = reinterpret_cast<const quint8 *>(&header);
    statusPckt.chksum     = slotComputeChksum(rawBytes , sizeof(PcktStatus));

    QString ssid = "0x" + QString("%1").arg(header.ssid, 2, 16, QChar('0')).toUpper();
//    QString identifier = "0x" + QString("%1").arg(header.identifier, 2, 16, QChar('0')).toUpper();

    storeData(ssid,header.identifier,ipuReceivedData);

    QByteArray statusBytes(reinterpret_cast<const char*>(&statusPckt), sizeof(PcktStatus));

    qDebug() << "Status packet: " << statusBytes.toHex('-');

    if(!sock_ipuDataReceive->writeDatagram(statusBytes,  QHostAddress(ieuIp), sendBackPort)){
        qDebug("UNABLE TO SEND DATA");
    }

}

quint8 Mainwindow::slotComputeChksum(const quint8 *data, size_t length) {
    quint8 checksum = 0;
    if (length == 0) return checksum;
    for (size_t i = 0; i < length - 1; i++) {
        checksum += data[i];
    }
    return checksum;
}


void Mainwindow::storeData( QString ssid , quint8 identifier, QByteArray ipuReceivedData) {
    enum Ssid : quint8 {
        NONE = 0,
        OBJ_LOCATION= 1,
        PANTILT_CONTROL= 2,
        VIDEO= 3,
        CAMERA= 4,
        IMGSTAB= 5,
        CONT= 6,
        EDGE= 8,
        FUSION= 9,
    };
    static QHash<QString, Ssid> SsidMap = {{"0x00", OBJ_LOCATION},
                                           {"0x01", PANTILT_CONTROL},
                                           {"0x02", VIDEO},
                                           {"0x03", CAMERA},
                                           {"0x0C", IMGSTAB},
                                           {"0x21", CONT},
                                           {"0x22", EDGE},
                                           {"0x70", FUSION},
                                           };

    qDebug()<<"ssid "<<ssid<<" identifier "<<identifier;

    switch (SsidMap[ssid]) {

    case OBJ_LOCATION: {


        if (identifier == 0x12) {
            memcpy(&winAcq, ipuReceivedData.constData(), sizeof(PcktWinAcq));

        } else if (identifier == 0x10) {
            memcpy(&posWinAcq, ipuReceivedData.constData(), sizeof(PcktPosWinAcq));


        } else if (identifier == 0x05) {
            memcpy(&detPolarity, ipuReceivedData.constData(), sizeof(PcktDetPolarity));


        } else if (identifier == 0x04) {
            memcpy(&detPriority, ipuReceivedData.constData(), sizeof(PcktDetPrority));


        } else if (identifier == 0x00) {
            memcpy(&trackLockData, ipuReceivedData.constData(), sizeof(PcktTrackLockData));


        } else if (identifier == 0x2B) {
            memcpy(&trackID, ipuReceivedData.constData(), sizeof(PcktTrackID));


        } else if (identifier == 0x0F) {
            memcpy(&trackingData, ipuReceivedData.constData(), sizeof(PcktTrackingData));


        } else if (identifier == 0xE5) {
            memcpy(&dataToCharm, ipuReceivedData.constData(), sizeof(PcktDataToCharm));


        } else if (identifier == 0x02) {
            memcpy(&filterWrap, ipuReceivedData.constData(), sizeof(PcktFilterWrap));


        } else if (identifier == 0x03) {
            memcpy(&resetWrap, ipuReceivedData.constData(), sizeof(PcktResetWrap));


        } else if (identifier == 0x0E) {
            memcpy(&algoMode, ipuReceivedData.constData(), sizeof(PcktAlgoMode));

        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    case PANTILT_CONTROL: {
        break;
    }

    case VIDEO: {

        if (identifier == 0x21) {
        memcpy(&symb, ipuReceivedData.constData(), sizeof(PcktDetSymbology));
        }
        else if (identifier == 0x00) {
        memcpy(&stat, ipuReceivedData.constData(), sizeof(PcktDetStat));
        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    case CAMERA: {
        if (identifier == 0x01) {
        memcpy(&videoRes, ipuReceivedData.constData(), sizeof(PcktVideoRes));
        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    case IMGSTAB: {

        if (identifier == 0x00) {
        memcpy(&stab, ipuReceivedData.constData(), sizeof(PcktImgStab));

        } else if (identifier == 0x11) {
        memcpy(&slewAlphaData, ipuReceivedData.constData(), sizeof(PcktSlewAlphaData));

        } else if (identifier == 0x12) {
        memcpy(&relaxAlphaData, ipuReceivedData.constData(), sizeof(PcktRelaxAlphaData));

        } else if (identifier == 0x13) {
        memcpy(&slewMagData, ipuReceivedData.constData(), sizeof(PcktSlewMagData));

        } else if (identifier == 0x10) {
        memcpy(&stabAlphaData, ipuReceivedData.constData(), sizeof(PcktStabAlphaData));

        } else if (identifier == 0x08) {
        memcpy(&borderXData, ipuReceivedData.constData(), sizeof(PcktBorderXData));

        } else if (identifier == 0x09) {
        memcpy(&borderYData, ipuReceivedData.constData(), sizeof(PcktBorderYData));

        } else if (identifier == 0x04) {
        memcpy(&estabZoomData, ipuReceivedData.constData(), sizeof(PcktEstabZoomData));

        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    case CONT: {

        if (identifier == 0x00) {
        memcpy(&contrast, ipuReceivedData.constData(), sizeof(PcktContrast));

        } else if (identifier == 0x01) {
        memcpy(&offsetData, ipuReceivedData.constData(), sizeof(PcktOffsetData));

        } else if (identifier == 0x02) {
        memcpy(&boostData, ipuReceivedData.constData(), sizeof(PcktBoostData));

        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    case EDGE: {

        if (identifier == 0x03) {
        memcpy(&edge, ipuReceivedData.constData(), sizeof(PcktEdge));

        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    case FUSION: {



        if (identifier == 0x10) {
        memcpy(&centerOfRotX, ipuReceivedData.constData(), sizeof(PcktCenterofRotX));


        } else if (identifier == 0x11) {
        memcpy(&centerOfRotY, ipuReceivedData.constData(), sizeof(PcktCenterofRotY));


        } else if (identifier == 0x12) {
        memcpy(&preRotX, ipuReceivedData.constData(), sizeof(PcktPreRotX));


        } else if (identifier == 0x13) {
        memcpy(&preRotY, ipuReceivedData.constData(), sizeof(PcktPreRotY));


        } else if (identifier == 0x16) {
        memcpy(&rotX, ipuReceivedData.constData(), sizeof(PcktRotX));


        } else if (identifier == 0x15) {
        memcpy(&rotY, ipuReceivedData.constData(), sizeof(PcktRotY));


        } else if (identifier == 0x00) {
        memcpy(&imgFusionAlgo, ipuReceivedData.constData(), sizeof(PcktImgFusionAlgo));


        } else if (identifier == 0x01) {
        memcpy(&imgFusionWeight, ipuReceivedData.constData(), sizeof(PcktImgFusionWeight));


        } else if (identifier == 0x14) {
        memcpy(&rotationValue, ipuReceivedData.constData(), sizeof(PcktRotationValue));


        } else if (identifier == 0x17) {
        memcpy(&translationXValue, ipuReceivedData.constData(), sizeof(PcktTranslationXValue));


        } else if (identifier == 0x18) {
        memcpy(&translationYValue, ipuReceivedData.constData(), sizeof(PcktTranslationYValue));


        }
        else{
        qDebug()<<"invalid Identifier";
        }
        break;
    }

    default:
        break;
    }
}

