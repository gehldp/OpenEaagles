#include "openeaagles/instruments/adi/GhostHorizon.h"
#include "openeaagles/base/functors/Tables.h"
#include "openeaagles/base/Rgba.h"
#include "openeaagles/graphics/Display.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(GhostHorizon, "GhostHorizon")
EMPTY_SERIALIZER(GhostHorizon)

// SLOTS
BEGIN_SLOTTABLE(GhostHorizon)
    "skyColor",     // 1) determine our sky color to draw
    "groundColor",  // 2) determine our ground color to draw
    "width",        // 3) width of the polygon to draw for the ground and sky
    "height",       // 4) height of the polyogn to draw for the ground and sky
END_SLOTTABLE(GhostHorizon)

BEGIN_SLOT_MAP(GhostHorizon)
    ON_SLOT(1, setSlotSkyColor, base::Color)
    ON_SLOT(1, setSlotSkyColor, base::String)
    ON_SLOT(2, setSlotGroundColor, base::String)
    ON_SLOT(2, setSlotGroundColor, base::Color)
    ON_SLOT(3, setSlotWidth, base::Number)
    ON_SLOT(4, setSlotHeight, base::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
GhostHorizon::GhostHorizon()
{
    STANDARD_CONSTRUCTOR()

    skyColor.set(1, 1, 1);
    groundColor.set(1, 1, 1);
    gColorName = nullptr;
    sColorName = nullptr;
    width = 20;
    height = 20;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void GhostHorizon::copyData(const GhostHorizon& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        gColorName = nullptr;
        sColorName = nullptr;
    }

    skyColor = org.skyColor;
    groundColor = org.groundColor;
    width = org.width;
    height = org.height;
    if (org.gColorName != nullptr) setSlotGroundColor(org.gColorName);
    if (org.sColorName != nullptr) setSlotSkyColor(org.sColorName);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void GhostHorizon::deleteData()
{
    if (gColorName != nullptr) gColorName->unref();
    gColorName = nullptr;
    if (sColorName != nullptr) sColorName->unref();
    sColorName = nullptr;
}

// Slot functions
//------------------------------------------------------------------------------
// setSlotSkyColor() - set the color of our Ghost Horizon "sky" by a Color obj.
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotSkyColor(const base::Color* const cobj)
{
    bool ok = false;
    if (cobj != nullptr) {
        skyColor.set(cobj->red(), cobj->green(), cobj->blue());
        ok = true;
    }

    return ok;
}

//------------------------------------------------------------------------------
// setSlotSkyColor() - set the color of our Ghost Horizon "sky" by a string.
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotSkyColor(const base::String* const cname)
{
    bool ok = false;
    if (cname != nullptr) {
        if (sColorName == nullptr) sColorName = new base::String(cname->getString());
        else sColorName->setStr(cname->getString());
        ok = true;
    }

    return ok;
}

//------------------------------------------------------------------------------
// setSlotGroundColor() - set our "ground" color by string
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotGroundColor(const base::String* const cname)
{
    bool ok = false;
    if (cname != nullptr) {
        if (gColorName == nullptr) gColorName = new base::String(cname->getString());
        else gColorName->setStr(cname->getString());
        ok = true;
    }

    return ok;
}

//------------------------------------------------------------------------------
// setSlotGroundColor() - set our "ground" color by Color obj.
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotGroundColor(const base::Color* const cobj)
{
    bool ok = false;
    if (cobj != nullptr) {
        groundColor.set(cobj->red(), cobj->green(), cobj->blue());
        ok = true;
    }

    return ok;
}

//------------------------------------------------------------------------------
// setSlotWidth() - set the width of our horizon
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotWidth(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = setWidth(x->getReal());
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotHeight() - set the height of our horizon
//------------------------------------------------------------------------------
bool GhostHorizon::setSlotHeight(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = setHeight(x->getReal());
    }
    return ok;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void GhostHorizon::drawFunc()
{
    GLfloat ocolor[4];
    glGetFloatv(GL_CURRENT_COLOR, ocolor);

    glPushMatrix();

    // if we are negative translation (pitch up), we show the ground background below us
    // ground
    // get our table to determine our min and max value
    double maxPitch = 90;
    double minPitch = -90;
    const base::Table1* table = getScalingTable();
    if (table != nullptr) {
        maxPitch = table->getMaxX();
        minPitch = table->getMinX();
    }

    double value = getPreScaleInstValue();

    if (value > maxPitch) {
            glColor3d(groundColor.x(), groundColor.y(), groundColor.z());
            glBegin(GL_POLYGON);
                lcVertex2(-width, -height);
                lcVertex2(-width, 0);
                lcVertex2( width, 0);
                lcVertex2( width, -height);
            glEnd();

    }
    // sky
    else if (value < minPitch) {
        glColor3d(skyColor.x(), skyColor.y(), skyColor.z());
        glBegin(GL_POLYGON);
            lcVertex2(-width, height);
            lcVertex2(-width, 0);
            lcVertex2(width, 0);
            lcVertex2(width, height);
        glEnd();

    }
    glPopMatrix();

    glColor4fv(ocolor);
}

//------------------------------------------------------------------------------
// updateData() -- update non time-critical threads here
//------------------------------------------------------------------------------
void GhostHorizon::updateData(const double dt)
{
    // Update our base class
    BaseClass::updateData(dt);

    // get our table to determine our min and max value
    double maxPitch = 90;
    double minPitch = -90;
    const base::Table1* table = getScalingTable();
    if (table != nullptr) {
        maxPitch = table->getMaxX();
        minPitch = table->getMinX();
    }
    double value = getPreScaleInstValue();

    if (value <= minPitch/* || value >= pitchLim*/) {
        setVisibility(true);
        // scale our location value
        //location = -(location - margin);
        if (sColorName != nullptr) {
            graphics::Display* d = getDisplay();
            if (d != nullptr) {
                base::Color* c = d->getColor(sColorName->getString());
                if (c != nullptr) {
                    skyColor.set(c->red(), c->green(), c->blue());
                }
            }
        }
    }
    else if (value >= maxPitch) {
        setVisibility(true);
        //location = -(location + margin);
        if (gColorName != nullptr) {
            graphics::Display* d = getDisplay();
            if (d != nullptr) {
                base::Color* c = d->getColor(gColorName->getString());
                if (c != nullptr) {
                    groundColor.set(c->red(), c->green(), c->blue());
                }
            }
        }
    }
    else setVisibility(false);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for GhostHorizon
//------------------------------------------------------------------------------
base::Object* GhostHorizon::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}; // end of Instruments namespace
}; // end of oe namespace
