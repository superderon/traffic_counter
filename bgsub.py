#!/usr/bin/env python
import numpy as np
import cv2
import sys
import copy
import time
#def processframe(frame):
    
    


def main(video):

    #vid =  cv2.VideoCapture(video)
    vid2 =  cv2.VideoCapture(video)
    #output = cv2.VideoWriter_fourcc(*'XVID')
    #background = cv2.BackgroundSubtractorMOG()
    background2 = cv2.BackgroundSubtractorMOG(history=200, nmixtures=5, backgroundRatio=0.01)
    #background2 = cv2.BackgroundSubtractorMOG2()
    kernel = np.array(([0,1,0],[1,1,1],[0,1,0]),dtype = np.uint8)
    kernel2 = np.ones((10,10),np.uint8)
    while(vid2.isOpened()):
        #ret, frame = vid.read()
        ret2, frame2 = vid2.read()
        imgray = cv2.cvtColor(frame2,cv2.COLOR_BGR2GRAY)
        #mask = background.apply(frame,learningRate=0.001)
        mask2 = background2.apply(frame2,learningRate=.05)
        erode = cv2.erode(mask2,kernel,iterations = 2)
        dilate = cv2.dilate(erode,kernel2,iterations = 2)
        #erode = cv2.erode(dilate,kernel2,iterations = 1)
        ret,thresh = cv2.threshold(dilate,127,255,0)
        contours,hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        for i contours:
            print cv2.contourArea(i)



        #cv2.drawContours(frame2,contours,-1,(0,255,0),3)
        #bg = background2.getBackgroundImage()
        cv2.imshow('mask', mask2)
        cv2.imshow('frame2',dilate)
        cv2.imshow('original',frame2)
        time.sleep(.01)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        #output.write(frame)

    #vid.release()
    vid2.release()
    #out.release()
    cv2.destroyAllWindows()
    

if __name__ == '__main__':
    
    main(sys.argv[1])
