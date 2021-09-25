#ifndef AWSIOT_H
#define AWSIOT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include <string>

using std::string;

class AwsIot
{
private:
    WiFiClientSecure net = WiFiClientSecure();
    MQTTClient client = MQTTClient();
    string aws_cert_ca;
    string aws_cert_crt;
    string aws_cert_private;
    string aws_iot_endpoint;
    string device_name;
    int aws_max_reconnect_tries;

public:
    AwsIot(string aws_cert_ca, string aws_cert_crt, string aws_cert_private, string aws_iot_endpoint, string device_name, int aws_max_reconnect_tries = 50);
    bool connect();
    void publishMessage(StaticJsonDocument<512> json, string aws_iot_topic);
};

#endif