/*
 * @(#)ContentSigner.java	1.4 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package com.sun.jarsigner;

import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;

/**
 * This class defines a content signing service.
 * Implementations must be instantiable using a zero-argument constructor.
 *
 * @since 1.5
 * @version 1.4, 03/23/10
 * @author Vincent Ryan
 */

public abstract class ContentSigner {

    /**
     * Generates a PKCS #7 signed data message.
     * This method is used when the signature has already been generated.
     * The signature, the signer's details, and optionally a signature 
     * timestamp and the content that was signed, are all packaged into a 
     * signed data message.
     *
     * @param parameters The non-null input parameters.
     * @param omitContent true if the content should be omitted from the
     *         signed data message. Otherwise the content is included.
     * @param applyTimestamp true if the signature should be timestamped.
     *         Otherwise timestamping is not performed.
     * @return A PKCS #7 signed data message.
     * @throws NoSuchAlgorithmException The exception is thrown if the signature
     *         algorithm is unrecognised.
     * @throws CertificateException The exception is thrown if an error occurs
     *         while processing the signer's certificate or the TSA's 
     *         certificate.
     * @throws IOException The exception is thrown if an error occurs while
     *         generating the signature timestamp or while generating the signed
     *         data message.
     * @throws NullPointerException The exception is thrown if parameters is 
     *         null.
     */
    public abstract byte[] generateSignedData(
	ContentSignerParameters parameters, boolean omitContent,
	boolean applyTimestamp)
            throws NoSuchAlgorithmException, CertificateException, IOException;
}
