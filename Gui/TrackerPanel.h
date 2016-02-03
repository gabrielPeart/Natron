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




#ifndef TRACKERPANEL_H
#define TRACKERPANEL_H


// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include <set>

#if !defined(Q_MOC_RUN) && !defined(SBK_RUN)
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#endif

#include "Global/Macros.h"
CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QWidget>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Global/GlobalDefines.h"
#include "Gui/GuiFwd.h"

NATRON_NAMESPACE_ENTER;

/**
* @brief This is the new tracker panel, the previous version TrackerPanelV1 (used for TrackerPM) can be found in MultiInstancePanel.h
**/
struct TrackerPanelPrivate;
class TrackerPanel
: public QWidget
{
        Q_OBJECT

public:
    
    TrackerPanel(const NodeGuiPtr& n,
                 QWidget* parent);
    
    virtual ~TrackerPanel();
    
    void addTableRow(const boost::shared_ptr<TrackMarker> & marker);
    
    void insertTableRow(const boost::shared_ptr<TrackMarker> & marker, int index);
    
    void removeRow(int row);
    
    void removeMarker(const boost::shared_ptr<TrackMarker> & marker);
    
    TableItem* getItemAt(int row, int column) const;
    
    int getMarkerRow(const boost::shared_ptr<TrackMarker> & marker) const;
    
    TableItem* getItemAt(const boost::shared_ptr<TrackMarker> & marker, int column) const;
    
    boost::shared_ptr<TrackMarker> getRowMarker(int row) const;
    
    boost::shared_ptr<KnobI> getKnobAt(int row, int column, int* dimension) const;
    
    boost::shared_ptr<TrackerContext> getContext() const;
    
    NodeGuiPtr getNode() const;
    
    void getSelectedRows(std::set<int>* rows) const;
    
    void blockSelection();
    void unblockSelection();
    
    void pushUndoCommand(QUndoCommand* command);
    
    void clearAndSelectTracks(const std::list<boost::shared_ptr<TrackMarker> >& markers, int reason);
    
public Q_SLOTS:
    
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onSelectAllButtonClicked();
    void onResetButtonClicked();
    void onAverageButtonClicked();
    void onExportButtonClicked();
    
    void onGoToPrevKeyframeButtonClicked();
    void onGoToNextKeyframeButtonClicked();
    void onAddKeyframeButtonClicked();
    void onRemoveKeyframeButtonClicked();
    void onRemoveAnimationButtonClicked();
    
    void onModelSelectionChanged(const QItemSelection & oldSelection,const QItemSelection & newSelection);
    void onItemDataChanged(TableItem* item);
    void onItemEnabledCheckBoxChecked(bool checked);
    void onItemMotionModelChanged(int index);
    void onItemRightClicked(TableItem* item);
    
    void onContextSelectionAboutToChange(int reason);
    void onContextSelectionChanged(int reason);
    
    void onTrackKeyframeSet(const boost::shared_ptr<TrackMarker>& marker, int key);
    void onTrackKeyframeRemoved(const boost::shared_ptr<TrackMarker>& marker, int key);
    void onTrackAllKeyframesRemoved(const boost::shared_ptr<TrackMarker>& marker);
    
    void onKeyframeSetOnTrackCenter(const boost::shared_ptr<TrackMarker> &marker, int key);
    void onKeyframeRemovedOnTrackCenter(const boost::shared_ptr<TrackMarker> &marker, int key);
    void onAllKeyframesRemovedOnTrackCenter(const boost::shared_ptr<TrackMarker> &marker);
    void onMultipleKeyframesSetOnTrackCenter(const boost::shared_ptr<TrackMarker> &marker, const std::list<double>& keys);
    
    void onSettingsPanelClosed(bool closed);
    void onTrackAboutToClone(const boost::shared_ptr<TrackMarker>& marker);
    void onTrackCloned(const boost::shared_ptr<TrackMarker>& marker);
    
    void onTrackInserted(const boost::shared_ptr<TrackMarker>& marker, int index);
    void onTrackRemoved(const boost::shared_ptr<TrackMarker>& marker);
    
    void onEnabledChanged(const boost::shared_ptr<TrackMarker>& marker,int reason);
    
    void onCenterKnobValueChanged(const boost::shared_ptr<TrackMarker>& marker,int,int);
    void onOffsetKnobValueChanged(const boost::shared_ptr<TrackMarker>& marker,int,int);
    void onCorrelationKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    void onWeightKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    void onMotionModelKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    
    /*void onPatternTopLeftKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    void onPatternTopRightKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    void onPatternBtmRightKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    void onPatternBtmLeftKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    
    void onSearchBtmLeftKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);
    void onSearchTopRightKnobValueChanged(const boost::shared_ptr<TrackMarker> &marker,int,int);*/
    
    
    void onTimeChanged(SequenceTime time, int reason);
    
private:
    
    void onSelectionAboutToChangeInternal(const std::list<boost::shared_ptr<TrackMarker> >& markers);
    void selectInternal(const std::list<boost::shared_ptr<TrackMarker> >& markers, int reason);
    boost::shared_ptr<TrackMarker> makeTrackInternal();
    
    boost::scoped_ptr<TrackerPanelPrivate> _imp;
};

NATRON_NAMESPACE_EXIT;

#endif // TRACKERPANEL_H
