/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooMyFitResult.cxx 24307 2008-06-17 13:21:58Z wouter $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
// RooMyFitResult is a container class to hold the input and output
// of a PDF fit to a dataset. It contains:
//
//   - Values of all constant parameters
//   - Initial and final values of floating parameters with error
//   - Correlation matrix and global correlation coefficients
//   - NLL and EDM at mininum
//
// No references to the fitted PDF and dataset are stored
//

#include "RooFit.h"
#include "Riostream.h"

#include <iomanip>
#include "TMinuit.h"
#include "TMath.h"
#include "TMarker.h"
#include "TLine.h"
#include "TBox.h"
#include "TGaxis.h"
#include "TMatrix.h"
#include "TVector.h"
#include "TDirectory.h"
#include "TClass.h"
#include "RooMyFitResult.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooEllipse.h"
#include "RooRandom.h"
#include "RooMsgService.h"



ClassImp(RooMyFitResult) 
;


//_____________________________________________________________________________
RooMyFitResult::RooMyFitResult(const char* name, const char* title) : 
  TNamed(name,title), _constPars(0), _initPars(0), _finalPars(0), _globalCorr(0), _randomPars(0), _Lt(0)
{  
  // Constructor with name and title

  //if (name) appendToDir(this,kTRUE) ;
}


//_____________________________________________________________________________
RooMyFitResult::RooMyFitResult(const RooMyFitResult& other) : 
  TNamed(other),
  RooPrintable(other),
  RooDirItem(other),
  _status(other._status),
  _covQual(other._covQual),
  _numBadNLL(other._numBadNLL),
  _minNLL(other._minNLL),
  _edm(other._edm),
  _randomPars(0),
  _Lt(0)
{
  // Copy constructor

  _constPars = (RooArgList*) other._constPars->snapshot() ;
  _initPars = (RooArgList*) other._initPars->snapshot() ;
  _finalPars = (RooArgList*) other._finalPars->snapshot() ;
  _globalCorr = (RooArgList*) other._globalCorr->snapshot() ;
  if (other._randomPars) _randomPars = (RooArgList*) other._randomPars->snapshot() ;
  if (other._Lt) _Lt = new TMatrix(*other._Lt);
  TIterator* iter = other._corrMatrix.MakeIterator() ;
  RooArgList* corrMatrixRow(0);
  while ((corrMatrixRow=(RooArgList*)iter->Next())) 
    _corrMatrix.Add((RooArgList*)corrMatrixRow->snapshot() );
}



//_____________________________________________________________________________
RooMyFitResult::~RooMyFitResult() 
{
  // Destructor

  if (_constPars) delete _constPars ;
  if (_initPars)  delete _initPars ;
  if (_finalPars) delete _finalPars ;
  if (_globalCorr) delete _globalCorr;
  if (_randomPars) delete _randomPars;
  if (_Lt) delete _Lt;

  _corrMatrix.Delete();

  removeFromDir(this) ;
}


//_____________________________________________________________________________
void RooMyFitResult::setConstParList(const RooArgList& list) 
{
  // Fill the list of constant parameters

  if (_constPars) delete _constPars ;
  _constPars = (RooArgList*) list.snapshot() ;
  TIterator* iter = _constPars->createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    RooRealVar* rrv = dynamic_cast<RooRealVar*>(arg) ;
    if (rrv) {
      rrv->deleteSharedProperties() ;
    }
  }
  delete iter ;
}



//_____________________________________________________________________________
void RooMyFitResult::setInitParList(const RooArgList& list)
{
  // Fill the list of initial values of the floating parameters 

  if (_initPars) delete _initPars ;
  _initPars = (RooArgList*) list.snapshot() ;
  TIterator* iter = _initPars->createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    RooRealVar* rrv = dynamic_cast<RooRealVar*>(arg) ;
    if (rrv) {
      rrv->deleteSharedProperties() ;
    }
  }
  delete iter ;
}


//___________________________________________________________________________

void RooMyFitResult::setGlobalCorr(const RooArgList& list)
{

  if (_globalCorr) delete _globalCorr ;
  _globalCorr = (RooArgList*) list.snapshot() ;
  TIterator* iter = _globalCorr->createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    RooRealVar* rrv = dynamic_cast<RooRealVar*>(arg) ;
    if (rrv) {
      rrv->deleteSharedProperties() ;
    }
  }
  delete iter ;
}
//
//
void RooMyFitResult::setCorrMatrix(const TList& list)
{
  _corrMatrix.Delete();
  for(int it=0;it<list.GetSize();it++)
  {
  RooArgList * corrMatrixRow = (RooArgList*)list.At(it);
   _corrMatrix.Add(corrMatrixRow);
  } 
}

//_____________________________________________________________________________
void RooMyFitResult::setFinalParList(const RooArgList& list)
{
  // Fill the list of final values of the floating parameters 

  if (_finalPars) delete _finalPars ;
  _finalPars = (RooArgList*) list.snapshot() ;

  TIterator* iter = _finalPars->createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    RooRealVar* rrv = dynamic_cast<RooRealVar*>(arg) ;
    if (rrv) {
      rrv->deleteSharedProperties() ;
    }
  }
  delete iter ;
}


