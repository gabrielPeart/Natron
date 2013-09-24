//  Powiter
//
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012. 
*contact: immarespond at gmail dot com
*
*/


#include "WriteQt.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <QtGui/QImage>
#include <QtGui/QImageWriter>

#include "Engine/Lut.h"
#include "Engine/Format.h"
#include "Writers/Writer.h"
#include "Engine/Row.h"

using namespace std;
using namespace Powiter;

/*Should return the list of file types supported by the encoder: "png","jpg", etc..*/
std::vector<std::string> WriteQt::fileTypesEncoded() const {
    std::vector<std::string> out;
    // Qt Image reader should be the last solution (it cannot read 16-bits ppm or png)
    const QList<QByteArray>& supported = QImageWriter::supportedImageFormats();
    // Qt 4 supports: BMP, JPG, JPEG, PNG, PBM, PGM, PPM, TIFF, XBM, XPM
    // Qt 5 doesn't support TIFF
    for (int i = 0; i < supported.count(); ++i) {
        out.push_back(std::string(supported.at(i).toLower().data()));
    }
    return out;
}

/*Should return the name of the write handle : "ffmpeg", "OpenEXR" ...*/
std::string WriteQt::encoderName() const {
    return "QImage (Qt)";
}

/*Must be implemented to tell whether this file type supports stereovision*/
bool WriteQt::supports_stereo() const {
    return false;
}


/*Must implement it to initialize the appropriate colorspace  for
 the file type. You can initialize the _lut member by calling the
 function getLut(datatype) */
void WriteQt::initializeColorSpace(){
    _lut = Color::getLut(Color::LUT_DEFAULT_INT8);
}

/*This function initialises the output file/output storage structure and put necessary info in it, like
 meta-data, channels, etc...This is called on the main thread so don't do any extra processing here,
 otherwise it would stall the GUI.*/
void WriteQt::setupFile(const std::string& filename){
    _filename = filename;
    const Format& frmt = op->info().displayWindow();
    size_t dataSize = 4* frmt.width() * frmt.height();
    _buf = (uchar*)malloc(dataSize);
}

/*This function must fill the pre-allocated structure with the data calculated by engine.
 This function must close the file as writeAllData is the LAST function called before the
 destructor of Write.*/
void WriteQt::writeAllData(){
    const Format& frmt = op->info().displayWindow();
    const ChannelSet& channels = op->requestedChannels();
    QImage::Format type;
    if (channels & Channel_alpha && _premult) {
        type = QImage::Format_ARGB32_Premultiplied;
    }else if(channels & Channel_alpha && !_premult){
        type = QImage::Format_ARGB32;
    }else{
        type = QImage::Format_RGB32;
    }
    QImage img(_buf,frmt.width(),frmt.height(),type);
    img.save(QString(_filename.c_str()));
    free(_buf);
}

void WriteQt::supportsChannelsForWriting(ChannelSet& channels) const {
    foreachChannels(z, channels){
        if(z!= Channel_red &&
           z!= Channel_green &&
           z!= Channel_blue &&
           z!= Channel_alpha){
            throw std::runtime_error("Qt only supports writing image files with red/green/blue/alpha channels.");
            return;
        }
    }
}

void WriteQt::engine(int y,int offset,int range,ChannelSet channels,Row*){
    Row* row = op->input(0)->get(y,offset,range);
    const Format& frmt = op->info().displayWindow();
    
    /*invert y to be in top-to-bottom increasing order*/
    y = frmt.top()-y-1;
    uchar* toR=0;
    uchar* toG=0;
    uchar* toB=0;
    uchar* toA=0;
    if (channels & Channel_alpha && _premult) {
        toA = _buf + y*4*frmt.width();
        toR = toA+1;
        toG = toR+1;
        toB = toG+1;
    }else if(channels & Channel_alpha && !_premult){
        toA = _buf + y*4*frmt.width();
        toR = toA+1;
        toG = toR+1;
        toB = toG+1;
    }else{
        toB  = _buf + y*4*frmt.width();
        toG = toB+1;
        toR = toG+1;
        toA = toR+1;
    }

    const float* red = (*row)[Channel_red];
    const float* green = (*row)[Channel_green];
    const float* blue = (*row)[Channel_blue];
    const float* alpha = (*row)[Channel_alpha];
    if(!red){
        row->turnOn(Channel_red);
    }
    red = (*row)[Channel_red]+row->offset();

    if(!green){
        row->turnOn(Channel_green);
    }
    green = (*row)[Channel_green]+row->offset();

    if(!blue){
        row->turnOn(Channel_blue);
    }
    blue = (*row)[Channel_blue]+row->offset();

    if(!alpha){
        row->turnOn(Channel_alpha);
    }
    alpha = (*row)[Channel_alpha]+row->offset();

    to_byte(Channel_red, toR, red, alpha, row->right() - row->offset(),4);
    to_byte(Channel_green, toG, green, alpha, row->right() - row->offset(),4);
    to_byte(Channel_blue, toB, blue, alpha, row->right() - row->offset(),4);
    to_byte(Channel_alpha, toA, alpha, alpha, row->right() - row->offset(),4);
    row->release();
    // row is unlocked by release()
}

WriteQt::WriteQt(Writer* writer):Write(writer),_buf(0){
    
}
WriteQt::~WriteQt(){
    
}
