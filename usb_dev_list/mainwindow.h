#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTreeWidget>

#include "libusb.h"

#include <QPointer>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    


struct device_data {
    QString Info;
    QString Error;
    QString Manufacturer;
    QString Product;
    QString Serial;
};

private slots:
void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    int LoadDevices();
    int test_wrapped_device(const char *device_name);
    void print_device(libusb_device *dev, libusb_device_handle *handle, int devNumber);
    void print_configuration(libusb_config_descriptor *config);
    void print_interface(const libusb_interface *interface);
    void print_bos(libusb_device_handle *handle);
    void print_ss_usb_cap(libusb_ss_usb_device_capability_descriptor *ss_usb_cap);
    void print_2_0_ext_cap(libusb_usb_2_0_extension_descriptor *usb_2_0_ext_cap);
    void print_altsetting(const libusb_interface_descriptor *interface);
    void print_endpoint(const libusb_endpoint_descriptor *endpoint);
    void print_endpoint_comp(const libusb_ss_endpoint_companion_descriptor *ep_comp);

    QTreeWidgetItem* addTreeRoot(QString name);
    QTreeWidgetItem *addTreeChild(QTreeWidgetItem *parent,     QString busNumber);
    void addBusNode(QTreeWidgetItem* parent, QStringList devicesList, QString busName , QString Prefix);


    QStringList ExtractDevices(QStringList buses, QStringList devs, QString busName);
};
#endif // MAINWINDOW_H
