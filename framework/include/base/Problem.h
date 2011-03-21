#ifndef PROBLEM_H
#define PROBLEM_H

//#include "Output.h"
#include "ParallelUniqueId.h"
#include "InputParameters.h"
//#include "MaterialProperty.h"
#include "Function.h"
#include "MooseMesh.h"
#include "SubProblemInterface.h"
// libMesh
#include "libmesh_common.h"
#include "equation_systems.h"
#include "quadrature.h"
#include "elem.h"
#include "node.h"
#include "nonlinear_implicit_system.h"

class MooseVariable;
class Output;

class Problem
{
public:
  Problem();
  virtual ~Problem();

  virtual EquationSystems & es() = 0;
  virtual Problem * parent() = 0;

  // Variables /////
  virtual bool hasVariable(const std::string & var_name) = 0;
  virtual MooseVariable & getVariable(THREAD_ID tid, const std::string & var_name) = 0;

  virtual void subdomainSetup(unsigned int subdomain, THREAD_ID tid) = 0;

  virtual void prepare(const Elem * elem, THREAD_ID tid) = 0;

  virtual void reinitElem(const Elem * elem, THREAD_ID tid) = 0;
  virtual void reinitElemFace(const Elem * elem, unsigned int side, unsigned int bnd_id, THREAD_ID tid) = 0;
  virtual void reinitNode(const Node * node, THREAD_ID tid) = 0;
  virtual void reinitNodeFace(const Node * node, unsigned int bnd_id, THREAD_ID tid) = 0;

  // Materials /////
  virtual void reinitMaterials(unsigned int blk_id, THREAD_ID tid) = 0;
  virtual void reinitMaterialsFace(unsigned int blk_id, unsigned int side, THREAD_ID tid) = 0;

  /// Returns true if the Problem has Dirac kernels it needs to compute on elem.
  virtual bool reinitDirac(const Elem * elem, THREAD_ID tid){ mooseError("Cannont reinit this Problem with arbitrary quadrature points!"); };

  /// Fills "elems" with the elements that should be looped over for Dirac Kernels
  virtual void getDiracElements(std::set<const Elem *> & elems){ mooseError("Cannont retrieve Dirac elements from this problem!"); };

  /// Get's called before Dirac Kernels are asked to add the points they are supposed to be evaluated in
  virtual void clearDiracInfo(){ mooseError("Cannont clear Dirac Info this problem!"); };

  // Solve /////
  virtual void init() = 0;
  virtual void update() = 0;

  virtual void computeResidual(NonlinearImplicitSystem & sys, const NumericVector<Number> & soln, NumericVector<Number> & residual) = 0;
  virtual void computeJacobian(NonlinearImplicitSystem & sys, const NumericVector<Number> & soln, SparseMatrix<Number> & jacobian) = 0;

  // Initial conditions /////
  virtual Number initialValue (const Point & p, const Parameters & parameters, const std::string & sys_name, const std::string & var_name) = 0;
  virtual Gradient initialGradient (const Point & p, const Parameters & parameters, const std::string & sys_name, const std::string & var_name) = 0;

  virtual void initialCondition(EquationSystems & es, const std::string & system_name) = 0;

  // Postprocessors /////
  virtual void computePostprocessors(int pps_type = Moose::PPS_TIMESTEP) = 0;
  virtual void outputPostprocessors() = 0;
  virtual Real & getPostprocessorValue(const std::string & name, THREAD_ID tid = 0) = 0;

  // Function /////
  virtual void addFunction(std::string type, const std::string & name, InputParameters parameters);
  virtual Function & getFunction(const std::string & name, THREAD_ID tid = 0);

  // Transient /////
  virtual void transient(bool trans) = 0;
  virtual bool transient() = 0;

  virtual Real & time() = 0;
  virtual int & timeStep() = 0;
  virtual Real & dt() = 0;
  virtual Real & dtOld() = 0;
  virtual std::vector<Real> & timeWeights() { return _time_weights; }

  virtual void copySolutionsBackwards() = 0;

  // Output system /////

  virtual Output & out() = 0;
  virtual void output() = 0;

public:
  /**
   * Convenience zeros
   */
  Array<Real> _real_zero;
  Array<Array<Real> > _zero;
  Array<Array<RealGradient> > _grad_zero;
  Array<Array<RealTensor> > _second_zero;

protected:
  // functions
  std::vector<std::map<std::string, Function *> > _functions;

  std::vector<Real> _time_weights;
};

#endif /* PROBLEM_H */
