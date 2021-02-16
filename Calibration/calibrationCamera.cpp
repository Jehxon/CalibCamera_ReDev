#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include "include/opencv2/ccalib/omnidir.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	vector<Mat> imgs; //The images we work with
	
	Size boardSize(8,6);
	double squareSize = 50.0d; //In milimeters, to be modified when we will use real images from the camera, for now we only use test images from https://github.com/jiuerbujie/omnidirectional_calibration
	
	
	vector<Mat> imagePoints, objectPoints;
	//imagePoints will be filled by findChessboardCorners with a matrix of 2D points per image, objectPoints will contain the corresponding points in world coordinates (3D points)
	//These matrices will be 1xN, with N = 6*8 = 48 (number of corners in the chessboard), and each component is supposed to be of type CV_64FC2 (for imagePoints) and CV_64FC3 (for objectPoints)
	//However this with these specific types the program crashes, for an unknown reason.
	
	
	//The online documentation (https://docs.opencv.org/master/dd/d12/tutorial_omnidir_calib_main.html) specify we can use vectors of vectors or vectors of matrices.
	//We tried both solutions, but nothing seems to work, we got the same error.
	
	/*vector<vector<Vec2f> > imagePoints;
	vector<vector<Vec3f> > objectPoints;*/
	
	string virobImagesPathPrefix = "../Virob/calibrationImages/img"; //Where we take the images (these are from the Virob classes in robotics)
	string gitHubImagesPathPrefix = "../GitHubData/omnidir_images/"; //From the omnidirectionnal camera calibration github 
		
	for (int i = 0; i < 3; i++)
	{
		imgs.push_back(imread(gitHubImagesPathPrefix + to_string(i+1) +".jpg", IMREAD_GRAYSCALE));//We fill the images vector
		imshow("image " + to_string(i), imgs[i]); //Showing the images used 
	}
	
	
	//Filling imagePoints and objectPoints
	for (int i = 0; i < 3; i++)	
	{
		/*vector<Vec3f> tempObj;
		vector<Vec2f> tempImg;*/
		Mat tempImg;
		Mat tempObj;
		
		tempImg.create(1,48,CV_64FC2); //We tried to force the right type
		tempObj.create(tempImg.size(),CV_64FC3); //Same size
		
		bool found = findChessboardCorners(imgs[i], boardSize, tempImg); //We noticed this function transforms the tempImg matrix in type CV_32FC2 instead of CV_64FC2
		tempImg.convertTo(tempImg, CV_64FC2); //So once again we force the right type
		
		cout << "Chessboard " << i << " found ? : " << found << endl; // this line was used for trying to debug
		
		//Filling tempObj
		for (int k = 0; k < 8; k++)
		{
			for (int j = 0; j < 6; j++)
			{
				Vec3d point((double)k, (double)j, 0.0d); //Once again forcing the type (even if it is useless here), just making sure the error does not come from here
				point *= squareSize;
				tempObj.at<Vec3d>(0,j*8+k) = point;
			}
		}
		
		//Putting the constructed matrices in the vectors
		imagePoints.push_back(tempImg);
		objectPoints.push_back(tempObj);
	}
	
	
	//Uncomment to see that the matrices are filled correctly
	/*
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << imagePoints[0].at<Vec2d>(0,j*8+i) << " ";
		}
		cout << endl;
		
	}
	*/
	
	//Debuging outputs
	cout << "objectPoints right type ? : " << (objectPoints[0].type() == CV_64FC3) << " imagePoints right type ? : " << (imagePoints[0].type() == CV_64FC2) << " objectPoints is empty ? : " << objectPoints[1].empty() << endl;
	cout << "Number of images in obectPoints : " << objectPoints.size() << ", should be the same as in imagePoints : " << imagePoints.size() << endl;
	cout << "OpenCV version : " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;
	cout << "Image of size " << imgs[0].size() << endl;
	
	//Parametric martices of the camera 
	Mat K, xi, D, idx;
	TermCriteria criteria(TermCriteria::COUNT + TermCriteria::EPS, 200, 0.0001);
	vector<Mat> rvecs, tvecs;
	int flags = 0;
	
	//It is this line that crashes the program.
	double rms = cv::omnidir::calibrate(objectPoints, imagePoints, imgs[0].size(), K, xi, D, rvecs, tvecs, flags, criteria, idx);
	cout << rms << endl;
	
	
	//Waiting for the user to press "q" to quit.
	int k = waitKey(0);
	if(k == 'q')
	{
		destroyAllWindows();
	}
	
	return 0;
}
