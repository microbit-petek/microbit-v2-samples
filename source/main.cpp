#include "CodalFiber.h"
#include "MicroBit.h"
#include "MicroBitAccelerometerService.h"
#include "MicroBitBLEService.h"
#include "MicroBitDevice.h"

MicroBit uBit;

void startPairing()
{

    uBit.display.print(microbit_friendly_name());
    uBit.ble->advertise();
    uBit.ble->pairingMode(uBit.display, uBit.buttonA);
}

void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}

void onAB(MicroBitEvent)
{
    create_fiber(startPairing);
}

static void rxFiber(void *uartBle)
{
    ((UartBle *)uartBle)->runRx();
}

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_DOWN, onAB);

    uBit.serial.setBaudrate(230400);

    uBit.ble->init(microbit_friendly_name(), uBit.getSerial(), uBit.messageBus, uBit.storage, true);

    UartBle *serial = new UartBle(uBit.serial);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer, *serial);
    new MicroBitMagnetometerService(*uBit.ble, uBit.compass, *serial);
    new MicroBitIOPinService(*uBit.ble, uBit.io, *serial);

    create_fiber(rxFiber, serial);

    uBit.display.printChar('i');

    release_fiber();

    microbit_panic(999);
}