//_____________________________________________________________________________
RooPlot *RooMyFitResult::plotOn(RooPlot *frame, const char *parName1, const char *parName2,
			      const char *options) const 
{
  // Add objects to a 2D plot that represent the fit results for the
  // two named parameters.  The input frame with the objects added is
  // returned, or zero in case of an error.  Which objects are added
  // are determined by the options string which should be a concatenation
  // of the following (not case sensitive):
  //
  //   M - a marker at the best fit result
  //   E - an error ellipse calculated at 1-sigma using the error matrix at the minimum
  //   1 - the 1-sigma error bar for parameter 1
  //   2 - the 1-sigma error bar for parameter 2
  //   B - the bounding box for the error ellipse
  //   H - a line and horizontal axis for reading off the correlation coefficient
  //   V - a line and vertical axis for reading off the correlation coefficient
  //   A - draw axes for reading off the correlation coefficients with the H or V options
  //
  // You can change the attributes of objects in the returned RooPlot using the
  // various RooPlot::getAttXxx(name) member functions, e.g.
  //
  //   plot->getAttLine("contour")->SetLineStyle(kDashed);
  //
  // Use plot->Print() for a list of all objects and their names (unfortunately most
  // of the ROOT builtin graphics objects like TLine are unnamed). Drag the left mouse
  // button along the labels of either axis button to interactively zoom in a plot.

  // lookup the input parameters by name: we require that they were floated in our fit
  const RooRealVar *par1= dynamic_cast<const RooRealVar*>(floatParsFinal().find(parName1));
  if(0 == par1) {
    coutE(InputArguments) << "RooMyFitResult::correlationPlot: parameter not floated in fit: " << parName1 << endl;
    return 0;
  }
  const RooRealVar *par2= dynamic_cast<const RooRealVar*>(floatParsFinal().find(parName2));
  if(0 == par2) {
    coutE(InputArguments) << "RooMyFitResult::correlationPlot: parameter not floated in fit: " << parName2 << endl;
    return 0;
  }

  // options are not case sensitive
  TString opt(options);
  opt.ToUpper();

  // lookup the 2x2 covariance matrix elements for these variables
  Double_t x1= par1->getVal();
  Double_t x2= par2->getVal();
  Double_t s1= par1->getError();
  Double_t s2= par2->getError();
  Double_t rho= correlation(parName1, parName2);

  // add a 1-sigma error ellipse, if requested
  if(opt.Contains("E")) {
    RooEllipse *contour= new RooEllipse("contour",x1,x2,s1,s2,rho);
    frame->addPlotable(contour);
  }

  // add the error bar for parameter 1, if requested
  if(opt.Contains("1")) {
    TLine *hline= new TLine(x1-s1,x2,x1+s1,x2);
    hline->SetLineColor(kRed);
    frame->addObject(hline);
  }

  if(opt.Contains("2")) {
    TLine *vline= new TLine(x1,x2-s2,x1,x2+s2);
    vline->SetLineColor(kRed);
    frame->addObject(vline);
  }

  if(opt.Contains("B")) {
    TBox *box= new TBox(x1-s1,x2-s2,x1+s1,x2+s2);
    box->SetLineStyle(kDashed);
    box->SetLineColor(kRed);
    box->SetFillStyle(0);
    frame->addObject(box);
  }

  if(opt.Contains("H")) {
    TLine *line= new TLine(x1-rho*s1,x2-s2,x1+rho*s1,x2+s2);
    line->SetLineStyle(kDashed);
    line->SetLineColor(kBlue);
    frame->addObject(line);
    if(opt.Contains("A")) {
      TGaxis *axis= new TGaxis(x1-s1,x2-s2,x1+s1,x2-s2,-1.,+1.,502,"-=");
      axis->SetLineColor(kBlue);
      frame->addObject(axis);
    }
  }

  if(opt.Contains("V")) {
    TLine *line= new TLine(x1-s1,x2-rho*s2,x1+s1,x2+rho*s2);
    line->SetLineStyle(kDashed);
    line->SetLineColor(kBlue);
    frame->addObject(line);
    if(opt.Contains("A")) {
      TGaxis *axis= new TGaxis(x1-s1,x2-s2,x1-s1,x2+s2,-1.,+1.,502,"-=");
      axis->SetLineColor(kBlue);
      frame->addObject(axis);
    }
  }

  // add a marker at the fitted value, if requested
  if(opt.Contains("M")) {
    TMarker *marker= new TMarker(x1,x2,20);
    marker->SetMarkerColor(kBlack);
    frame->addObject(marker);
  }

  return frame;
}


