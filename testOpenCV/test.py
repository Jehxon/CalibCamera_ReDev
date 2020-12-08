import cv2 as cv
import numpy as np

videoName = "../Data/2020_04_23_goupil_3_aller_HEVC_56K.mp4"
#frame is 2686*5376 !

vid = cv.VideoCapture(videoName)

while(vid.isOpened()):
    ret, frame = vid.read()
    print(frame.shape)
    resizedImage = cv.resize(frame,(1920,1080))
    
    gray = cv.cvtColor(resizedImage, cv.COLOR_RGB2GRAY)
    cv.imshow('video', gray)

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()

cv.destroyAllWindows()

print(np.ones((2,5)))

