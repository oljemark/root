/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooFormulaVar.h,v 1.29 2007/08/09 19:55:47 wouter Exp $
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
#ifndef ROO_FORMULA_VAR
#define ROO_FORMULA_VAR

#include "RooAbsReal.h"
#include "RooArgList.h"
#include "RooListProxy.h"
#include "RooTrace.h"

#include <memory>
#include <list>

class RooArgSet ;
class RooFormula ;

class RooFormulaVar : public RooAbsReal {
public:
  // Constructors, assignment etc
  RooFormulaVar();
  ~RooFormulaVar() override;
  RooFormulaVar(const char *name, const char *title, const char* formula, const RooArgList& dependents, bool checkVariables = true);
  RooFormulaVar(const char *name, const char *title, const RooArgList& dependents, bool checkVariables = true);
  RooFormulaVar(const RooFormulaVar& other, const char* name=nullptr);
  TObject* clone(const char* newname=nullptr) const override { return new RooFormulaVar(*this,newname); }

  bool ok() const;
  const char* expression() const { return _formExpr.Data(); }
  const RooArgList& dependents() const { return _actualVars; }

  /// Return pointer to parameter with given name.
  inline RooAbsArg* getParameter(const char* name) const {
    return _actualVars.find(name) ;
  }
  /// Return pointer to parameter at given index.
  inline RooAbsArg* getParameter(Int_t index) const {
    return _actualVars.at(index) ;
  }
  /// Return the number of parameters.
  inline size_t nParameters() const {
    return _actualVars.size();
  }

  // I/O streaming interface (machine readable)
  bool readFromStream(std::istream& is, bool compact, bool verbose=false) override ;
  void writeToStream(std::ostream& os, bool compact) const override ;

  // Printing interface (human readable)
  void printMultiline(std::ostream& os, Int_t contents, bool verbose=false, TString indent= "") const override ;
  void printMetaArgs(std::ostream& os) const override ;

  // Debugging
  /// Dump the formula to stdout.
  void dumpFormula();

  double defaultErrorLevel() const override ;

  std::list<double>* binBoundaries(RooAbsRealLValue& /*obs*/, double /*xlo*/, double /*xhi*/) const override ;
  std::list<double>* plotSamplingHint(RooAbsRealLValue& /*obs*/, double /*xlo*/, double /*xhi*/) const override ;

  // Function evaluation
  double evaluate() const override ;
  void doEval(RooFit::EvalContext &ctx) const override;

  std::string getUniqueFuncName() const;

  protected:
  // Post-processing of server redirection
  bool redirectServersHook(const RooAbsCollection& newServerList, bool mustReplaceAll, bool nameChange, bool isRecursive) override ;

  bool isValidReal(double /*value*/, bool /*printError*/) const override {return true;}

  private:
  RooFormula& getFormula() const;

  RooListProxy _actualVars ;     ///< Actual parameters used by formula engine
  mutable RooFormula *_formula = nullptr; ///<! Formula engine
  mutable RooArgSet* _nset{nullptr}; ///<! Normalization set to be passed along to contents
  TString _formExpr ;            ///< Formula expression string

  ClassDefOverride(RooFormulaVar,1) // Real-valued function of other RooAbsArgs calculated by a TFormula expression
};

#endif
