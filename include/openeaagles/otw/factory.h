
#ifndef __oe_otw_factory_H__
#define __oe_otw_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace otw {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
