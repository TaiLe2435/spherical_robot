import tkinter as tk
import math

class JoystickGUI(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        # Create a joystick widget
        self.joystick = Joystick(self, size=100)
        self.joystick.pack(side="left", padx=50, pady=50)
        # self.joystick.place(relx=0.50, rely=0.5, anchor="center")

        # Create a directional pad widget
        self.dpad = DPad(self, size=100)
        self.dpad.pack(side="right", padx=50, pady=50)
        # self.dpad.place(relx=0.60, rely=0.5, anchor="center")

class Joystick(tk.Canvas):
    def __init__(self, master=None, size=100, *args, **kwargs):
        super().__init__(master, width=size, height=size, *args, **kwargs)
        self.size = size
        self.center = size/2
        self.handle_size = size/4
        self.handle_x = self.center
        self.handle_y = self.center
        self.x = 0
        self.y = 0
        self.draw()

        # Bind the mouse events to the canvas
        self.bind("<Button-1>", self.move_handle)
        self.bind("<B1-Motion>", self.move_handle)
        self.bind("<ButtonRelease-1>", self.reset_handle)

    def move_handle(self, event):
        # Move the handle to the mouse position
        self.x = event.x - self.center
        self.y = event.y - self.center
        print(self.x, " ", self.y)
        r = min(self.center, math.sqrt(self.x**2 + self.y**2))
        theta = math.atan2(self.y, self.x)
        self.handle_x = r * math.cos(theta) + self.center
        self.handle_y = r * math.sin(theta) + self.center
        self.draw()

    def reset_handle(self, event):
        # Snap the handle back to the center
        self.handle_x = self.center
        self.handle_y = self.center
        self.x = 0
        self.y = 0
        print(self.x, " ", self.y)
        self.draw()

    def draw(self):
        # Draw the joystick background and handle
        self.delete(tk.ALL)
        self.create_oval(0, 0, self.size, self.size, fill="gray")
        self.create_oval(self.handle_x-self.handle_size, self.handle_y-self.handle_size,
                         self.handle_x+self.handle_size, self.handle_y+self.handle_size,
                         fill="red")


class DPad(tk.Canvas):
    def __init__(self, master=None, size=100, handle_size=10):
        super().__init__(master, width=size, height=size)
        self.size = size
        self.bind("<Button-1>", self.move)
        self.handle_size = handle_size
        self.center = self.size/2
        self.draw()


    def draw(self):
        # Draw the D-pad background and arrows
        self.delete(tk.ALL)
        self.create_rectangle(0, 0, self.size, self.size, fill="gray")
        self.create_polygon(self.center-self.handle_size, self.center-self.handle_size,
                            self.center, self.center-2*self.handle_size,
                            self.center+self.handle_size, self.center-self.handle_size,
                            fill="red", outline="black")
        self.create_polygon(self.center+self.handle_size, self.center-self.handle_size,
                            self.center+2*self.handle_size, self.center,
                            self.center+self.handle_size, self.center+self.handle_size,
                            fill="red", outline="black")
        self.create_polygon(self.center+self.handle_size, self.center+self.handle_size,
                            self.center, self.center+2*self.handle_size,
                            self.center-self.handle_size, self.center+self.handle_size,
                            fill="red", outline="black")
        self.create_polygon(self.center-self.handle_size, self.center+self.handle_size,
                            self.center-2*self.handle_size, self.center,
                            self.center-self.handle_size, self.center-self.handle_size,
                            fill="red", outline="black")


    def move(self, event):
        # Determine which arrow was clicked
        if event.x >= 2*self.size/3 and event.y <= 2*self.size/3 and event.y >= self.size/3:
            print("Right")
        elif event.x <= self.size/3 and event.y >= self.size/3:
            print("Left")
        elif event.x >= self.size/3 and event.x <= 2*self.size/3 and event.y < self.size/2:
            print("Up")
        elif event.x >= self.size/3 and event.x <= 2*self.size/3 and event.y > self.size/2:
            print("Down")

if __name__ == "__main__":
    root = tk.Tk()
    root.geometry("400x200")
    app = JoystickGUI(master=root)
    app.mainloop()