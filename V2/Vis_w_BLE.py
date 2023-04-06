import asyncio
from bleak import BleakScanner, BleakClient
import cv2
import numpy as np
import math
import time

import sys

# function looking for clicks
def clickEvent(event, x, y, flags, params):
    global flag, point
    # checking for left mouse clicks
    if event == cv2.EVENT_LBUTTONDOWN:
        flag = True 
        # displaying the coordinates
        print(x, ' ', y)
        point = (x,y)
 
    # checking for right mouse clicks    
    if event==cv2.EVENT_RBUTTONDOWN:
        flag = True
        # displaying the coordinates
        print(x, ' ', y)
        point = (x,y)

def euclid(center1, center2):
    dist = np.sqrt((center1[0]-center2[0])**2+(center1[1]-center2[1])**2)
    dir = np.arctan2(center2[0]-center1[0], center2[1]-center1[1])
    
    return dist, dir

address = "81:e7:1e:ed:7d:55"
headingCharacteristic = "4401ba75-2b0f-4b18-9028-696b467dafe9"
distanceCharacteristic = "59077dfa-9f09-454b-8ad3-52870c7cbe19"

async def main():
    # devices = await BleakScanner.discover()
    # for d in devices:
    #     print(d)
    async with BleakClient(address) as client:
        heading_pub = await client.write_gatt_char(headingCharacteristic, data=bytearray([168]), response=True)
        distance_pub = await client.write_gatt_char(distanceCharacteristic, data=bytearray([70]), response=True)
        
        heading = await client.read_gatt_char(headingCharacteristic)
        distance = await client.read_gatt_char(distanceCharacteristic)

        print(heading)
        int_heading = int.from_bytes(heading, byteorder=sys.byteorder)
        print(int_heading)
        
        print(distance)
        int_distance = int.from_bytes(distance, byteorder=sys.byteorder)
        print(int_distance)

while __name__ == "__main__":
    try:
        asyncio.run(main())
    except:
        print("Run has been interrupted")
        break
