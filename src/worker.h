#pragma once


#include "ofThread.h"


class ThreadedObject: public ofThread
{
public:
    bool isNew;
    bool isAvailable;
    
    string cmd_path;
    string cmd_chainer;
    string model;
    
    ofImage cam_img;
    ofImage chainer_img;

    int index;

    ThreadedObject(): count(0), isNew(false), shouldThrowTestException(false){
        isNew = false;
        isAvailable = true;
    }
    
    void setIndex(int index) {
        this->index = index;
        setCommands();
    }

    void setStyleModel(string model) {
        this->model = model;
        setCommands();
    }

    void setCommands() {
        string neuralStylePath = NEURAL_STYLE_PATH;
        string path = PATH;
        string ld_library_path = LD_LIBRARY_PATH;
        cmd_path = "export PATH="+path+"; export LD_LIBRARY_PATH="+ld_library_path+";";
        cmd_chainer = "/usr/local/bin/python "+neuralStylePath+"/generate.py "+ofToDataPath("cam_"+ofToString(index)+".png")+" -m "+model+" -o "+ofToDataPath("chainer_img_"+ofToString(index)+".png")+" --gpu "+(GPU_ENABLED==1?"0":"-1");
    }
    
    bool hasNew() {
        return isNew;
    }
    
    void reset() {
        isNew = false;
    }
    
    void start(){
        isAvailable = false;
        startThread();
    }

    void stop(){
        stopThread();
    }
    
    bool available() {
        return isAvailable;
    }

    void threadedFunction()
    {
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock()) {
                unlock();
                
                float t1 = ofGetElapsedTimef();
                string cmd = cmd_path+cmd_chainer;
                string res = ofSystem(cmd);
                float t2 = ofGetElapsedTimef();

                count++;
                
                isNew = true;
                isAvailable = true;
                stop();
                
                if(shouldThrowTestException > 0) {
                    shouldThrowTestException = 0;
                    throw Poco::ApplicationException("We just threw a test exception!");
                }
            }
            else {
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }
    
    int getCount() {
        unique_lock<std::mutex> lock(mutex);
        return count;
    }
    
    void throwTestException() {
        shouldThrowTestException = 1;
    }
    
protected:
    Poco::AtomicCounter shouldThrowTestException;
    int count;
};
