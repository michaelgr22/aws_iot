#include "aws_iot.h"

AwsIot::AwsIot(string aws_cert_ca, string aws_cert_crt, string aws_cert_private, string aws_iot_endpoint, string device_name, int aws_max_reconnect_tries)
    : aws_cert_ca(aws_cert_ca), aws_cert_crt(aws_cert_crt), aws_cert_private(aws_cert_private), aws_iot_endpoint(aws_iot_endpoint),
      device_name(device_name), aws_max_reconnect_tries(aws_max_reconnect_tries) {}

AwsIot::~AwsIot()
{
    delete client;
}

bool AwsIot::connect()
{
    net.setCACert(aws_cert_ca.c_str());
    net.setCertificate(aws_cert_crt.c_str());
    net.setPrivateKey(aws_cert_private.c_str());

    // Connect to the MQTT broker on the AWS endpoint
    client = new PubSubClient(aws_iot_endpoint.c_str(), 8883, net);

    int retries = 0;

    while (!client->connect(device_name.c_str()) && retries < aws_max_reconnect_tries)
    {
        delay(100);
        retries++;
    }

    if (!client->connected())
    {
        return false;
    }

    return true;
}

string AwsIot::publish(StaticJsonDocument<512> json, string aws_iot_topic)
{
    if (connected() || connect())
    {
        return publishMessage(json, aws_iot_topic);
    }
    return "{}";
}

bool AwsIot::refresh()
{
    return client->loop();
}

bool AwsIot::connected()
{
    return client->connected();
}

string AwsIot::publishMessage(StaticJsonDocument<512> json, string aws_iot_topic)
{
    char jsonBuffer[512];
    serializeJson(json, jsonBuffer);
    if (client->publish(aws_iot_topic.c_str(), jsonBuffer))
        return string(jsonBuffer);
    return "{}";
}
