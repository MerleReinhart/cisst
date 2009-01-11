/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: nmrFminSolver.h,v 1.5 2007/04/26 19:33:57 anton Exp $
  
  Author(s):	Ankur Kapoor
  Created on: 2004-10-26

  (C) Copyright 2004-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


/*!
  \file
  \brief Declaration of nmrFminSolver
*/


#ifndef _nmrFminSolver_h
#define _nmrFminSolver_h


#include <cisstVector/vctDynamicMatrix.h>
#include <cisstNumerical/nmrCallBack.h>

#include <cnetlib.h>
#include <string.h>

/*!
  \ingroup cisstNumerical

*/

class nmrFminSolver {
	// we have this class so that we reserve memory only one
	// would help if svd of a same size matrix (or a matrix)
	// that doesnt change much is desired.
    
protected:
	struct O8USERFUNC UserFn;
	struct O8USERCONFIG UserCfg;
	long int N, NLin, NNonLin, NIter, NStep;
    
public:
    /*! Default constructor.  This constructor doesn't allocate any
      memory.  If you use this constructor, you will need to use one
      of the Allocate() methods before you can use the Solve
      method.  */
    nmrFminSolver(void)
    {
        Allocate(0, 0, 0, 4000, 20);
    }
    
    
    /*! Constructor with memory allocation.  This constructor
      allocates the memory based on N.  It relies on the
      method Allocate().  The next call to the Solve() method will
      check that the parameters match the dimension.
      
      \param n Number of variables
      \param nlin Number of linear constraints
      \param nnonlin Number of non linear constraints
      \param niter Max number of iterations
      \param nstep (typ. 20)
      This order will be used for the output as well.
    */
	nmrFminSolver(long int n, long int nlin, long int nnonlin, long int niter, long int nstep)
    {
        Allocate(n, nlin, nnonlin, niter, nstep);
    }
    
	~nmrFminSolver()
	{
		donlp2_memory_free();
	}
    /*! This method allocates the memory based on N.  The next
      call to the Solve() method will check that the parameters match
      the dimension.
      
      \param n Number of variables
    */
    inline void Allocate(long int n, long int nlin, long int nnonlin, long int niter, long int nstep)
    {
        memset (&UserFn, 0, sizeof(struct O8USERFUNC));
        donlp2_init_size(n, nlin, nnonlin, niter, nstep);
        donlp2_memory_malloc();
        N = n; NLin = nlin; NNonLin = nnonlin; NIter = niter; NStep = nstep;
    }

    inline struct O8USERCONFIG & GetOptions(void) {
	    donlp2_getoptions(&UserCfg);
	    return this->UserCfg;
    }

    inline void SetOptions(struct O8USERCONFIG & userCfg) {
	    memcpy (&UserCfg, &userCfg, sizeof(struct O8USERCONFIG));
	    donlp2_setoptions(&UserCfg);
    }
    
    inline void PrintOptions(void) {
	    donlp2_printoptions(stderr);
    }

    template <int __instanceLine, class __elementType>
    inline void Solve(nmrCallBackFunctionF1<__instanceLine, __elementType> &callBack, vctDynamicVector<double> &X) 
	    throw (std::runtime_error) {
        if (N+1 != (int) X.size()) {
            cmnThrow(std::runtime_error("nmrFminSolver Solve: Size used for Allocate was different"));
        }
        /* set the user functions */
        UserFn.user_ef = callBack.FunctionFdonlp2;
        donlp2_setuserfunctions(&UserFn);
        donlp2_setuserinit(X.Pointer(), NULL, NULL, NULL);
        /* call the DONLP2 C function */
        donlp2();
        double fx;
        donlp2_getresult(X.Pointer(), &fx);
    }
    
    template <int __instanceLine, class __elementType>
    inline void Solve(nmrCallBackFunctionF1<__instanceLine, __elementType> &callBack, vctDynamicVector<double> &X,
                      vctDynamicVector<double> &lbound, vctDynamicVector<double> &ubound,
                      vctDynamicMatrix<double> &alin) 
	    throw (std::runtime_error) {
        if (N+1 != (int) X.size()) {
            cmnThrow(std::runtime_error("nmrFminSolver Solve: Size used for Allocate was different"));
        }
        /* set the user functions */
        UserFn.user_ef = callBack.FunctionFdonlp2;
        donlp2_setuserfunctions(&UserFn);
        vctDynamicVector<double*> alinRowPointers;
        alinRowPointers.SetSize(alin.rows());
        for (unsigned int i = 0; i < alin.rows(); i++) {
            alinRowPointers(i) = alin.Row(i).Pointer();
        }
        donlp2_setuserinit(X.Pointer(), lbound.Pointer(), ubound.Pointer(), alinRowPointers.Pointer());
        /* call the DONLP2 C function */
        donlp2();
        double fx;
        donlp2_getresult(X.Pointer(), &fx);
    }
    
