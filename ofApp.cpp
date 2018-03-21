#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open(1);
	this->cap_size = cv::Size(640, 360);

	this->eye_cascade.load("opencv-3.3.1\\build\\install\\etc\\haarcascades\\haarcascade_eye.xml");

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat cap_frame;
	this->cap >> cap_frame;
	if (cap_frame.empty()) {

		return;
	}
	cv::resize(cap_frame, this->frame, this->cap_size);
	cv::flip(this->frame, this->frame, 1);
	cv::cvtColor(this->frame, this->frame, CV_RGB2BGR);


	if (ofGetFrameNum() == 0) {

		this->frame.copyTo(this->save_frame);
	}

	vector<cv::Rect> eyes;
	this->eye_cascade.detectMultiScale(this->frame, eyes);

	for (int x = 0; x < this->frame.cols; x++) {

		for (int y = this->frame.rows - 1; y > -1; y--) {

			ofVec2f point(x, y);
			for (cv::Rect eye : eyes) {

				ofVec2f eye_point = ofVec2f(eye.x + eye.size().width / 2, eye.y + eye.size().height / 2);
				if (eye_point.distance(point) < 35) {

					this->frame.at<cv::Vec3b>(y, x) = this->save_frame.at<cv::Vec3b>(y, x);
				}
			}
		}
	}

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->image.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 's') {

		this->frame.copyTo(this->save_frame);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(640, 360, OF_WINDOW);
	ofRunApp(new ofApp());
}