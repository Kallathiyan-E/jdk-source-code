/*
 * @(#)MMenuItemPeer.java	1.45 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
package sun.awt.motif;

import java.awt.*;
import java.awt.peer.*;
import java.awt.event.ActionEvent;
import sun.awt.AppContext;

class MMenuItemPeer implements MenuItemPeer {
    long	pData;
    long        jniGlobalRef;
    boolean	isCheckbox = false;
    MenuItem	target;
    boolean     nativeCreated = false;

    private boolean disposed = false;

    static {
        initIDs();
    }

    /**
     * Initialize JNI field and method IDs
     */
    private static native void initIDs();

    native void createMenuItem(MMenuPeer parent);

    void create(MMenuPeer parent) {
        if (parent.nativeCreated) {
            createMenuItem(parent);
            nativeCreated = true;
            setEnabled(target.isEnabled());
        }
    }

    protected MMenuItemPeer() {
    }

    MMenuItemPeer(MenuItem target) {
	this.target = target;
	MMenuPeer parent = (MMenuPeer) MToolkit.targetToPeer(getParent_NoClientCode(target));
	create(parent);
    }

    static native MenuContainer getParent_NoClientCode(MenuComponent menuComponent);

    protected void finalize() throws Throwable {
        dispose();
	super.finalize();
    }

    public void setEnabled(boolean b) {
	if (b) {
	    enable();
	} else {
	    disable();
	}
    }

    public void setLabel(String label) {
        if (!nativeCreated) {
            return;
        }
        pSetLabel(label);
        // Fix for bug 4234266 AWT component : MenuItem  throw NullPointer exception.
        MenuShortcut sc = target.getShortcut();
        setShortcut(sc != null ? sc.toString() : null );
    }

    public void setShortcut(String shortCut) {
        if (!nativeCreated) {
            return;
        }
        pSetShortcut(shortCut);
    }

    native void pSetLabel(String label);
    native void pSetShortcut(String shortCut);

    /**
     * DEPRECATED but, for now, called by setEnabled(boolean).
     */
    public void enable() {
        if (!nativeCreated) {
            return;
        }
        pEnable();
    }
    native void pEnable();

    /**
     * DEPRECATED but, for now, called by setEnabled(boolean).
     */
    public void disable() {
        if (!nativeCreated) {
            return;
        }
        pDisable();
    }
    native void pDisable();

    private void destroyNativeWidgetImpl() {
        if (nativeCreated) {
	    pDispose();
	    nativeCreated = false;
	}
    }

    void destroyNativeWidget() {
        // We do not need to synchronize this method because the caller
        // always holds the tree lock

        destroyNativeWidgetImpl();
    }

    /*
     * Subclasses should override disposeImpl() instead of dispose(). Client
     * code should always invoke dispose(), never disposeImpl().
     */
    protected void disposeImpl() {
        // Don't call destroyNativeWidget() because on a Menu, this will
        // cause a traversal of all the menu's MenuItems. This traversal was
        // already done once by java.awt.Menu.removeNotify().

        destroyNativeWidgetImpl();
        MToolkit.targetDisposedPeer(target, this);
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

    native void pDispose();

    void postEvent(AWTEvent event) {
        MToolkit.postEvent(MToolkit.targetToAppContext(target), event);
    }

    // NOTE: This method may be called by privileged threads.
    //       DO NOT INVOKE CLIENT CODE ON THIS THREAD!
    public void action(final long when, final int modifiers) {
    
	MToolkit.executeOnEventHandlerThread(target, new Runnable() {
	    public void run() {
                postEvent(new ActionEvent(target, ActionEvent.ACTION_PERFORMED,
                                          target.getActionCommand(), when,
                                          modifiers));
	    }
	});
    }

    // Needed for MenuComponentPeer.
    public void setFont(Font f) {
    }
}
