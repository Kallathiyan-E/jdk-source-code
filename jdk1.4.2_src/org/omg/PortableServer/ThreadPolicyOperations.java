package org.omg.PortableServer;


/**
* org/omg/PortableServer/ThreadPolicyOperations.java .
* Generated by the IDL-to-Java compiler (portable), version "3.1"
* from ../../../../src/share/classes/org/omg/PortableServer/poa.idl
* Friday, June 20, 2003 8:34:37 AM GMT
*/


/**
	 * The ThreadPolicy specifies the threading model 
	 * used with the created POA. The default is 
	 * ORB_CTRL_MODEL.
	 */
public interface ThreadPolicyOperations  extends org.omg.CORBA.PolicyOperations
{

  /**
	 * specifies the policy value
	 */
  org.omg.PortableServer.ThreadPolicyValue value ();
} // interface ThreadPolicyOperations
