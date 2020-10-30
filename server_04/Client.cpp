#include "Client.h"

using namespace std;

ESPClient::ESPClient(std::string new_MacAdress) 
{
  MacAdress = new_MacAdress;
}

std::string ESPClient::getMacAdress()
{
    return MacAdress;
}

std::string ESPClient::getIP()
{
    return IP;
}

void ESPClient::setIP(std::string new_IP)
{
    IP = new_IP;
}

float ESPClient::getTempMeasure()
{
    return TempMeasure;
}

void ESPClient::setTempMeasure(float new_temp_measure)
{
    TempMeasure = new_temp_measure;
}

float ESPClient::getWeightMeasure()
{
    return WeightMeasure;
}

void ESPClient::setWeightMeasure(float new_weight_measure)
{
    WeightMeasure = new_weight_measure;
}

bool ESPClient::getIsMeasureSend()
{
    return isMeasureSend;
}

void ESPClient::setIsMeasureSend(bool new_isMeasureSend)
{
    isMeasureSend = new_isMeasureSend;
}
