
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2013     (Visual Studio 2013)
--     vs2015     (Visual Studio 2015)
--
if (_ACTION == nil) then
    return
end

--
-- directory location for 3rd party dependencies
--
OE_3RD_PARTY_ROOT = "../../../OpenEaagles3rdParty"

--
-- set include and library paths
--
OEIncPath         = "../../include"
OE3rdPartyIncPath = OE_3RD_PARTY_ROOT.."/include"

--
-- directory location for HLA include and library paths
--
--HLA_ROOT = "../../../portico-2.0.1"
--HLAIncPath = HLA_ROOT.."/include/hla13"
HLA_ROOT = "../../../openrti"
HLAIncPath = HLA_ROOT.."/include/RTI13"
if (_ACTION == "vs2012") then
  HLALibPath = HLA_ROOT.."/lib/vc11"
end
if (_ACTION == "vs2013") then
  HLALibPath = HLA_ROOT.."/lib/vc12"
end
if (_ACTION == "vs2015") then
  HLALibPath = HLA_ROOT.."/lib/vc14"
end
print ("HLA Paths:")
print ("  Include   : "..HLALibPath)
--print ("  Libraries : "..OELibPath)

--
-- determine target directories for project/solution files and 
-- compiled libraries
--
locationPath  = "../" .. _ACTION
if (_ACTION == "vs2013") or (_ACTION == "vs2015") then
  targetDirPath = "../../lib/".._ACTION
end
print ("Target directory path: "..targetDirPath)

workspace "oe"

   -- destination directory for generated solution/project files
   location (locationPath)

   -- destination directory for compiled binary target
   targetdir (targetDirPath)

   -- creating static libraries
   kind "StaticLib"

   -- C++ code in all projects
   language "C++"

   -- common include directories (all configurations/all projects)
   includedirs { OEIncPath, OE3rdPartyIncPath }

   -- target suffix (all configurations/all projects)
   targetprefix "oe"
   if (_ACTION == "codelite") or (_ACTION == "codeblocks") then
      targetprefix "liboe"
   end

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release32", "Debug32" }

   -- common release configuration flags and symbols
   filter { "Release32" }
      flags { "Optimize" }
      if (_ACTION == "vs2013") or (_ACTION == "vs2015") then
         -- enable compiler intrinsics and favour speed over size
         buildoptions { "/Oi", "/Ot" }
         defines { "WIN32", "_LIB", "NDEBUG" }
      end

   -- common debug configuration flags and symbols
   filter { "Debug32" }
      targetsuffix "_d"
      flags { "Symbols" }
      if (_ACTION == "vs2013") or (_ACTION == "vs2015") then
         -- enable compiler intrinsics
         buildoptions { "/Oi" }
         defines { "WIN32", "_LIB", "_DEBUG" }
      end

   --
   -- libraries
   --

   -- base library
   project "base"
      files {
         "../../include/openeaagles/base/**.h",
         "../../include/openeaagles/base/**.inl",
         "../../include/openeaagles/base/**.epp",
         "../../include/openeaagles/base/osg/*",
         "../../src/base/**.cpp"
      }
      excludes {
         "../../src/base/osg/Matrix_implementation.cpp",
         "../../src/base/util/platform/system_linux.cpp",
         "../../src/base/util/platform/system_mingw.cpp",
         "../../src/base/platform/Thread_linux.cpp"
      }
      targetname "base"

   -- OpenGL-based graphics library
   project "graphics"
      files {
         "../../include/openeaagles/graphics/**.h",
         "../../src/graphics/**.cpp"
      }
      includedirs { OE3rdPartyIncPath.."/freetype2" }
      defines { "FTGL_LIBRARY_STATIC" }
      targetname "graphics"

   -- OpenGL GLUT interface library
   project "glut"
      files {
         "../../include/openeaagles/gui/glut/**.h",
         "../../src/gui/glut/**.cpp"
      }
      targetname "glut"

   -- DAFIF airport loader library
   project "dafif"
      files {
         "../../include/openeaagles/dafif/**.h",
         "../../src/dafif/**.cpp"
      }
      targetname "dafif"

   -- IEEE DIS interface library
   project "dis"
      files {
         "../../include/openeaagles/networks/dis/**.h",
         "../../src/networks/dis/**.cpp"
      }
      targetname "dis"

   -- IEEE HLA interface library (abstract support)
--   project "hla"
--      files {
--         "../../include/openeaagles/networks/hla/**.h",
--         "../../src/networks/hla/**.cpp"
--      }
--      includedirs { HLAIncPath }
--      defines { "RTI_USES_STD_FSTREAM" }
--      targetname "hla"
	  
   -- graphical instruments library
   project "instruments"
      files {
         "../../include/openeaagles/instruments/**.h",
         "../../include/openeaagles/instruments/**.epp",
         "../../src/instruments/**.cpp"
      }
      targetname "instruments"

   -- i/o device library
   project "iodevice"
      files {
         "../../include/openeaagles/iodevice/**.h",
         "../../src/iodevice/**.*"
      }
      excludes { "../../src/iodevice/platform/UsbJoystick_linux.*"   }
      targetname "iodevice"

   -- linear systems library
   project "linearsystem"
      files {
         "../../include/openeaagles/linearsystem/**.h",
         "../../src/linearsystem/**.cpp"
      }
      targetname "linearsystem"

   -- models library
   project "models"
      files {
         "../../include/openeaagles/models/**.h",
         "../../src/models/**.cpp"
      }
      includedirs { OE3rdPartyIncPath.."/JSBSim" }
      targetname "models"

   -- otw library
   project "otw"
      files {
         "../../include/openeaagles/otw/**.h",
         "../../src/otw/**.h",
         "../../src/otw/**.cpp"
      }
      excludes {
         "../../include/openeaagles/otw/OtwCigiClV2.h",
         "../../src/otw/OtwCigiClV2.cpp"
      }
      targetname "otw"

   project "recorder"
      files {
         "../../include/openeaagles/recorder/**.h",
         "../../include/openeaagles/recorder/*.inl",
         "../../include/openeaagles/recorder/**.proto",
         "../../src/recorder/**.cpp",
         "../../src/recorder/**.cc"
      }
      defines { "_SCL_SECURE_NO_WARNINGS" } -- suppress protocol buffer warning
      targetname "recorder"

   -- raster product format maps library
   project "rpf"
      files {
         "../../include/openeaagles/maps/rpf/**.h",
         "../../src/maps/rpf/**.cpp"
      }
      targetname "rpf"

   -- IEEE HLA interface library for RPR FOM
--   project "rprfom"
--      files {
--         "../../include/openeaagles/networks/rprfom/**.h",
--         "../../src/networks/rprfom/**.cpp"
--      }
--      includedirs { HLAIncPath }
--      defines { "RTI_USES_STD_FSTREAM" }
--      targetname "rprfom"

   -- simulation library
   project "simulation"
      files {
         "../../include/openeaagles/simulation/**.h",
         "../../include/openeaagles/simulation/*.inl",
         "../../src/simulation/**.cpp"
      }
      targetname "simulation"

   -- terrain library
   project "terrain"
      files {
         "../../include/openeaagles/terrain/**.h",
         "../../src/terrain/**.cpp"
      }
      targetname "terrain"
