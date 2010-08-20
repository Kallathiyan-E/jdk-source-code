/*
 * @(#)MetaEventListener.java	1.15 03/01/23
 *
 * Copyright 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package javax.sound.midi;		  	 

import java.util.EventListener;


/**
 * The <code>MetaEventListener</code> interface should be implemented
 * by classes whose instances need to be notified when a <code>{@link Sequencer}</code> 
 * has processed a <code>{@link MetaMessage}</code>.  
 * To register a <code>MetaEventListener</code> object to receive such 
 * notifications, pass it as the argument to the 
 * <code>{@link Sequencer#addMetaEventListener(MetaEventListener) addMetaEventListener}</code>
 * method of <code>Sequencer</code>.
 *
 * @version 1.15, 03/01/23
 * @author Kara Kytle
 */
public interface MetaEventListener extends EventListener {

    /**
     * Invoked when a <code>{@link Sequencer}</code> has encountered and processed 
     * a <code>MetaMessage</code> in the <code>{@link Sequence}</code> it is processing.
     * @param meta the meta-message that the sequencer encountered 
     */
    public void meta(MetaMessage meta);
}
