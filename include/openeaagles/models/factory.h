
#ifndef __oe_models_factory_H__
#define __oe_models_factory_H__

namespace oe {

namespace basic { class Object; }

namespace models {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
