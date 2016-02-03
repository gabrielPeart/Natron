/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
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

#ifndef Engine_TrackMarker_h
#define Engine_TrackMarker_h

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****
#include <set>
#include <list>

#if !defined(Q_MOC_RUN) && !defined(SBK_RUN)
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#endif
#include "Engine/EngineFwd.h"
#include "Engine/Knob.h"

#define kTrackerParamSearchWndBtmLeft "searchWndBtmLeft"
#define kTrackerParamSearchWndBtmLeftLabel "Search Window Bottom Left"
#define kTrackerParamSearchWndBtmLeftHint "The bottom left point of the search window, relative to the center point."

#define kTrackerParamSearchWndTopRight "searchWndTopRight"
#define kTrackerParamSearchWndTopRightLabel "Search Window Top Right"
#define kTrackerParamSearchWndTopRightHint "The top right point of the search window, relative to the center point."

#define kTrackerParamPatternTopLeft "patternTopLeft"
#define kTrackerParamPatternTopLeftLabel "Pattern Top Left"
#define kTrackerParamPatternTopLeftHint "The top left point of the quad defining the pattern to track"

#define kTrackerParamPatternTopRight "patternTopRight"
#define kTrackerParamPatternTopRightLabel "Pattern Top Right"
#define kTrackerParamPatternTopRightHint "The top right point of the quad defining the pattern to track"

#define kTrackerParamPatternBtmRight "patternBtmRight"
#define kTrackerParamPatternBtmRightLabel "Pattern Bottom Right"
#define kTrackerParamPatternBtmRightHint "The bottom right point of the quad defining the pattern to track"

#define kTrackerParamPatternBtmLeft "patternBtmLeft"
#define kTrackerParamPatternBtmLeftLabel "Pattern Bottom Left"
#define kTrackerParamPatternBtmLeftHint "The bottom left point of the quad defining the pattern to track"

#define kTrackerParamCenter "centerPoint"
#define kTrackerParamCenterLabel "Center"
#define kTrackerParamCenterHint "The point to track"

#define kTrackerParamOffset "offset"
#define kTrackerParamOffsetLabel "Offset"
#define kTrackerParamOffsetHint "The offset applied to the center point relative to the real tracked position"

#define kTrackerParamTrackWeight "trackWeight"
#define kTrackerParamTrackWeightLabel "Weight"
#define kTrackerParamTrackWeightHint "The weight determines the amount this marker contributes to the final solution"

#define kTrackerParamMotionModel "motionModel"
#define kTrackerParamMotionModelLabel "Motion model"
#define kTrackerParamMotionModelHint "The motion model to use for tracking."

#define kTrackerParamMotionModelTranslation "Search for markers that are only translated between frames."
#define kTrackerParamMotionModelTransRot "Search for markers that are translated and rotated between frames."
#define kTrackerParamMotionModelTransScale "Search for markers that are translated and scaled between frames."
#define kTrackerParamMotionModelTransRotScale "Search for markers that are translated, rotated and scaled between frames."
#define kTrackerParamMotionModelAffine "Search for markers that are affine transformed (t,r,k and skew) between frames."
#define kTrackerParamMotionModelPerspective "Search for markers that are perspectively deformed (homography) between frames."

#define kTrackerParamCorrelation "correlation"
#define kTrackerParamCorrelationLabel "Correlation"
#define kTrackerParamCorrelationHint "The correlation score obtained after tracking each frame"

NATRON_NAMESPACE_ENTER;

struct TrackMarkerPrivate;
class TrackMarker : public KnobHolder, public boost::enable_shared_from_this<TrackMarker>
{
    Q_OBJECT
    
public:
    
    TrackMarker(const boost::shared_ptr<TrackerContext>& context);
    
    ~TrackMarker();
    
    void clone(const TrackMarker& other);
    
    void load(const TrackSerialization& serialization);
    
    void save(TrackSerialization* serialization) const;
    
    boost::shared_ptr<TrackerContext> getContext() const;
    
    bool setScriptName(const std::string& name);
    std::string getScriptName() const;
    
    void setLabel(const std::string& label);
    std::string getLabel() const;
    
    boost::shared_ptr<KnobDouble> getSearchWindowBottomLeftKnob() const;
    boost::shared_ptr<KnobDouble> getSearchWindowTopRightKnob() const;
    boost::shared_ptr<KnobDouble> getPatternTopLeftKnob() const;
    boost::shared_ptr<KnobDouble> getPatternTopRightKnob() const;
    boost::shared_ptr<KnobDouble> getPatternBtmRightKnob() const;
    boost::shared_ptr<KnobDouble> getPatternBtmLeftKnob() const;
    boost::shared_ptr<KnobDouble> getWeightKnob() const;
    boost::shared_ptr<KnobDouble> getCenterKnob() const;
    boost::shared_ptr<KnobDouble> getOffsetKnob() const;
    boost::shared_ptr<KnobDouble> getCorrelationKnob() const;
    boost::shared_ptr<KnobChoice> getMotionModelKnob() const;
    
    const std::list<boost::shared_ptr<KnobI> >& getKnobs() const;
    
    int getReferenceFrame(int time, bool forward) const;
    
    bool isUserKeyframe(int time) const;
    
    int getPreviousKeyframe(int time) const;
    
    int getNextKeyframe( int time) const;
    
    void getUserKeyframes(std::set<int>* keyframes) const;
    
    void getCenterKeyframes(std::set<double>* keyframes) const;
    
    bool isEnabled() const;
    
    void setEnabled(bool enabled, int reason);
    
    void resetCenter();
    
    void resetOffset();
    
    void resetTrack();
    
    void setUserKeyframe(int time);
    
    void removeUserKeyframe(int time);
    
    void removeAllKeyframes();
    
    std::pair<boost::shared_ptr<Natron::Image>,RectI> getMarkerImage(int time,const RectI& roi) const;
    
    RectI getMarkerImageRoI(int time) const;
    
    public Q_SLOTS:
    
    void onCenterKeyframeSet(double time,int dimension,int reason,bool added);
    void onCenterKeyframeRemoved(double time,int dimension,int reason);
    void onCenterKeyframeMoved(int dimension,double oldTime,double newTime);
    void onCenterKeyframesSet(const std::list<double>& keys, int dimension, int reason);
    void onCenterAnimationRemoved(int dimension);
    
    void onCenterKnobValueChanged(int dimension,int reason);
    void onOffsetKnobValueChanged(int dimension,int reason);
    void onCorrelationKnobValueChanged(int dimension,int reason);
    void onWeightKnobValueChanged(int dimension,int reason);
    void onMotionModelKnobValueChanged(int dimension,int reason);
    
    /*void onPatternTopLeftKnobValueChanged(int dimension,int reason);
     void onPatternTopRightKnobValueChanged(int dimension,int reason);
     void onPatternBtmRightKnobValueChanged(int dimension,int reason);
     void onPatternBtmLeftKnobValueChanged(int dimension,int reason);*/
    
    void onSearchBtmLeftKnobValueChanged(int dimension,int reason);
    void onSearchTopRightKnobValueChanged(int dimension,int reason);
    
private:
    
    void evaluate(KnobI* /*knob*/,bool /*isSignificant*/,ValueChangedReasonEnum /*reason*/) OVERRIDE FINAL {}
    
    virtual void initializeKnobs() OVERRIDE FINAL {}
    
    boost::scoped_ptr<TrackMarkerPrivate> _imp;
    
};

NATRON_NAMESPACE_EXIT;


#endif // Engine_TrackMarker_h
