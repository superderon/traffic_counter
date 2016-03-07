#!/usr/bin/env python
import numpy as np
import cv2
import sys
import copy
import time
#def processframe(frame):
    





def main(video):
    framecount = 0
    #vid =  cv2.VideoCapture(video)
    vid2 =  cv2.VideoCapture(video)
    #output = cv2.VideoWriter_fourcc(*'XVID')
    #background = cv2.BackgroundSubtractorMOG()
    background2 = cv2.BackgroundSubtractorMOG(history=200, nmixtures=5, backgroundRatio=0.01)
    #background2 = cv2.BackgroundSubtractorMOG2()
    kernel = np.array(([0,1,0],[1,1,1],[0,1,0]),dtype = np.uint8)
    kernel2 = np.ones((10,10),np.uint8)
    kernel3 = np.ones((8,8),np.uint8)
    new = 0
    old = 0
    count = 0
    while(vid2.isOpened()):
        framecount = framecount + 1
        #ret, frame = vid.read()
        ret2, frame2 = vid2.read()


        imgray = cv2.cvtColor(frame2,cv2.COLOR_BGR2GRAY)
        #mask = background.apply(frame,learningRate=0.001)
        mask2 = background2.apply(frame2,learningRate=.05)
        erode = cv2.erode(mask2,kernel,iterations =1)
        dilate = cv2.dilate(erode,kernel2,iterations = 1)

        ret,thresh = cv2.threshold(dilate,127,255,0)
        contours,hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

        if framecount>150:
            for i in contours:
                if cv2.contourArea(i) > 2000:
                    #print cv2.contourArea(i)
                    x,y,w,h = cv2.boundingRect(i)

                    if y+h/2>400:
                        #print x,y,w,h                        
                        #box = cv2.cv.BoxPoints(rect)
                        #box = np.int0(box)
                        #cv2.drawContours(frame2,[box],0,(0,0,255),2)
                        cv2.rectangle(frame2,(x,y),(x+w,y+h),(0,255,0),2)
                        #cv2.rectangle(frame2,(100,400),(110,450),(0,255,0),2)                        
                        #cv2.rectangle(dilate,(x,y),(x+w,y+h),(0,255,0),2)
                        new = new + y+h/2
                        #print "_______________________________"
                        #print "new", new, "old", old
                        #print "++++++++++++++++++++++++"
                        #print "END FRAME"
            #print old, new
            if new > old +400:
                count = count + 1
                print "_____________________________________"
                print "new", new, "old", old, "count", count
            number = str(count)
            cv2.putText(frame2,number,(700,100),cv2.FONT_HERSHEY_SIMPLEX,4,(0,0,255),10)
            old = new
            new = 0

                    


            #cv2.drawContours(frame2,contours,-1,(0,255,0),3)
            #bg = background2.getBackgroundImage()
            cv2.imshow('mask', mask2)
            cv2.imshow('frame2',dilate)
            cv2.imshow('original',frame2)
            time.sleep(.025)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        #output.write(frame)

    #vid.release()
    vid2.release()
    #out.release()
    cv2.destroyAllWindows()
    

if __name__ == '__main__':
    
    main(sys.argv[1])
