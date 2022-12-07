#include <iostream>
#include <libusb-1.0/libusb.h>
#include <map>

using namespace std;

void printdev(libusb_device *dev);

int main() {
    libusb_device **devs; // указатель на указатель на устройство,
    // используется для получения списка устройств
    libusb_context *ctx = NULL; // контекст сессии libusb
    int r; // для возвращаемых значений
    ssize_t cnt; // число найденных USB-устройств

    // инициализировать библиотеку libusb, открыть сессию работы с libusb
    r = libusb_init(&ctx);
    if (r < 0) {
        cerr << "Ошибка: инициализация не выполнена, код:\n" << r << endl;
        return 1;
    }
    // получить список всех найденных USB- устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        cerr << "Ошибка: список USB устройств не получен.\n" << endl;
        return 1;
    }
    cout<<"Найдено устройств"<<cnt<<endl;
    ssize_t i; // индексная переменная цикла перебора всех устройств
    for (i = 0; i < cnt; i++) // цикл перебора всех устройств
        printdev(devs[i]); // печать параметров устройства

    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx); // завершить работу с библиотекой libusb,
    // закрыть сессию работы с libusb
    return 0;
}

void printdev(libusb_device *dev) {
    map<int, string> classes = {   //основные классы устройств
            {0,   "Code Missing (0)"},
            {1,   "Audio device"},
            {2,   "Network adapter"},
            {3,   "User Interface Device"},
            {5,   "Physical Device"},
            {6,   "Images"},
            {7,   "Printer"},
            {8,   "Storage"},
            {9,   "Concentrator"},
            {10,  "CDC-Data"},
            {11,  "Smart Card"},
            {13,  "Content Security"},
            {14,  "Video device"},
            {15,  "Personal Medical Device"},
            {16,  "Audio and video devices"},
            {220, "Diagnostic Device"},
            {224, "Wireless Controller"},
            {239, "Various Devices"},
            {254, "Specific device"},
    };
    libusb_device_descriptor desc; // дескриптор устройства
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        cerr << "Ошибка: дескриптор устройства не получен, код:\n" << r << std::endl;
        return;
    }
    int deviceClass = static_cast<int>(desc.bDeviceClass);
    if(classes.count(deviceClass)>0)
        cout << "Devise class: " << classes[deviceClass] << endl;
    else
        cout << "Devise class is unkown" << endl;
    cout << "Vendor id: " << hex << desc.idVendor << endl;
    cout << "Product id: " << desc.idProduct << endl;

    libusb_device_handle *handle = nullptr;
    if (libusb_open(dev, &handle) == LIBUSB_SUCCESS) {
        unsigned char str[256];
        r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, str, sizeof(str));
        if (r > 0)
            cout << "Serial number: " << str << endl;
        else
            cout << "Serial number: empty" << endl;

        r = libusb_get_string_descriptor_ascii(handle, desc.iProduct, str, sizeof(str));
        if (r > 0)
            cout << "Product: " << str << endl;
        else
            cout << "Product: empty" << endl;

        r = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, str, sizeof(str));
        if (r > 0)
            cout << "Manufacturer: " << str << endl;
        else
            cout << "Manufacturer: empty" << endl;
    } else
        cerr << "FAILURE" << endl;
    cout << endl;
    libusb_close(handle);

}

