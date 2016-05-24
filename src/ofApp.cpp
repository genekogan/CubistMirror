#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);
    
    grab.initGrabber(320, 240);
    chainer_img.load(ofToDataPath("chainer_img_1.png"));
    last = 0;
    min_lapse = 0.8;
    
    cout << ofToDataPath("") << endl;

    worker1 = new ThreadedObject();
    worker2 = new ThreadedObject();
    worker3 = new ThreadedObject();
    worker4 = new ThreadedObject();
    
    worker1->setIndex(1);
    worker2->setIndex(2);
    worker3->setIndex(3);
    worker4->setIndex(4);
    
    usd = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    grab.update();
    
    t = ofGetElapsedTimef() - last;
    
    if (grab.isFrameNew()) {
        if (worker1->available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_1.png");
            worker1->start();
            last = ofGetElapsedTimef();
        }
        else if (worker2->available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_2.png");
            worker2->start();
            last = ofGetElapsedTimef();
        }
        else if (worker3->available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_3.png");
            worker3->start();
            last = ofGetElapsedTimef();
        }
        else if (worker4->available() && t > min_lapse) {
            cam_img.setFromPixels(grab.getPixels());
            cam_img.save("cam_4.png");
            worker4->start();
            last = ofGetElapsedTimef();
        }
    }
    
    
    if (worker1->hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_1.png"));
        worker1->reset();
    }
    if (worker2->hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_2.png"));
        worker2->reset();
    }
    if (worker3->hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_3.png"));
        worker3->reset();
    }
    if (worker4->hasNew()) {
        chainer_img.load(ofToDataPath("chainer_img_4.png"));
        worker4->reset();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (usd) {
//        chainer_img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight()*(1.3333));
        chainer_img.draw(ofGetWidth(), 0, -ofGetWidth(), ofGetHeight()*(1.3333));
    }
    else {
        chainer_img.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == '1'){
        worker1->setStyle("cubist_new_5");
        worker2->setStyle("cubist_new_5");
        worker3->setStyle("cubist_new_5");
        worker4->setStyle("cubist_new_5");
    }
    if (key == '2') {
        worker1->setStyle("hokusai");
        worker2->setStyle("hokusai");
        worker3->setStyle("hokusai");
        worker4->setStyle("hokusai");
    }
    if (key == '3') {
        worker1->setStyle("maps");
        worker2->setStyle("maps");
        worker3->setStyle("maps");
        worker4->setStyle("maps");
    }
    if (key == '4') {
        worker1->setStyle("starrynight");
        worker2->setStyle("starrynight");
        worker3->setStyle("starrynight");
        worker4->setStyle("starrynight");
    }
    if (key == '5') {
        worker1->setStyle("calligraphy_0");
        worker2->setStyle("calligraphy_0");
        worker3->setStyle("calligraphy_0");
        worker4->setStyle("calligraphy_0");
    }
    
    if (key== ' ') {
        usd = !usd;
    }

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
