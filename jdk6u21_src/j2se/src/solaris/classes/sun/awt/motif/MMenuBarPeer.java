/*
 * @(#)MMenuBarPeer.java	1.32 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
package sun.awt.motif;

import java.awt.*;
import java.awt.peer.*;
import sun.awt.*;

public class MMenuBarPeer implements MenuBarPeer {
    long	pData;
    MenuBar	target;
    private X11GraphicsConfig	graphicsConfig=null;

    private boolean disposed = false;

    static {
	initIDs();
    }

     /**
     * Initialize JNI field and method IDs for fields that may be accessed
       from C.
     */
    private static native void initIDs();

    native void create(MFramePeer f);

    public MMenuBarPeer(MenuBar target) {
	this.target = target;
	MFramePeer parent = (MFramePeer) MToolkit.targetToPeer(MMenuItemPeer.getParent_NoClientCode(target));
	create(parent);
    }

    protected void finalize() throws Throwable {
        dispose();
	super.finalize();
    }

    /*
     * Subclasses should override disposeImpl() instead of dispose(). Client
     * code should always invoke dispose(), never disposeImpl().
     */
    private native void pDispose();
    protected void disposeImpl() {
        MToolkit.targetDisposedPeer(target, this);
	pDispose();
    }
    public final void dispose() {
        boolean call_disposeImpl = false;

        if (!disposed) {
            synchronized (this) {
                if (!disposed) {
                    disposed = call_disposeImpl = true;
                }
            }
        }

        if (call_disposeImpl) {
            disposeImpl();
        }
    }
    public void addMenu(Menu m) {
    }
    public void delMenu(int index) {
    }
    public void addHelpMenu(Menu m) {
    }

    static final int GAP = 10;
    static final int W_DIFF = (MFramePeer.CROSSHAIR_INSET + 1) * 2;
    static final int H_DIFF = MFramePeer.BUTTON_Y + MFramePeer.BUTTON_H;

    /*
     * Print the native component by rendering the Motif look ourselves.
     * ToDo(aim): needs to query native motif for more appropriate size and
     * color information.
     */
    void print(Graphics g) {
        MenuBar mb = (MenuBar)target;
        Frame f = (Frame)MMenuItemPeer.getParent_NoClientCode(target);
	Dimension fd = f.size();
	Insets insets = f.insets();
        
        /* Calculate menubar dimension. */
        int width = fd.width;
        int height = insets.top;
        if (f.getPeer() instanceof MFramePeer) {
            MFramePeer fpeer = (MFramePeer)f.getPeer();
            if (fpeer.hasDecorations(MWindowAttributes.AWT_DECOR_BORDER)) {
                width -= W_DIFF;
                height -= MFramePeer.BUTTON_Y;
            }
            if (fpeer.hasDecorations(MWindowAttributes.AWT_DECOR_MENU)) {
                height -= MFramePeer.BUTTON_H;
            }
        }
        Dimension d = new Dimension(width, height);

	Shape oldClipArea = g.getClip();
	g.clipRect(0, 0, d.width, d.height);

	Color bg = f.getBackground();
	Color fg = f.getForeground();
	Color highlight = bg.brighter();
	Color shadow = bg.darker();

        // because we'll most likely be drawing on white paper,
        // for aesthetic reasons, don't make any part of the outer border
        // pure white
        if (highlight.equals(Color.white)) {
            g.setColor(new Color(230, 230, 230));
        }
        else {
            g.setColor(highlight);
        }
	g.drawLine(0, 0, d.width, 0);
	g.drawLine(1, 1, d.width - 1, 1);
	g.drawLine(0, 0, 0, d.height);
	g.drawLine(1, 1, 1, d.height - 1);
	g.setColor(shadow);
	g.drawLine(d.width, 1, d.width, d.height);
	g.drawLine(d.width - 1, 2, d.width - 1, d.height);
	g.drawLine(1, d.height, d.width, d.height);
	g.drawLine(2, d.height - 1, d.width, d.height - 1);

	int x = GAP;
	int nitems = mb.countMenus();

	Menu helpMenu = target.getHelpMenu();

	for (int i = 0 ; i < nitems ; i++) {
	    Menu mn = target.getMenu(i);
	    String item = mn.getLabel();
            if (item == null) {
                item = "";
            }
	    Font menuFont = mn.getFont();
	    g.setFont(menuFont);
	    FontMetrics menuMetrics = g.getFontMetrics();
	    int y = (d.height / 2) + menuMetrics.getMaxDescent();
	    int w = menuMetrics.stringWidth(item) + GAP * 2;

	    if (x >= d.width) {
	        break;
	    }
	    if (mn.isEnabled()) {
	        g.setColor(fg);
	    }
	    else {
	          // draw text as grayed out
	        g.setColor(shadow);
	    }

	    if (helpMenu == mn) {
	        g.drawString(item, d.width - w + GAP, y);
	    }
	    else {
	        g.drawString(item, x, y);
		x += w;
	    }
	}

	g.setClip(oldClipArea);
    }

    // Needed for MenuComponentPeer.
    public void setFont(Font f) {
    }
}
