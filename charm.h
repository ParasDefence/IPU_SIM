#ifndef CHARM_H
#define CHARM_H


struct RecvHeader{
    quint8 header;      // B0
    quint8 ssid;        // B1
    quint8 reserved0;   // B2
    quint8 datalength;  // B3
    quint8 identifier;  // B4
};

struct PcktHeader{
    quint8 header;      // B0
    quint8 ssid;        // B1
    quint8 reserved0;    // B2
    quint8 datalength;    // B3
    //        quint8 identifier;    // B4
};

struct PcktContrast{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 reserved1;    // B5
    quint8 reserved2;    // B6
    quint8 firstDecimal;    // B7
    quint8 secondDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktEdge{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 reserved1;    // B5
    quint8 reserved2;    // B6
    quint8 firstDecimal;    // B7
    quint8 secondDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktVideoRes{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 cmd;    // B5
    quint8 chksum;    // B6
};

struct PcktCenterofRotX{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};


struct PcktCenterofRotY{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};


struct PcktPreRotX{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktPreRotY{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktRotY{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktRotX{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktImgStab{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 cmd;    // B5
    quint8 chksum;    // B6
};

struct PcktImgFusionAlgo{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktAlgoMode{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};


struct PcktImgFusionWeight{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 reserved0;    // B5
    quint8 reserved1;    // B6
    quint8 value1;    // B7
    quint8 value2;    // B8
    quint8 chksum;    // B9
};

struct PcktImgFusion{
    PcktImgFusionAlgo fusionAlgo;
    PcktImgFusionWeight fusionWeight;
};

struct PcktDetStat{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktWinAcq{
    PcktHeader headerpckt;
    quint8 xlen;    // B4
    quint8 xFirst;    // B5
    quint8 xSecond;    // B6
    quint8 yLen;    // B7
    quint8 yFirst;    // B8
    quint8 ySecond;  //B9
    quint8 chksum;    // B10
};

struct PcktPosWinAcq{
    PcktHeader headerpckt;
    quint8 xlen;    // B4
    quint8 xFirst;    // B5
    quint8 xSecond;    // B6
    quint8 yLen;    // B7
    quint8 yFirst;    // B8
    quint8 ySecond;  //B9
    quint8 chksum;    // B10
};

struct PcktDetPrority{
    PcktHeader headerpckt;
    quint8 priorMetric;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktDetPolarity{
    PcktHeader headerpckt;
    quint8 polarity;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktDetSymbology{
    PcktHeader headerpckt;
    quint8 symbologyId;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktDetectionData{
    PcktDetStat  detStat;
    PcktWinAcq   winAcq;
    PcktPosWinAcq posWinAcq;
    PcktImgFusionAlgo detAlgo;
    PcktDetPrority priority;
    PcktDetPolarity polarity;
    PcktDetSymbology symbology;
};


struct PcktRotationValue{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktTrackLockData{
    PcktHeader headerpckt;
    quint8 TrackAq;    // B4
    quint8 TrackStart;    // B5
    quint8 chksum;    // B6
};


struct PcktTrackID{
    PcktHeader headerpckt;
    quint8 trackMode;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};


struct PcktTrackingData{
    PcktHeader headerpckt;
    quint8 trackAlgo;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktTranslationXValue{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktTranslationYValue{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct TrackData {
    QString id;
    int x;
    int y;
    int sizex;
    int sizey;
}structTrackData;
QList<TrackData> trackList;

struct PcktDataToCharm{
    PcktHeader headerpckt;
    quint8 identifier1;    // B4
    quint8 identifier2;    // B5
};

struct PcktSlewAlphaData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};


struct PcktRelaxAlphaData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktSlewMagData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktStabAlphaData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktBorderXData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};


struct PcktBorderYData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktFilterWrap{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktResetWrap{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 value;    // B5
    quint8 chksum;    // B6
};

struct PcktEstabZoomData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktOffsetData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 firstDecimal;    // B5
    quint8 secondDecimal;    // B6
    quint8 thirdDecimal;    // B7
    quint8 fourthDecimal;    // B8
    quint8 chksum;    // B9
};

struct PcktBoostData{
    PcktHeader headerpckt;
    quint8 identifier;    // B4
    quint8 reserved0;    // B5
    quint8 reserved1;    // B6
    quint8 value1;    // B7
    quint8 value2;    // B8
    quint8 chksum;    // B9
};


struct FeedBackTrackData
{
    qint32 trackID;
    qint32 xPos;
    qint32 yPos;
    qint32 xSize;
    qint32 ySize;
    int    status;
    int    track_conf;
    qint32 time;
    qint32 prior;
};

struct PcktStatus
{
    quint8 header;      // B0
    quint8 ssid;        // B1
    quint8 status;      // B2
    quint8 datalength;  // B3
    quint8 chksum;      // B4
};




#endif // CHARM_H
