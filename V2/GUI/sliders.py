import tkinter as tk

class ColorSelector:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Color Selector")

        # Set default color to black
        self.color = "#000000"

        # Create sliders for R, G, B values
        self.r_slider = tk.Scale(self.root, from_=0, to=255, orient=tk.HORIZONTAL, label="R", command=self.update_color)
        self.r_slider.pack()
        self.g_slider = tk.Scale(self.root, from_=0, to=255, orient=tk.HORIZONTAL, label="G", command=self.update_color)
        self.g_slider.pack()
        self.b_slider = tk.Scale(self.root, from_=0, to=255, orient=tk.HORIZONTAL, label="B", command=self.update_color)
        self.b_slider.pack()

        # Create canvas to display selected color
        self.canvas = tk.Canvas(self.root, width=200, height=200, bg=self.color)
        self.canvas.pack()

        # Run main loop
        self.root.mainloop()

    def update_color(self, val):
        # Update color based on slider values
        r = self.r_slider.get()
        g = self.g_slider.get()
        b = self.b_slider.get()
        self.color = "#{:02x}{:02x}{:02x}".format(r, g, b)
        print(r, ' ', g, ' ', b)
        self.canvas.config(bg=self.color)

if __name__ == "__main__":
    color_selector = ColorSelector()
