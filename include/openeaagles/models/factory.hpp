
#ifndef __oe_models_factory_H__
#define __oe_models_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace models {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
