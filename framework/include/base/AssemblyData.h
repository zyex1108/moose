#ifndef ASSEMBLYDATA_H
#define ASSEMBLYDATA_H

// libMesh includes
#include "fe.h"
#include "quadrature.h"
#include "elem.h"
#include "node.h"

// MOOSE Forward Declares
class MooseMesh;
class ArbitraryQuadrature;  

class AssemblyData
{
public:
  AssemblyData(MooseMesh & mesh);
  virtual ~AssemblyData();

  FEBase * & getFE(FEType type);
  QBase * & qRule() { return _qrule; }
  const std::vector<Point> & qPoints() { return _q_points; }
  const std::vector<Real> & JxW() { return _JxW; }

  FEBase * & getFEFace(FEType type);
  QBase * & qRuleFace() { return _qrule_face; }
  const std::vector<Point> & qPointsFace() { return _q_points_face; }
  const std::vector<Real> & JxWFace() { return _JxW_face; }

  const std::vector<Point> & normals() { return _normals; }

  const Elem * & elem() { return _current_elem; }
  unsigned int & side() { return _current_side; }
  const Elem * & sideElem() { return _current_side_elem; }

  const Node * & node() { return _current_node; }

  /**
   * Creates the volume, face and arbitrary qrules based on the Order passed in.
   */
  void createQRules(Order o);

  /**
   * Set the qrule to be used for volume integration.
   *
   * Note: This is normally set internally, only use if you know what you are doing!
   */
  void setVolumeQRule(QBase * qrule);

  /**
   * Set the qrule to be used for face integration.
   *
   * Note: This is normally set internally, only use if you know what you are doing!
   */
  void setFaceQRule(QBase * qrule);

  void reinit(const Elem * elem);

  /**
   * Reinitialize the assembly data at specific physical point in the given element.
   */
  void reinitAtPhysical(const Elem * elem, const std::vector<Point> & physical_points);

  /**
   * Reinitialize the assembly data at specific points in the reference element.
   */
  void reinit(const Elem * elem, const std::vector<Point> & reference_points);
  
  void reinit(const Elem * elem, unsigned int side);
  void reinit(const Node * node);

  Real computeVolume();

protected:
  MooseMesh & _mesh;

  std::map<FEType, FEBase *> _fe;               /// types of finite elements
  FEBase * _fe_helper;                          /// helper object for transforming coordinates
  QBase * _qrule;
  QBase * _qrule_volume;
  ArbitraryQuadrature * _qrule_arbitrary;
  
  const std::vector<Point> & _q_points;
  const std::vector<Real> & _JxW;

  std::map<FEType, FEBase *> _fe_face;          /// types of finite elements
  FEBase * _fe_face_helper;                          /// helper object for transforming coordinates
  QBase * _qrule_face;
  const std::vector<Point> & _q_points_face;
  const std::vector<Real> & _JxW_face;
  const std::vector<Point> & _normals;          /// Normal vectors at the quadrature points.


  const Elem * _current_elem;                   // The current "element" we are currently on.
  unsigned int _current_side;
  const Elem * _current_side_elem;              // The current "element" making up the side we are currently on.

  const Node * _current_node;


};

#endif /* ASSEMBLYDATA_H */
