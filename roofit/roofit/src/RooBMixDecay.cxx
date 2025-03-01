/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 * @(#)root/roofit:$Id$
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

/** \class RooBMixDecay
    \ingroup Roofit

Class RooBMixDecay is a RooAbsAnaConvPdf implementation that describes
the decay of B mesons with the effects of B0/B0bar mixing.
This function can be analytically convolved with any RooResolutionModel implementation
**/

#include "Riostream.h"
#include "TMath.h"
#include "RooRealVar.h"
#include "RooBMixDecay.h"
#include "RooRealIntegral.h"
#include "RooRandom.h"
#include "RooBatchCompute.h"


/// \brief Constructor for RooBMixDecay.
///
/// Creates an instance of RooBMixDecay with the specified parameters.
///
/// \param[in] name         The name of the PDF.
/// \param[in] title        The title of the PDF.
/// \param[in] t            The time variable.
/// \param[in] mixState     The mixing state category.
/// \param[in] tagFlav      The flavour of tagged B0 category.
/// \param[in] tau          The mixing life time parameter.
/// \param[in] dm           The mixing frequency parameter.
/// \param[in] mistag       The mistag rate parameter.
/// \param[in] delMistag    The delta mistag rate parameter.
/// \param[in] model        The resolution model.
/// \param[in] type         The decay type.

RooBMixDecay::RooBMixDecay(const char *name, const char *title,
            RooRealVar& t, RooAbsCategory& mixState,
            RooAbsCategory& tagFlav,
            RooAbsReal& tau, RooAbsReal& dm,
            RooAbsReal& mistag, RooAbsReal& delMistag,
            const RooResolutionModel& model,
            DecayType type) :
  RooAbsAnaConvPdf(name,title,model,t),
  _type(type),
  _mistag("mistag","Mistag rate",this,mistag),
  _delMistag("delMistag","Delta mistag rate",this,delMistag),
  _mixState("mixState","Mixing state",this,mixState),
  _tagFlav("tagFlav","Flavour of tagged B0",this,tagFlav),
  _tau("tau","Mixing life time",this,tau),
  _dm("dm","Mixing frequency",this,dm),
  _t("_t","time",this,t), _genMixFrac(0)
{
  switch(type) {
  case SingleSided:
    _basisExp = declareBasis("exp(-@0/@1)",RooArgList(tau)) ;
    _basisCos = declareBasis("exp(-@0/@1)*cos(@0*@2)",RooArgList(tau,dm)) ;
    break ;
  case Flipped:
    _basisExp = declareBasis("exp(@0/@1)",RooArgList(tau)) ;
    _basisCos = declareBasis("exp(@0/@1)*cos(@0*@2)",RooArgList(tau,dm)) ;
    break ;
  case DoubleSided:
    _basisExp = declareBasis("exp(-abs(@0)/@1)",RooArgList(tau)) ;
    _basisCos = declareBasis("exp(-abs(@0)/@1)*cos(@0*@2)",RooArgList(tau,dm)) ;
    break ;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// Copy constructor

RooBMixDecay::RooBMixDecay(const RooBMixDecay& other, const char* name) :
  RooAbsAnaConvPdf(other,name),
  _type(other._type),
  _mistag("mistag",this,other._mistag),
  _delMistag("delMistag",this,other._delMistag),
  _mixState("mixState",this,other._mixState),
  _tagFlav("tagFlav",this,other._tagFlav),
  _tau("tau",this,other._tau),
  _dm("dm",this,other._dm),
  _t("t",this,other._t),
  _basisExp(other._basisExp),
  _basisCos(other._basisCos),
  _genMixFrac(other._genMixFrac),
  _genFlavFrac(other._genFlavFrac),
  _genFlavFracMix(other._genFlavFracMix),
  _genFlavFracUnmix(other._genFlavFracUnmix)
{
}

////////////////////////////////////////////////////////////////////////////////
/// Comp with tFit MC: must be (1 - tagFlav*...)

double RooBMixDecay::coefficient(Int_t basisIndex) const
{
  if (basisIndex==_basisExp) {
    return (1 - _tagFlav*_delMistag) ;
  }

  if (basisIndex==_basisCos) {
    return _mixState*(1-2*_mistag) ;
  }

  return 0 ;
}

void RooBMixDecay::doEval(RooFit::EvalContext &ctx) const
{
   RooBatchCompute::compute(ctx.config(this), RooBatchCompute::BMixDecay, ctx.output(),
                            {ctx.at(&_convSet[0]), ctx.at(&_convSet[1]), ctx.at(_tagFlav),
                             ctx.at(_delMistag), ctx.at(_mixState), ctx.at(_mistag)});
}

////////////////////////////////////////////////////////////////////////////////
///   std::cout << "RooBMixDecay::getCoefAI " ; allVars.Print("1") ;

Int_t RooBMixDecay::getCoefAnalyticalIntegral(Int_t /*code*/, RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const
{
  if (rangeName) {
    return 0 ;
  }

  if (matchArgs(allVars,analVars,_mixState,_tagFlav)) return 3 ;
  if (matchArgs(allVars,analVars,_mixState)) return 2 ;
  if (matchArgs(allVars,analVars,_tagFlav)) return 1 ;
  return 0 ;
}

////////////////////////////////////////////////////////////////////////////////

double RooBMixDecay::coefAnalyticalIntegral(Int_t basisIndex, Int_t code, const char* /*rangeName*/) const
{
  switch(code) {
    // No integration
  case 0: return coefficient(basisIndex) ;

    // Integration over 'mixState' and 'tagFlav'
  case 3:
    if (basisIndex==_basisExp) {
      return 4.0 ;
    }
    if (basisIndex==_basisCos) {
      return 0.0 ;
    }
    break ;

    // Integration over 'mixState'
  case 2:
    if (basisIndex==_basisExp) {
      return 2.0*coefficient(basisIndex) ;
    }
    if (basisIndex==_basisCos) {
      return 0.0 ;
    }
    break ;

    // Integration over 'tagFlav'
  case 1:
    if (basisIndex==_basisExp) {
      return 2.0 ;
    }
    if (basisIndex==_basisCos) {
      return 2.0*coefficient(basisIndex) ;
    }
    break ;

  default:
    assert(0) ;
  }

  return 0 ;
}

////////////////////////////////////////////////////////////////////////////////

Int_t RooBMixDecay::getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, bool staticInitOK) const
{
  if (staticInitOK) {
    if (matchArgs(directVars,generateVars,_t,_mixState,_tagFlav)) return 4 ;
    if (matchArgs(directVars,generateVars,_t,_mixState)) return 3 ;
    if (matchArgs(directVars,generateVars,_t,_tagFlav)) return 2 ;
  }

  if (matchArgs(directVars,generateVars,_t)) return 1 ;
  return 0 ;
}

////////////////////////////////////////////////////////////////////////////////

void RooBMixDecay::initGenerator(Int_t code)
{
  switch (code) {
  case 2:
    {
      // Calculate the fraction of B0bar events to generate
      double sumInt = RooRealIntegral("sumInt","sum integral",*this,RooArgSet(_t.arg(),_tagFlav.arg())).getVal() ;
      _tagFlav = 1 ; // B0
      double flavInt = RooRealIntegral("flavInt","flav integral",*this,RooArgSet(_t.arg())).getVal() ;
      _genFlavFrac = flavInt/sumInt ;
      break ;
    }
  case 3:
    {
      // Calculate the fraction of mixed events to generate
      double sumInt = RooRealIntegral("sumInt","sum integral",*this,RooArgSet(_t.arg(),_mixState.arg())).getVal() ;
      _mixState = -1 ; // mixed
      double mixInt = RooRealIntegral("mixInt","mix integral",*this,RooArgSet(_t.arg())).getVal() ;
      _genMixFrac = mixInt/sumInt ;
      break ;
    }
  case 4:
    {
      // Calculate the fraction of mixed events to generate
      double sumInt = RooRealIntegral("sumInt","sum integral",*this,RooArgSet(_t.arg(),_mixState.arg(),_tagFlav.arg())).getVal() ;
      _mixState = -1 ; // mixed
      double mixInt = RooRealIntegral("mixInt","mix integral",*this,RooArgSet(_t.arg(),_tagFlav.arg())).getVal() ;
      _genMixFrac = mixInt/sumInt ;

      // Calculate the fraction of B0bar tags for mixed and unmixed
      RooRealIntegral dtInt("mixInt","mix integral",*this,RooArgSet(_t.arg())) ;
      _mixState = -1 ; // Mixed
      _tagFlav  =  1 ; // B0
      _genFlavFracMix   = dtInt.getVal() / mixInt ;
      _mixState =  1 ; // Unmixed
      _tagFlav  =  1 ; // B0
      _genFlavFracUnmix = dtInt.getVal() / (sumInt - mixInt) ;
      break ;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// Generate mix-state dependent

void RooBMixDecay::generateEvent(Int_t code)
{
  switch(code) {
  case 2:
    {
      double rand = RooRandom::uniform() ;
      _tagFlav = (Int_t) ((rand<=_genFlavFrac) ?  1 : -1) ;
      break ;
    }
  case 3:
    {
      double rand = RooRandom::uniform() ;
      _mixState = (Int_t) ((rand<=_genMixFrac) ? -1 : 1) ;
      break ;
    }
  case 4:
    {
      double rand = RooRandom::uniform() ;
      _mixState = (Int_t) ((rand<=_genMixFrac) ? -1 : 1) ;

      rand = RooRandom::uniform() ;
      double genFlavFrac = (_mixState==-1) ? _genFlavFracMix : _genFlavFracUnmix ;
      _tagFlav = (Int_t) ((rand<=genFlavFrac) ?  1 : -1) ;
      break ;
    }
  }

  // Generate delta-t dependent
  while(true) {
    double rand = RooRandom::uniform() ;
    double tval(0) ;

    switch(_type) {
    case SingleSided:
      tval = -_tau*log(rand);
      break ;
    case Flipped:
      tval= +_tau*log(rand);
      break ;
    case DoubleSided:
      tval = (rand<=0.5) ? -_tau*log(2*rand) : +_tau*log(2*(rand-0.5)) ;
      break ;
    }

    // Accept event if T is in generated range
    double dil = 1-2.*_mistag ;
    double maxAcceptProb = 1 + std::abs(_delMistag) + std::abs(dil) ;
    double acceptProb = (1-_tagFlav*_delMistag) + _mixState*dil*cos(_dm*tval);
    bool mixAccept = maxAcceptProb*RooRandom::uniform() < acceptProb ? true : false ;

    if (tval<_t.max() && tval>_t.min() && mixAccept) {
      _t = tval ;
      break ;
    }
  }
}
