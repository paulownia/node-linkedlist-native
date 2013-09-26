#include <node.h>
#include "list.h"
#include <iostream>

using namespace v8;


Entry::Entry() {
	prev = nullptr;
	next = nullptr;
};

Entry::~Entry() {
	value.Dispose();
};

LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
	length = 0;
};

LinkedList::~LinkedList() {
	while (head != nullptr) {
		Entry* current = head;
		head = head->next;
		delete current;
	}
};

void LinkedList::Init(Handle<Object> exports) {

	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	
	tpl->SetClassName(String::NewSymbol("LinkedList"));
	tpl->InstanceTemplate()->SetInternalFieldCount(3);
	tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("length"), GetLength);
	tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("first"), GetFirst);
	tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("last"), GetLast);

	tpl->PrototypeTemplate()->Set(String::NewSymbol("addFirst"), FunctionTemplate::New(AddFirst)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("addLast"), FunctionTemplate::New(AddLast)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("removeFirst"), FunctionTemplate::New(RemoveFirst)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("removeLast"), FunctionTemplate::New(RemoveLast)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(String::NewSymbol("unshift"), FunctionTemplate::New(AddFirst)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("push"), FunctionTemplate::New(AddLast)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("shift"), FunctionTemplate::New(RemoveFirst)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("pop"), FunctionTemplate::New(RemoveLast)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("forEach"), FunctionTemplate::New(ForEach)->GetFunction());
	
	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());

	exports->Set(String::NewSymbol("LinkedList"), constructor);
}

Handle<Value> LinkedList::New(const Arguments& args) {
	HandleScope scope;
	
	LinkedList* obj = new LinkedList();
	obj->Wrap(args.This());

	int len = args.Length();
	for (int i = 0; i < len; i++) {
		obj->Push(args[i]);		
	}
	return args.This();
}

void LinkedList::Push(const Handle<Value>& value) {
	
	Entry* entry = new Entry();
	entry->value = Persistent<Value>::New(value);

	if (head == nullptr) {
		head = entry;	
	}
	if (tail != nullptr) {
		tail->next = entry;
		entry->prev = tail;	
	}
	length++;
	tail = entry;
}	

Handle<Value> LinkedList::ForEach(const Arguments& args) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.This());
	
	if (args.Length() == 0 || !args[0]->IsFunction()) {
		return Undefined();
	}

	Local<Object> self = args.This();

	Local<Object> context = (args.Length() > 1) ? Local<Object>::Cast(args[1]) : v8::Context::GetCurrent()->Global();
	
	Local<Function> callback = Local<Function>::Cast(args[0]);

	int index = 0;
	Entry* current = obj->head;
	while (current != nullptr) {
		Handle<Value> callbackArgs[3] = {
			current->value, Integer::New(index++), self
		};
		callback->Call(context, 3, callbackArgs);
		current = current->next;
	}
	return Undefined();
}


Handle<Value> LinkedList::AddFirst(const Arguments& args) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.This());

	Entry* entry = new Entry();
	entry->value = Persistent<Value>::New(args[0]);

	if (obj->tail == nullptr) {
		obj->tail = entry;	
	}
	if (obj->head != nullptr) {
		obj->head->prev = entry;
		entry->next = obj->head;	
	}
	obj->head = entry;
	
	obj->length++;

	return Undefined();
}

Handle<Value> LinkedList::AddLast(const Arguments& args) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.This());
	obj->Push(args[0]);
	return Undefined();
}

Handle<Value> LinkedList::RemoveFirst(const Arguments& args) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.This());

	Entry* oldHead = obj->head;
	if (oldHead == nullptr) {
		return Undefined();
	}
		
	Local<Value> ret = Local<Value>::New(obj->head->value);
	
	if (oldHead->next == nullptr) {
		obj->head = nullptr;
	} else {
		obj->head = oldHead->next;
		obj->head->prev = nullptr;
	}
	delete oldHead;
	
	obj->length--;

	return scope.Close(ret);
}

Handle<Value> LinkedList::RemoveLast(const Arguments& args) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.This());

	Entry* oldTail = obj->tail;
	if (oldTail == nullptr) {
		return Undefined();
	}
		
	Local<Value> ret = Local<Value>::New(obj->tail->value);
	
	if (oldTail->prev == nullptr) {
		obj->tail = nullptr;
	} else {
		obj->tail = oldTail->prev;
		obj->tail->next = nullptr;
	}
	delete oldTail;
	
	obj->length--;

	return scope.Close(ret);
}

Handle<Value> LinkedList::GetLength(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(info.This());
	return scope.Close(Integer::New(obj->length));
}

Handle<Value> LinkedList::GetFirst(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(info.This());
	return scope.Close(Local<Value>::New(obj->head->value));
}

Handle<Value> LinkedList::GetLast(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;
	LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(info.This());
	return scope.Close(Local<Value>::New(obj->tail->value));
}

