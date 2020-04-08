#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp> //foreground ũ�������Ҷ� ����
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void overlayImage(const Mat &background, const Mat &foreground, Mat &output, Point2i location);

int main(int argc, char** argv)
{
	Mat image_foreground1 = imread("red.png", IMREAD_UNCHANGED); 
	Mat image_foreground2 = imread("yellow.png", IMREAD_UNCHANGED);
	Mat image_foreground3 = imread("blue.png", IMREAD_UNCHANGED);
	Mat image_foreground4 = imread("green.png", IMREAD_UNCHANGED);
	Mat image_background = imread("Black.jpg", IMREAD_COLOR);  

	if (image_background.empty() || image_foreground1.empty()|| image_foreground2.empty()|| image_foreground3.empty()|| image_foreground4.empty())  //���� �ҷ����� Ȯ��
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE); 

	//foreground �̹��� ũ�� ����
	/*Mat image_resize_foreground;
	resize(image_foreground, image_resize_foreground, Size(), 0.5, 0.5);*/ 

	Mat image_result1;
	Mat image_result2;
	Mat image_result3;
	Mat image_result4;
	int x = (image_background.cols - image_foreground1.cols) / 2;
	int y = (image_background.rows - image_foreground1.rows) / 2;

	overlayImage(image_background, image_foreground1, image_result1, cv::Point(x,y-image_background.rows/4));
	overlayImage(image_result1, image_foreground2, image_result2, cv::Point(x-image_background.cols/4, y));
	overlayImage(image_result2, image_foreground3, image_result3, cv::Point(x, y+ image_background.rows / 4));
	overlayImage(image_result3, image_foreground4, image_result4, cv::Point(x+ image_background.cols / 4, y));
	imshow("Display window", image_result4);

	waitKey(0);

	return 0;
}

void overlayImage(const Mat &background, const Mat &foreground,Mat &output, Point2i location)
{
	background.copyTo(output);

	// ������ y���� ������ 0���� ���� 
	for (int y = std::max(location.y, 0); y < background.rows; ++y)
	{
		int fY = y - location.y; 
		if (fY >= foreground.rows)
			break;		//foreground �� ó��

		//������ x���� ������ 0���� ����
		for (int x = std::max(location.x, 0); x < background.cols; ++x)
		{
			int fX = x - location.x; 
			if (fX >= foreground.cols)
				break;

			//foreground �ȼ��� ������ ����
			double opacity =((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])/ 255.;

			//�������� 0���� Ŭ ���� ����
			for (int c = 0; opacity > 0 && c < output.channels(); ++c)
			{
				unsigned char foregroundPx = foreground.data[fY * foreground.step + fX * foreground.channels() + c];
				unsigned char backgroundPx = background.data[y * background.step + x * background.channels() + c];
				output.data[y*output.step + output.channels()*x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
			}
		}
	}
}