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
#include "OfxClipInstance.h"

#include <cfloat>

#include "Engine/OfxNode.h"
#include "Engine/OfxImageEffectInstance.h"
#include "Engine/Settings.h"
#include "Engine/ImageFetcher.h"
#include "Engine/Model.h"

#include "Global/AppManager.h"
#include "Global/Macros.h"

using namespace Powiter;
using namespace std;

OfxClipInstance::OfxClipInstance(OfxNode* node,
                                 Powiter::OfxImageEffectInstance* effect,
                                 int /*index*/,
                                 OFX::Host::ImageEffect::ClipDescriptor* desc)
: OFX::Host::ImageEffect::ClipInstance(effect, *desc)
, _node(node)
, _effect(effect)
//, _clipIndex(index)
, _outputImage(NULL)
{
}

/// Get the Raw Unmapped Pixel Depth from the host. We are always 8 bits in our example
const std::string& OfxClipInstance::getUnmappedBitDepth() const
{
    // we always use floats
    static const std::string v(kOfxBitDepthFloat);
    return v;
}

/// Get the Raw Unmapped Components from the host. In our example we are always RGBA
const std::string &OfxClipInstance::getUnmappedComponents() const
{
    static const string rgbStr(kOfxImageComponentRGB);
    static const string noneStr(kOfxImageComponentNone);
    static const string rgbaStr(kOfxImageComponentRGBA);
    static const string alphaStr(kOfxImageComponentAlpha);
    
    //bool rgb = false;
    //bool alpha = false;
    
    //const ChannelSet& channels = _effect->info().channels();
    //if(channels & alpha) alpha = true;
    //if(channels & Mask_RGB) rgb = true;
    
//    if(!rgb && !alpha) return noneStr;
//    else if(rgb && !alpha) return rgbStr;
//    else if(!rgb && alpha) return alphaStr;
     return rgbaStr;
}


// PreMultiplication -
//
//  kOfxImageOpaque - the image is opaque and so has no premultiplication state
//  kOfxImagePreMultiplied - the image is premultiplied by it's alpha
//  kOfxImageUnPreMultiplied - the image is unpremultiplied
const std::string &OfxClipInstance::getPremult() const
{
    static const std::string v(kOfxImageUnPreMultiplied);
    return v;
}


// Pixel Aspect Ratio -
//
//  The pixel aspect ratio of a clip or image.
double OfxClipInstance::getAspectRatio() const
{
    return _effect->getProjectPixelAspectRatio();
}

// Frame Rate -
double OfxClipInstance::getFrameRate() const
{
    return _effect->getFrameRate();
}

// Frame Range (startFrame, endFrame) -
//
//  The frame range over which a clip has images.
void OfxClipInstance::getFrameRange(double &startFrame, double &endFrame) const
{
    if (_effect->getContext() == kOfxImageEffectContextGenerator) {
        startFrame = 0;
        endFrame = 0;
    }else{
        startFrame = _node->info().firstFrame();
        endFrame = _node->info().lastFrame();
    }
}

/// Field Order - Which spatial field occurs temporally first in a frame.
/// \returns
///  - kOfxImageFieldNone - the clip material is unfielded
///  - kOfxImageFieldLower - the clip material is fielded, with image rows 0,2,4.... occuring first in a frame
///  - kOfxImageFieldUpper - the clip material is fielded, with image rows line 1,3,5.... occuring first in a frame
const std::string &OfxClipInstance::getFieldOrder() const
{
    return _effect->getDefaultOutputFielding();
}

// Connected -
//
//  Says whether the clip is actually connected at the moment.
bool OfxClipInstance::getConnected() const
{
    if(!_node->isOutputNode())
        return _node->hasOutputConnected();
    else
        return true;
}

// Unmapped Frame Rate -
//
//  The unmaped frame range over which an output clip has images.
double OfxClipInstance::getUnmappedFrameRate() const
{
    
    return 25;
}

// Unmapped Frame Range -
//
//  The unmaped frame range over which an output clip has images.
// this is applicable only to hosts and plugins that allow a plugin to change frame rates
void OfxClipInstance::getUnmappedFrameRange(double &unmappedStartFrame, double &unmappedEndFrame) const
{
    unmappedStartFrame = 1;
    unmappedEndFrame = 1;
}

// Continuous Samples -
//
//  0 if the images can only be sampled at discreet times (eg: the clip is a sequence of frames),
//  1 if the images can only be sampled continuously (eg: the clip is infact an animating roto spline and can be rendered anywhen).
bool OfxClipInstance::getContinuousSamples() const
{
    return false;
}


