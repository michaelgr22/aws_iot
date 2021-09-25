#include "aws_iot.h"

AwsIot::AwsIot(string aws_cert_ca, string aws_cert_crt, string aws_cert_private, string aws_iot_endpoint, string device_name, int aws_max_reconnect_tries)
    : aws_cert_ca(aws_cert_ca), aws_cert_crt(aws_cert_crt), aws_cert_private(aws_cert_private), aws_iot_endpoint(aws_iot_endpoint),
      device_name(device_name), aws_max_reconnect_tries(aws_max_reconnect_tries) {}

bool AwsIot::connect()
{
    // Configure WiFiClientSecure to use the AWS certificates we generated
    net.setCACert(aws_cert_ca.c_str());
    net.setCertificate(aws_cert_crt.c_str());
    net.setPrivateKey(aws_cert_private.c_str());

    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    client.begin(aws_iot_endpoint.c_str(), 8883, net);

    // Try to connect to AWS and count how many times we retried.
    int retries = 0;
    Serial.print("Connecting to AWS IOT");

    while (!client.connect(device_name.c_str()) && retries < aws_max_reconnect_tries)
    {
        Serial.print(".");
        delay(100);
        retries++;
    }

    // Make sure that we did indeed successfully connect to the MQTT broker
    // If not we just end the function and wait for the next loop.
    if (!client.connected())
    {
        Serial.println(" Timeout!");
        return false;
    }

    // If we land here, we have successfully connected to AWS!
    // And we can subscribe to topics and send messages.
    Serial.println("Connected!");
    return true;
}

void AwsIot::publishMessage(StaticJsonDocument<512> json, string aws_iot_topic)
{
    char jsonBuffer[512];
    serializeJson(json, jsonBuffer);
    Serial.println("Publish:");
    Serial.println(jsonBuffer);
    client.publish(aws_iot_topic.c_str(), jsonBuffer);
}
