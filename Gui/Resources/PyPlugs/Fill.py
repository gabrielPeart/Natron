# -*- coding: utf-8 -*-
# DO NOT EDIT THIS FILE
# This file was automatically generated by Natron PyPlug exporter version 3.

# Hand-written code should be added in a separate file named FillExt.py
# See http://natron.readthedocs.org/en/workshop/groups.html#adding-hand-written-code-callbacks-etc
# Note that Viewers are never exported

import NatronEngine
import sys

# Try to import the extensions file where callbacks and hand-written code should be located.
try:
    from FillExt import *
except ImportError:
    pass

def getPluginID():
    return "fr.inria.Fill"

def getLabel():
    return "Fill"

def getVersion():
    return 1

def getGrouping():
    return "Filter"

def getPluginDescription():
    return "Add a constant color on the source image where the alpha channel not 0. You can control the blending between the original image and the constant color with the operator and the mix factor."

def createInstance(app,group):
    # Create all nodes in the group

    # Start of node "Dot1"
    lastNode = app.createNode("fr.inria.built-in.Dot", 1, group)
    lastNode.setScriptName("Dot1")
    lastNode.setLabel("Dot1")
    lastNode.setPosition(809, 199)
    lastNode.setSize(15, 15)
    lastNode.setColor(0.7, 0.7, 0.7)
    groupDot1 = lastNode

    del lastNode
    # End of node "Dot1"

    # Start of node "Unpremult1"
    lastNode = app.createNode("net.sf.openfx.Unpremult", 2, group)
    lastNode.setScriptName("Unpremult1")
    lastNode.setLabel("Unpremult1")
    lastNode.setPosition(1036, 185)
    lastNode.setSize(104, 34)
    lastNode.setColor(0.3, 0.37, 0.776)
    groupUnpremult1 = lastNode

    param = lastNode.getParam("NatronOfxParamProcessA")
    if param is not None:
        param.setValue(True)
        del param

    del lastNode
    # End of node "Unpremult1"

    # Start of node "Merge1"
    lastNode = app.createNode("net.sf.openfx.MergePlugin", 1, group)
    lastNode.setScriptName("Merge1")
    lastNode.setLabel("Merge1")
    lastNode.setPosition(1036, 302)
    lastNode.setSize(104, 51)
    lastNode.setColor(0.3, 0.37, 0.776)
    groupMerge1 = lastNode

    param = lastNode.getParam("userTextArea")
    if param is not None:
        param.setValue("<font size=\"6\" color=\"#000000\" face=\"Droid Sans\"><Natron>(over)</Natron></font>")
        del param

    del lastNode
    # End of node "Merge1"

    # Start of node "Solid1"
    lastNode = app.createNode("net.sf.openfx.Solid", 1, group)
    lastNode.setScriptName("Solid1")
    lastNode.setLabel("Solid1")
    lastNode.setPosition(1261, 187)
    lastNode.setSize(80, 34)
    lastNode.setColor(0.3, 0.5, 0.2)
    groupSolid1 = lastNode

    param = lastNode.getParam("size")
    if param is not None:
        param.setValue(1920, 0)
        param.setValue(1080, 1)
        del param

    param = lastNode.getParam("color")
    if param is not None:
        param.setValue(1, 0)
        del param

    param = lastNode.getParam("NatronOfxParamProcessA")
    if param is not None:
        param.setValue(True)
        del param

    del lastNode
    # End of node "Solid1"

    # Start of node "Shuffle1"
    lastNode = app.createNode("net.sf.openfx.ShufflePlugin", 1, group)
    lastNode.setScriptName("Shuffle1")
    lastNode.setLabel("Shuffle1")
    lastNode.setPosition(764, 314)
    lastNode.setSize(104, 34)
    lastNode.setColor(0.6, 0.24, 0.39)
    groupShuffle1 = lastNode

    param = lastNode.getParam("outputChannelsChoice")
    if param is not None:
        param.setValue("RGBA")
        del param

    param = lastNode.getParam("outputRChoice")
    if param is not None:
        param.setValue("A.r")
        del param

    param = lastNode.getParam("outputGChoice")
    if param is not None:
        param.setValue("A.g")
        del param

    param = lastNode.getParam("outputBChoice")
    if param is not None:
        param.setValue("A.b")
        del param

    param = lastNode.getParam("outputAChoice")
    if param is not None:
        param.setValue("B.a")
        del param

    del lastNode
    # End of node "Shuffle1"

    # Start of node "Premult1"
    lastNode = app.createNode("net.sf.openfx.Premult", 2, group)
    lastNode.setScriptName("Premult1")
    lastNode.setLabel("Premult1")
    lastNode.setPosition(584, 314)
    lastNode.setSize(104, 34)
    lastNode.setColor(0.3, 0.37, 0.776)
    groupPremult1 = lastNode

    param = lastNode.getParam("NatronOfxParamProcessA")
    if param is not None:
        param.setValue(True)
        del param

    del lastNode
    # End of node "Premult1"

    # Start of node "Input1"
    lastNode = app.createNode("fr.inria.built-in.Input", 1, group)
    lastNode.setScriptName("Input1")
    lastNode.setLabel("Source")
    lastNode.setPosition(764, 73)
    lastNode.setSize(104, 34)
    lastNode.setColor(0.3, 0.5, 0.2)
    groupInput1 = lastNode

    del lastNode
    # End of node "Input1"

    # Start of node "Output1"
    lastNode = app.createNode("fr.inria.built-in.Output", 1, group)
    lastNode.setScriptName("Output1")
    lastNode.setLabel("Output1")
    lastNode.setPosition(584, 442)
    lastNode.setSize(104, 34)
    lastNode.setColor(0.7, 0.7, 0.7)
    groupOutput1 = lastNode

    del lastNode
    # End of node "Output1"

    # Start of node "Input2"
    lastNode = app.createNode("fr.inria.built-in.Input", 1, group)
    lastNode.setScriptName("Input2")
    lastNode.setLabel("Mask")
    lastNode.setPosition(1227, 314)
    lastNode.setSize(104, 34)
    lastNode.setColor(0.3, 0.5, 0.2)
    groupInput2 = lastNode

    param = lastNode.getParam("optional")
    if param is not None:
        param.setValue(True)
        del param

    param = lastNode.getParam("isMask")
    if param is not None:
        param.setValue(True)
        del param

    del lastNode
    # End of node "Input2"

    # Create the parameters of the group node the same way we did for all internal nodes
    lastNode = group

    # Create the user parameters
    lastNode.controls = lastNode.createPageParam("controls", "Controls")
    param = lastNode.createColorParam("Solid1color", "Color", False)
    param.setMinimum(-2147483648, 0)
    param.setMaximum(2147483647, 0)
    param.setDisplayMinimum(0, 0)
    param.setDisplayMaximum(1, 0)
    param.setMinimum(-2147483648, 1)
    param.setMaximum(2147483647, 1)
    param.setDisplayMinimum(0, 1)
    param.setDisplayMaximum(1, 1)
    param.setMinimum(-2147483648, 2)
    param.setMaximum(2147483647, 2)
    param.setDisplayMinimum(0, 2)
    param.setDisplayMaximum(1, 2)

    # Add the param to the page
    lastNode.controls.addParam(param)

    # Set param properties
    param.setAddNewLine(True)
    param.setAnimationEnabled(True)
    param.setValue(1, 0)
    lastNode.Solid1color = param
    del param

    param = lastNode.createChoiceParam("Merge1operation", "Operation")
    param.setDefaultValue(28)

    # Add the param to the page
    lastNode.controls.addParam(param)

    # Set param properties
    param.setAddNewLine(True)
    param.setAnimationEnabled(True)
    lastNode.Merge1operation = param
    del param

    param = lastNode.createBooleanParam("Merge1enableMask_Mask", "Mask")

    # Add the param to the page
    lastNode.controls.addParam(param)

    # Set param properties
    param.setAddNewLine(False)
    param.setAnimationEnabled(False)
    lastNode.Merge1enableMask_Mask = param
    del param

    param = lastNode.createChoiceParam("Merge1maskChannel_Mask", "")
    param.setDefaultValue(4)

    # Add the param to the page
    lastNode.controls.addParam(param)

    # Set param properties
    param.setAddNewLine(False)
    param.setAnimationEnabled(False)
    lastNode.Merge1maskChannel_Mask = param
    del param

    param = lastNode.createBooleanParam("Merge1maskInvert", "Invert Mask")

    # Add the param to the page
    lastNode.controls.addParam(param)

    # Set param properties
    param.setAddNewLine(True)
    param.setAnimationEnabled(False)
    lastNode.Merge1maskInvert = param
    del param

    param = lastNode.createDoubleParam("Merge1mix", "Mix")
    param.setMinimum(0, 0)
    param.setMaximum(1, 0)
    param.setDisplayMinimum(0, 0)
    param.setDisplayMaximum(1, 0)
    param.setDefaultValue(1, 0)

    # Add the param to the page
    lastNode.controls.addParam(param)

    # Set param properties
    param.setAddNewLine(False)
    param.setAnimationEnabled(True)
    lastNode.Merge1mix = param
    del param

    # Refresh the GUI with the newly created parameters
    lastNode.setPagesOrder(['controls', 'Node', 'Info'])
    lastNode.refreshUserParamsGUI()
    del lastNode

    # Now that all nodes are created we can connect them together, restore expressions
    groupDot1.connectInput(0, groupInput1)
    groupUnpremult1.connectInput(0, groupDot1)
    groupMerge1.connectInput(0, groupUnpremult1)
    groupMerge1.connectInput(1, groupSolid1)
    groupMerge1.connectInput(2, groupInput2)
    groupShuffle1.connectInput(0, groupDot1)
    groupShuffle1.connectInput(1, groupMerge1)
    groupPremult1.connectInput(0, groupShuffle1)
    groupOutput1.connectInput(0, groupPremult1)

    param = groupMerge1.getParam("operation")
    group.Merge1operation.setAsAlias(groupMerge1.getParam("operation"))
    del param
    param = groupMerge1.getParam("maskInvert")
    group.Merge1maskInvert.setAsAlias(groupMerge1.getParam("maskInvert"))
    del param
    param = groupMerge1.getParam("mix")
    group.Merge1mix.setAsAlias(groupMerge1.getParam("mix"))
    del param
    param = groupMerge1.getParam("enableMask_Mask")
    group.Merge1enableMask_Mask.setAsAlias(groupMerge1.getParam("enableMask_Mask"))
    del param
    param = groupMerge1.getParam("maskChannel_Mask")
    group.Merge1maskChannel_Mask.setAsAlias(groupMerge1.getParam("maskChannel_Mask"))
    del param
    param = groupSolid1.getParam("color")
    group.Solid1color.setAsAlias(groupSolid1.getParam("color"))
    del param

    try:
        extModule = sys.modules["FillExt"]
    except KeyError:
        extModule = None
    if extModule is not None and hasattr(extModule ,"createInstanceExt") and hasattr(extModule.createInstanceExt,"__call__"):
        extModule.createInstanceExt(app,group)