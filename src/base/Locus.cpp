
#include "openeaagles/base/Locus.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Locus,"Locus")
EMPTY_SLOTTABLE(Locus)
EMPTY_SERIALIZER(Locus)

Locus::Locus()
{
   STANDARD_CONSTRUCTOR()

   data = nullptr;
   np = 0;
   refLat = 0;
   refLon = 0;
   angle = 0;
   minRng = 0;
   maxRng = 0;
   deltaRng = 0;
}

Locus::Locus(
       const double* const newData, // Array of data points
       const unsigned int n,        // Number of data points
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const double ang,            // True direction (heading) angle of the data (degs)
       const double mn,             // Range to first data point
       const double mx              // Range to last data point
       )
{
   STANDARD_CONSTRUCTOR()

   data = nullptr;
   setData(newData, n, lat, lon, ang, mn, mx);
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Locus::copyData(const Locus& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      data = nullptr;
   }

   // Let setData() copy the data
   setData(
         org.getDataArray(),
         org.getNumPts(),
         org.getRefLatitude(),
         org.getRefLongitude(),
         org.getDirection(),
         org.getMinRng(),
         org.getMaxRng()
      );
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Locus::deleteData()
{
   clearData();
}

//------------------------------------------------------------------------------
// Access (get) functions
//------------------------------------------------------------------------------
double Locus::getRange(const unsigned int idx) const
{
   double rng = 0;
   if (idx < np && isDataValid()) {
      rng = minRng + deltaRng * static_cast<double>(idx);
   }
   return rng;
}

double Locus::getData(const unsigned int idx) const
{
   double value = 0;
   if (idx < np && isDataValid()) {
      value = data[idx];
   }
   return value;
}

//------------------------------------------------------------------------------
// Sets new array data
//------------------------------------------------------------------------------
void Locus::setData(
       const double* const newData, // Array of data points
       const unsigned int n,        // Number of data points
       const double lat,            // Reference latitude (degs)
       const double lon,            // Reference longitude (degs)
       const double ang,            // True direction (heading) angle of the data (degs)
       const double mn,             // Range to first data point
       const double mx              // Range to last data point
       )
{
   // Remove any old data
   if (data != nullptr) {
      delete[] data;
      data = nullptr;
      np = 0;
   }

   // Copy the new data
   if (n < MAX_SIZE && newData != nullptr) {
      np = n;
      data = new double[np];
      double* p = data;
      const double* q = newData;
      for (unsigned int i = 0; i < np; i++) {
         *p++ = *q++;
      }
   }

   // copy the rest of the parameters
   refLat = lat;
   refLon = lon;
   angle = ang;
   minRng = mn;
   maxRng = mx;

   // Compute the delta range
   deltaRng = 0;
   if (n > 1) {
      deltaRng = (maxRng - minRng) / (n - 1);
   }
}

//------------------------------------------------------------------------------
// Clears out the data array
//------------------------------------------------------------------------------
void Locus::clearData()
{
   // Let setData() clear the data
   setData(nullptr, 0, 0, 0, 0, 0, 0);
}

}
}
