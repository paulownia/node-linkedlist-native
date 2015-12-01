#define BUILDING_NODE_EXTENTION
#include <node.h>
#include "list.h"

namespace list {

	using v8::Local;
	using v8::Object;


	void InitAll(Local<Object> exports) {
		LinkedList::Init(exports);
	}

	NODE_MODULE(addon, InitAll)
}
