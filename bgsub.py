#!/usr/bin/env python
import numpy as np
import cv2
import sys
import copy
import time


def main(video):
    framecount = 0
    vid =  cv2.VideoCapture(video)
    background2 = cv2.BackgroundSubtractorMOG(history=200, nmixtures=5, backgroundRatio=0.01)
    kernel = np.array(([0,1,0],[1,1,1],[0,1,0]),dtype = np.uint8)
    kernel2 = np.ones((10,10),np.uint8)
    kernel3 = np.ones((8,8),np.uint8)
    new = 0
    old = 0
    count = 0
    #fourcc = cv2.VideoWriter_fourcc(*'XVID')
    out = cv2.VideoWriter('output.mp4',2,30, (856,474))
    cv2.namedWindow("mask")
    cv2.moveWindow("mask",0,0)
    cv2.namedWindow("foreground")
    cv2.moveWindow("foreground", 1000,0)
    cv2.namedWindow("original")    
    while vid.isOpened():
        framecount = framecount + 1
        ret, frame = vid.read()
        if not ret:
            break


        imgray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
        mask = background2.apply(frame,learningRate=.05)
        erode = cv2.erode(mask,kernel,iterations =1)
        dilate = cv2.dilate(erode,kernel2,iterations = 1)

        ret,thresh = cv2.threshold(dilate,127,255,0)
        contours,hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

        if framecount>150:
            for i in contours:
                if cv2.contourArea(i) > 2000:
                    #print cv2.contourArea(i)
                    x,y,w,h = cv2.boundingRect(i)

                    if y+h/2>400:

                        cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),2)
                        new = new + y+h/2

            if new > old +400:
                count = count + 1
                print "_____________________________________"
                print "new", new, "old", old, "count", count
            number = str(count)
            cv2.putText(frame,number,(700,100),cv2.FONT_HERSHEY_SIMPLEX,4,(0,0,255),10)
            old = new
            new = 0

            
            cv2.imshow('mask', mask)
            cv2.imshow('foreground',dilate)
            cv2.imshow('original',frame)
        out.write(frame)
        #time.sleep(.025)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        #output.write(frame)

    vid.release()

    cv2.destroyAllWindows()


    out.release()

if __name__ == '__main__':
    
    main(sys.argv[1])
