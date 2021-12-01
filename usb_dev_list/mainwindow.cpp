#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStatusBar>
#include <QDesktopWidget>
#include <QScopedPointer>



// functions needed

#include <stdio.h>
#include <string.h>
#include "libusb.h"
#include <QDebug>

//
struct {
  QStringList devicename; //list of devices
  QStringList errorMsg; //error msg for each device
  QStringList infoMsg; //info for each device
  QList <uint8_t> busNumber; //bus for each device
  QStringList busNumberStr; //bus number for each device as string
  QList <uint8_t> devAdress; //adress for each device
  QStringList devAdressStr; //adress for each device

  QList <uint16_t> idVendor; //vendor id
  QList <uint16_t> idProdukt; //product id
  QStringList speed; //speed list of devices
  QStringList manufacturer; //list of manufacturers
  QStringList productName; //list of product names
  QStringList serialNumber; //serial number list
} Devices;

int verbose = 0;

 void MainWindow::print_endpoint_comp(const struct libusb_ss_endpoint_companion_descriptor *ep_comp)
{
    printf("      USB 3.0 Endpoint Companion:\n");
    printf("        bMaxBurst:           %u\n", ep_comp->bMaxBurst);
    printf("        bmAttributes:        %02xh\n", ep_comp->bmAttributes);
    printf("        wBytesPerInterval:   %u\n", ep_comp->wBytesPerInterval);
}

 void MainWindow::print_endpoint(const struct libusb_endpoint_descriptor *endpoint)
{
    int i, ret;

    printf("      Endpoint:\n");
    printf("        bEndpointAddress:    %02xh\n", endpoint->bEndpointAddress);
    printf("        bmAttributes:        %02xh\n", endpoint->bmAttributes);
    printf("        wMaxPacketSize:      %u\n", endpoint->wMaxPacketSize);
    printf("        bInterval:           %u\n", endpoint->bInterval);
    printf("        bRefresh:            %u\n", endpoint->bRefresh);
    printf("        bSynchAddress:       %u\n", endpoint->bSynchAddress);

    for (i = 0; i < endpoint->extra_length;) {
        if (LIBUSB_DT_SS_ENDPOINT_COMPANION == endpoint->extra[i + 1]) {
            struct libusb_ss_endpoint_companion_descriptor *ep_comp;

            ret = libusb_get_ss_endpoint_companion_descriptor(NULL, endpoint, &ep_comp);
            if (LIBUSB_SUCCESS != ret)
                continue;

            print_endpoint_comp(ep_comp);

            libusb_free_ss_endpoint_companion_descriptor(ep_comp);
        }

        i += endpoint->extra[i];
    }
}

 void MainWindow::print_altsetting(const struct libusb_interface_descriptor *interface)
{
    uint8_t i;

    printf("    Interface:\n");
    printf("      bInterfaceNumber:      %u\n", interface->bInterfaceNumber);
    printf("      bAlternateSetting:     %u\n", interface->bAlternateSetting);
    printf("      bNumEndpoints:         %u\n", interface->bNumEndpoints);
    printf("      bInterfaceClass:       %u\n", interface->bInterfaceClass);
    printf("      bInterfaceSubClass:    %u\n", interface->bInterfaceSubClass);
    printf("      bInterfaceProtocol:    %u\n", interface->bInterfaceProtocol);
    printf("      iInterface:            %u\n", interface->iInterface);

    for (i = 0; i < interface->bNumEndpoints; i++)
        print_endpoint(&interface->endpoint[i]);
}

 void MainWindow::print_2_0_ext_cap(struct libusb_usb_2_0_extension_descriptor *usb_2_0_ext_cap)
{
    printf("    USB 2.0 Extension Capabilities:\n");
    printf("      bDevCapabilityType:    %u\n", usb_2_0_ext_cap->bDevCapabilityType);
    printf("      bmAttributes:          %08xh\n", usb_2_0_ext_cap->bmAttributes);
}

 void MainWindow::print_ss_usb_cap(struct libusb_ss_usb_device_capability_descriptor *ss_usb_cap)
{
    printf("    USB 3.0 Capabilities:\n");
    printf("      bDevCapabilityType:    %u\n", ss_usb_cap->bDevCapabilityType);
    printf("      bmAttributes:          %02xh\n", ss_usb_cap->bmAttributes);
    printf("      wSpeedSupported:       %u\n", ss_usb_cap->wSpeedSupported);
    printf("      bFunctionalitySupport: %u\n", ss_usb_cap->bFunctionalitySupport);
    printf("      bU1devExitLat:         %u\n", ss_usb_cap->bU1DevExitLat);
    printf("      bU2devExitLat:         %u\n", ss_usb_cap->bU2DevExitLat);
}

 void MainWindow::print_bos(libusb_device_handle *handle)
{
    struct libusb_bos_descriptor *bos;
    uint8_t i;
    int ret;

    ret = libusb_get_bos_descriptor(handle, &bos);
    if (ret < 0)
        return;

    printf("  Binary Object Store (BOS):\n");
    printf("    wTotalLength:            %u\n", bos->wTotalLength);
    printf("    bNumDeviceCaps:          %u\n", bos->bNumDeviceCaps);

    for (i = 0; i < bos->bNumDeviceCaps; i++) {
        struct libusb_bos_dev_capability_descriptor *dev_cap = bos->dev_capability[i];

        if (dev_cap->bDevCapabilityType == LIBUSB_BT_USB_2_0_EXTENSION) {
            struct libusb_usb_2_0_extension_descriptor *usb_2_0_extension;

            ret = libusb_get_usb_2_0_extension_descriptor(NULL, dev_cap, &usb_2_0_extension);
            if (ret < 0)
                return;

            print_2_0_ext_cap(usb_2_0_extension);
            libusb_free_usb_2_0_extension_descriptor(usb_2_0_extension);
        } else if (dev_cap->bDevCapabilityType == LIBUSB_BT_SS_USB_DEVICE_CAPABILITY) {
            struct libusb_ss_usb_device_capability_descriptor *ss_dev_cap;

            ret = libusb_get_ss_usb_device_capability_descriptor(NULL, dev_cap, &ss_dev_cap);
            if (ret < 0)
                return;

            print_ss_usb_cap(ss_dev_cap);
            libusb_free_ss_usb_device_capability_descriptor(ss_dev_cap);
        }
    }

    libusb_free_bos_descriptor(bos);
}

 void MainWindow::print_interface(const struct libusb_interface *interface)
{
    int i;

    for (i = 0; i < interface->num_altsetting; i++)
        print_altsetting(&interface->altsetting[i]);
}

 void MainWindow::print_configuration(struct libusb_config_descriptor *config)
{
    uint8_t i;

    printf("  Configuration:\n");
    printf("    wTotalLength:            %u\n", config->wTotalLength);
    printf("    bNumInterfaces:          %u\n", config->bNumInterfaces);
    printf("    bConfigurationValue:     %u\n", config->bConfigurationValue);
    printf("    iConfiguration:          %u\n", config->iConfiguration);
    printf("    bmAttributes:            %02xh\n", config->bmAttributes);
    printf("    MaxPower:                %u\n", config->MaxPower);

    for (i = 0; i < config->bNumInterfaces; i++)
        print_interface(&config->interface[i]);
}

 void MainWindow::print_device(libusb_device *dev, libusb_device_handle *handle, int devNumber)
{
    struct libusb_device_descriptor desc;
    unsigned char string[256];
    const char *speed;
    int ret;
    uint8_t i;

    switch (libusb_get_device_speed(dev))
    {
    case LIBUSB_SPEED_LOW:		speed = "1.5M"; break;
    case LIBUSB_SPEED_FULL:		speed = "12M"; break;
    case LIBUSB_SPEED_HIGH:		speed = "480M"; break;
    case LIBUSB_SPEED_SUPER:	speed = "5G"; break;
    case LIBUSB_SPEED_SUPER_PLUS:	speed = "10G"; break;
    default:
        speed = "Unknown";
    }

    ret = libusb_get_device_descriptor(dev, &desc);
    if (ret < 0) {
        fprintf(stderr, "failed to get device descriptor");
        Devices.errorMsg.append("failed to get device descriptor");


        return;
    }

    Devices.busNumber.append(libusb_get_bus_number(dev));
    Devices.busNumberStr.append(QString::number(libusb_get_bus_number(dev)));


    Devices.devAdress.append(libusb_get_device_address(dev));
    Devices.devAdressStr.append(QString::number(libusb_get_device_address(dev)));


    Devices.idVendor.append(desc.idVendor);
    Devices.idProdukt.append(desc.idProduct);
    Devices.speed.append(QString(speed));

    printf("Dev (bus %u, device %u): %04X - %04X speed: %s\n",
           libusb_get_bus_number(dev), libusb_get_device_address(dev),
           desc.idVendor, desc.idProduct, speed);

    if (!handle)
        libusb_open(dev, &handle);

    if (handle) {
        if (desc.iManufacturer) {
            ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, sizeof(string));
            if (ret > 0)
            {
                printf("  Manufacturer:              %s\n", (char *)string);
                Devices.manufacturer.append(QString((char*)string));
            }
        }
        else
        {
            Devices.manufacturer.append("?");
        }

        if (desc.iProduct) {
            ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, string, sizeof(string));
            if (ret > 0)
            {
                printf("  Product:                   %s\n", (char *)string);
                Devices.productName.append(QString((char*)string));
            }
        } else {
            Devices.productName.append(" ");
        }

        if (desc.iSerialNumber && verbose) {
            ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, string, sizeof(string));
            if (ret > 0)
            {
                printf("  Serial Number:             %s\n", (char *)string);
                Devices.serialNumber.append(QString((char*)string));
            }
        }
    }

    //pending
    if (1) {
        for (i = 0; i < desc.bNumConfigurations; i++) {
            struct libusb_config_descriptor *config;

            ret = libusb_get_config_descriptor(dev, i, &config);
            if (LIBUSB_SUCCESS != ret) {
                printf("  Couldn't retrieve descriptors\n");
                continue;
            }

            print_configuration(config);

            libusb_free_config_descriptor(config);
        }

        if (handle && desc.bcdUSB >= 0x0201)
            print_bos(handle);
    }

    if (handle)
        libusb_close(handle);
}

