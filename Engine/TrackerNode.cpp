/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2015 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */


// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "TrackerNode.h"
#include "Engine/Node.h"
#include "Engine/TrackerContext.h"

using namespace Natron;

TrackerNode::TrackerNode(boost::shared_ptr<Natron::Node> node)
: Natron::EffectInstance(node)
{
}

TrackerNode::~TrackerNode()
{
    
}

std::string
TrackerNode::getPluginID() const
{
    return PLUGINID_NATRON_TRACKER;
}


std::string
TrackerNode::getPluginLabel() const
{
    return "Tracker";
}

std::string
TrackerNode::getDescription() const
{
    return "Track a 2D point using LibMV from the Blender open-source software.";
}

std::string
TrackerNode::getInputLabel (int inputNb) const
{
    switch (inputNb) {
        case 0:
            return "Source";
        case 1:
            return "Mask";
    }
    return "";
}

bool
TrackerNode::isInputMask(int inputNb) const
{
    return inputNb == 1;
}

bool
TrackerNode::isInputOptional(int inputNb) const
{
    return inputNb == 1;
}

void
TrackerNode::addAcceptedComponents(int inputNb,std::list<Natron::ImageComponents>* comps)
{
    
    if (inputNb != 1) {
        comps->push_back(ImageComponents::getRGBAComponents());
        comps->push_back(ImageComponents::getRGBComponents());
        comps->push_back(ImageComponents::getXYComponents());
    }
    comps->push_back(ImageComponents::getAlphaComponents());
}

void
TrackerNode::addSupportedBitDepth(std::list<Natron::ImageBitDepthEnum>* depths) const
{
    depths->push_back(Natron::eImageBitDepthFloat);
}

void
TrackerNode::getPreferredDepthAndComponents(int inputNb,std::list<Natron::ImageComponents>* comp,Natron::ImageBitDepthEnum* depth) const
{
    
    if (inputNb != 1) {
        comp->push_back(ImageComponents::getRGBAComponents());
    } else {
        comp->push_back(ImageComponents::getAlphaComponents());
    }
    *depth = eImageBitDepthFloat;
}


Natron::ImagePremultiplicationEnum
TrackerNode::getOutputPremultiplication() const
{
    
    EffectInstance* input = getInput(0);
    Natron::ImagePremultiplicationEnum srcPremult = eImagePremultiplicationOpaque;
    if (input) {
        srcPremult = input->getOutputPremultiplication();
    }
    bool processA = getNode()->getProcessChannel(3);
    if (srcPremult == eImagePremultiplicationOpaque && processA) {
        return eImagePremultiplicationUnPremultiplied;
    }
    return eImagePremultiplicationPremultiplied;
}

double
TrackerNode::getPreferredAspectRatio() const
{
    EffectInstance* input = getInput(0);
    if (input) {
        return input->getPreferredAspectRatio();
    } else {
        return 1.;
    }
    
}


void
TrackerNode::initializeKnobs()
{
    
}

bool
TrackerNode::isIdentity(double time,
                        const RenderScale & /*scale*/,
                        const RectI & /*roi*/,
                        int /*view*/,
                        double* inputTime,
                        int* inputNb)
{
    // Identity for now, until we can apply a transform
    *inputTime = time;
    *inputNb = 0;
    return true;
}

void
TrackerNode::onInputChanged(int inputNb)
{
    boost::shared_ptr<TrackerContext> ctx = getNode()->getTrackerContext();
    ctx->s_onNodeInputChanged(inputNb);
}
