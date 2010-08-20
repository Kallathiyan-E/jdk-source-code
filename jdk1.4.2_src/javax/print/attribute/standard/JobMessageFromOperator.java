/*
 * @(#)JobMessageFromOperator.java	1.6 03/01/23
 *
 * Copyright 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
package javax.print.attribute.standard;

import java.util.Locale;

import javax.print.attribute.TextSyntax;
import javax.print.attribute.PrintJobAttribute;

/**
 * Class JobMessageFromOperator is a printing attribute class, a text attribute, 
 * that provides a message from an operator, system administrator, or 
 * "intelligent" process to indicate to the end user the reasons for 
 * modification or other management action taken on a job. 
 * <P>
 * A Print Job's attribute set includes zero instances or one instance of a 
 * JobMessageFromOperator attribute, not more than one instance. A new 
 * JobMessageFromOperator attribute replaces an existing JobMessageFromOperator 
 * attribute, if any. In other words, JobMessageFromOperator is not intended to 
 * be a history log. If it wishes, the client can detect changes to a Print 
 * Job's JobMessageFromOperator attribute and maintain the client's own history 
 * log of the JobMessageFromOperator attribute values. 
 * <P>
 * <B>IPP Compatibility:</B> The string value gives the IPP name value. The 
 * locale gives the IPP natural language. The category name returned by 
 * <CODE>getName()</CODE> gives the IPP attribute name. 
 * <P>
 *
 * @author  Alan Kaminsky
 */
public final class JobMessageFromOperator extends TextSyntax
	implements PrintJobAttribute {

    /**
     * Constructs a new job message from operator attribute with the given 
     * message and locale. 
     *
     * @param  message  Message.
     * @param  locale   Natural language of the text string. null
     * is interpreted to mean the default locale as returned
     * by <code>Locale.getDefault()</code>
     *
     * @exception  NullPointerException
     *     (unchecked exception) Thrown if <CODE>message</CODE> is null. 
     */
    public JobMessageFromOperator(String message, Locale locale) {
	super (message, locale);
    }

    /**
     * Returns whether this job message from operator attribute is equivalent to 
     * the passed in object. To be equivalent, all of the following conditions 
     * must be true: 
     * <OL TYPE=1>
     * <LI>
     * <CODE>object</CODE> is not null.
     * <LI>
     * <CODE>object</CODE> is an instance of class JobMessageFromOperator.
     * <LI>
     * This job message from operator attribute's underlying string and 
     * <CODE>object</CODE>'s underlying string are equal. 
     * <LI>
     * This job message from operator attribute's locale and 
     * <CODE>object</CODE>'s locale are equal. 
     * </OL>
     *
     * @param  object  Object to compare to.
     *
     * @return  True if <CODE>object</CODE> is equivalent to this job
     *          message from operator attribute, false otherwise. 
     */
    public boolean equals(Object object) {
	return (super.equals (object) &&
		object instanceof JobMessageFromOperator);
    }

    /**
     * Get the printing attribute class which is to be used as the "category" 
     * for this printing attribute value.
     * <P>
     * For class JobMessageFromOperator, the 
     * category is class JobMessageFromOperator itself. 
     *
     * @return  Printing attribute class (category), an instance of class
     *          {@link java.lang.Class java.lang.Class}.
     */
    public final Class getCategory() {
	return JobMessageFromOperator.class;
    }
    
    /**
     * Get the name of the category of which this attribute value is an 
     * instance. 
     * <P>
     * For class JobMessageFromOperator, the 
     * category name is <CODE>"job-message-from-operator"</CODE>. 
     *
     * @return  Attribute category name.
     */
    public final String getName() {
	return "job-message-from-operator";
    }

}
