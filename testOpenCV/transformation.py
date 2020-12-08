import cv2 as cv
import numpy as np
import time

videoName = "../Data/2020_04_23_goupil_3_aller_HEVC_56K.mp4"
#frame is 2686*5376 !

vid = cv.VideoCapture(videoName)
ratio = 30

t = time.time()

n=0
descriptors = []
keypoints = []
images = []

matcher = cv.FlannBasedMatcher()

while(vid.isOpened()):
	ret, frame = vid.read()
	n += 1
	if(n%ratio == 0):
		#frame = cv.resize(frame,(1280,720))
		t = time.time()
		
		#Some image treatment
		extractor = cv.BRISK_create()
		
		keypoint = extractor.detect(frame)
		keypoint, descriptor = extractor.compute(frame,keypoint)
		
		if(n <= 2*ratio):
			descriptors.append(np.float32(descriptor))
			keypoints.append(keypoint)
			images.append(frame)
		else:
			descriptors[-1] = np.float32(descriptor)
			keypoints[-1] = keypoint
			images[-1] = frame
		
		
		#if(not descriptors[0]):
		#	descriptors[0] = descriptors[1]
		if ( descriptors[-1].size == 0 ):
			cvError(0,"MatchFinder","present descriptor empty",__FILE__,__LINE__)
		
		matches = matcher.match(descriptors[0],descriptors[-1])
		
		max_dist = 0
		min_dist = 100
		
		for match in matches:
			d = match.distance
			if(d < min_dist):
				min_dist = d
			if(d > max_dist):
				max_dist = d
		
		print("Max dist = ", max_dist)
		print("Min dist = ", min_dist)
		
		
		goodMatches=[]
		for match in matches:
			if(match.distance < 2*min_dist):
				goodMatches.append(match)
		
		#resizedImage = cv.resize(frame,(1920,1080))
		#gray = cv.cvtColor(resizedImage, cv.COLOR_RGB2GRAY)
		#cv.imshow('video', gray)
		#im = frame
		#im = cv.drawMatches(images[0],keypoints[0], images[-1], keypoints[-1], goodMatches,im)
		
		#cv.imshow('video', im)
		
		if(n >= 2*ratio):
			descriptors[0] = descriptors[-1]
			keypoints[0] = keypoints[-1]
			images[0] = images[-1]
		
	if cv.waitKey(1) & 0xFF == ord('q'):
		break

vid.release()

cv.destroyAllWindows()


