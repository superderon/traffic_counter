#!/usr/bin/env python
import numpy as np
import cv2
import sys
import copy

def main(video):

    vid =  cv2.VideoCapture(video)
    
    while(vid.isOpened()):
        ret, frame = vid.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame',gray)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    vid.release()
    cv2.destroyAllWindows

if __name__ == '__main__':
    
    main(sys.argv[1])
