#ifndef DEF_CLIENT
#define DEF_CLIENT

#include <string>

class ESPClient
{
    public:

    ESPClient(std::string new_MacAdress);
    std::string getMacAdress();
    std::string getIP();
    void setIP(std::string new_IP);
    float getTempMeasure();
    void setTempMeasure(float new_weight_measure);
    float getWeightMeasure();
    void setWeightMeasure(float new_weight_measure);
    bool getIsMeasureSend();
    void setIsMeasureSend(bool new_isMeasureSend);

    private:

    std::string MacAdress;
    std::string IP;
    float TempMeasure;
    float WeightMeasure;
    bool isMeasureSend;
};

#endif
