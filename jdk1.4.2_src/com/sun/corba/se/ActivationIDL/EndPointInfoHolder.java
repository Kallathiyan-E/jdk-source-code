package com.sun.corba.se.ActivationIDL;

/**
* com/sun/corba/se/ActivationIDL/EndPointInfoHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.1"
* from ../../../../../../src/share/classes/com/sun/corba/se/ActivationIDL/activation.idl
* Friday, June 20, 2003 1:46:26 AM PDT
*/


// passing a struct containing endpointType and port-#s
public final class EndPointInfoHolder implements org.omg.CORBA.portable.Streamable
{
  public com.sun.corba.se.ActivationIDL.EndPointInfo value = null;

  public EndPointInfoHolder ()
  {
  }

  public EndPointInfoHolder (com.sun.corba.se.ActivationIDL.EndPointInfo initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = com.sun.corba.se.ActivationIDL.EndPointInfoHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    com.sun.corba.se.ActivationIDL.EndPointInfoHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return com.sun.corba.se.ActivationIDL.EndPointInfoHelper.type ();
  }

}