//_____________________________________________________________________________
const RooArgList& RooMyFitResult::randomizePars() const 
{
  // Return a list of floating parameter values that are perturbed from the final
  // fit values by random amounts sampled from the covariance matrix. The returned
  // object is overwritten with each call and belongs to the RooMyFitResult. Uses
  // the "square root method" to decompose the covariance matrix, which makes inverting
  // it unnecessary.
  
  Int_t nPar= _finalPars->getSize();
//  cout<<"nPar =  "<<nPar<<endl;
  if(0 == _randomPars) { // first-time initialization
    assert(0 != _finalPars);
    // create the list of random values to fill
    _randomPars= (RooArgList*)_finalPars->snapshot();
    // calculate the elements of the upper-triangular matrix L that gives Lt*L = C
    // where Lt is the transpose of L (the "square-root method")
    TMatrix L(nPar,nPar);
    for(Int_t iPar= 0; iPar < nPar; iPar++) {
      // calculate the diagonal term first
      L(iPar,iPar)= covariance(iPar,iPar);
      for(Int_t k= 0; k < iPar; k++) {
	Double_t tmp= L(k,iPar);
	L(iPar,iPar)-= tmp*tmp;
      }
      //cout<<"L(iPar,iPar) = "<<L(iPar,iPar)<<endl;
      L(iPar,iPar)= sqrt(L(iPar,iPar));

      //cout<<"L(iPar,iPar) = "<<L(iPar,iPar)<<endl;
      // then the off-diagonal terms
      for(Int_t jPar= iPar+1; jPar < nPar; jPar++) {
	L(iPar,jPar)= covariance(iPar,jPar);
	for(Int_t k= 0; k < iPar; k++) {
	  L(iPar,jPar)-= L(k,iPar)*L(k,jPar);
	}
	L(iPar,jPar)/= L(iPar,iPar);
      }
    }
    // remember Lt
    _Lt= new TMatrix(TMatrix::kTransposed,L);
  //  _Lt.Print();
  }
  else {
    // reset to the final fit values
    *_randomPars= *_finalPars;
  }

  // create a vector of unit Gaussian variables
  TVector g(nPar);
  for(Int_t k= 0; k < nPar; k++) { g(k)= RooRandom::gaussian(); }//cout<<"g(k) = "<<g(k)<<endl;}
  // multiply this vector by Lt to introduce the appropriate correlations
  g*= (*_Lt);
  // add the mean value offsets and store the results
  TIterator *iter= _randomPars->createIterator();
  RooRealVar *par(0);
  Int_t index(0);
  while((0 != (par= (RooRealVar*)iter->Next()))) {
//    cout<<"par = " <<g(index)<<endl;
    par->setVal(par->getVal() + g(index++));
  }
  delete iter;

  return *_randomPars;
}


//_____________________________________________________________________________
Double_t RooMyFitResult::correlation(const char* parname1, const char* parname2) const 
{
  // Return the correlation between parameters 'par1' and 'par2'

  const RooArgList* row = correlation(parname1) ;
  if (!row) return 0. ;
  RooAbsArg* arg = _initPars->find(parname2) ;
  if (!arg) {
    //coutE(InputArguments) << "RooMyFitResult::correlation: variable " << parname2 << " not a floating parameter in fit" << endl ;
    cout << "RooMyFitResult::correlation: variable " << parname2 << " not a floating parameter in fit" << endl ;
    return 0. ;
  }
  return ((RooRealVar*)row->at(_initPars->index(arg)))->getVal() ;
}



//_____________________________________________________________________________
const RooArgList* RooMyFitResult::correlation(const char* parname) const 
{
  // Return the set of correlation coefficients of parameter 'par' with
  // all other floating parameters

  RooAbsArg* arg = _initPars->find(parname) ;
  if (!arg) {
    coutE(InputArguments) << "RooMyFitResult::correlation: variable " << parname << " not a floating parameter in fit" << endl ;
    return 0 ;
  }    
  return (RooArgList*)_corrMatrix.At(_initPars->index(arg)) ;
}



//_____________________________________________________________________________
Double_t RooMyFitResult::globalCorr(const char* parname) 
{
  // Return the global correlation of the named parameter

  RooAbsArg* arg = _initPars->find(parname) ;
  //_initPars->printValue(cout);
  if (!arg) {
   // coutE(InputArguments) << "RooMyFitResult::globalCorr: variable " << parname << " not a floating parameter in fit" << endl ;
    return 0 ;
  }    

  if (_globalCorr) {
    return ((RooAbsReal*)_globalCorr->at(_initPars->index(arg)))->getVal() ;
  } else {
    return 1.0 ; 
  }
}



//_____________________________________________________________________________
const RooArgList* RooMyFitResult::globalCorr() 
{
  // Return the list of all global correlations

  return _globalCorr ;
}



//_____________________________________________________________________________
Double_t RooMyFitResult::correlation(Int_t row, Int_t col) const 
{
  // Return a correlation matrix element addressed with numeric indices.
  
  const RooArgList *rowVec= (const RooArgList*)_corrMatrix.At(row);
  assert(0 != rowVec);
  const RooRealVar *elem= (const RooRealVar*)rowVec->at(col);
  assert(0 != elem);
  return elem->getVal();
}