/// override this to return the rod on the clip cannoical coords!
OfxRectD OfxClipInstance::getRegionOfDefinition(OfxTime) const
{
    OfxRectD v;
    if((_node->isInputNode() && getName() == kOfxImageEffectOutputClipName)
       || !_node->isInputNode()){
        const Box2D& bbox = _node->info().dataWindow();
        v.x1 = bbox.left();
        v.y1 = bbox.bottom();
        v.x2 = bbox.right();
        v.y2 = bbox.top();
//        cout << "RoD output clip with w = " << v.x2-v.x1 << " and h = " << v.y2-v.y1 << endl;
    }else{
        const Format& format = _node->getModel()->getApp()->getProjectFormat();
        v.x1 = format.left();
        v.y1 = format.bottom();
        v.x2 = format.right();
        v.y2 = format.top();
    }
    return v;
}


/// override this to fill in the image at the given time.
/// The bounds of the image on the image plane should be
/// 'appropriate', typically the value returned in getRegionsOfInterest
/// on the effect instance. Outside a render call, the optionalBounds should
/// be 'appropriate' for the.
/// If bounds is not null, fetch the indicated section of the canonical image plane.
OFX::Host::ImageEffect::Image* OfxClipInstance::getImage(OfxTime time, OfxRectD *optionalBounds)
{
    OfxRectD roi;
    if(optionalBounds){
        roi = *optionalBounds;
    }else{
        double w,h,x,y;
        _effect->getProjectExtent(w, h);
        _effect->getProjectOffset(x, y);
        roi.x1 = x;
        roi.x2 = x+w;
        roi.y1 = y;
        roi.y2 = y+h;
    }
    
    /*Return an empty image if the input is optional*/
    if(isOptional()){
        return new OfxImage(OfxImage::eBitDepthFloat,roi,*this,0);
    }
    
    /*SHOULD CHECK WHAT BIT DEPTH IS SUPPORTED BY THE PLUGIN INSTEAD OF GIVING FLOAT
     _effect->isPixelDepthSupported(...)
     */

    if(isOutput()){
        if (!_outputImage) {
            //   cout << "allocating output clip with w = " << roi.x2-roi.x1 << " and h = " << roi.y2-roi.y1 << endl;
            _outputImage = new OfxImage(OfxImage::eBitDepthFloat,roi,*this,0);
        }else{
            OfxRectI outputImageBounds = _outputImage->getROD();
            if(outputImageBounds.x1 != roi.x1 ||
               outputImageBounds.x2 != roi.x2 ||
               outputImageBounds.y1 != roi.y1 ||
               outputImageBounds.y2 != roi.y2){
                delete _outputImage;
                _outputImage = new OfxImage(OfxImage::eBitDepthFloat,roi,*this,0);
            }
        }
        _outputImage->addReference();
        return _outputImage;
    }else{
        Node* input = getAssociatedNode();
        assert(input);
        if(input->isOpenFXNode()){
            OfxRectI roiInput;
            roiInput.x1 = (int)std::floor(roi.x1);
            roiInput.x2 = (int)std::ceil(roi.x2);
            roiInput.y1 = (int)std::floor(roi.y1);
            roiInput.y2 = (int)std::ceil(roi.y2);
            OfxPointD renderScale;
            renderScale.x = renderScale.y = 1.0;
            OfxNode* ofxInputNode = dynamic_cast<OfxNode*>(input);
            assert(ofxInputNode);
            assert(ofxInputNode->effectInstance());
            OfxStatus stat = ofxInputNode->effectInstance()->renderAction(0, kOfxImageFieldNone, roiInput , renderScale);
            assert(stat == kOfxStatOK);
            OFX::Host::ImageEffect::ClipInstance* clip = ofxInputNode->effectInstance()->getClip(kOfxImageEffectOutputClipName);
            assert(clip);
            return clip->getImage(time, optionalBounds);
        } else {
            ImageFetcher srcImg(input, (int)floor(roi.x1), (int)std::floor(roi.y1), (int)std::ceil(roi.x2)-1, (int)std::ceil(roi.y2)-1,Mask_RGBA);
            srcImg.claimInterest(true);
            // all rows are locked
            OfxImage* ret = new OfxImage(OfxImage::eBitDepthFloat,roi,*this,0);
            assert(ret);
            /*Copying all rows living in the InputFetcher to the ofx image*/
            for (int y = (int)std::floor(roi.y1); y < (int)std::ceil(roi.y2); ++y) {
                OfxRGBAColourF* dstImg = ret->pixelF(0, y);
                assert(dstImg);
                Row* row = srcImg.at(y);
                if(!row){
                    cout << "Couldn't find row for index " << y << "...skipping this row" << endl;
                    continue;
                }
                const float* r = (*row)[Channel_red];
                const float* g = (*row)[Channel_green];
                const float* b = (*row)[Channel_blue];
                const float* a = (*row)[Channel_alpha];
                rowPlaneToOfxPackedBuffer(Channel_red,   r ? r+row->offset() : NULL, row->right()-row->offset(), dstImg);
                rowPlaneToOfxPackedBuffer(Channel_green, g ? g+row->offset() : NULL, row->right()-row->offset(), dstImg);
                rowPlaneToOfxPackedBuffer(Channel_blue,  b ? b+row->offset() : NULL, row->right()-row->offset(), dstImg);
                rowPlaneToOfxPackedBuffer(Channel_alpha, a ? a+row->offset() : NULL, row->right()-row->offset(), dstImg);
                srcImg.erase(y);
             }
            
            return ret;
        }
    }
    return NULL;
}

