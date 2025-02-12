/*
 * @(#)MScrollbarPeer.java	1.32 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
package sun.awt.motif;

import java.awt.*;
import java.awt.peer.*;
import java.awt.event.AdjustmentEvent;

class MScrollbarPeer extends MComponentPeer implements ScrollbarPeer {
    static {
	initIDs();
    }

    private boolean inUpCall = false;

    native void create(MComponentPeer parent);

    MScrollbarPeer(Scrollbar target) {
	super(target);
    }

    // Initialize JNI field and method IDs
    private static native void initIDs();

    public native void pSetValues(int value, int visible, int minimum, int maximum);
    public native void setLineIncrement(int l);
    public native void setPageIncrement(int l);

    /**
     * Returns default size of Motif scrollbar on the platform
     * Currently uses hardcoded values
     */
    int getDefaultDimension() {
        if (System.getProperty("os.name").equals("Linux")) {
            return 15;
        } else {
            return 19;
        }
    }

    public Dimension getMinimumSize() {
	if (((Scrollbar)target).getOrientation() == Scrollbar.VERTICAL) {
	    return new Dimension(getDefaultDimension(), 50);
	} else {
	    return new Dimension(50, getDefaultDimension());
	}
    }

    // NOTE: Callback methods are called by privileged threads.
    //	     DO NOT INVOKE CLIENT CODE ON THIS THREAD!

    private void postAdjustmentEvent(final int type, final int value,
				     final boolean isAdjusting)
    {
	final Scrollbar sb = (Scrollbar)target;
	MToolkit.executeOnEventHandlerThread(sb, new Runnable() {
	    public void run() {
                inUpCall = true;
		sb.setValueIsAdjusting(isAdjusting);
		sb.setValue(value);
		postEvent(new AdjustmentEvent(sb,
				AdjustmentEvent.ADJUSTMENT_VALUE_CHANGED,
				type, value, isAdjusting));
                inUpCall = false;
	    }
	});
    }

    void lineUp(int value) {
	postAdjustmentEvent(AdjustmentEvent.UNIT_DECREMENT, value, false);
    }

    void lineDown(int value) {
	postAdjustmentEvent(AdjustmentEvent.UNIT_INCREMENT, value, false);
    }

    void pageUp(int value) {
	postAdjustmentEvent(AdjustmentEvent.BLOCK_DECREMENT, value, false);
    }

    void pageDown(int value) {
	postAdjustmentEvent(AdjustmentEvent.BLOCK_INCREMENT, value, false);
    }

    // SB_TOP/BOTTOM are mapped to tracking
    void warp(int value) {
	postAdjustmentEvent(AdjustmentEvent.TRACK, value, false);
    }

    private boolean dragInProgress = false;

    void drag(final int value) {
	if (!dragInProgress) {
	    dragInProgress = true;
	}
	postAdjustmentEvent(AdjustmentEvent.TRACK, value, true);
    }

    void dragEnd(final int value) {
	final Scrollbar sb = (Scrollbar)target;

	if (!dragInProgress) {
	    return;
	}

	dragInProgress = false;
	MToolkit.executeOnEventHandlerThread(sb, new Runnable() {
	    public void run() {
		// NB: notification only, no sb.setValue()
		// last TRACK event will have done it already
                inUpCall = true;
		sb.setValueIsAdjusting(false);
		postEvent(new AdjustmentEvent(sb,
				AdjustmentEvent.ADJUSTMENT_VALUE_CHANGED,
				AdjustmentEvent.TRACK, value, false));
                inUpCall = false;
	    }
	});
    }

    /**
     * Set the value of the slider in the ScrollBar.
     */
    public void setValues(int value, int visible, int minimum, int maximum) {
        // Fix for BugTraq ID 4048060.  Prevent unnecessary redrawing
        // of the slider, when the slider is already in the correct
        // position.  Since the ScrollBar widget now receives the
        // ButtonRelease X event before the Java Adjustor event is
        // handled, the slider is already in the correct position and
        // does not need to be set again and redrawn, when processing
        // the Adjustor event.
        if (!inUpCall) {
            pSetValues(value, visible, minimum, maximum);
        }
    }

    public void print(Graphics g) {
        Scrollbar sb = (Scrollbar)target;
	Dimension d = sb.size();
	Color bg = sb.getBackground();

	boolean horiz = (sb.getOrientation() == Scrollbar.HORIZONTAL);

	drawScrollbar(g, bg, horiz? d.height : d.width,
		          horiz? d.width : d.height,
			  sb.getMinimum(), sb.getMaximum(),
			  sb.getValue(), sb.getVisible(),
			  horiz);

	target.print(g);
    }


    /**
     * DEPRECATED
     */
    public Dimension minimumSize() {
	    return getMinimumSize();
    }

    protected boolean shouldFocusOnClick() {
        // Changed in 1.4 - scroll bars are made focusable by mouse clicks.
        return true;
    }
}