//_____________________________________________________________________________
Double_t RooMyFitResult::covariance(Int_t row, Int_t col) const 
{
  // Return the covariance matrix element addressed with numeric indices.

  const RooRealVar *rowVar= (const RooRealVar*)_finalPars->at(row);
  const RooRealVar *colVar= (const RooRealVar*)_finalPars->at(col);
  assert(0 != rowVar && 0 != colVar);
  return rowVar->getError()*colVar->getError()*correlation(row,col);  
}



//_____________________________________________________________________________
void RooMyFitResult::printMultiline(ostream& os, Int_t /*contents*/, Bool_t verbose, TString indent) const
{
  // Print fit result to stream 'os'. In Verbose mode, the contant parameters and
  // the initial and final values of the floating parameters are printed. 
  // Standard mode only the final values of the floating parameters are printed

  os << endl 
     << indent << "  RooMyFitResult: minimized FCN value: " << _minNLL << ", estimated distance to minimum: " << _edm << endl
     << indent << "                coviarance matrix quality: " ;
  switch(_covQual) {
  case 0: os << "Not calculated at all" ; break ;
  case 1: os << "Approximation only, not accurate" ; break ;
  case 2: os << "Full matrix, but forced positive-definite" ; break ;
  case 3: os << "Full, accurate covariance matrix" ; break ;
  }
  os << endl 
     << endl ;

  Int_t i ;
  if (verbose) {
    if (_constPars->getSize()>0) {
      os << indent << "    Constant Parameter    Value     " << endl
	 << indent << "  --------------------  ------------" << endl ;

      for (i=0 ; i<_constPars->getSize() ; i++) {
	os << indent << "  " << setw(20) << ((RooAbsArg*)_constPars->at(i))->GetName()
	   << "  " << setw(12) << Form("%12.4e",((RooRealVar*)_constPars->at(i))->getVal())
	   << endl ;
      }

      os << endl ;
    }

    // Has any parameter asymmetric errors?
    Bool_t doAsymErr(kFALSE) ;
    for (i=0 ; i<_finalPars->getSize() ; i++) {
      if (((RooRealVar*)_finalPars->at(i))->hasAsymError()) {
	doAsymErr=kTRUE ;
	break ;
      }
    }

    if (doAsymErr) {
      os << indent << "    Floating Parameter  InitialValue    FinalValue (+HiError,-LoError)    GblCorr." << endl
	 << indent << "  --------------------  ------------  ----------------------------------  --------" << endl ;
    } else {
      os << indent << "    Floating Parameter  InitialValue    FinalValue +/-  Error     GblCorr." << endl
	 << indent << "  --------------------  ------------  --------------------------  --------" << endl ;
    }

    for (i=0 ; i<_finalPars->getSize() ; i++) {
      os << indent << "  "    << setw(20) << ((RooAbsArg*)_finalPars->at(i))->GetName() ;
      os << indent << "  "    << setw(12) << Form("%12.4e",((RooRealVar*)_initPars->at(i))->getVal())
	 << indent << "  "    << setw(12) << Form("%12.4e",((RooRealVar*)_finalPars->at(i))->getVal()) ;
      
      if (((RooRealVar*)_finalPars->at(i))->hasAsymError()) {
	os << setw(21) << Form(" (+%8.2e,-%8.2e)",((RooRealVar*)_finalPars->at(i))->getAsymErrorHi(),
	                       -1*((RooRealVar*)_finalPars->at(i))->getAsymErrorLo()) ;
      } else {
	Double_t err = ((RooRealVar*)_finalPars->at(i))->getError() ;
	os << (doAsymErr?"        ":"") << " +/- " << setw(9)  << Form("%9.2e",err) ;
      }

      if (_globalCorr) {
	os << "  "    << setw(8)  << Form("%8.6f" ,((RooRealVar*)_globalCorr->at(i))->getVal()) ;
      } else {
	os << "  <none>" ;
      } 

      os << endl ;
    }

  } else {
    os << indent << "    Floating Parameter    FinalValue +/-  Error   " << endl
       << indent << "  --------------------  --------------------------" << endl ;

    for (i=0 ; i<_finalPars->getSize() ; i++) {
      Double_t err = ((RooRealVar*)_finalPars->at(i))->getError() ;
      os << indent << "  "    << setw(20) << ((RooAbsArg*)_finalPars->at(i))->GetName()
	 << "  "    << setw(12) << Form("%12.4e",((RooRealVar*)_finalPars->at(i))->getVal())
	 << " +/- " << setw(9)  << Form("%9.2e",err)
	 << endl ;
    }
  }
  

  os << endl ;
}



