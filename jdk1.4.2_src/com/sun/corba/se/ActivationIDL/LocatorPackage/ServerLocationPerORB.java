package com.sun.corba.se.ActivationIDL.LocatorPackage;


/**
* com/sun/corba/se/ActivationIDL/LocatorPackage/ServerLocationPerORB.java .
* Generated by the IDL-to-Java compiler (portable), version "3.1"
* from ../../../../../../src/share/classes/com/sun/corba/se/ActivationIDL/activation.idl
* Friday, June 20, 2003 1:46:26 AM PDT
*/


// ORB
public final class ServerLocationPerORB implements org.omg.CORBA.portable.IDLEntity
{
  public String hostname = null;
  public com.sun.corba.se.ActivationIDL.EndPointInfo ports[] = null;

  public ServerLocationPerORB ()
  {
  } // ctor

  public ServerLocationPerORB (String _hostname, com.sun.corba.se.ActivationIDL.EndPointInfo[] _ports)
  {
    hostname = _hostname;
    ports = _ports;
  } // ctor

} // class ServerLocationPerORB
