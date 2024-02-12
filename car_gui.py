import serial
from tkinter import *
from tkinter import messagebox
import tkinter
import time
import serial
import threading
import continuous_threading

### Display car GUI

bg_color = "Lightpink1"
def create_circle(x,y,r,color,canvasName):
    x0 = x-r
    y0 = y-r
    x1 = x+r
    y1 = y+r
    return canvasName.create_oval(x0,y0,x1,y1,fill=color)
def create_label(root,xval,yval,tvar, color= bg_color):
    lab = Label(root, textvariable=tvar, bg = color)
    lab.pack()
    lab.place(x=xval, y= yval)
    return lab
#Build GUI on canvas
root = Tk()
root.geometry("800x600")
root.title("TESTBED")
root.configure(bg =bg_color)
canvas = Canvas(root, bg =bg_color, width = 800, height = 600)
canvas.pack()
#Branding

green_status = StringVar()
yellow_status = StringVar()
red_status = StringVar()
distance_status = IntVar()
speed_status = IntVar()
temp_status = DoubleVar()
headlight_status = StringVar()

water_status = IntVar()
distance_title_status = StringVar()
speed_title_status = StringVar()
temp_title_status = StringVar()
headlight_title_status = StringVar()
water_title_status = StringVar()
distance_unit_status = StringVar()
speed_unit_status = StringVar()
temp_unit_status = StringVar()

green_status.set('OFF')
yellow_status.set('OFF')
red_status.set('OFF')
distance_status.set(0)
speed_status.set(0)
temp_status.set(0)
headlight_status.set("OFF")
water_status.set(0)
distance_title_status.set("DISTANCE:")
speed_title_status.set("MOTOR SPEED:")
temp_title_status.set("TEMPERATURE:")
headlight_title_status.set("HEADLIGHTS:")
water_title_status.set("COOLANT LEVEL: ")
distance_unit_status.set("CM")
# speed_unit_status.set("%")
temp_unit_status.set("C")

green_label = create_label(root, 70, 50, green_status)
yellow_label = create_label(root, 70, 100, yellow_status)
red_label = create_label(root, 70, 150, red_status)

distance_label = create_label(root, 375, 400, distance_status)
speed_label = create_label(root, 375, 450, speed_status)
temp_label = create_label(root, 375, 500, temp_status)
headlight_label = create_label(root, 375, 350, headlight_status)
water_label = create_label(root, 375, 300, water_status)
distance_label.config(font=("Calibri Light", 20))
speed_label.config(font=("Calibri Light", 20))
temp_label.config(font=("Calibri Light", 20))
headlight_label.config(font=("Calibri Light", 20))
water_label.config(font=("Calibri Light", 20))

distance_title = create_label(root, 50, 400, distance_title_status)
distance_title.config(font=("Calibri Light", 20))
speed_title = create_label(root, 50, 450, speed_title_status)
speed_title.config(font=("Calibri Light", 20))
temp_title = create_label(root, 50, 500, temp_title_status)
temp_title.config(font=("Calibri Light", 20))
headlight_title = create_label(root, 50, 350, headlight_title_status)
headlight_title.config(font=("Calibri Light", 20))
water_title = create_label(root, 50, 300, water_title_status)
water_title.config(font=("Calibri Light", 20))
distance_unit = create_label(root, 440, 400, distance_unit_status)
distance_unit.config(font=("Calibri Light", 20))
speed_unit = create_label(root, 440, 450, speed_unit_status)
speed_unit.config(font=("Calibri Light", 20))
temp_unit = create_label(root, 440, 500, temp_unit_status)
temp_unit.config(font=("Calibri Light", 20))
gled = create_circle(50,50,10,"green",canvas)
yled = create_circle(50,100,10,"yellow",canvas)
rled = create_circle(50,150,10,"red",canvas)

value =[];
fault =0
def startCallBack():
    arduino = serial.Serial('COM7', 9600, timeout=5)
    #distance, water level, temp,speed
    a = str(arduino.readline().decode("utf-8"))
    value = a.split(",")
    distance = int(value[0])
    water = int(value[1])
    temp = int(value[2])
    speed = int(value[3])
    if distance <= 50:
        green_status.set('ON')
    else:
        green_status.set('OFF')
    if distance <= 40:
        yellow_status.set('ON')
    else:
        yellow_status.set('OFF')
    if distance <= 30:
        red_status.set('ON')
    else:
        red_status.set('OFF')
    distance_status.set(distance)
    if water >= 400 or water <= 50:
        fault = 1

        if water >=400:
            water_status.set('HIGH')
        if water <=50:
            water_status.set('LOW')
    else:
        fault = 0
        water_status.set(water)
    temp_status.set(temp)
    if fault == 1:
        speed_status.set(speed)
        headlight_status.set('OFF')
    else:
        headlight_status.set('ON')
        speed_status.set(speed+50)


# start_button = Button(root,text = "Start", command = startCallBack)
# start_button.place(x=700,y=20)

thread = continuous_threading.PeriodicThread(1,startCallBack)
thread.start()
root.mainloop()