//_____________________________________________________________________________
void RooMyFitResult::fillCorrMatrix()
{
  // Internal utility method to extract the correlation matrix and the
  // global correlation coefficients from the MINUIT memory buffer and
  // fill the internal arrays.

  // Sanity check
  if (gMinuit->fNpar <= 1) {
    coutI(Minimization) << "RooMyFitResult::fillCorrMatrix: number of floating parameters <=1, correlation matrix not filled" << endl ;
    return ;
  }

  if (!_initPars) {
    coutE(Minimization) << "RooMyFitResult::fillCorrMatrix: ERROR: list of initial parameters must be filled first" << endl ;
    return ;
  }

  // Delete eventual prevous correlation data holders
  if (_globalCorr) delete _globalCorr ;

  _corrMatrix.Delete();

  // Build holding arrays for correlation coefficients
  _globalCorr = new RooArgList("globalCorrelations") ;
  TIterator* vIter = _initPars->createIterator() ;
  RooAbsArg* arg ;
  Int_t idx(0) ;
  while((arg=(RooAbsArg*)vIter->Next())) {
    // Create global correlation value holder
    TString gcName("GC[") ;
    gcName.Append(arg->GetName()) ;
    gcName.Append("]") ;
    TString gcTitle(arg->GetTitle()) ;
    gcTitle.Append(" Global Correlation") ;
    _globalCorr->addOwned(*(new RooRealVar(gcName.Data(),gcTitle.Data(),0.))) ;

    // Create array with correlation holders for this parameter
    TString name("C[") ;
    name.Append(arg->GetName()) ;
    name.Append(",*]") ;
    RooArgList* corrMatrixRow = new RooArgList(name.Data()) ;
    _corrMatrix.Add(corrMatrixRow) ;
    TIterator* vIter2 = _initPars->createIterator() ;
    RooAbsArg* arg2 ;
    while((arg2=(RooAbsArg*)vIter2->Next())) {

      TString cName("C[") ;
      cName.Append(arg->GetName()) ;
      cName.Append(",") ;
      cName.Append(arg2->GetName()) ;
      cName.Append("]") ;
      TString cTitle("Correlation between ") ;
      cTitle.Append(arg->GetName()) ;
      cTitle.Append(" and ") ;
      cTitle.Append(arg2->GetName()) ;
      corrMatrixRow->addOwned(*(new RooRealVar(cName.Data(),cTitle.Data(),0.))) ;      
    }
    delete vIter2 ;
    idx++ ;
  }
  delete vIter ;

  TIterator *gcIter = _globalCorr->createIterator() ;
  TIterator *parIter = _finalPars->createIterator() ;

  // Extract correlation information for MINUIT (code taken from TMinuit::mnmatu() )

  // WVE: This code directly manipulates minuit internal workspace, 
  //      if TMinuit code changes this may need updating
  Int_t ndex, i, j, m, n, ncoef, nparm, /*id,*/ it, ix ;
  Int_t ndi, ndj /*, iso, isw2, isw5*/;
  ncoef = (gMinuit->fNpagwd - 19) / 6;
  nparm = TMath::Min(gMinuit->fNpar,ncoef);
  RooRealVar* gcVal = 0;
  for (i = 1; i <= gMinuit->fNpar; ++i) {
    ix  = gMinuit->fNexofi[i-1];
    ndi = i*(i + 1) / 2;
    for (j = 1; j <= gMinuit->fNpar; ++j) {
      m    = TMath::Max(i,j);
      n    = TMath::Min(i,j);
      ndex = m*(m-1) / 2 + n;
      ndj  = j*(j + 1) / 2;
      gMinuit->fMATUvline[j-1] = gMinuit->fVhmat[ndex-1] / TMath::Sqrt(TMath::Abs(gMinuit->fVhmat[ndi-1]*gMinuit->fVhmat[ndj-1]));
    }
    nparm = TMath::Min(gMinuit->fNpar,ncoef);

    // Find the next global correlation slot to fill, skipping fixed parameters
    gcVal = (RooRealVar*) gcIter->Next() ;
    gcVal->setVal(gMinuit->fGlobcc[i-1]) ;

    // Fill a row of the correlation matrix
    TIterator* cIter = ((RooArgList*)_corrMatrix.At(i-1))->createIterator() ;
    for (it = 1; it <= gMinuit->fNpar ; ++it) {
      RooRealVar* cVal = (RooRealVar*) cIter->Next() ;
      cVal->setVal(gMinuit->fMATUvline[it-1]) ;
    }
    delete cIter ;
  }

  delete gcIter ;
  delete parIter ;
} 


