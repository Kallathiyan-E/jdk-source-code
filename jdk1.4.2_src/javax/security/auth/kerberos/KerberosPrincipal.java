/*
 * @(#)KerberosPrincipal.java	1.15 03/01/23
 *
 * Copyright 2003 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
  
package javax.security.auth.kerberos;

import java.io.*;
import sun.security.krb5.Asn1Exception;
import sun.security.krb5.KrbException;
import sun.security.krb5.PrincipalName;
import sun.security.krb5.Realm;
import sun.security.util.*;

/**
 * This class encapsulates a Kerberos principal.
 *
 * @author Mayank Upadhyay
 * @version 1.15, 01/23/03
 * @since 1.4
 */

public final class KerberosPrincipal 
    implements java.security.Principal, java.io.Serializable {

    //name types
    
    /**
     * unknown name type
     */

    public static final int KRB_NT_UNKNOWN =   0;
    
    /**
     * user principal name type
     */

    public static final int KRB_NT_PRINCIPAL = 1;
    
    /**
     * service and other unique instance (krbtgt) name type
     */
    public static final int KRB_NT_SRV_INST =  2;
    
    /**
     * service with host name as instance (telnet, rcommands) name type
     */

    public static final int KRB_NT_SRV_HST =   3;
    
    /**
     * service with host as remaining components name type
     */

    public static final int KRB_NT_SRV_XHST =  4;
    
    /**
     * unique ID name type
     */

    public static final int KRB_NT_UID = 5;


    private transient String fullName;

    private transient String realm;

    private transient int nameType;

    private static final char NAME_REALM_SEPARATOR = '@';

    /** 
     * Constructs a KerberosPrincipal from the provided string input. The 
     * name type for this  principal defaults to 
     * {@link #KRB_NT_PRINCIPAL KRB_NT_PRINCIPAL}
     * This string is assumed to contain a name in the format
     * that is specified in Section 2.1.1. (Kerberos Principal Name Form) of 
     * <a href=http://www.ietf.org/rfc/rfc1964.txt> RFC 1964 </a>
     * (for example, <i>duke@FOO.COM</i>, where <i>duke</i>
     * represents a principal, and <i>FOO.COM</i> represents a realm).
     * 
     * <p>If the input name does not contain a realm, the default realm
     * is used. The default realm can be specified either in a Kerberos 
     * configuration file or via the java.security.krb5.realm
     * system property. For more information, 
     * <a href="../../../../../guide/security/jgss/tutorials/index.html">
     * Kerberos Requirements </a>
     * 
     * @param name the principal name
     * @throws IllegalArgumentException if name is improperly
     * formatted, if name is null, or if name does not contain 
     * the realm to use and the default realm is not specified
     * in either a Kerberos configuration file or via the 
     * java.security.krb5.realm system property.
     */
    /*
     * TBD:  Research what encoding would be most appropriate to use
     *       when converting the String to bytes. And document that.
     */
    public KerberosPrincipal(String name) {

	PrincipalName krb5Principal = null;

	try {
	    // Appends the default realm if it is missing
	    krb5Principal = new PrincipalName(name, KRB_NT_PRINCIPAL);
	} catch (KrbException e) {
	    throw new IllegalArgumentException(e.getMessage());
	}
	nameType = KRB_NT_PRINCIPAL;  // default name type
	fullName = krb5Principal.toString();
	realm = krb5Principal.getRealmString();
    }

    /** 
     * Constructs a KerberosPrincipal from the provided string and
     * name type input.  The string is assumed to contain a name in the
     * format that is specified in Section 2.1 (Mandatory Name Forms) of
     * <a href=http://www.ietf.org/rfc/rfc1964.txt>RFC 1964</a>.
     * Valid name types are specified in Section 7.2 (Principal Names) of
     * <a href=http://www.ietf.org/rfc/rfc1510.txt>RFC 1510</a>.
     * The input name must be consistent with the provided name type.
     * (for example, <i>duke@FOO.COM</i>, is a valid input string for the
     * name type, KRB_NT_PRINCIPAL where <i>duke</i>
     * represents a principal, and <i>FOO.COM</i> represents a realm).

     * <p> If the input name does not contain a realm, the default realm
     * is used. The default realm can be specified either in a Kerberos
     * configuration file or via the java.security.krb5.realm
     * system property. For more information, see
     * <a href="../../../../../guide/security/jgss/tutorials/index.html">
     * Kerberos Requirements</a>.
     * 
     * @param name the principal name
     * @param nameType the name type of the principal 
     * @throws IllegalArgumentException if name is improperly
     * formatted, if name is null, if the nameType is not supported,  
     * or if name does not contain the realm to use and the default 
     * realm is not specified in either a Kerberos configuration 
     * file or via the java.security.krb5.realm system property.
     */

    public KerberosPrincipal(String name, int nameType) {

	PrincipalName krb5Principal = null;

	try {
	    // Appends the default realm if it is missing
	    krb5Principal  = new PrincipalName(name,nameType);
	} catch (KrbException e) {
	    throw new IllegalArgumentException(e.getMessage());
	}
	 
	this.nameType = nameType;
	fullName = krb5Principal.toString();
	realm = krb5Principal.getRealmString();
    }
    /**
     * Returns the realm component of this Kerberos principal.
     *
     * @return the realm component of this Kerberos principal.
     */
    public String getRealm() {
	return realm;
    }

    /**
     * Returns a hashcode for this principal. The hash code is defined to 
     * be the result of the following  calculation:
     * <pre><code>
     *  hashCode = getName().hashCode();
     * </code></pre>
     * 
     * @return a hashCode() for the <code>KerberosPrincipal</code>
     */
    public int hashCode() {
	return getName().hashCode();
    }

    /**
     * Compares the specified Object with this Principal for equality.
     * Returns true if the given object is also a 
     * <code>KerberosPrincipal</code> and the two
     * <code>KerberosPrincipal</code> instances are equivalent. 
     * More formally two <code>KerberosPrincipal</code> instances are equal 
     * if the values returned by <code>getName()</code> are equal and the 
     * values returned by <code>getNameType()</code> are equal.
     *
     * @param other the Object to compare to
     * @return true if the Object passed in represents the same principal
     * as this one, false otherwise.
     */
    public boolean equals(Object other) {

	if (other == this)
	    return true;

	if (! (other instanceof KerberosPrincipal)) {
	    return false;
	} else {
	    String myFullName = getName();
	    String otherFullName = ((KerberosPrincipal) other).getName();
	    if (nameType == ((KerberosPrincipal)other).nameType && 
		myFullName.equals(otherFullName)) {
		 return true;
	    }
	} 
	return false;
    }

    /**
     * Save the KerberosPrincipal object to a stream
     *
     * @serialData this <code>KerberosPrincipal</code> is serialized
     *		by writing out the PrincipalName and the
     *		realm in their DER-encoded form as specified in Section 5.2 of
     *		<a href=http://www.ietf.org/rfc/rfc1510.txt> RFC1510</a>. 
     */ 

    private synchronized void writeObject(ObjectOutputStream oos) 
	throws IOException {

	PrincipalName krb5Principal = null;
	try {
	    krb5Principal  = new PrincipalName(fullName,nameType);
	    oos.writeObject(krb5Principal.asn1Encode());
	    oos.writeObject(krb5Principal.getRealm().asn1Encode());
	} catch (Exception e) {
	    IOException ioe = new IOException(e.getMessage());
	    ioe.initCause(e);
	    throw ioe; 
	}
    }

    /**
     * Reads this object from a stream (i.e., deserializes it)
     */

    private synchronized void readObject(ObjectInputStream ois)
	 throws IOException, ClassNotFoundException {
	byte[] asn1EncPrincipal = (byte [])ois.readObject();
	byte[] encRealm = (byte [])ois.readObject();
	try {
	   PrincipalName krb5Principal = new PrincipalName(new 
						DerValue(asn1EncPrincipal));
	   realm = (new Realm(new DerValue(encRealm))).toString();
	   fullName = krb5Principal.toString() + NAME_REALM_SEPARATOR +
			 realm.toString(); 
	   nameType = krb5Principal.getNameType();
	} catch (Exception e) {
	    IOException ioe = new IOException(e.getMessage());
	    ioe.initCause(e);
	    throw ioe; 
	}
    }	
	
    /**
     * The returned string corresponds to the single-string
     * representation of a Kerberos Principal name as specified in
     * Section 2.1 of <a href=http://www.ietf.org/rfc/rfc1964.txt>RFC 1964</a>.
     *
     * @return the principal name.
     */
    public String getName() {
	return fullName;
    }

    /**
     * Returns the name type of the KerberosPrincipal. Valid name types
     * are specified in Section 7.2 of
     * <a href=http://www.ietf.org/rfc/rfc1510.txt> RFC1510</a>. 
     *
     * @return the name type.
     *
     */

    public int getNameType() {
	return nameType;
    }
    
    // Inherits javadocs from Object
    public String toString() {
	return getName();
    }
}
