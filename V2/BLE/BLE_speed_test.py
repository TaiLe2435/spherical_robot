import asyncio
import time
from bleak import BleakClient

ADDRESS = "81:e7:1e:ed:7d:55"
CHARACTERISTIC_UUID = "4401ba75-2b0f-4b18-9028-696b467dafe9"

async def connect(address):
    async with BleakClient(address) as client:
        print("Connected!")
        value = 0
        while True:
            # Convert the data to bytes
            data = [value, value+1, value+2]
            print(data)
            data_bytes = bytes([value, value+1, value+2])
            print(f"Sending data: {data_bytes}")
            # Write the data to the BLE device
            await client.write_gatt_char(CHARACTERISTIC_UUID, data_bytes, True)

            # Increment the value for the next packet
            value += 1

            # Wait for a short period of time before sending the next data packet
            await asyncio.sleep(0.1)

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(connect(ADDRESS))