//*************************Edited by Ai X.C.**************************
void RooMyFitResult::MyfillCorrMatrix(int Npar,std::vector<double> GlobalCorre,vector<double> ErrorMatrix)
{

   RooArgList initPars = this->floatParsInit(); 
 if (!initPars) {
     cout << "RooFitResult::fillCorrMatrix: ERROR: list of initial parameters must be filled first" << endl ;
     return ;
   }
 
   // Delete eventual prevous correlation data holders
   //if (_globalCorr) delete _globalCorr ;
 
   //_corrMatrix.Delete();

   cout<<__LINE__<<endl; 
   // Build holding arrays for correlation coefficients
   RooArgList * my_globalCorr = new RooArgList("globalCorrelations") ;
   TList my_corrMatrix;

   TIterator* vIter = initPars.createIterator() ;
   RooAbsArg* arg ;
   Int_t idx(0) ;
   while((arg=(RooAbsArg*)vIter->Next())) {
     // Create global correlation value holder
     TString gcName("GC[") ;
     gcName.Append(arg->GetName()) ;
     gcName.Append("]") ;
     TString gcTitle(arg->GetTitle()) ;
     gcTitle.Append(" Global Correlation") ;
   cout<<__LINE__<<endl;
   my_globalCorr->addOwned(*(new RooRealVar(gcName.Data(),gcTitle.Data(),0.))) ;
   cout<<__LINE__<<endl; 
 
     // Create array with correlation holders for this parameter
     TString name("C[") ;
     name.Append(arg->GetName()) ;
     name.Append(",*]") ;
     RooArgList* corrMatrixRow = new RooArgList(name.Data()) ;
   cout<<__LINE__<<endl; 
     my_corrMatrix.Add(corrMatrixRow) ;
     TIterator* vIter2 = initPars.createIterator() ;
     RooAbsArg* arg2 ;
     while((arg2=(RooAbsArg*)vIter2->Next())) {
 
       TString cName("C[") ;
       cName.Append(arg->GetName()) ;
       cName.Append(",") ;
       cName.Append(arg2->GetName()) ;
       cName.Append("]") ;
       TString cTitle("Correlation between ") ;
       cTitle.Append(arg->GetName()) ;
       cTitle.Append(" and ") ;
       cTitle.Append(arg2->GetName()) ;
       corrMatrixRow->addOwned(*(new RooRealVar(cName.Data(),cTitle.Data(),0.))) ;      
     }
   cout<<__LINE__<<endl; 
     delete vIter2 ;
     idx++ ;
   }
   delete vIter ;
 
   cout<<__LINE__<<endl; 
   TIterator *gcIter = my_globalCorr->createIterator() ;
  
   Int_t nparm = Npar;

   Int_t ndi, ndj,ndex;
   std::vector<double> value_line;

  RooRealVar* gcVal = 0;
  for (int i = 1; i <= nparm; ++i) {
     ndi = i*(i + 1) / 2;
     for (int j = 1; j <= nparm; ++j) {
       int m  = TMath::Max(i,j);
       int n   = TMath::Min(i,j);
       ndex = m*(m-1) / 2 + n;
       ndj  = j*(j + 1) / 2;
       value_line.push_back( ErrorMatrix[ndex-1] / TMath::Sqrt(TMath::Abs(ErrorMatrix[ndi-1]*ErrorMatrix[ndj-1])));
     }
 
     // Find the next global correlation slot to fill, skipping fixed parameters
     gcVal = (RooRealVar*) gcIter->Next() ;
     gcVal->setVal(GlobalCorre[i-1]) ;
 
     // Fill a row of the correlation matrix
     TIterator* cIter = ((RooArgList*)my_corrMatrix.At(i-1))->createIterator() ;
     for (int it = 1; it <= nparm ; ++it) {
       RooRealVar* cVal = (RooRealVar*) cIter->Next() ;
       cVal->setVal(value_line[it-1]) ;
     }
     delete cIter ;
   }
 
   delete gcIter ;

}



