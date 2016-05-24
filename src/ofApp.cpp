#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);

    // number of seconds to wait before calling next thread.
    // should be roughly (sec it takes to generate / number of threads)
    min_lapse = 0.6;
    
    // select style model
    string styleModel = ofToDataPath("cubist.model");

    // set up camera
    grab.initGrabber(320, 240);

    // set up style threads
    worker1.setStyleModel(styleModel);
    worker2.setStyleModel(styleModel);
    worker3.setStyleModel(styleModel);
    worker4.setStyleModel(styleModel);
    worker1.setIndex(1);
    worker2.setIndex(2);
    worker3.setIndex(3);
    worker4.setIndex(4);
}

//--------------------------------------------------------------
void ofApp::update(){
    grab.update();
    
    t = ofGetElapsedTimef() - last;
    
    if (grab.isFrameNew()) {
        if (worker1.available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_1.png");
            worker1.start();
            last = ofGetElapsedTimef();
        }
        else if (worker2.available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_2.png");
            worker2.start();
            last = ofGetElapsedTimef();
        }
        else if (worker3.available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_3.png");
            worker3.start();
            last = ofGetElapsedTimef();
        }
        else if (worker4.available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_4.png");
            worker4.start();
            last = ofGetElapsedTimef();
        }
    }
    
    
    if (worker1.hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_1.png"));
        worker1.reset();
    }
    if (worker2.hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_2.png"));
        worker2.reset();
    }
    if (worker3.hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_3.png"));
        worker3.reset();
    }
    if (worker4.hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_4.png"));
        worker4.reset();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    chainer_img.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
