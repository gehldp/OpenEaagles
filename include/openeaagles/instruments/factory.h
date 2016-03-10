
#ifndef __oe_instruments_factory_H__
#define __oe_instruments_factory_H__

namespace oe {

namespace base { class Object; }

namespace instruments {

// factory function
base::Object* factory(const char* name);

}
}

#endif
