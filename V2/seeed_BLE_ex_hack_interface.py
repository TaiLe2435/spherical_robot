import asyncio
from bleak import BleakScanner, BleakClient

address = "81:e7:1e:ed:7d:55"
headingCharacteristic = "4401ba75-2b0f-4b18-9028-696b467dafe9"
distanceCharacteristic = "59077dfa-9f09-454b-8ad3-52870c7cbe19"

async def main():
    devices = await BleakScanner.discover()
    for d in devices:
        print(d)
    async with BleakClient(address) as client:
        heading = await client.read_gatt_char(headingCharacteristic)
        distance = await client.read_gatt_char(distanceCharacteristic)
        print("Heading: {0}".format("".join(map(chr, heading)))) 
        print("Distance: {0}".format("".join(map(chr, distance))))   

asyncio.run(main())