import sensor, image, time,math
from pyb import LED
import pyb
import math
import time
from pyb import UART
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
thresholds = [

(28, 78, -22, 27, 19, 100),
(12, 64, 17, 74, -12, 59)

             ]
p=1
uart = UART(3, 9600)
center_Roi=[126,120,100,80]
uart.init(9600, bits=8, parity=None, stop=1)
def pixwl_to_realty(cx,cy):
    y2=-0.0982*cx-0.0055*cy+29.0392
    x2=0.000728*cx-0.0961*cy+4.7144
    x2=x2/100
    y2=y2/100
    return x2,y2
def recive_data():
    global uart
    global tmp_data
    if uart.any():
        tmp_data = uart.readline()
        print(temp_data)
        print("OK")
def distancen(blobn):
    wincx=160
    wincy=120
    x=blobn.x()
    y=blobn.y()
    return   math.sqrt(math.pow(wincx-x,2)+math.pow(wincy-y,2))
def compareblobs(blob1,blob2):
    temp=distancen(blob1)-distancen(blob2)
    if temp>0:
        return 1
    elif temp<0:
        return -1
    else:
        return 0
rechar=0x00
rechar1=0x00
rechar2=0x00
sendbuff=[0]*5;
while(True):
    while(True and p==1):
        clock.tick()
        img = sensor.snapshot().lens_corr(1.8)
        nearblob=None
        blobs= img.find_blobs( thresholds, pixels_threshold=100, area_threshold=100,roi=center_Roi, merge=True)
        img.draw_rectangle(center_Roi, thickness = 2, fill = False)
        if len(blobs)==0:
         continue
        nearblob=blobs[0]
        for blo in blobs:
            if compareblobs(nearblob,blo)==1:
                nearblob=blo
        a= pixwl_to_realty(nearblob.cx(),nearblob.cy())
        if(len(a)):
            sendbuff[0]=0xA5;
            sendbuff[1]=10;
            sendbuff[2]=0x5A;
            data1=bytearray([11,22,33,44])
            out_str="HEAD,%f,%f,-0.14,%d,TAIL"%(a[0],a[1],nearblob.code())
           # if(160>nearblob.cx()>100 and 40<nearblob.cy()<90):
            uart.write(out_str)
            print(out_str)
            print(nearblob.cx(),nearblob.cy(),nearblob.code())
            img.draw_cross(160,120,clolr=(255,0,0))
            img.draw_line(160,120,nearblob.cx(),nearblob.cy(),color=(255,0,0))
            p=1
        time.sleep(1)
