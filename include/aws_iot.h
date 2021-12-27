#ifndef AWSIOT_H
#define AWSIOT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string>

using std::string;

class AwsIot
{
private:
    WiFiClientSecure net = WiFiClientSecure();
    PubSubClient *client;
    string aws_cert_ca;
    string aws_cert_crt;
    string aws_cert_private;
    string aws_iot_endpoint;
    string device_name;
    int aws_max_reconnect_tries;

    string publishMessage(StaticJsonDocument<512> json, string aws_iot_topic);

public:
    AwsIot(string aws_cert_ca, string aws_cert_crt, string aws_cert_private, string aws_iot_endpoint, string device_name, int aws_max_reconnect_tries = 50);
    ~AwsIot();
    bool connect();
    string publish(StaticJsonDocument<512> json, string aws_iot_topic);
    bool connected();
    bool refresh();
};

#endif