#ifdef __linux__
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int MainWindow::test_wrapped_device(const char *device_name)
{
    libusb_device_handle *handle;
    int r, fd;

    fd = open(device_name, O_RDWR);
    if (fd < 0) {
        printf("Error could not open %s: %s\n", device_name, strerror(errno));
        return 1;
    }
    r = libusb_wrap_sys_device(NULL, fd, &handle);
    if (r) {
        printf("Error wrapping device: %s: %s\n", device_name, libusb_strerror(r));
        close(fd);
        return 1;
    }
    print_device(libusb_get_device(handle), handle);
    close(fd);
    return 0;
}
#else
 int MainWindow::test_wrapped_device(const char *device_name)
{
    (void)device_name;
    printf("Testing wrapped devices is not supported on your platform\n");
    return 1;
}
#endif




using namespace std;

//this function extracts the device name that correspond to a bus name and return a stringlist
QStringList MainWindow::ExtractDevices(QStringList buses, QStringList devs, QString busName)
{
    QStringList x; //output
    for (int i=0; i<buses.size(); i++)
    {
        if (buses.at(i)==busName)
        {
            QString a;
            a=devs.at(i)+ " " + Devices.productName.at(i);
            //qDebug() << devs.at(i) << " " << Devices.devicename << " " << Devices.idProdukt;
            x.append(a);
        }
    }
    return x;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    this->LoadDevices(); //to sort the devices here

    //start GUI //
   ui->treeWidget->setColumnCount(1);
   QTreeWidgetItem* rootPtr=addTreeRoot("My Computer");   // Add root nodes

   //add devices example

   //search for unique bus names in Devices.busname
   QStringList Buses = Devices.busNumberStr;
   Buses.sort(Qt::CaseInsensitive);
   Buses.removeDuplicates();


   for (int i=0; i< Buses.size(); i++)
   {
       //find devices on this bus
       QStringList devices = ExtractDevices(Devices.busNumberStr,Devices.devAdressStr,Buses.at(i));
       devices.sort(Qt::CaseInsensitive); devices.removeDuplicates();
       addBusNode(rootPtr, devices,Buses.at(i),"Port ");
   }
   ui->treeWidget->expandAll();



}


