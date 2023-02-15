import socket               

sock = socket.socket()

host = "10.0.0.72" #ESP32 IP in local network
port = 80             #ESP32 Server Port    

sock.connect((host, port))

while True:         #inf loop
    message = input('Please Enter a Number: ') #take user input
    message = message + '\n'    #add \r to indicate end of string
    # message = "hi"
    message = bytes(message, 'utf-8')
    sock.send(message) #.encode after message used to work

sock.close()

# data = ""       

# while len(data) < len(message):
#     data += sock.recv(1).decode()

# print(data)

