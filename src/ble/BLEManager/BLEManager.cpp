#include "BLEManager.h"

BLEManager::BLEManager() {
    this->serverCallbacks = new MyServerCallbacks();
}

void BLEManager::init(std::string deviceName) {
    BLEDevice::init(deviceName);
    this->createBLEServer();
    this->createBLEService();
    this->createBLECharacteristic();
    this->service->start();
    this->startAdvertising();
}

void BLEManager::createBLEServer() {
    this->server = BLEDevice::createServer();
    this->serverCallbacks = new MyServerCallbacks();
    this->server->setCallbacks(this->serverCallbacks);
}

void BLEManager::createBLEService() {
    this->service = this->server->createService(SERVICE_UUID);
}

void BLEManager::createBLECharacteristic() {
    this->characteristic = this->service->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    this->characteristic->addDescriptor(new BLE2902());
}

void BLEManager::sendDataToClient(uint32_t &data) {
    this->characteristic->setValue(data);
    this->characteristic->notify();
}

void BLEManager::startAdvertising() {
    this->advertising = BLEDevice::getAdvertising();
    this->advertising->addServiceUUID(SERVICE_UUID);
    this->advertising->setScanResponse(false);
    this->advertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

bool BLEManager::getIsDeviceConnected() {
    return this->serverCallbacks->deviceConnected;
}