QTreeWidgetItem* MainWindow::addTreeRoot(QString name)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    //add the header
    if(QTreeWidgetItem* header = ui->treeWidget->headerItem()) {
      header->setText(0, "My USB Devices");
    } else {
      ui->treeWidget->setHeaderLabel("My USB Devices");
    }

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name); //MyComputer


    return treeItem;

}

//this function add devics to a certain bus, it needs busname, device list and the pointer to parent node
void MainWindow::addBusNode(QTreeWidgetItem* parent,QStringList devicesList,QString busName, QString Prefix )
{
    QTreeWidgetItem* childPtr=addTreeChild(parent, "Bus " + busName);
    for (int i=0; i<devicesList.size(); i++) //loop the number of devices and add each one
    {
        QString n=Prefix;
        QString n1=devicesList.at(i);
        n.trimmed();n1.trimmed();
        addTreeChild(childPtr, "["+n+n1+"]");
    }

}

// add subnode with its name
// return the pointer of the child
QTreeWidgetItem* MainWindow::addTreeChild(QTreeWidgetItem *parent,
                  QString busNumber)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(); //Bus Number
    treeItem->setText(0, busNumber);
    parent->addChild(treeItem);

    return treeItem;

}


MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::LoadDevices()
{
    libusb_device **devs;
    ssize_t cnt;
    int r, i;


    r = libusb_init(NULL);
    if (r < 0)
        return r;


    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0) {
        libusb_exit(NULL);
        return 1;
    }

    for (i = 0; devs[i]; i++)
        print_device(devs[i], NULL,i);

    libusb_free_device_list(devs, 1);


    libusb_exit(NULL);







    return r;



}








