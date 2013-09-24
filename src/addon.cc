#define BUILDING_NODE_EXTENTION
#include <node.h>
#include "list.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
	LinkedList::Init(exports);
}

NODE_MODULE(addon, InitAll)
