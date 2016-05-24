#pragma once


#include "ofThread.h"


class ThreadedObject: public ofThread
{
public:
    bool isNew;
    bool isAvailable;
    
    string cmd_path;
    string cmd_chainer;
    string style;
    
    ofImage cam_img;
    ofImage chainer_img;

    int index;

    ThreadedObject(): count(0), isNew(false), shouldThrowTestException(false){
        isNew = false;
        isAvailable = true;
        style = "style_13";
    }
    
    void setIndex(int index) {
        this->index = index;
        cmd_path = "export PATH=/usr/local/bin:/usr/local/sbin:/Users/gene/torch/install/bin:/Developer/NVIDIA/CUDA-7.5/bin:/usr/local/cuda/bin:/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin; export LD_LIBRARY_PATH=/Users/gene/torch/install/lib:/usr/local/cuda/lib:;";
        cmd_chainer = "/usr/local/bin/python /Users/gene/Learn/chainer-fast-neuralstyle/generate.py /Users/gene/Code/of_v0.9.0_osx_release/apps/myApps/ThreadedChainerFast/bin/data/cam_"+ofToString(index)+".png -m /Users/gene/Learn/chainer-fast-neuralstyle/models/"+style+".model -o "+ofToDataPath("chainer_img_"+ofToString(index)+".png")+" --gpu 0";
    }

    void setStyle(string style_) {
        this->style = style_;
        cmd_path = "export PATH=/usr/local/bin:/usr/local/sbin:/Users/gene/torch/install/bin:/Developer/NVIDIA/CUDA-7.5/bin:/usr/local/cuda/bin:/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin; export LD_LIBRARY_PATH=/Users/gene/torch/install/lib:/usr/local/cuda/lib:;";
        cmd_chainer = "/usr/local/bin/python /Users/gene/Learn/chainer-fast-neuralstyle/generate.py /Users/gene/Code/of_v0.9.0_osx_release/apps/myApps/ThreadedChainerFast/bin/data/cam_"+ofToString(index)+".png -m /Users/gene/Learn/chainer-fast-neuralstyle/models/"+style+".model -o "+ofToDataPath("chainer_img_"+ofToString(index)+".png")+" --gpu 0";
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
                cout << "timing "<<this->index<<" : " << (t2-t1) << endl;
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