void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //when item is clicked come here
    QString x=item->text(0); //"[Port 2 FaceTime HD Camera (Built-in)]"
    QString portno; QStringList dummy;    dummy= x.split(" ");
    portno=dummy.at(1);   dummy=item->parent()->text(0).split(" ");
    QString bus=dummy.at(1);

    qDebug()<< " bus = " << bus << "portno" << portno;
    int entry=0;
    for (int i=0; i<Devices.busNumber.size(); i++)
    {
        //qDebug() << "bus=" <<  Devices.busNumberStr.at(i) << " " << Devices.devAdress.at(i);
        if (Devices.busNumberStr.at(i)==bus && QString::number(Devices.devAdress.at(i))==portno)
        {
            //qDebug() << "this";
            entry=i;
            ui->listWidget->clear();
            //qDebug() << "device name = " << Devices.devicename.at(i);
            //ui->listWidget->addItem(Devices.devicename.at(i));
            //ui->listWidget->addItem(Devices.serialNumber.at(i));
            ui->listWidget->addItem("Manufacturer: " +Devices.manufacturer.at(i));
            ui->listWidget->addItem("Speed: " + Devices.speed.at(i));
            ui->listWidget->addItem("Product: " + QString::number(Devices.idProdukt.at(i)));
            ui->listWidget->addItem("Vendor: " + QString::number(Devices.idVendor.at(i)));

        }
    }



}