//_____________________________________________________________________________
Bool_t RooMyFitResult::isIdentical(const RooMyFitResult& other, Double_t tol, Double_t tolCorr, Bool_t /*verbose*/) const 
{
  // Return true if this fit result is identical to other within tolerance 'tol' on fitted values
  // and tolerance 'tolCor' on correlation coefficients

  Bool_t ret = kTRUE ;

  if (fabs(_minNLL-other._minNLL)>=tol) {
    cout << "RooMyFitResult::isIdentical: minimized value of -log(L) is different " << _minNLL << " vs. " << other._minNLL << endl ;
    ret = kFALSE ;
  }

  for (Int_t i=0 ; i<_constPars->getSize() ; i++) {
    RooAbsReal* ov = static_cast<RooAbsReal*>(other._constPars->find(_constPars->at(i)->GetName())) ;
    if (!ov) {
      cout << "RooMyFitResult::isIdentical: cannot find constant parameter " << _constPars->at(i)->GetName() << " in reference" << endl ;
      ret = kFALSE ;
    }
    if (fabs(static_cast<RooAbsReal*>(_constPars->at(i))->getVal()-ov->getVal())>=tol) {
      cout << "RooMyFitResult::isIdentical: constant parameter " << _constPars->at(i)->GetName() 
	   << " differs in value: " << static_cast<RooAbsReal*>(_constPars->at(i))->getVal() << " vs. " << ov->getVal() << endl ;
      ret = kFALSE ;
    }
  }

  for (Int_t i=0 ; i<_initPars->getSize() ; i++) {
    RooAbsReal* ov = static_cast<RooAbsReal*>(other._initPars->find(_initPars->at(i)->GetName())) ;
    if (!ov) {
      cout << "RooMyFitResult::isIdentical: cannot find initial parameter " << _initPars->at(i)->GetName() << " in reference" << endl ;
      ret = kFALSE ;
    }
    if (fabs(static_cast<RooAbsReal*>(_initPars->at(i))->getVal()-ov->getVal())>=tol) {
      cout << "RooMyFitResult::isIdentical: initial parameter " << _initPars->at(i)->GetName() 
	   << " differs in value: " << static_cast<RooAbsReal*>(_initPars->at(i))->getVal() << " vs. " << ov->getVal() << endl ;
      ret = kFALSE ;
    }
  }

  for (Int_t i=0 ; i<_finalPars->getSize() ; i++) {
    RooAbsReal* ov = static_cast<RooAbsReal*>(other._finalPars->find(_finalPars->at(i)->GetName())) ;
    if (!ov) {
      cout << "RooMyFitResult::isIdentical: cannot find final parameter " << _finalPars->at(i)->GetName() << " in reference" << endl ;
      ret = kFALSE ;
    }
    if (fabs(static_cast<RooAbsReal*>(_finalPars->at(i))->getVal()-ov->getVal())>=tol) {
      cout << "RooMyFitResult::isIdentical: final parameter " << _finalPars->at(i)->GetName() 
	   << " differs in value: " << static_cast<RooAbsReal*>(_finalPars->at(i))->getVal() << " vs. " << ov->getVal() << endl ;
      ret = kFALSE ;
    }
  }

  // Only examine correlations for cases with >1 floating paramater
  if (_finalPars->getSize()>1) {
    
    for (Int_t i=0 ; i<_globalCorr->getSize() ; i++) {
      RooAbsReal* ov = static_cast<RooAbsReal*>(other._globalCorr->find(_globalCorr->at(i)->GetName())) ;
      if (!ov) {
	cout << "RooMyFitResult::isIdentical: cannot find global correlation coefficient " << _globalCorr->at(i)->GetName() << " in reference" << endl ;
	ret = kFALSE ;
      }
      if (fabs(static_cast<RooAbsReal*>(_globalCorr->at(i))->getVal()-ov->getVal())>=tolCorr) {
	cout << "RooMyFitResult::isIdentical: global correlation coefficient " << _globalCorr->at(i)->GetName() 
	     << " differs in value: " << static_cast<RooAbsReal*>(_globalCorr->at(i))->getVal() << " vs. " << ov->getVal() << endl ;
	ret = kFALSE ;
      }
    }
    
    for (Int_t j=0 ; j<_corrMatrix.GetSize() ; j++) {
      RooArgList* row = (RooArgList*) _corrMatrix.At(j) ;
      RooArgList* orow = (RooArgList*) other._corrMatrix.At(j) ;
      for (Int_t i=0 ; i<row->getSize() ; i++) {
	RooAbsReal* ov = static_cast<RooAbsReal*>(orow->find(row->at(i)->GetName())) ;
	if (!ov) {
	  cout << "RooMyFitResult::isIdentical: cannot find correlation coefficient " << row->at(i)->GetName() << " in reference" << endl ;
	  ret = kFALSE ;
	}
	if (fabs(static_cast<RooAbsReal*>(row->at(i))->getVal()-ov->getVal())>=tolCorr) {
	  cout << "RooMyFitResult::isIdentical: correlation coefficient " << row->at(i)->GetName() 
	       << " differs in value: " << static_cast<RooAbsReal*>(row->at(i))->getVal() << " vs. " << ov->getVal() << endl ;
	  ret = kFALSE ;
	}
      }
    }
  }    

  return ret ;
}



