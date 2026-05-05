project hardware use:
* OLED SH1106 I2C adress 3C
* ESP32-C3 
* INA219  I2C adress 41
* INA219  I2C adress 44
* USB type-c charger
* USB charger

first INA219 monitoring the USB type-c charger
second INA219 monitoring the USB charger
use example oled_screen_template.png to show data on OLED screen 

INA219 are conected to the Vout of each charger.


make a web page to monitor all data like on the OLED.
use websoket to send data
make a AP with Wi-Fi Manager
use ElegantOTA library for OTA
at boot start wifi manager (web server at 192.168.4.1) and set as current config tool if no wifi setup.
if no wifi setup by the user in 3 minutes activate AP with the local webpage to monitor data.
make a boot screen whereto show  my brandin info :
"USB Charger
by E-gadget Iasi"
on the next line wifi status then IP addres of the device. 
use template for boot screen : boot_screen_template.png

Setup INA219 to the fastet sampling rate. send data to the web page in the fastest way. 
make the web part code on a separate file "index_html.h" use base64 encoding to atach the favicon.png in the HTML. 
use favicon.png for that. 
create "firmware" directory and use to save all code files


