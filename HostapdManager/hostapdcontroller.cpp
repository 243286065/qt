#include "hostapdcontroller.h"

#include "aes_encryptor.h"

#include <qdir.h>
#include <memory.h>
#include <qdebug.h>

unsigned char kEncodeKey[] = "maxhub";
const char kHostapdConfigFile[] = "hostapd.conf";

HostapdController::HostapdController():_configFile(QString(kHostapdConfigFile))
{}

HostapdController::HostapdController(const QString& configFilePath):_configFile(configFilePath)
{}

bool HostapdController::GenerateConfig(const QString& wifi_ssid, const QString& wifi_passwd, const QString& interface, const QString& drviceType) {
    // First delete old config file
    QDir floder;
    if(floder.exists(_configFile)){
        QFile::remove(_configFile);
    }

    // Create new config file
    VendorElement vendor;
    vendor.element_id = 0xdd;
    vendor.oui[0] = 0x11;
    vendor.oui[1] = 0x22;
    vendor.oui[2] = 0x33;
    vendor.oui_type = 0x02;

    //QString message = QString("%1###%2").arg(wifi_ssid).arg(wifi_passwd);
    AesEncryptor encoder(kEncodeKey);
    auto messageEncode = encoder.EncryptString(wifi_passwd.toStdString());
    qDebug() << messageEncode.c_str();

    vendor.length = sizeof(vendor.oui) + sizeof(vendor.oui_type) + messageEncode.size()/2;

    memset(vendor.data, 0, sizeof(vendor.data));
    strcpy((char*)vendor.data, messageEncode.c_str());

    char vendorBuf[1024] = {0};
    sprintf(vendorBuf, "%02x%02x%02x%02x%02x%02x%s", vendor.element_id, vendor.length, vendor.oui[0], vendor.oui[1], vendor.oui[2], vendor.oui_type, (char*)vendor.data);
    qDebug() << vendorBuf;

    // Write hostapd coinfig to file
    QFile file(_configFile);
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }

    //写入文件
    QTextStream out(&file);
    out << "interface=" + interface << endl;
    out << "driver=" + drviceType << endl;
    out << "ssid=" + wifi_ssid << endl;
    out << "hw_mode=g" << endl;
    out << "channel=7" << endl;
    out << "macaddr_acl=0" << endl;
    out << "auth_algs=3" << endl;
    out << "ignore_broadcast_ssid=0" << endl;
    out << "wmm_enabled=1" << endl;
    out << "ieee80211n=1" << endl;
    out << "ht_capab=[HT40-][SHORT-GI-20][SHORT-GI-40]" << endl;
    out << "wpa=2" << endl;
    out << "wpa_passphrase=" + wifi_passwd << endl;
    out << "wpa_key_mgmt=WPA-PSK" << endl;
    out << "wpa_pairwise=TKIP" << endl;
    out << "rsn_pairwise=CCMP" << endl;
    out << "vendor_elements=" + QString(vendorBuf) << endl;
    //qDebug() << content;
    file.close();
    qDebug()<<"关闭文件";
    return true;
}

bool HostapdController::RunHostapd() {
    StopHostapd();
    char bufCmd[1024] = {0};
    sprintf(bufCmd, "hostapd -B %s", _configFile.toStdString().c_str());
    return system(bufCmd);
}

void HostapdController::StopHostapd() {
    system("killall hostapd");
}
