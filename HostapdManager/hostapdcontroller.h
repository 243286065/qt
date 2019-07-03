#ifndef HOSTAPDCONTROLLER_H
#define HOSTAPDCONTROLLER_H

#include <string>
#include <qstring.h>

struct VendorElement{
    uint8_t element_id;
    uint8_t length;
    uint8_t oui[3];
    uint8_t oui_type;
    uint8_t data[256];
};

class HostapdController
{
public:
    HostapdController();
    HostapdController(const QString& configFilePath);

    bool GenerateConfig(const QString& wifi_ssid, const QString& wifi_passwd, const QString& interface, const QString& drviceType);
    bool RunHostapd();
    void StopHostapd();
private:
    QString _configFile;
};

#endif // HOSTAPDCONTROLLER_H