//_____________________________________________________________________________
RooMyFitResult* RooMyFitResult::lastMinuitFit(const RooArgList& varList) 
{
  // Import the results of the last fit performed by gMinuit, interpreting
  // the fit parameters as the given varList of parameters.

  // Verify length of supplied varList
  if (varList.getSize()>0 && varList.getSize()!=gMinuit->fNu) {
    oocoutE((TObject*)0,InputArguments) << "RooMyFitResult::lastMinuitFit: ERROR: supplied variable list must be either empty " << endl 
					<< "                             or match the number of variables of the last fit (" << gMinuit->fNu << ")" << endl ;
    return 0 ;
  }

  // Verify that all members of varList are of type RooRealVar
  TIterator* iter = varList.createIterator() ;
  RooAbsArg* arg  ;
  while((arg=(RooAbsArg*)iter->Next())) {
    if (!dynamic_cast<RooRealVar*>(arg)) {
      oocoutE((TObject*)0,InputArguments) << "RooMyFitResult::lastMinuitFit: ERROR: variable '" << arg->GetName() << "' is not of type RooRealVar" << endl ;
      return 0 ;
    }
  }
  delete iter ;

  RooMyFitResult* r = new RooMyFitResult("lastMinuitFit","Last MINUIT fit") ;

  // Extract names of fit parameters from MINUIT 
  // and construct corresponding RooRealVars
  RooArgList constPars("constPars") ;
  RooArgList floatPars("floatPars") ;

  Int_t i ;
  for (i = 1; i <= gMinuit->fNu; ++i) {
    if (gMinuit->fNvarl[i-1] < 0) continue;
    Int_t l = gMinuit->fNiofex[i-1];
    TString varName(gMinuit->fCpnam[i-1]) ;
    Bool_t isConst(l==0) ;
    
    Double_t xlo = gMinuit->fAlim[i-1];
    Double_t xhi = gMinuit->fBlim[i-1];
    Double_t xerr = gMinuit->fWerr[l-1];
    Double_t xval = gMinuit->fU[i-1] ;

    RooRealVar* var ;
    if (varList.getSize()==0) {

      if ((xlo<xhi) && !isConst) {
	var = new RooRealVar(varName,varName,xval,xlo,xhi) ;
      } else {
	var = new RooRealVar(varName,varName,xval) ;
      }
      var->setConstant(isConst) ;
    } else {

      var = (RooRealVar*) varList.at(i-1)->Clone() ;
      var->setConstant(isConst) ;
      var->setVal(xval) ;
      if (xlo<xhi) {
	var->setRange(xlo,xhi) ;
      }
      if (varName.CompareTo(var->GetName())) {
	oocoutI((TObject*)0,Eval) << "RooMyFitResult::lastMinuitFit: fit parameter '" << varName 
				  << "' stored in variable '" << var->GetName() << "'" << endl ;
      }

    }

    if (isConst) {
      constPars.addOwned(*var) ;
    } else {
      var->setError(xerr) ;
      floatPars.addOwned(*var) ;
    }
  }

  Int_t icode,npari,nparx ;
  Double_t fmin,edm,errdef ;
  gMinuit->mnstat(fmin,edm,errdef,npari,nparx,icode) ;
  
  r->setConstParList(constPars) ;
  r->setInitParList(floatPars) ;
  r->setFinalParList(floatPars) ;
  r->setMinNLL(fmin) ;
  r->setEDM(edm) ; 
  r->setCovQual(icode) ;
  r->setStatus(gMinuit->fStatus) ;
  r->fillCorrMatrix() ;

  return r ;
}



//_____________________________________________________________________________
void RooMyFitResult::SetName(const char *name) 
{
  // Change name of RooMyFitResult object

  if (_dir) _dir->GetList()->Remove(this);
  TNamed::SetName(name) ;
  if (_dir) _dir->GetList()->Add(this);
}


//_____________________________________________________________________________
void RooMyFitResult::SetNameTitle(const char *name, const char* title) 
{
  // Change name and title of RooMyFitResult object

  if (_dir) _dir->GetList()->Remove(this);
  TNamed::SetNameTitle(name,title) ;
  if (_dir) _dir->GetList()->Add(this);
}


//_____________________________________________________________________________
void RooMyFitResult::printName(ostream& os) const 
{
  // Print name of fit result

  os << GetName() ;
}


//_____________________________________________________________________________
void RooMyFitResult::printTitle(ostream& os) const 
{
  // Print title of fit result

  os << GetTitle() ;
}



//_____________________________________________________________________________
void RooMyFitResult::printClassName(ostream& os) const 
{
  // Print class name of fit result

  os << IsA()->GetName() ;
}


//_____________________________________________________________________________
void RooMyFitResult::printArgs(ostream& os) const 
{
  // Print arguments of fit result, i.e. the parameters of the fit

  os << "[constPars=" << *_constPars << ",floatPars=" << *_finalPars << "]" ;
}



//_____________________________________________________________________________
void RooMyFitResult::printValue(ostream& os) const 
{
  // Print the value of the fit result, i.e.g the status, minimized FCN, edm and covariance quality code

  os << "(status=" << _status << ",FCNmin=" << _minNLL << ",EDM=" << _edm << ",covQual=" << _covQual << ")" ;
}

void RooMyFitResult::printValue() const
{
  // Print the value of the fit result, i.e.g the status, minimized FCN, edm and covariance quality code

  cout << "(status=" << _status << ",FCNmin=" << _minNLL << ",EDM=" << _edm << ",covQual=" << _covQual << ")" ;
}


//_____________________________________________________________________________
Int_t RooMyFitResult::defaultPrintContents(Option_t* /*opt*/) const 
{
  // Configure default contents to be printed

  return kName|kClassName|kArgs|kValue ;
}


//_____________________________________________________________________________
RooPrintable::StyleOption RooMyFitResult::defaultPrintStyle(Option_t* opt) const 
{
  // Configure mapping of Print() arguments to RooPrintable print styles
  if (!opt || strlen(opt)==0) {
    return kStandard ;
  }
  return RooPrintable::defaultPrintStyle(opt) ;
}

