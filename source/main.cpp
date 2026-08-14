#include "MicroBit.h"
#include "MicroBitAccelerometerService.h"

MicroBit uBit;

void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}

int main()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    uBit.ble->init(microbit_friendly_name(), uBit.getSerial(), uBit.messageBus, uBit.storage, true);

    if (uBit.ble->getBondCount() == 0)
    {
        uBit.display.print(microbit_friendly_name());
        uBit.ble->advertise();
        uBit.ble->pairingMode(uBit.display, uBit.buttonA);
    }

    release_fiber();

    microbit_panic(999);
}
