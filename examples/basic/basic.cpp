#include <WiFi.h>
#include <aws_iot.h>

void connectToWifi()
{
    //..connect to Wifi
}

void setup()
{
    connectToWifi();
    AwsIot awsIot = AwsIot(aws_cert_ca, aws_cert_crt, aws_cert_private, aws_iot_endpoint, device_name, aws_max_reconnect_tries);
    awsIot.connect()
}

void loop()
{
    StaticJsonDocument<512> json;
    JsonObject stateObj = json.createNestedObject("state");
    JsonObject reportedObj = stateObj.createNestedObject("reported");
    reportedObj["voltage"] = 13.0;

    awsIot.publishMessage(json, aws_iot_topic);
}