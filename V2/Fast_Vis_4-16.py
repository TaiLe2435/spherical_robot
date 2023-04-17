import numpy as np
import cv2
import asyncio
import time
import random
from bleak import BleakClient

ADDRESS = "81:e7:1e:ed:7d:55"
CHARACTERISTIC_UUID = "4401ba75-2b0f-4b18-9028-696b467dafe9"

async def connect(address):
    async with BleakClient(address) as client:
        print("Connected!")
        cap = cv2.VideoCapture(0)
        value = 0
        while True:
            ret, frame = cap.read()
            cx, cy = 0, 0
            if not ret:
                break
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            lower_range = np.array([94, 80, 2])
            upper_range = np.array([110, 255, 255])
            mask = cv2.inRange(hsv, lower_range, upper_range)
            contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            if len(contours) > 0:
                c = max(contours, key=cv2.contourArea)
                x, y, w, h = cv2.boundingRect(c)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                M = cv2.moments(c)
                if M["m00"] != 0:
                    cx = int(M["m10"] / M["m00"])
                    cy = int(M["m01"] / M["m00"])
                else:
                    cx, cy = 0, 0
                    
                # print(cx, cy)
                    
            # Convert the coordinates to a list of integers
            if cx > 255:
                cx = 77
            if cy > 25:
                cy = 77
            # data = [cx, cy]
            # print(data)
            # data_bytes = bytes([cx, cy])
            # print(f"Sending data: {data_bytes}")

            # # Write the data to the BLE device
            # await client.write_gatt_char(CHARACTERISTIC_UUID, data_bytes)

            data = [cx, cy]
            print(data)
            data_bytes = bytes([cx, cy])
            print(f"Sending data: {data_bytes}")
            # Write the data to the BLE device
            await client.write_gatt_char(CHARACTERISTIC_UUID, data_bytes, True)

            # Increment the value for the next packet
            value += 1

            # Wait for a short period of time before sending the next data packet
            await asyncio.sleep(0.1)
            cv2.imshow("Robot Tracking", frame)
            # cap.stop()

if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(connect(ADDRESS))
