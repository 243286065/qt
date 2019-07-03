1. 安装hostapd
```
sudo apt install hostapd
```

2. 配置dhcp，否者热点无法连接
安装isc-dhcp-server
```
sudo apt-get install isc-dhcp-server
```

编辑配置文件`/etc/default/isc-dhcp-server`,将`INTERFACES`设为无线网卡名字，无线网卡名字可以通过`iwconfig`命令查看：
```
INTERFACES="wlan0"
```
然后编辑`/etc/dhcp/dhcpd.conf`，在文本最后添加
```
subnet 10.5.5.0 netmask 255.255.255.0 {
  range 10.5.5.26 10.5.5.30;
  option domain-name-servers 8.8.8.8;
  option routers 10.5.5.1;
  option broadcast-address 10.5.5.255;
  default-lease-time 600;
  max-lease-time 7200;
}
```
注意`10.5.5.0`是我定义的我的热点的网段，你如需更改请将配置里的ip池和网关地址、广播地址一起修改。

然后启动后dhcp server:
```
sudo dhcpd

```

3. 发射热点
首先我们需要执行以下命令:
```
sudo nmcli radio wifi off && rfkill unblock wlan
sudo ifconfig wlan0 10.5.5.1/24 up
```
注意wlan0名字和你上面配置的网卡名一致。

然后就可以运行我们的这个程序。注意interface名称和上面的网卡名一致。

本质这个程序是为了自动生成hostapd.conf而写的，最后还是调用`hostapd -B hostapd.conf`命令启动热点。

注意：并不是所有的网卡都支持hostapd开启热点。

若要打包这个程序，可参考： https://www.cnblogs.com/xl2432/p/11124415.html