This repo contains code and instructions for making a "mirror" which applies the style transfer in real-time. It was initially made for [Cubist Mirror](https://vimeo.com/167910860), an installation that was on display for [alt-AI](alt-ai.net), a conference about art & artificial intelligence which occurred in May 2016 at [The School for Poetic Computation](http://www.sfpc.io) in New York. 

![Cubist mom](https://github.com/genekogan/CubistMirror/blob/master/photos/cubist_mirror_1.jpg =320x)
![Cubist Victor](https://github.com/genekogan/CubistMirror/blob/master/photos/cubist_mirror_2.jpg =280x)
![Cubist Gene](https://github.com/genekogan/CubistMirror/blob/master/photos/cubist_mirror_3.jpg =190x)

### Overview

The basic pipeline is an [OpenFrameworks](openframeworks.cc) app which repeatedly saves the webcam image to disk and then calls a chainer-based style transfer script onto it. The style transfer code is implemented with [chainer-fast-neuralstyle](https://github.com/yusuketomoto/chainer-fast-neuralstyle), a python script that applies a pre-computed style model to an image. It is similar to the iterative, optimization-based style transfer technique demonstrated by [Gatys et al](http://arxiv.org/abs/1508.06576) in 2015, but is able to do so in a single forward pass, and is thus 1000 times faster, enabling near real-time application, albeit at a slightly reduced quality.

### Speed

On my 2013 MacBook Pro, it takes roughly 0.7 sec to generate the restyled image (with CUDA enabled, about 2x that if no GPU). The openframeworks app speeds this up slightly by threading multiple calls which overlap with each other. With 4 simultaneous threads, I'm able to get a frame rate of around 2 fps on the Macbook. With a dedicated GPU however, you could probably get around 4-5 fps. As the technique could only be done in 5-10 minutes per image 6 months ago, it's reasonable to expect that it will be fully real-time (~30fps) in the near future.

The original paper demonstrating this technique by Johnson, Alahi, and Li: [Perceptual Losses for Real-Time Style Transfer and Super-Resolution](arxiv.org/abs/1603.08155).

### Instructions

##### 1) Set up the chainer code

More detailed instructions for setting up can be found in that repo, but basically:

Install chainer:

    pip install chainer

Clone chainer-fast-neuralstyle, go to that folder, and download the VGG model:

    sh setup_model.sh

By default, this app is GPU-accelerated using CUDA. If you don't have CUDA you have to disable the CUDA flag in `ofApp.cpp`, in which case it will run in CPU which is roughly half the speed.

##### 2) Setting up a style model

The app comes with several pre-computed styles that you can use: cubist (unidentified painter on Pinterest), Hokusai (Great Wave off Kanagawa), and van Gogh's Starry Night (which comes with Yusuke's chainer code). They can be found in the app's data folder.

To create your own style model, you need a large dataset of images. I used [MS-COCO](http://mscoco.org/dataset/#download) (around 80k images) to train the cubist and hokusai models. More detailed instructions for training a style model can be found in [chainer-fast-neuralstyle](https://github.com/yusuketomoto/chainer-fast-neuralstyle) but bascially it's:

    python train.py -s <style_image_path> -d <training_dataset_path> -g 0 

Where `<style_image_path>` refers to your style image and `<training_dataset_path>` refers to your images, e.g. MS-COCO. `-g 0` enables CUDA which can be disabled (`-g -1`) but is **highly** recommended, as trying to train on CPU is prohibitively slow. This needs to be done on a proper GPU with plenty of memory. I trained mine on an Amazon GPU cluster which was CUDA-accelerated. It's possible to do this on a laptop but not practical. The lack of memory means you have to reduce the batch size and it is very slow. 

Under the ideal setup (GPU cluster with CUDA acceleration) it took me around 1 day to train a decent cubist model, however the packaged one is from 7 days of further training (the Hokusai model was just 1 day, and could probably be improved with more training).

I should also note that because this implementation is of lesser quality than the optimization-based approach, some styles are better than others. I found cubism worked well because it was able to replicate the hard edges and corners pretty easily, but styles with a lot of detail don't work as well as the technique by Gatys et al.

##### 3) Generating images with chainer

Once you've got chainer and a model set up, to generate a restyled image, you need to run (from the chainer-fast-neuralstyle directory) generate.py, e.g.

    python generate.py sample_images/tubingen.jpg -m models/starrynight.model -o sample_images/output.jpg -g 0

Once this is verified as working, you can:

##### 4) Setup the real-time app in openframeworks

This repo contains an [openframeworks](http://www.openframeworks.cc) application which repeatedly calls #3 on a webcam image saved to disk. 

Pay careful attention to the flags in `ofApp::setup()` as this will need to be slightly modified on your machine. Since openframeworks overwrites your PATH you need to modify this to match your system path so it can find CUDA and chainer. If you are not familiar with the PATH stuff, this is going to be a bit annoying. If you are, this should be self-explanatory but I am going to write this in more detail shortly....

The app has four separate threaded workers calling the chainer script in sequential and overlapping chunks. Your speed may vary and you can add more threads, but there's a diminishing return and in practice I've only been able to achieve roughly 2 fps on my 2013 macbook.

##### 5) The "mirror" part

The code above can probably be useful in other contexts as well (thinking VR headsets) but this section shows you how to set it up as a "mirror".  

The mirror is simply a dismembered LCD screen with a webcam to the side, running the app in full-screen and continuously applying the chainer script to the camera feed. I set mine up with the help of [openframe](http://openframe.io) which gives instructions on how to turn LCD screens into electronic frames.