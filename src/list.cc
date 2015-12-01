#include <node.h>
#include "list.h"
#include <iostream>

namespace list {

	using v8::Object;
	using v8::Function;
	using v8::Persistent;
	using v8::Isolate;
	using v8::Local;
	using v8::FunctionTemplate;
	using v8::FunctionCallbackInfo;
	using v8::PropertyCallbackInfo;
	using v8::Value;
	using v8::String;
	using v8::Integer;
	using v8::HandleScope;
	using v8::Context;
	using v8::Null;
	using v8::Persistent;
	using v8::UniquePersistent;
	using v8::External;

	Entry::Entry() {
		prev = nullptr;
		next = nullptr;
	};

	Entry::~Entry() {
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

	void LinkedList::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();

		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);

		tpl->SetClassName(String::NewFromUtf8(isolate, "LinkedList"));
		tpl->InstanceTemplate()->SetInternalFieldCount(3);


		tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "length"), GetLength);
		tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "first"), GetFirst);
		tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "last"), GetLast);


		NODE_SET_PROTOTYPE_METHOD(tpl, "addFirst", AddFirst);
		NODE_SET_PROTOTYPE_METHOD(tpl, "addLast", AddLast);
		NODE_SET_PROTOTYPE_METHOD(tpl, "removeFirst", RemoveFirst);
		NODE_SET_PROTOTYPE_METHOD(tpl, "removeLast", RemoveLast);

		NODE_SET_PROTOTYPE_METHOD(tpl, "unshift", AddFirst);
		NODE_SET_PROTOTYPE_METHOD(tpl, "push", AddLast);
		NODE_SET_PROTOTYPE_METHOD(tpl, "shift", RemoveFirst);
		NODE_SET_PROTOTYPE_METHOD(tpl, "pop", RemoveLast);

		NODE_SET_PROTOTYPE_METHOD(tpl, "forEach", ForEach);

		exports->Set(String::NewFromUtf8(isolate, "LinkedList"), tpl->GetFunction());
	}

	void LinkedList::New(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		if (args.IsConstructCall()) {
			LinkedList* linkedList = new LinkedList();
			linkedList->Wrap(args.This());

			int len = args.Length();
			for (int i = 0; i < len; i++) {
				linkedList->Push(isolate, args[i]);
			}

			args.GetReturnValue().Set(args.This());
		}
	}

	void LinkedList::Push(Isolate* isolate, Local<Value> value) {
		Entry* entry = new Entry();
		entry->value.Reset(isolate, value);

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

	void LinkedList::ForEach(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		if (args.Length() == 0 || !args[0]->IsFunction()) {
			return;
		}

		Local<Object> self = args.This();

		Local<Function> callback = Local<Function>::Cast(args[0]);

		Local<Value> context = args.Length() > 1 ? args[1] : Local<Value>::Cast(Null(isolate));

		int index = 0;
		Entry* current = obj->head;
		while (current != nullptr) {
			Local<Value> callbackArgs[3] = {
				Local<Value>::New(isolate, current->value),
				Integer::New(isolate, index++),
				self
			};

			callback->Call(context, 3, callbackArgs);
			current = current->next;
		}
	}

	void LinkedList::AddFirst(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		if (args.Length() == 0) {
			return;
		}

		Entry* entry = new Entry();
		entry->value.Reset(isolate, args[0]);

		if (obj->tail == nullptr) {
			obj->tail = entry;
		}
		if (obj->head != nullptr) {
			obj->head->prev = entry;
			entry->next = obj->head;
		}
		obj->head = entry;

		obj->length++;
	}

	void LinkedList::AddLast(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		if (args.Length() == 0) {
			return;
		}

		Entry* entry = new Entry();
		entry->value.Reset(isolate, args[0]);

		if (obj->head == nullptr) {
			obj->head = entry;
		}

		if (obj->tail != nullptr) {
			obj->tail->next = entry;
			entry->prev = obj->tail;
		}

		obj->tail = entry;

		obj->length++;
	}

	void LinkedList::RemoveFirst(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		Entry* oldHead = obj->head;
		if (oldHead == nullptr) {
			return;
		}

		if (oldHead->next == nullptr) {
			obj->head = nullptr;
		} else {
			obj->head = oldHead->next;
			obj->head->prev = nullptr;
		}
		obj->length--;

		Local<Value> ret = Local<Value>::New(isolate, oldHead->value);
		args.GetReturnValue().Set(ret);

		delete oldHead;
	}

	void LinkedList::RemoveLast(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		Entry* oldTail = obj->tail;
		if (oldTail == nullptr) {
			return;
		}

		if (oldTail->prev == nullptr) {
			obj->tail = nullptr;
		} else {
			obj->tail = oldTail->prev;
			obj->tail->next = nullptr;
		}
		obj->length--;

		Local<Value> ret = Local<Value>::New(isolate, oldTail->value);
		args.GetReturnValue().Set(ret);

		delete oldTail;
	}

	void LinkedList::GetLength(Local<String> property, const PropertyCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		Local<Integer> ret = Integer::New(isolate, obj->length);
		args.GetReturnValue().Set(ret);
	}


	void LinkedList::GetFirst(Local<String> property, const PropertyCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		if (obj->length > 0) {
			Local<Value> ret = Local<Value>::New(isolate, obj->head->value);
			args.GetReturnValue().Set(ret);
		}
	}

	void LinkedList::GetLast(Local<String> property, const PropertyCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);

		LinkedList* obj = ObjectWrap::Unwrap<LinkedList>(args.Holder());

		if (obj->length > 0) {
			Local<Value> ret = Local<Value>::New(isolate, obj->tail->value);
			args.GetReturnValue().Set(ret);
		}
	}
}
