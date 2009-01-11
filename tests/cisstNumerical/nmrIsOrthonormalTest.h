/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: nmrIsOrthonormalTest.h,v 1.11 2007/04/26 20:12:05 anton Exp $
  
  Author(s):  Anton Deguet
  Created on: 2005-07-27
  
  (C) Copyright 2005-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


#ifndef _nmrIsOrthonormalTest_h
#define _nmrIsOrthonormalTest_h

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cisstVector/vctRandomFixedSizeMatrix.h>
#include <cisstVector/vctRandomDynamicMatrix.h>

#include <cisstNumerical/nmrNetlib.h>

#ifdef CISST_HAS_NETLIB
#include <cisstNumerical/nmrSVD.h>
#endif // CISST_HAS_NETLIB

#include <cisstNumerical/nmrIsOrthonormal.h>

class nmrIsOrthonormalTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(nmrIsOrthonormalTest);

    CPPUNIT_TEST(TestRotationMatrix);

#ifdef CISST_HAS_NETLIB
    CPPUNIT_TEST(TestFunctionDynamic);
    CPPUNIT_TEST(TestFunctionWithReferenceDynamic);
    CPPUNIT_TEST(TestDynamicData);
    CPPUNIT_TEST(TestDynamicDataWithReferenceDynamic);

    CPPUNIT_TEST(TestFunctionFixedSize);
    CPPUNIT_TEST(TestFixedSizeData);
#endif // CISST_HAS_NETLIB

    CPPUNIT_TEST_SUITE_END();

protected:
#ifdef CISST_HAS_NETLIB
    vctDynamicMatrix<double> UDynamic, VtDynamic;
    enum {SIZE = 12};
    vctFixedSizeMatrix<double, SIZE, SIZE> UFixedSize, VtFixedSize;
#endif // CISST_HAS_NETLIB

public:

    void setUp()
    {
#ifdef CISST_HAS_NETLIB
        // create a random matrix (size and content) to decompose
        // using SVD and then use U and Vt to test nmrIsOrthonormal
        int rows, cols;
        bool storageOrder = VCT_ROW_MAJOR;
        vctDynamicMatrix<double> ADynamic;
        vctDynamicVector<double> SDynamic;
        cmnRandomSequence & randomSequence = cmnRandomSequence::GetInstance();
        randomSequence.ExtractRandomValue(storageOrder);
        randomSequence.ExtractRandomValue(10, 20, rows);
        randomSequence.ExtractRandomValue(10, 20, cols);
        ADynamic.SetSize(rows, cols, storageOrder);
        vctRandom(ADynamic, -10.0, 10.0);
        UDynamic.SetSize(rows, rows, storageOrder);
        SDynamic.SetSize(std::min(rows, cols));
        VtDynamic.SetSize(cols, cols, storageOrder);
        // Decompose, after this U and Vt can be used
        nmrSVD(ADynamic, UDynamic, SDynamic, VtDynamic);

        // create a fixed size matrix, decompose and copy results to
        // data members
        vctFixedSizeMatrix<double, SIZE, SIZE, VCT_ROW_MAJOR> A;
        vctRandom(A, -10.0, 10.0);
        nmrSVDFixedSizeData<SIZE, SIZE, VCT_ROW_MAJOR> svdData;
        nmrSVD(A, svdData);
        UFixedSize.Assign(svdData.U());
        VtFixedSize.Assign(svdData.Vt());
#endif // CISST_HAS_NETLIB
    }
    
    void tearDown()
    {}

    /* Test if a rotation matrix is orthonormal */
    void TestRotationMatrix(void);

#ifdef CISST_HAS_NETLIB
    /* Test the function without a data object */
    void TestFunctionDynamic(void);

    /* Test the function with a workspace reference */
    void TestFunctionWithReferenceDynamic(void);

    /* Test the function with a data object */
    void TestDynamicData(void);

    /* Test the function with a data object using an existing workspace */
    void TestDynamicDataWithReferenceDynamic(void);

    /* Test the function without a data object */
    void TestFunctionFixedSize(void);

    /* Test the function with a data object */
    void TestFixedSizeData(void);
#endif // CISST_HAS_NETLIB

};


#endif // _nmrIsOrthonormalTest_h

