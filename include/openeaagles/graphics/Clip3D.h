//------------------------------------------------------------------------------
// Class: Clip3D
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Clip3D_H__
#define __oe_graphics_Clip3D_H__

#include "openeaagles/base/Object.h"
#include "Polygon.h"
#include <cfloat>

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Clip3D
// Base class: Object -> Clip3D
//
// Factory name: Clip3D
// Description: 3D polygon clipping
//
//------------------------------------------------------------------------------
class Clip3D : public base::Object
{
    DECLARE_SUBCLASS(Clip3D,base::Object)

public:
   Clip3D();

   //------------------------------------------------------------------------------
   // Sets the limits of the clipping box
   //------------------------------------------------------------------------------
   void setClippingBox(
         const double xmin, const double xmax,
         const double ymin, const double ymax,
         const double zmin = -FLT_MAX, const double zmax = FLT_MAX
      );

   //------------------------------------------------------------------------------
   // clip() --  Clips the convex polygon 'inPoly' to the clipping box defined by
   // setClippingBox().  The function returns a pointer to a new, clipped polygon,
   // which has been pre-ref()'d, or zero if the whole 'inPoly' polygon is outside of
   // the clipping box.
   //------------------------------------------------------------------------------
   Polygon* clip(const Polygon& inPoly) const;

   //------------------------------------------------------------------------------
   // clip() --  Clips the convex polygon 'in', which contains 'num' vertices, to
   // the clipping box defined by setBox().  The function returns the number of
   // vertices in the clipped polygon or zero if the whole polygon is outside of
   // the clipping box.  The vertices of the clipped polygon are returned in the
   // user provided output arrays, which must be of at least size 'num'.
   //------------------------------------------------------------------------------
   unsigned int clip(
         osg::Vec3* const out,            // Output polygon vertices
         osg::Vec3* const outNorm,        // Output polygon vertices
         osg::Vec2* const outTCoord,      // Output polygon vertices
         const osg::Vec3* const in,       // Input polygon vertices
         const osg::Vec3* const inNorm,   // Input polygon vertices
         const osg::Vec2* const inTCoord, // Input polygon vertices
         const unsigned int num           // Number of vertices in arrays
      ) const;

   //------------------------------------------------------------------------------
   // polyClip2Halfspace() -- clips one coordinate of a convex polygon p against
   // an upper or lower plane.  The clipped polygon is returned in q.  The number
   // of vertices in the clipped polygon is returned by the function.
   //------------------------------------------------------------------------------
   static unsigned int polyClip2Halfspace(
      osg::Vec3* const q,           // Output polygon vertices
      osg::Vec3* const qn,          // (optional) Output polygon normals
      osg::Vec2* const qt,          // (optional) Output polygon texture coordinates
      const osg::Vec3* const p,     // Input polygon vertices
      const osg::Vec3* const pn,    // (optional) Input polygon normals
      const osg::Vec2* const pt,    // (optional) Input polygon texture coordinates
      const unsigned int n,         // Number of vertices/normals
      const unsigned int index,     // Coordinate index: X -> 0; Y -> 1; and Z -> 2
      const double k,               // Value of the clipping plane
      const bool clipUpperPlane);   // if clipping against an upper plane, 
                                    //  else against a lower plane

private:
   double x0, x1;     // left and right
   double y0, y1;     // top and bottom
   double z0, z1;     // near and far
};

}
}

#endif

