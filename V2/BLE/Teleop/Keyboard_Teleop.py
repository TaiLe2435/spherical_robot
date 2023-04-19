import asyncio
import sys
import bleak
from bleak import BleakClient

# UUIDs for the Arduino BLE service and characteristic
SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214"
CHARACTERISTIC_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"
ADDRESS = "81:e7:1e:ed:7d:55"

async def send_input(input_value, client):
    # Send the input value over BLE to the Arduino board
    await client.write_gatt_char(CHARACTERISTIC_UUID, bytes(input_value.encode()), True)

async def keyboard_input_handler(client):
    # Detect keyboard inputs and send them over BLE to the Arduino board
    while True:
        input_value = sys.stdin.readline().strip()
        await send_input(input_value, client)

async def notification_handler(sender, data):
    # Print the received notification data to the console
    print(f"Received notification: {data.decode()}")

async def run():
    # Connect to the Arduino BLE device
    async with BleakClient(ADDRESS) as client:
        # Enable notifications on the Arduino BLE characteristic
        await client.start_notify(CHARACTERISTIC_UUID, notification_handler)

        # Start the keyboard input handler
        await keyboard_input_handler(client)

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run())