    template <int __instanceLineF, class __elementTypeF, int __instanceLineC, class __elementTypeC>
    inline void Solve(nmrCallBackFunctionF1<__instanceLineF, __elementTypeF> &callBackF, vctDynamicVector<double> &X,
                      nmrCallBackFunctionC<__instanceLineC, __elementTypeC> &callBackC,
                      vctDynamicVector<double> &lbound, vctDynamicVector<double> &ubound)
	    throw (std::runtime_error) {
        if (N+1 != (int) X.size()) {
            cmnThrow(std::runtime_error("nmrFminSolver Solve: Size used for Allocate was different"));
        }
        /* set the user functions */
        UserFn.user_ef = callBackF.FunctionFdonlp2;
        UserFn.user_econ = callBackC.FunctionFdonlp2;
        donlp2_setuserfunctions(&UserFn);
        donlp2_setuserinit(X.Pointer(), lbound.Pointer(), ubound.Pointer(), NULL);
        /* call the DONLP2 C function */
        donlp2();
        double fx;
        donlp2_getresult(X.Pointer(), &fx);
    }
    
    template <int __instanceLineF, class __elementTypeF, int __instanceLineC, class __elementTypeC>
    inline void Solve(nmrCallBackFunctionF1<__instanceLineF, __elementTypeF> &callBackF, vctDynamicVector<double> &X,
                      nmrCallBackFunctionC<__instanceLineC, __elementTypeC> &callBackC,
                      vctDynamicVector<double> &lbound, vctDynamicVector<double> &ubound,
                      vctDynamicMatrix<double> &alin)
        //			    vctDynamicMatrix<double> &alin)
	    throw (std::runtime_error) {
        if (N+1 != (int) X.size()) {
            cmnThrow(std::runtime_error("nmrFminSolver Solve: Size used for Allocate was different"));
        }
        /* set the user functions */
        UserFn.user_ef = callBackF.FunctionFdonlp2;
        UserFn.user_econ = callBackC.FunctionFdonlp2;
        donlp2_setuserfunctions(&UserFn);
        vctDynamicVector<double*> alinRowPointers;
        alinRowPointers.SetSize(alin.rows());
        for (unsigned int i = 0; i < alin.rows(); i++) {
            alinRowPointers(i) = alin.Row(i).Pointer();
        }
        donlp2_setuserinit(X.Pointer(), lbound.Pointer(), ubound.Pointer(), alinRowPointers.Pointer());
        //donlp2_setuserinit(X.Pointer(), lbound.Pointer(), ubound.Pointer(), alina);
        /* call the DONLP2 C function */
        donlp2();
        double fx;
        donlp2_getresult(X.Pointer(), &fx);
    }
    
    inline void Solve(void (*efFunctionPointer)(int,double[],double*),
                      vctDynamicVector<double> &X, 
                      void (*econFunctionPointer)(int,int,int,int[],double[],double[],int[]),
                      vctDynamicVector<double> &lbound,
                      vctDynamicVector<double> &ubound,
                      vctDynamicMatrix<double> &alin) throw (std::runtime_error) {
        if (N+1 != (int) X.size()) {
            cmnThrow(std::runtime_error("nmrFminSolver Solve: Size used for Allocate was different"));
        }
        UserFn.user_ef = efFunctionPointer;
        UserFn.user_econ = econFunctionPointer;
        donlp2_setuserfunctions(&UserFn);
        vctDynamicVector<double*> alinRowPointers;
        alinRowPointers.SetSize(alin.rows());
        for (unsigned int i = 0; i < alin.rows(); i++) {
            alinRowPointers(i) = alin.Row(i).Pointer();
        }
        donlp2_setuserinit(X.Pointer(), lbound.Pointer(), ubound.Pointer(), alinRowPointers.Pointer());
        //donlp2_setuserinit(X.Pointer(), lbound.Pointer(), ubound.Pointer(), NULL);
        /* call the DONLP2 C function */
        donlp2();
        double fx;
        donlp2_getresult(X.Pointer(), &fx);
    }
    
#if 0
    template <int __instanceLineF, class __elementTypeF, int __instanceLineG, class __elementTypeG,
              int __instanceLineC, class __elementTypeC, int __instanceLineCG, class __elementTypeCG>
    inline void Solve(nmrCallBackFunctionF1<__instanceLineF, __elementTypeF> &callBackF, vctDynamicVector<double> &X,
                      nmrCallBackFunctionF<__instanceLineG, __elementTypeG> &callBackG,
                      nmrCallBackFunctionC<__instanceLineC, __elementTypeC> &callBackC,
                      nmrCallBackFunctionCG<__instanceLineCG, __elementTypeCG> &callBackCG,
                      vctDynamicVector<double> &Lb,
                      vctDynamicVector<double> &Ub,
                      vctDynamicVector<double> &ALin) 
        throw (std::runtime_error) {
        if (N != (int) X.size()) {
            cmnThrow(std::runtime_error("nmrFminSolver Solve: Size used for Allocate was different"));
        }
        /* set the user functions */
        UserFn.user_ef = callBackF.FunctionFdonlp2;
        UserFn.user_egradf = callBackG.FunctionFdonlp2;
        UserFn.user_econ = callBackC.FunctionFdonlp2;
        UserFn.user_econgrad = callBackCG.FunctionFdonlp2;
        donlp2_setuserfunctions(&UserFn);
        donlp2_setuserinit(X.Pointer, Lb.Pointer(), Ub.Pointer(), ALin.Pointer());
        donlp2();
        /* call the DONLP2 C function */
    }
#endif
};

#endif // _nmrFminSolver_h