OfxImage::OfxImage(BitDepthEnum bitDepth,const OfxRectD& bounds,OfxClipInstance &clip, OfxTime /*t*/):
OFX::Host::ImageEffect::Image(clip),_bitDepth(bitDepth){
    size_t pixSize = 0;
    if(bitDepth == eBitDepthUByte){
        pixSize = 4;
    }else if(bitDepth == eBitDepthUShort){
        pixSize = 8;
    }else if(bitDepth == eBitDepthFloat){
        pixSize = 16;
    }
    assert(bounds.x1 != kOfxFlagInfiniteMin); // what should we do in this case?
    assert(bounds.y1 != kOfxFlagInfiniteMin);
    assert(bounds.x2 != kOfxFlagInfiniteMax);
    assert(bounds.y2 != kOfxFlagInfiniteMax);
    int xmin = (int)std::floor(bounds.x1);
    int xmax = (int)std::ceil(bounds.x2);
    _rowBytes = (xmax - xmin) * pixSize;
    int ymin = (int)std::floor(bounds.y1);
    int ymax = (int)std::ceil(bounds.y2);

    _data = malloc(_rowBytes * (ymax-ymin)) ;
    // render scale x and y of 1.0
    setDoubleProperty(kOfxImageEffectPropRenderScale, 1.0, 0);
    setDoubleProperty(kOfxImageEffectPropRenderScale, 1.0, 1);
    // data ptr
    setPointerProperty(kOfxImagePropData,_data);
    // bounds and rod
    setIntProperty(kOfxImagePropBounds, xmin, 0);
    setIntProperty(kOfxImagePropBounds, ymin, 1);
    setIntProperty(kOfxImagePropBounds, xmax, 2);
    setIntProperty(kOfxImagePropBounds, ymax, 3);
    setIntProperty(kOfxImagePropRegionOfDefinition, xmin, 0);
    setIntProperty(kOfxImagePropRegionOfDefinition, ymin, 1);
    setIntProperty(kOfxImagePropRegionOfDefinition, xmax, 2);
    setIntProperty(kOfxImagePropRegionOfDefinition, ymax, 3);
    // row bytes
    setIntProperty(kOfxImagePropRowBytes, _rowBytes);
    setStringProperty(kOfxImageEffectPropComponents, kOfxImageComponentRGBA);
}

OfxRGBAColourB* OfxImage::pixelB(int x, int y) const{
    assert(_bitDepth == eBitDepthUByte);
    OfxRectI bounds = getBounds();
    if ((x >= bounds.x1) && ( x< bounds.x2) && ( y >= bounds.y1) && ( y < bounds.y2) )
    {
        OfxRGBAColourB* p = reinterpret_cast<OfxRGBAColourB*>((U8*)_data + (y-bounds.y1)*_rowBytes);
        return &(p[x - bounds.x1]);
    }
    return 0;
}
OfxRGBAColourS* OfxImage::pixelS(int x, int y) const{
    assert(_bitDepth == eBitDepthUShort);
    OfxRectI bounds = getBounds();
    if ((x >= bounds.x1) && ( x< bounds.x2) && ( y >= bounds.y1) && ( y < bounds.y2) )
    {
        OfxRGBAColourS* p = reinterpret_cast<OfxRGBAColourS*>((U8*)_data + (y-bounds.y1)*_rowBytes);
        return &(p[x - bounds.x1]);
    }
    return 0;
}
OfxRGBAColourF* OfxImage::pixelF(int x, int y) const{
    assert(_bitDepth == eBitDepthFloat);
    OfxRectI bounds = getBounds();
    if ((x >= bounds.x1) && ( x< bounds.x2) && ( y >= bounds.y1) && ( y < bounds.y2) )
    {
        OfxRGBAColourF* p = reinterpret_cast<OfxRGBAColourF*>((U8*)_data + (y-bounds.y1)*_rowBytes);
        return &(p[x - bounds.x1]);
    }
    return 0;
}

Node* OfxClipInstance::getAssociatedNode() const {
    if(isOutput())
        return _node;
    else{
        int index = 0;
        OfxNode::MappedInputV inputs = _node->inputClipsCopyWithoutOutput();
        for (U32 i = 0; i < inputs.size(); ++i) {
            if (inputs[i]->getName() == getName()) {
                index = i;
                break;
            }
        }
        return _node->input(inputs.size()-1-index);
    }
}

