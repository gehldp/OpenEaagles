//------------------------------------------------------------------------------
// Class: StrokeFont
//------------------------------------------------------------------------------
#ifndef __oe_graphics_StrokeFont_H__
#define __oe_graphics_StrokeFont_H__

#include "Font.h"

namespace oe {
namespace graphics {

//------------------------------------------------------------------------------
// Class:  StrokeFont
// Base class:  Object -> Font -> StrokeFont
//
// Description:  (SGI's) Stroke font
// Factory name: StrokeFont
//
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//
//      static GLenum createStrokeFont(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class StrokeFont : public Font {
    DECLARE_SUBCLASS(StrokeFont,Font)

public:
    StrokeFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;

    static GLenum createStrokeFont(GLuint fontBase);
};


} // End graphics namespace
} // End oe namespace

#endif
